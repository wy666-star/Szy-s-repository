#include "stm32f10x.h"
#include "MyRTC.h"
#include "OLED.h"
#include "MyButton.h"
#include "delay.h"
volatile uint32_t cnt=0;
volatile uint32_t start=0;
uint8_t flag =0;
uint8_t preserve=0;
//显示走动时间
void Show_SecTimer_UI(void)
{

	
	OLED_Clear();
	
	if(flag ==1)
	{
		
		uint32_t realTime =cnt-start;
		uint32_t sec =realTime%60;
		uint32_t min =realTime/60%60;
		uint32_t hour =realTime/3600;
		OLED_Printf(16, 16, OLED_12X24, "%02d-%02d-%02d", hour, min, sec);
		OLED_ShowImage(0,0,16,16,Enter);
		OLED_ShowString(0,48,"开始",OLED_8X16);
		OLED_ShowString(48,48,"结束",OLED_8X16);
		OLED_ShowString(96,48,"清除",OLED_8X16);
	}
	
	else if(flag == 0)
	{		
		
			OLED_Printf(16, 16, OLED_12X24, "00-00-00");
			OLED_ShowImage(0,0,16,16,Enter);
			OLED_ShowString(0,48,"开始",OLED_8X16);
			OLED_ShowString(48,48,"结束",OLED_8X16);
			OLED_ShowString(96,48,"清除",OLED_8X16);
	}
	else if(flag == 2)
	{
	
			uint32_t preserveTime = preserve-start;
			uint32_t sec =preserveTime%60;
			uint32_t min =preserveTime/60%60;
			uint32_t hour =preserveTime/3600;
			OLED_Printf(16, 16, OLED_12X24, "%02d-%02d-%02d", hour, min, sec);
			OLED_ShowImage(0,0,16,16,Enter);
			OLED_ShowString(0,48,"开始",OLED_8X16);
			OLED_ShowString(48,48,"结束",OLED_8X16);
			OLED_ShowString(96,48,"清除",OLED_8X16);
	
	
	
	
	
	}



}

//实现秒表界面按键选择
uint8_t secTime_flag=1;//1~4

uint8_t SecTimer_Select()
{

	OLED_Clear();

    
	while(1)
		
{	    
    uint8_t btn = Button_GetNum();
	 
	
	
	if(btn == 3)//确认键
    {
        
		if(secTime_flag == 1)
			return secTime_flag;
		else if(secTime_flag ==2)
		{
			start = cnt;
			flag =1;
			
		}
		else if(secTime_flag ==3)
		{
			preserve =cnt;
			flag=2;
		}
		else if(secTime_flag ==4)
		{
			flag=0;
		}
		
    }
   


	else if(btn == 1)//上一项       
    {
        secTime_flag--;
        if(secTime_flag < 1)
        {
            secTime_flag = 4;
        }
    }
    
	
	
	else if(btn == 2)//下一项   
    {
        secTime_flag++;
        if(secTime_flag > 4)
        {
            secTime_flag = 1;
        }
    }


    switch(secTime_flag)
    {
        case 1:
            Show_SecTimer_UI();
            OLED_ReverseArea(0,0,16,16);
            OLED_Update();
            break;
        case 2:
            Show_SecTimer_UI();
            OLED_ReverseArea(0,48,32,16);
            OLED_Update();
		
            break;
		case 3:
            Show_SecTimer_UI();
            OLED_ReverseArea(48,48,32,16);
            OLED_Update();
            break;
		case 4:
            Show_SecTimer_UI();
            OLED_ReverseArea(96,48,32,16);
            OLED_Update();
            break;
    }
	
	}
   
	
}


