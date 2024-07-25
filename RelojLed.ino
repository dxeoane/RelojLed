#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include "Configuration.h"
#include "display.h"
#include "utils.h"
#include "clock.h"

#define HOME

#ifdef HOME
  #define TEMPERATURE_SENSOR_ENABLED
#else

#endif

#ifdef TEMPERATURE_SENSOR_ENABLED 
  #include "sensor.h"
#endif  

void setup() {
  Serial.begin(115200);
  // Esperamos un poco a que todo arranque
  delay(1000);
  Serial.println("\n\n\n");  

  displaySetup();

  // Nos conectamos al WiFi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  

  // Inicializamos el reloj
  clockSetup();

  #ifdef TEMPERATURE_SENSOR_ENABLED
    // Inicializamos el sensor
    sensorSetup(); 
  #endif 
}

void loop() {    
  enum Mode {
    TEST,
    CLOCK,
    CHRONOMETER,
    TEMPERATURE
  };
  
  static Mode mode = CLOCK;

  switch (mode) {
    case TEST: 
      displayText("HELLO   ");
      break;
    case CLOCK:    
      clockLoop();
      break;
     case CHRONOMETER:    
      chronometerLoop();
      break;
     #ifdef TEMPERATURE_SENSOR_ENABLED 
       case TEMPERATURE:
         sensorLoop();
         break;  
     #endif 
  } 

  switch (buttonsRead()) {
    case 0x01: // Primer boton
      mode = CLOCK;
      break;
    case 0x02: // Segundo boton
      if (mode != CHRONOMETER) chronometerReset();
      mode = CHRONOMETER;
      break;  
    #ifdef TEMPERATURE_SENSOR_ENABLED   
      case 0x04: // Tercer boton
        mode = TEMPERATURE;
        break;
    #endif    
    case 0x81: // Primer y útimo botón, pulsados a la vez
      mode = TEST;
      break;  
  }
  
}
