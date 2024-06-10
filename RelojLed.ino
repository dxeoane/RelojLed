#include <ESP8266WiFi.h>
#include <TimeLib.h>
#include "Configuration.h"
#include "display.h"
#include "utils.h"
#include "clock.h"

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
}

void loop() {
  
  enum Mode {
    TEST,
    CLOCK
  };
  
  static Mode mode = CLOCK;

  switch (mode) {
    case TEST: 
      displayText("HELLO   ");
      break;
    case CLOCK:
      clockLoop();
      break;
  }

  switch (buttonsRead()) {
    case 0x01: // Primer boton
      mode = CLOCK;
      break;
    case 0x81: // Primer y útimo botón, pulsados a la vez
      mode = TEST;
      break;  
  }
  
}
