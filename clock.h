#ifndef _CLOCK_H_
#define _CLOCK_H_

void clockSetup();
void clockLoop();

time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

#endif
