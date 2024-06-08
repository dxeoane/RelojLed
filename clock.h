#ifndef _CLOCK_H_
#define _CLOCK_H_

void clockSetup();
time_t getNtpTime();
void sendNTPpacket(IPAddress &address);

#endif
