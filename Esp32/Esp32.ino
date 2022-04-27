
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "wifi_secrets.h"

// #define USE_SERIAL Serial

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////

const char *ssid = SECRET_SSID;
const char *password = SECRET_PASS;

// Difining the pins used for buttons
const int missionSelectorButton = 34;
const int potPin = 36;

// Properties to keep data
int stationID = 0;
int temp = 0;
int mission = 0;

// Throttle properties
int currentPotValue; // do not change
int newPotValue;     // do not change
float voltage = 0;   // do not change

// Server settings
char serverAddress[] = "192.168.0.200"; // server address for the API at school
int port = 8001;                        // Port number for the API

WiFiClient wifi;
HttpClient client = HTTPClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup()
{
    Serial.begin(115200);

    pinMode(missionSelectorButton, INPUT); // Setting the pin to INPUT mode

    // delay(4000); // Delay needed before calling the WiFi.begin

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    { // Check for the connection
      // delay(1000);
        Serial.print("Attempting to connect to Network named: ");
        Serial.println(ssid); // print the network name (SSID);
    }

    Serial.print("Connected to SSID: ");
    Serial.println(WiFi.SSID());

    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("with IP Address: ");
    Serial.println(ip);

    setPotValueStart();
    printCurrentPotValue();
    delay(4000);
}

void loop()
{
    missionSelector();

    readPotMeterValue();

    sendDataToApi();

    delay(50); // Delay for stability, as the potentiometer is not very stable in its readings
}

void sendDataToApi()
{

    // filling in the data to send
    String path = "/api/speed";
    Serial.println("making POST request");
    String contentType = "application/json";

    String postData = "{value:" + String(currentPotValue)+ "}";

    // Sending the data to API
    client.post(path, contentType, postData);

    // read the status code and body of the response
    int statusCode = client.responseStatusCode();
    String response = client.responseBody();
    // Print staus code and response
    Serial.print("Status code: ");
    Serial.println(statusCode);
    Serial.print("Response: ");
    Serial.println(response);
}

// Setting the starting value for the potentiometer
void setPotValueStart()
{
    newPotValue = analogRead(potPin);
    currentPotValue = newPotValue;
}

void printCurrentPotValue()
{
    Serial.print("PotValue: ");
    Serial.print(newPotValue);
    Serial.print("  -  Voltage: ");
    Serial.println(voltage);
}

// read the input on analog pin potPin:
void readPotMeterValue()
{
    newPotValue = analogRead(potPin);
    if (newPotValue <= currentPotValue - 50 || newPotValue >= currentPotValue + 50)
    {
        currentPotValue = newPotValue;
        voltage = (3.3 / 4095.0) * currentPotValue;

        printCurrentPotValue();
    }
}

// Taking input from the mission selector button
void missionSelector()
{
    temp = digitalRead(missionSelectorButton);
    if (temp == HIGH)
    {
        mission++;
        Serial.println("Selector on");
    }
}
