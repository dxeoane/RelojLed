#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include "Arduino.h"
#include "utils.h"
#include "display.h"
#include "clock.h"


WiFiUDP Udp;
unsigned int localPort = 8888;

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

// NTP
static const char ntpServer[] = "us.pool.ntp.org";
const int timeZone = 2; 

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  
  Serial.println("Transmit NTP Request");
  WiFi.hostByName(ntpServer, ntpServerIP);
  
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

void clockSetup() {
  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  setSyncInterval(300);
}

void clockLoop() {
  String timenow = lpad(day(),2) + "." + lpad(month(),2) + "." + lpad(hour(),2) + "." + lpad(minute(),2);
  displayText(timenow);
}

unsigned long chronometerStart = 0;

void chronometerReset() {
  chronometerStart = millis();
}

void chronometerLoop() {
  unsigned long currentMillis = millis() - chronometerStart;
  unsigned long h = currentMillis / (3600 * 1000);
  currentMillis = currentMillis % (3600 * 1000);
  unsigned long m = currentMillis / (60 * 1000);
  currentMillis = currentMillis % (60 * 1000);
  unsigned long s = currentMillis / 1000;
  currentMillis = currentMillis % 1000;
  unsigned long c = currentMillis / 100;
  String timenow = lpad(h,3) + "." + lpad(m,2) + "." + lpad(s,2) + "." + String(c);
  displayText(timenow);
}
