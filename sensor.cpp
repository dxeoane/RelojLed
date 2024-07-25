#include <ESP8266WiFi.h>
#include "Arduino.h"
#include "utils.h"
#include "display.h"
#include "sensor.h"
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp; 

void sensorSetup(){
  if (!aht.begin()) 
  {
    Serial.println("Could not find AHT? Check wiring");
  } else {
    Serial.println("AHT10 or AHT20 found");
  }

  if (!bmp.begin()) 
  {
    Serial.println("Could not find BMP280? Check wiring");
  } else {
    Serial.println("BMP280 found");
  }
}
void sensorLoop(){
  static float pressure = 0.0;
  static float temperature = 0.0;
  static float humidity = 0.0;
  static unsigned long lastMeasureMillis = 0;

  // Nos aseguramos que no se hace mas de una medicion por segundo, para que no se caliente el sensor (ver pdf)
  if (millis() - lastMeasureMillis > 1000) {
    sensors_event_t hum, temp;
    aht.getEvent(&hum, &temp); 
    pressure = bmp.readPressure() / 100.0F;
    temperature = temp.temperature;
    humidity = hum.relative_humidity;
    lastMeasureMillis = millis();
  }
   
  String text = lpad(pressure - 1013,3) + "." + lpad(humidity,2) + "." + lpad(temperature,3); 
  displayText(text);
}
