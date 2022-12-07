// * data generator
#include <ESP8266WiFi.h>
#include "ESPAsyncWebServer.h"



// Set your access point network credentials
const char *ssid = "ESP8266-Access-Point";
const char *password = "123456789";


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

    server.begin();
}

void loop()
{
}