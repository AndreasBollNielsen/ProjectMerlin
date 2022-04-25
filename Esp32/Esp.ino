
//The MKR1000 board is very unstable, and do stop working after a while. It can be hours
// or sometimes days. But it do fail often. There for it is not suitable for production environment.


// Simple POST client for ArduinoHttpClient library
// Connects to server once every five seconds, sends a POST request
// and a request body

#include <DHT.h>
#include <SPI.h>
#include <WiFi101.h>
#include "arduino_secrets.h"
#include <ArduinoHttpClient.h>

// Constants
#define DHTPIN 2          // what pin we're connected to
#define DHTTYPE DHT22     // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

// Variables
float hum;                // Stores humidity value
float temp;               // Stores temperature value
int messuringTimer = 500; // Time between messurement

// How many messurements shall be made before calculating the sending dataList
//  All three numbers needs to be the same or the code will break
int spreadSize = 10;
float hums[10];
float temps[10];

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
const char* ssid = SECRET_SSID;
const char* pass = SECRET_PASS;

//char serverAddress[] = "192.168.2.24"; // server address for the API at home
char serverAddress[] = "192.168.2.105"; // server address for the API at school
int port = 8001;                       // Port number for the API

WiFiClient wifi;
HttpClient client = HttpClient(wifi, serverAddress, port);
int status = WL_IDLE_STATUS;

void setup()
{
  Serial.begin(9600);
  dht.begin();
  while (status != WL_CONNECTED)
  {
    Serial.print("Attempting to connect to Network named: ");
    Serial.println(ssid); // print the network name (SSID);

    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
}

void loop()
{
  // Reading and calculating the numbers
  readTempAndHum();

  // filling in the data to send
  String path = "/api/sensor/post";
  Serial.println("making POST request");
  String contentType = "application/json";

  String postData = "{\"humidity\":" + String(hum, 2) + ",\"temperature\":" + String(temp, 2) + "}";

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

// Function to do the messurements, calculating avarage an
void readTempAndHum()
{
  // Filling in data from sensor to the arrays to be used for calculating the avarage 
  for (int i = 0; i < spreadSize; i++)
  {
    hum = dht.readHumidity();
    temp = dht.readTemperature();
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");
    hums[i] = hum;
    temps[i] = temp;
    delay(messuringTimer);
  }
  calculateAvarage();
}

// Function to calculate avarage for each messurement
void calculateAvarage()
{
  float humsCount = 0;
  float tempsCount = 0;
  for (int i = 0; i < spreadSize; i++)
  {
    humsCount += hums[i];
    tempsCount += temps[i];
  }
  hum = humsCount / spreadSize;
  temp = tempsCount / spreadSize;
  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.println(temp);
}