
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "wifi_secrets.h"

// #define USE_SERIAL Serial

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
/////// WiFi Settings ///////
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

// const char *ssid = SECRET_SSID;
// const char *password = SECRET_PASS;

void setup()
{

    Serial.begin(115200);
    delay(4000); // Delay needed before calling the WiFi.begin

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    { // Check for the connection
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println("Connected to the WiFi network");
}

void loop()
{
    if (WiFi.status() == WL_CONNECTED) // Check WiFi connection status
    { 
        Serial.println("Still connected to wifi");
    }
    else
    {
        Serial.println("Wifi connection dropped");
    }
    delay(2000);
}