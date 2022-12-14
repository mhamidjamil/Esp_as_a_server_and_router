// * dual conectivity
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti WiFiMulti;

const char *ssid = "ESP8266-Access-Point";
const char *password = "123456789";

// Your IP address or domain name with URL path
const char *serverNameTemp = "http://192.168.4.1/temperature";
const char *serverNameHumi = "http://192.168.4.1/humidity";
const char *serverNamePres = "http://192.168.4.1/pressure";
bool newDataFetched = false;

// ? thingspeak starts
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SECRET_SSID "Revenant"           // replace MySSID with your WiFi network name
#define SECRET_PASS "12345679"           // replace MyPassword with your WiFi password
#define SECRET_CH_ID 1769780             // esp_gyro
#define myWriteAPIKey "BZ5TOKEQQGJOVVJA" // API Key for Test channel
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
char ssid_thingSpeak[] = SECRET_SSID; // your network SSID (name)
char pass_thingSpeak[] = SECRET_PASS; // your network password
int keyIndex = 0;                     // your network key Index number (needed only for WEP)
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;

// Initialize our values
// int number1 = 0;
// int number2 = random(0, 100);
// int number3 = random(0, 100);
String myStatus = "";
// ? thingspeak ends
String temperature;
String humidity;
String pressure;

unsigned long previousMillis = 0;
const long interval = 5000;
void getData();
void sendDataToThingspeak();
void setup()
{
    Serial.begin(115200);
    Serial.println();
}
void Connect(int flag)
{
    WiFi.disconnect();
    delay(100);
    if (flag == 1)
    {
        WiFi.begin(ssid, password);
        Serial.println("Connecting to AP");
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println("Connected to AP");
    }
    else if (flag == 2)
    {
        WiFi.begin(ssid_thingSpeak, pass_thingSpeak);
        Serial.println("Connecting to ThingSpeak");
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(500);
            Serial.print(".");
        }
        Serial.println("Connected to ThingSpeak");
    }
}
void loop()
{
    if (!newDataFetched)
    {
        getData();
    }
    else if (newDataFetched)
    {
        sendDataToThingspeak();
    }
    else
    {
        Serial.println("Unknown error!");
    }
    delay(2000);
}
void getData()
{
    Connect(1);
    // * data fetching from other esp8266
    unsigned long currentMillis = millis();

    // Check WiFi connection status
    if ((WiFiMulti.run() == WL_CONNECTED))
    {
        temperature = httpGETRequest(serverNameTemp);
        humidity = httpGETRequest(serverNameHumi);
        pressure = httpGETRequest(serverNamePres);
        Serial.println("Temperature: " + temperature + " *C - Humidity: " + humidity + " % - Pressure: " + pressure + " hPa");

        // save the last HTTP GET Request
        previousMillis = currentMillis;
        newDataFetched = true;
        // WiFi.disconnect();
    }
    else
    {
        Serial.println("WiFi Disconnected (" + String(ssid) + ")");
    }
}
void sendDataToThingspeak()
{
    Connect(2);

    // set the fields with the values
    int x = ThingSpeak.writeField(myChannelNumber, 4, temperature, myWriteAPIKey);
    if (x == 200)
    {
        Serial.println("Channel update successful.");
    }
    else
    {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
}
String httpGETRequest(const char *serverName)
{
    WiFiClient client;
    HTTPClient http;

    // Your IP address with path or Domain name with URL path
    http.begin(client, serverName);

    // Send HTTP POST request
    int httpResponseCode = http.GET();

    String payload = "--";

    if (httpResponseCode > 0)
    {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();

    return payload;
}