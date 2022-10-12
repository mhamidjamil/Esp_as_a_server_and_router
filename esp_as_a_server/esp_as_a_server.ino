// / Import required libraries
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"

// #include <Wire.h>
// #include <Adafruit_Sensor.h>
// #include <Adafruit_BME280.h>

// Set your access point network credentials
const char *ssid = "ESP8266-Access-Point";
const char *password = "123456789";

/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

// Adafruit_BME280 bme; // I2C
// Adafruit_BME280 bme(BME_CS); // hardware SPI
// Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String writeTemp()
{
    //   return String(bme.writeTemperature());
    return String(random(0, 50));
    // return String(1.8 * bme.writeTemperature() + 32);
}

String writeHumi()
{
    // return String(bme.writeHumidity());
    return String(random(50, 100));
}

String writePres()
{
    // return String(bme.writePressure() / 100.0F);
    return String(random(100, 150));
}

void setup()
{
    // Serial port for debugging purposes
    Serial.begin(115200);
    Serial.println();

    // Setting the ESP as an access point
    Serial.print("Setting AP (Access Point)…");
    // Remove the password parameter, if you want the AP (Access Point) to be open
    WiFi.softAP(ssid, password);

    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", writeTemp().c_str()); });
    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", writeHumi().c_str()); });
    server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/plain", writePres().c_str()); });

    // bool status;

    // // default settings
    // // (you can also pass in a Wire library object like &Wire2)
    // status = bme.begin(0x76);
    // if (!status)
    // {
    //     Serial.println("Could not find a valid BME280 sensor, check wiring!");
    //     while (1)
    //         ;
    // }

    // Start server
    server.begin();
}

void loop()
{
}