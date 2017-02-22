/*******************************************************************
	************************************************************
	*
	*@file     bsp_lcd1602.c
	*@author   Bin
	*@data     2017.02
	*@brief    LCD1602������ģ��
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
	*@brief		�򵥵������ʱ����
	*@param		�β�c����������ʱ�ĳ���
	*@retvel	��
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
	*@brief		LCDд�����
	*@param		�β�command����д��������
	*@retvel	��
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
	*@brief		LCDд���ݺ���
	*@param		�β�date����д��������
	*@retvel	��
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
	*@brief		LCD��ʼ������
	*@param		��
	*@retvel	��
*****************************************************************************************/
void LCD_Init(void)
{
	LCD_WRITE_COM(0x38) ; 	//����ʾ
	LCD_WRITE_COM(0x0c) ;	//����ʾ����ʾ���
	LCD_WRITE_COM(0x06) ;  	//дһ��ָ���һ
	LCD_WRITE_COM(0x01) ;  	//����
	LCD_WRITE_COM(0x80) ;  	//��������ָ�����
}

/****************************************************************************************
	*@brief		LCD��ʾ�¶Ⱥ���
	*@param		��
	*@retvel	��
*****************************************************************************************/
void display_temperature()
{
	bit neg_sign = 0 ;
	uchar display_buff[6] ; 
	uchar i ;
	if((temp_value[1]&0xf0) == 0xf0 ) 				// ������յ����¶��Ǹ������򽫲���ת����ԭ�� ��
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
	*@brief		���ȫ�¶���ʾ����
	*@param		��
	*@retvel	��
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
	*@brief		��С��ȫ�¶���ʾ����
	*@param		��
	*@retvel	��
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
	*@brief		�����л���ʾ����
	*@param		��
	*@retvel	��
*****************************************************************************************/
void Mode_switch()
{
	uint i ; 
	P3 = 0x00 ;										//��������ʼ��
	switch(status_deal(key_check()))				//��ⰴ��״̬
	{
		case 0:										//���0����ʾ��ǰ���¶�ֵ
			LCD_Init() ;							//LCD��ʼ������
			while((dis_type==0)&&(P1==0xff))		
			{
				read_temperature() ;				//��ȡ��ǰ��⵽���¶�ֵ
				if(read_temp_is_ok==1)				//������ݳɹ�
				{
					for( i=0 ; i<13 ; i++ )
						LCD_WRITE_DATA(title_1[i]) ;//˵����ǰ�¶�ֵ������
					display_temperature() ; 		//��ʾ�¶�ֵ
				}
				if((current_temp>max_safe_number)||(current_temp<min_safe_number)) //�����ǰ�¶�ֵ���ڰ�ȫ�¶ȷ�Χ�ڣ�����
				{
					Alarm(90);
					Alarm(120);
				}
			}
			break ;
			
		case 1:															//���1����ʾ��߰�ȫ�¶Ȳ���ͨ��������������
			LCD_Init() ;												//lcd��ʼ��
			while((dis_type==1)&&((P1==0xff)||(P1==0xfd)||(P1==0xfb)))
				{	
					safe_temp_set(key_check()) ;						//������ð���״̬
					for( i=0 ; i<14 ; i++ )
						LCD_WRITE_DATA(title_2[i]) ;					//��ʾ��ǰ�¶�ֵ����߰�ȫ�¶�
					display_max() ;	
				}
				break ;
		
		case 2:															//���1����ʾ��Ͱ�ȫ�¶Ȳ���ͨ��������������
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






