#ifndef __bsp_ds18b20_H
#define __bsp_ds18b20_H

#include <reg51.h>
#include <intrins.h>

#define uchar unsigned char
#define uint unsigned int

sbit	DQ = P2^3 ;

/*º¯ÊıÉùÃ÷*/
void delay(uint i) ; 
uchar ds18b20_init() ; 
void ds18b20_write_byte(uchar dat) ;
uchar ds18b20_read_byte() ; 
void read_temperature() ; 

#endif


