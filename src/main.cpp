#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <Wire.h>
#include "../../config.h"

// LCD display data
LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

// ntp server data
#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET 3600
#define UTC_OFFSET_DST 3600

// global minute variable
int minute;

//
// Get the local time
//
void printLocalTime()
{
    struct tm timeinfo;
   // Get the current time
    if (!getLocalTime(&timeinfo))
    {
        LCD.setCursor(0, 1);
        LCD.println("Connection Err");
        return;
    }  
    // Print the time
    LCD.setCursor(0, 0);
    LCD.print(&timeinfo, "%H:%M:%S");
    // Print the date
    LCD.setCursor(0, 1);
    LCD.print(&timeinfo, "%d-%m-%Y");
    // Get the minute
    minute = timeinfo.tm_min;
}

//
// Print the temperature, humidity and pressure
//
void printTempHumPres()
{
    // Read sensor data
    float temp = 22.5;
    float humidity = 42;
    float pressure = 1002;
    // Print temperature and humidity
    if (minute % 2 == 0)
    {
        LCD.setCursor(12, 0);
        LCD.print(String(temp, 1));
    }
    else
    {
        LCD.setCursor(12, 0);
        String hums = String(humidity, 0) + " %";
        LCD.print(hums);
    }
    // Print pressure
    LCD.setCursor(12, 1);
    LCD.print(String(pressure, 0));
}

//
// Setup the LCD display
//
void setup()
{
    // Setup the serial port
    Serial.begin(115200);
    // Setup the LCD
    LCD.init();
    LCD.backlight();
    LCD.setCursor(0, 0);
    LCD.print("Connecting to ");
    LCD.setCursor(0, 1);
    LCD.print("WiFi ");
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
   // WiFi.begin("Wokwi-GUEST", "", 6);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(250);
    }
    // Print the IP address
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    delay(2000);
    LCD.clear();
    // Configure the NTP client
    configTime(UTC_OFFSET, UTC_OFFSET_DST, NTP_SERVER);
}

//
// Main loop
//
void loop()
{
    printLocalTime();
    printTempHumPres();
    delay(250);
}