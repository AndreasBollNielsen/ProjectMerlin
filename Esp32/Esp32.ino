
#include <Arduino.h>
#include <WiFi.h>
#include "wifi_secrets.h"
#include <HTTPClient.h>

// #define USE_SERIAL Serial

///////please enter your sensitive data in the Secret tab/wifi_secrets.h

/////// WiFi Settings ///////
const char *ssid = SECRET_SSID;
const char *password = SECRET_PASS;

// server address for the API
const char *serverAddress = "http://192.168.0.103:3000/api/speeds";

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

WiFiClient wifi;
int status = WL_IDLE_STATUS;
int httpResponseCode;

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
    doorOpenClose();

    readPotMeterValue();

    sendDataToApi();

    delay(500); // Delay for stability, as the potentiometer is not very stable in its readings
}

void sendDataToApi()
{
    if (WiFi.status() == WL_CONNECTED && String(currentPotValue) != String(httpResponseCode))
    {

        HTTPClient http;
        http.begin(wifi, serverAddress);

        // filling in the data to send
        Serial.println("making POST request to " + String(serverAddress));

        // APIkey: 7ea445c7-a737-4b9d-8b7d-5f8899a70515
        http.addHeader("Content-Type", "application/json");
        // Sending the data to API and getting responscode back
        httpResponseCode = http.POST("{\"api_key\":\"7ea445c7-a737-4b9d-8b7d-5f8899a70515\",\"speed\":" + String(currentPotValue) + "}");

        // Printing the responsecode to serial
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        // Free resources
        http.end();
    }
    else
    {
        Serial.println("WiFi Disconnected");
    }
}

// Setting the starting value for the potentiometer
void setPotValueStart()
{
    newPotValue = analogRead(potPin);
    currentPotValue = newPotValue;
}

// Print potValue to serial
void printCurrentPotValue()
{
    Serial.print("PotValue: ");     // DEBUG
    Serial.print(newPotValue);      // DEBUG
    Serial.print("  -  Voltage: "); // DEBUG
    Serial.println(voltage);        // DEBUG
}

// read the input on analog pin potPin:
void readPotMeterValue()
{
    newPotValue = analogRead(potPin);
    if (newPotValue <= currentPotValue - 50 || newPotValue >= currentPotValue + 50)
    {
        currentPotValue = newPotValue;
        voltage = (3.3 / 4095.0) * currentPotValue;

        printCurrentPotValue(); // DEBUG
    }
}

// Taking input from the mission selector button
void doorOpenClose()
{
    temp = digitalRead(missionSelectorButton);
    if (temp == HIGH)
    {
        mission++;
        Serial.println("Selector on"); // DEBUG
    }
}

// Light on and off the light
void lightOnOff()
{
}

// Toggling the horn when holding down the button
void horn()
{
}

// EMergency stop button. setting speed to 0
void emergencyStop()
{
}

// Misison selector up button
void missionSelectorUp()
{
}

// Misison selector down button
void missionSelectorDown()
{
}

// Mission confirm button
void missionConfirm()
{
}

// Mission abort button
void missionAbort()
{
}
