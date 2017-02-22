#ifndef __BSP_WARN_H
#define __BSP_WARN_H
	
#include <reg51.h>

#define uchar unsigned char
#define uint unsigned int
	
sbit LED = P3^6;
sbit BEEP = P3^7;

void Alarm(uchar t) ;

#endif
