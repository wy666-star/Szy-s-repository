#include "stm32f10x.h"
#include "MyRTC.h"
#include "OLED.h"
#include "MyButton.h"
//
//
void Show_SetTime_FirstUI(void)
{

	OLED_Clear();
	OLED_ShowImage(0,0,16,16,Enter);
	OLED_Printf(0,16,OLED_8X16,"年:%4d",MyRTC_Time[0]);
	OLED_Printf(0,32,OLED_8X16,"月:%2d",MyRTC_Time[1]);
	OLED_Printf(0,48,OLED_8X16,"日:%2d",MyRTC_Time[2]);



}
//
//
void Show_SetTime_SecondUI(void)
{

	OLED_Clear();
	OLED_ShowImage(0,0,16,16,Enter);
	OLED_Printf(0,16,OLED_8X16,"时:%2d",MyRTC_Time[3]);
	OLED_Printf(0,32,OLED_8X16,"分:%2d",MyRTC_Time[4]);
	OLED_Printf(0,48,OLED_8X16,"秒:%2d",MyRTC_Time[5]);



}
uint8_t Change_RTC_Time(uint8_t i,uint8_t but)
{

	if(but == 1)
	
		MyRTC_Time[i]--;
		
		
	else if(but == 2)
	
		MyRTC_Time[i]++;
	

	MyRTC_SetTime();

	return 0;
}
uint8_t SetYear(void)
{
	while(1)
	{
		uint8_t but = Button_GetNum();
		if(but == 1)
		{
			Change_RTC_Time(0,1);
		}
		else if(but == 2)
		{
			Change_RTC_Time(0,2);
		}
		else if(but == 3)
		{
			return 0;
		}
			
		Show_SetTime_FirstUI();
	OLED_ReverseArea(24,16,32,16);
	OLED_Update();
	}
	
}
uint8_t SetMonth(void)
{
	while(1)
	{
		uint8_t but = Button_GetNum();
		if(but == 1)
		{
			Change_RTC_Time(1,1);
			if(MyRTC_Time[1]<1)
			{
				MyRTC_Time[1] = 12;
				MyRTC_SetTime();
			}	
			
		}
		else if(but == 2)
		{
			Change_RTC_Time(1,2);
			if(MyRTC_Time[1]>12)
				
			{
				MyRTC_Time[1] = 1;
				MyRTC_SetTime();
			}	
		}
		else if(but == 3)
		{
			return 0;
		}
	Show_SetTime_FirstUI();
	OLED_ReverseArea(24,32,16,16);
	OLED_Update();
			
	}
	
}
uint8_t SetDay(void)
{
	while(1)
	{
		uint8_t but = Button_GetNum();
		if(but == 1)
		{
			Change_RTC_Time(2,1);
			if(MyRTC_Time[2]<1)
			{
				MyRTC_Time[2] = 31;
				MyRTC_SetTime();
			}	
		}
		else if(but == 2)
		{
			Change_RTC_Time(2,2);
			if(MyRTC_Time[2]>31)
			{
				MyRTC_Time[2] = 1;
				MyRTC_SetTime();
			}	
		}
		else if(but == 3)
		{
			return 0;
		}
			
	Show_SetTime_FirstUI();
	OLED_ReverseArea(24,48,16,16);
	OLED_Update();
	}
	
}
uint8_t SetHour(void)
{
	while(1)
	{
		uint8_t but = Button_GetNum();
		if(but == 1)
		{
			Change_RTC_Time(3,1);
			if(MyRTC_Time[3]<1)
			{
				MyRTC_Time[3] = 23;
				MyRTC_SetTime();
			}	
		}
		else if(but == 2)
		{
			Change_RTC_Time(3,2);
			if(MyRTC_Time[3]>23)
			{
				MyRTC_Time[3] = 0;
				MyRTC_SetTime();
			}	
		}
		else if(but == 3)
		{
			return 0;
		}
		
	Show_SetTime_SecondUI();
	OLED_ReverseArea(24,16,16,16);
	OLED_Update();
			
	}
	
}
uint8_t SetMin(void)
{
	while(1)
	{
		uint8_t but = Button_GetNum();
		if(but == 1)
		{
			Change_RTC_Time(4,1);
			if(MyRTC_Time[4]<0)
			{
				MyRTC_Time[4] = 59;
				MyRTC_SetTime();
			}	
		}
		else if(but == 2)
		{
			Change_RTC_Time(4,2);
			if(MyRTC_Time[4]>59)
			{
				MyRTC_Time[4] = 0;
				MyRTC_SetTime();
			}	
		}
		else if(but == 3)
		{
			return 0;
		}
		
	Show_SetTime_SecondUI();
	OLED_ReverseArea(24,32,16,16);
	OLED_Update();
			
	}
	
}
uint8_t SetSec(void)
{
	while(1)
	{
		uint8_t but = Button_GetNum();
		if(but == 1)
		{
			Change_RTC_Time(5,1);
			if(MyRTC_Time[5]<0)
			{
				MyRTC_Time[5] = 59;
				MyRTC_SetTime();
			}	
		}
		else if(but == 2)
		{
			Change_RTC_Time(5,2);
			if(MyRTC_Time[5]>59)
			{
				MyRTC_Time[5] = 0;
				MyRTC_SetTime();
			}	
		}
		else if(but == 3)
		{
			return 0;
		}
		Show_SetTime_SecondUI();
	OLED_ReverseArea(24,48,16,16);
	OLED_Update();
			
	}
	

}
//-------------设置时间界面按键选择-------------------
//
uint8_t SetTime_flag = 1;//1是回车，2是年，3是月，4是日.。。。。。
uint8_t SetTime_Select(void)
{

    
	while(1)
		
	{	    
   
		uint8_t btn = Button_GetNum();
	
		if(btn == 3)//确认键
    {
        OLED_Clear();
        OLED_Update();
		
		if(SetTime_flag == 1)
		{
			return SetTime_flag;
		}
		if(SetTime_flag == 2)
		{
			SetYear();
			
		}
		if(SetTime_flag == 3)
		{
			SetMonth();
		}
		if(SetTime_flag == 4)
		{
			SetDay();
		}
		if(SetTime_flag == 5)
		{
			return SetTime_flag;
		}
		if(SetTime_flag == 6)
		{
			SetHour();
		}
		if(SetTime_flag == 7)
		{
			SetMin();
		}
		if(SetTime_flag == 8)
		{
			SetSec();
		}
		
        
    }
    else if(btn == 1)//上一项       
   {
        SetTime_flag--;
        if(SetTime_flag < 1)
        {
            SetTime_flag = 8;
		}
        
    }
    else if(btn == 2)//下一项   
    {
        SetTime_flag++;
        if(SetTime_flag > 8)
        {
            SetTime_flag = 1;
        }
    }


    switch(SetTime_flag)
    {
        case 1:
            Show_SetTime_FirstUI();
            OLED_ReverseArea(0,0,16,16);
            OLED_Update();
            break;
        case 2:
            Show_SetTime_FirstUI();
            OLED_ReverseArea(0,16,16,16);
            OLED_Update();
            break;
		case 3:
            Show_SetTime_FirstUI();
            OLED_ReverseArea(0,32,16,16);
            OLED_Update();
            break;
		case 4:
            Show_SetTime_FirstUI();
            OLED_ReverseArea(0,48,16,16);
            OLED_Update();
            break;
		case 5:
            Show_SetTime_SecondUI();
            OLED_ReverseArea(0,0,16,16);
            OLED_Update();
            break;
		case 6:
            Show_SetTime_SecondUI();
            OLED_ReverseArea(0,16,16,16);
            OLED_Update();
            break;
		case 7:
            Show_SetTime_SecondUI();
            OLED_ReverseArea(0,32,16,16);
            OLED_Update();
            break;
		case 8:
            Show_SetTime_SecondUI();
            OLED_ReverseArea(0,48,16,16);
            OLED_Update();
            break;
    }
	
	}
   
}

