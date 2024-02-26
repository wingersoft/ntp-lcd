#include <Arduino.h>
#include <Adafruit_BME280.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <Wire.h>
#include "../../config.h"

// LCD display data
LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);

// Set sensor pin numbers
const int BME_I2C_SDA  = 21;
const int BME_I2C_SCL  = 22;
const int INTERNAL_LED = 5;

// Create BME280 object
Adafruit_BME280 bme;

// ntp server data
#define NTP_SERVER "pool.ntp.org"
#define UTC_OFFSET 3600
#define UTC_OFFSET_DST 3600

// global minute variables
int seconds;
int minutes;
float temp;
float humidity;
float pressure;

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
    // Get the seconds
    seconds = timeinfo.tm_sec;
    // Get the minute
    minutes = timeinfo.tm_min;
}

//
// Print the temperature, humidity and pressure
//
void printTempHumPres()
{
    if (seconds == 0)
    { 
        // Read sensor data
        temp = bme.readTemperature();
        humidity = bme.readHumidity();
        pressure = bme.readPressure() / 100.0F;
    }
     // Print temperature and humidity
    if (minutes % 2 == 0)
    {
        LCD.setCursor(12, 0);
        LCD.print(String(temp, 1));
    }
    else
    {
        LCD.setCursor(12, 0);
        String humStr = String(humidity, 0) + " %";
        LCD.print(humStr);
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
    // Initialize BME280 sensor
    Wire.begin(BME_I2C_SDA, BME_I2C_SCL);
    bool status = bme.begin(0x76, &Wire);
    // Check if sensor is connected
    if (!status) 
    {
        Serial.println("Could not find BME280 sensor!");
        while (1);
    }   
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
