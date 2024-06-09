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
  String timenow = twoDigits(day()) + "." + twoDigits(month()) + "." + twoDigits(hour()) + "." + twoDigits(minute());
  displayText(timenow);

  int buttons = buttonsRead();
  if (buttons) {
    String value = "";
    for (int i=0; i< 8; i++) {
      if (buttons % 2 == 0) {
        value = value + "0";
      } else {
        value = value + "1";
      }  
      buttons = buttons / 2;      
    }
    displayText(value);
    delay(1000);
  }
}
