#ifndef _CLOCK_H_
#define _CLOCK_H_

void clockSetup();
void clockLoop();
void chronometerReset();
void chronometerLoop();

time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

#endif
