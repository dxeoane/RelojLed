#include <ESP8266WiFi.h>
#include <TM1638plus.h>
#include <TimeLib.h>
#include "Configuration.h"
#include "utils.h"
#include "clock.h"

// STB, CLK, DIO
TM1638plus TM1638(13, 12 , 14);

void setup() {
  Serial.begin(115200);
  // Esperamos un poco a que todo arranque
  delay(1000);
  Serial.println("\n\n\n");  

  // Inicializamos el display
  TM1638.displayBegin();
  TM1638.displayText("Conn ...");

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
  String timenow = twoDigits(hour()) + "-" + twoDigits(minute()) + "-" + twoDigits(second());
  TM1638.displayText(timenow.c_str());
}
