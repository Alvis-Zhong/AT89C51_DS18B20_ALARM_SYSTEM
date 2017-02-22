#ifndef __BSP_KEY
#define __BSP_KEY

#include <reg51.h>

#define 	uchar	unsigned char
#define 	uint 	unsigned int 
#define 	KEY		P1	

/*º¯ÊıÉùÃ÷*/
uchar key_check() ;
uchar status_deal(uchar temp) ;
void safe_temp_set(uchar temp) ;

#endif

