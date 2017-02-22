/*******************************************************************
	************************************************************
	*
	*@file     bsp_lcd1602.c
	*@author   Bin
	*@data     2017.02
	*@brief    LCD1602的驱动模块
	*
	*************************************************************
*********************************************************************/

#include "bsp_lcd1602.h"

uchar code df_Table[]=
{
 	0,1,1,2,3,3,4,4,5,6,6,7,8,8,9,9
};

uchar title_1[13] = {"Current Temp:"} ;
uchar title_2[14] = {"Max safe Temp:"} ;
uchar title_3[14] = {"Min safe Temp:"} ;

uchar convert_value[4] = {0,0,0,0} ;
uchar convert_max[4] = {0,0,0,0} ;
uchar convert_min[4] = {0,0,0,0} ;

extern uchar temp_value[] ;
extern bit read_temp_is_ok ;
extern uchar Display_status[] ;
extern uchar max_safe_number ;
extern uchar min_safe_number ;
extern uchar dis_type ;

uchar current_temp ; 

/****************************************************************************************
	*@brief		简单的软件延时函数
	*@param		形参c用于设置延时的长短
	*@retvel	无
*****************************************************************************************/
void Delayms(uint c)
{
    uchar a,b;
	for (; c>0; c--)
	{
		 for (b=199;b>0;b--)
		 {
		  	for(a=1;a>0;a--);
		 }      
	}   	
}

/****************************************************************************************
	*@brief		LCD写命令函数
	*@param		形参command代表写进的命令
	*@retvel	无
*****************************************************************************************/
void LCD_WRITE_COM(uchar commend)
{
  	EN = 0 ; 
  	RW = 0 ; 
  	RS = 0 ; 
	
	DATA_PORT = commend ;
	Delayms(1) ; 
	
	EN = 1 ; 
	Delayms(5) ; 
	EN = 0 ;  
}

/****************************************************************************************
	*@brief		LCD写数据函数
	*@param		形参date代表写进的数据
	*@retvel	无
*****************************************************************************************/
void LCD_WRITE_DATA(uchar date)
{
	EN = 0 ; 
  	RW = 0 ; 
  	RS = 1 ; 
	
	DATA_PORT = date ;
	Delayms(1) ; 
	
	EN = 1 ; 
	Delayms(5) ; 
	EN = 0 ;  
}

/****************************************************************************************
	*@brief		LCD初始化函数
	*@param		无
	*@retvel	无
*****************************************************************************************/
void LCD_Init(void)
{
	LCD_WRITE_COM(0x38) ; 	//开显示
	LCD_WRITE_COM(0x0c) ;	//开显示不显示光标
	LCD_WRITE_COM(0x06) ;  	//写一个指针加一
	LCD_WRITE_COM(0x01) ;  	//清屏
	LCD_WRITE_COM(0x80) ;  	//设置数据指针起点
}

/****************************************************************************************
	*@brief		LCD显示温度函数
	*@param		无
	*@retvel	无
*****************************************************************************************/
void display_temperature()
{
	bit neg_sign = 0 ;
	uchar display_buff[6] ; 
	uchar i ;
	if((temp_value[1]&0xf0) == 0xf0 ) 				// 如果接收到的温度是负数，则将补码转换成原码 ；
	{
		temp_value[1] = ~temp_value[1] ; 
		temp_value[0] = ~temp_value[0] + 0x01 ; 
		neg_sign = 1 ;
	}
	
	convert_value[0] = df_Table[temp_value[0]&0x0f]; 
	current_temp = ((temp_value[0]&0xf0)>>4)|((temp_value[1]&0x0f)<<4) ; 
	convert_value[3] = current_temp/100 ; 
	convert_value[2] = current_temp%100/10 ; 
	convert_value[1] = current_temp%10 ; 
	if(neg_sign == 1)
	{
		display_buff[0] = '-' ;
		neg_sign = 0 ;
	}
	else
		display_buff[0] = ' ' ;
		
	if(convert_value[3] == 0 )
		display_buff[1] = ' ' ; 
	else
		display_buff[1] = convert_value[3]+'0' ;
	
	if(convert_value[2] == 0 && convert_value[3] == 0 )
		display_buff[2] = ' ' ; 
	else
		display_buff[2] = convert_value[2]+'0' ;
	
	display_buff[3] = convert_value[1]+'0' ;
	display_buff[4] = '.' ;
	display_buff[5] = convert_value[0]+'0' ;
	LCD_WRITE_COM(0x80+0x40) ;
	for(i=0;i<6;i++)
		LCD_WRITE_DATA(display_buff[i]) ;
    LCD_WRITE_COM(0x80+0x40+0x07) ;
	LCD_WRITE_DATA(0xdf) ;
	LCD_WRITE_DATA('C') ;
	LCD_WRITE_COM(0x80) ;	
}

/****************************************************************************************
	*@brief		最大安全温度显示函数
	*@param		无
	*@retvel	无
*****************************************************************************************/
void display_max()
{
	uchar display_buff[6] ; 
	uchar i ;
	
	convert_max[0] = convert_value[0];  
	convert_max[3] = max_safe_number/100 ; 
	convert_max[2] = max_safe_number%100/10 ; 
	convert_max[1] = max_safe_number%10 ; 
		
	if(convert_max[3] == 0 )
		display_buff[0] = ' ' ; 
	else
		display_buff[0] = convert_max[3]+'0' ;
	
	if(convert_max[2] == 0 && convert_max[3] == 0 )
		display_buff[1] = ' ' ; 
	else
		display_buff[1] = convert_max[2]+'0' ;
	
	display_buff[2] =convert_max[1]+'0' ;
	display_buff[3] = '.' ;
	display_buff[4] = convert_max[0]+'0' ;
	LCD_WRITE_COM(0x80+0x40) ;
	for(i=0;i<5;i++)
		LCD_WRITE_DATA(display_buff[i]) ;
    LCD_WRITE_COM(0x80+0x40+0x07) ;
	LCD_WRITE_DATA(0xdf) ;
	LCD_WRITE_DATA('C') ;
	LCD_WRITE_COM(0x80) ;	
}

/****************************************************************************************
	*@brief		最小安全温度显示函数
	*@param		无
	*@retvel	无
*****************************************************************************************/
void display_min()
{
	uchar display_buff[6] ; 
	uchar i ;
	
	convert_min[0] = convert_value[0];  
	convert_min[3] = min_safe_number/100 ; 
	convert_min[2] = min_safe_number%100/10 ; 
	convert_min[1] = min_safe_number%10 ; 
		
	if(convert_min[3] == 0 )
		display_buff[0] = ' ' ; 
	else
		display_buff[0] = convert_min[3]+'0' ;
	
	if(convert_min[2] == 0 && convert_min[3] == 0 )
		display_buff[1] = ' ' ; 
	else
		display_buff[1] = convert_min[2]+'0' ;
	
	display_buff[2] =convert_min[1]+'0' ;
	display_buff[3] = '.' ;
	display_buff[4] = convert_min[0]+'0' ;
	LCD_WRITE_COM(0x80+0x40) ;
	for(i=0;i<5;i++)
		LCD_WRITE_DATA(display_buff[i]) ;
    LCD_WRITE_COM(0x80+0x40+0x07) ;
	LCD_WRITE_DATA(0xdf) ;
	LCD_WRITE_DATA('C') ;
	LCD_WRITE_COM(0x80) ;	
}

/****************************************************************************************
	*@brief		按键切换显示类型
	*@param		无
	*@retvel	无
*****************************************************************************************/
void Mode_switch()
{
	uint i ; 
	P3 = 0x00 ;										//报警器初始化
	switch(status_deal(key_check()))				//检测按键状态
	{
		case 0:										//情况0是显示当前的温度值
			LCD_Init() ;							//LCD初始化函数
			while((dis_type==0)&&(P1==0xff))		
			{
				read_temperature() ;				//读取当前检测到的温度值
				if(read_temp_is_ok==1)				//检测数据成功
				{
					for( i=0 ; i<13 ; i++ )
						LCD_WRITE_DATA(title_1[i]) ;//说明当前温度值的类型
					display_temperature() ; 		//显示温度值
				}
				if((current_temp>max_safe_number)||(current_temp<min_safe_number)) //如果当前温度值不在安全温度范围内，报警
				{
					Alarm(90);
					Alarm(120);
				}
			}
			break ;
			
		case 1:															//情况1是显示最高安全温度并可通过按键进行设置
			LCD_Init() ;												//lcd初始化
			while((dis_type==1)&&((P1==0xff)||(P1==0xfd)||(P1==0xfb)))
				{	
					safe_temp_set(key_check()) ;						//检测设置按键状态
					for( i=0 ; i<14 ; i++ )
						LCD_WRITE_DATA(title_2[i]) ;					//显示当前温度值是最高安全温度
					display_max() ;	
				}
				break ;
		
		case 2:															//情况1是显示最低安全温度并可通过按键进行设置
			LCD_Init() ;
			while((dis_type==2)&&((P1==0xff)||(P1==0xfd)||(P1==0xfb)))
				{	
					safe_temp_set(key_check()) ;
					for( i=0 ; i<14 ; i++ )
						LCD_WRITE_DATA(title_3[i]) ;
					display_min() ;
				}
				break ;					
	}
}






