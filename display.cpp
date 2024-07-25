#include <TM1638plus.h>

const long intervalButton = 200; 

// STB, CLK, DIO
TM1638plus TM1638(13, 12 , 14);

void displaySetup() {
  // Inicializamos el display
  TM1638.displayBegin();
  TM1638.displayText("HELLO ...");
}

void displayText(String text) {
  TM1638.displayText(text.c_str());
}

uint8_t buttonsRead(void)
{
  uint8_t buttons = 0;
  static unsigned long previousMillisButton = 0;  // executed once 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisButton >= intervalButton) {
    previousMillisButton = currentMillis;
    buttons = TM1638.readButtons();
  }
  return buttons;
}
