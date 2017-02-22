/*******************************************************************
	************************************************************
	*
	*@file     bsp_ds18b20.c
	*@author   Bin
	*@data     2017.02
	*@brief    DS18B20的驱动模块
	*
	*************************************************************
*********************************************************************/
#include <bsp_ds18b20.h>

uchar temp_value[] = {0x00,0x00} ;
bit read_temp_is_ok = 0 ;

/****************************************************************************************
	*@brief		简单的软件延时函数
	*@param		形参i用于设置延时的长短
	*@retvel	无
*****************************************************************************************/
void delay(uint i)
{
	while(i--) ; 
}

/****************************************************************************************
	*@brief		DS18B20初始化函数
	*@param		无
	*@retvel	无
*****************************************************************************************/
uchar ds18b20_init()
{
	uchar status = 0; 
	DQ = 1  ; 
	delay(8) ; 
	DQ = 0 ; 
	delay(80) ; 
	DQ = 1 ;
	delay(14) ;
	status = DQ ; 
	delay(20) ; 
	return status ;  
}

/****************************************************************************************
	*@brief		向DS18B20写入一个比特
	*@param		参数dat是要写入的数值
	*@retvel	无
*****************************************************************************************/
void ds18b20_write_byte(uchar dat)
{
	uchar i ;												
	for(i=0;i<8;i++)
	{
		DQ = 0 ; 
		DQ = dat & 0x01 ; 
		delay(5) ; 
		DQ = 1 ; 
		dat >>= 1 ;
	}
}

/****************************************************************************************
	*@brief		从DS18B20读取数值
	*@param		无
	*@retvel	返回读取到的一个比特的数值
*****************************************************************************************/
uchar ds18b20_read_byte()
{
	uchar i,dat=0 ;
	DQ = 1 ;
	_nop_() ; 
	for(i=0;i<8;i++)
	{
		DQ = 0 ;
		dat>>=1 ; 
		DQ = 1 ;
		_nop_() ; 
		_nop_() ; 
		if(DQ==1)
			dat |= 0x80 ; 
		delay(30) ;
		DQ = 1 ; 		
	}
	return dat ; 
}

/****************************************************************************************
	*@brief		完整的读取一次温度
	*@param		无
	*@retvel	如果读取成功，将读取成功的标志位置1
*****************************************************************************************/
void read_temperature()
{
	if(ds18b20_init()==1)
	{
		read_temp_is_ok = 0 ;
	}
	else
	{
	ds18b20_write_byte(0xcc) ;         			//不提供64位ROM编码，因为只有一个DS18B20器件
		ds18b20_write_byte(0x44) ; 				//读取寄存器 ；
		ds18b20_init() ; 						//初始化
		ds18b20_write_byte(0xcc) ; 				
		ds18b20_write_byte(0xbe) ; 
		temp_value[0] =ds18b20_read_byte() ; 	//读取寄存器，DS18B20从最低位开始传送数据值
		temp_value[1] =ds18b20_read_byte() ;
		read_temp_is_ok = 1 ;
	}
}













































