

#include <ESP8266WiFi.h>
// #include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define SECRET_SSID "Revenant"                        // replace MySSID with your WiFi network name
#define SECRET_PASS "12345679"                        // replace MyPassword with your WiFi password
#define SECRET_CH_ID 1769780                          // esp_gyro
#define myWriteAPIKey "BZ5TOKEQQGJOVVJA"              // API Key for Test channel
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
char ssid[] = SECRET_SSID; // your network SSID (name)
char pass[] = SECRET_PASS; // your network password
int keyIndex = 0;          // your network key Index number (needed only for WEP)
WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;

// Initialize our values
int number1 = 0;
int number2 = random(0, 100);
int number3 = random(0, 100);
String myStatus = "";

void setup()
{
    Serial.begin(115200); // Initialize serial
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for Leonardo native USB port only
    }

    WiFi.mode(WIFI_STA);
    ThingSpeak.begin(client); // Initialize ThingSpeak
}

void loop()
{

    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(SECRET_SSID);
        while (WiFi.status() != WL_CONNECTED)
        {
            WiFi.begin(ssid, pass); // Connect to WPA/WPA2 network. Change this line if using open or WEP network
            Serial.print(".");
            delay(5000);
        }
        Serial.println("\nConnected.");
    }

    // set the fields with the values
    ThingSpeak.setField(4, number1++);
    ThingSpeak.setField(5, number2++);
    ThingSpeak.setField(6, number3++);
    ThingSpeak.setStatus(myStatus);

    // write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200)
    {
        Serial.println("Channel update successful.");
    }
    else
    {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
}