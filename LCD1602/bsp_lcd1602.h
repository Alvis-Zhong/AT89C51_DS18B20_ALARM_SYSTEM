#ifndef __LCD1602_H
#define __LCD1602_H

#include <reg51.h>
#include <intrins.h>
#include "bsp_warn.h"
#include "bsp_key.h"
#include "bsp_ds18b20.h"


#define		uint 	unsigned int
#define 	uchar 	unsigned char
	
#define 	DATA_PORT	P0

sbit		RS = P2^0 ;  
sbit		RW = P2^1 ; 
sbit    	EN = P2^2 ;

void Delayms(uint ms) ; 
void LCD_WRITE_COM(uchar commend) ;
void LCD_WRITE_DATA(uchar date) ; 
void LCD_Init(void) ;
void display_temperature() ;
void display_max() ;
void display_min() ;

void Mode_switch() ; 

#endif 

