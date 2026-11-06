#include "stm32f10x.h"
#include "MyRTC.h"
#include "OLED.h"
#include "MyButton.h"
#include "delay.h"
#include "setTime.h"
#include "SecTimer.h"
#include "Light.h"
#include "mpu6050.h"
#include "math.h"

extern uint16_t cnt;
extern uint16_t start;


//实现计时
void TIM2_Init(void)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_TimeBaseInitTypeDef tim_timebaseinitstruct ={};
	tim_timebaseinitstruct.TIM_ClockDivision = 0;
	tim_timebaseinitstruct.TIM_CounterMode = TIM_CounterMode_Up;
	tim_timebaseinitstruct.TIM_Prescaler = 35999;//1ms
	tim_timebaseinitstruct.TIM_Period = 999;//1s

	TIM_TimeBaseInit(TIM2,&tim_timebaseinitstruct);
		
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);	
	
	TIM_Cmd(TIM2,ENABLE);

		
	NVIC_InitTypeDef nv;
	nv.NVIC_IRQChannel=TIM2_IRQn;
	nv.NVIC_IRQChannelCmd =ENABLE;
	nv.NVIC_IRQChannelPreemptionPriority=0;
	nv.NVIC_IRQChannelSubPriority =0;
	NVIC_Init(&nv);
		
	
}
void TIM2_IRQHandler()
{

	if(TIM_GetFlagStatus(TIM2,TIM_IT_Update) ==SET)
	{
	
		cnt++;
	

	}
	
	TIM_ClearFlag(TIM2,TIM_IT_Update);

}
// @简介：初始化外设
void Button_Init(void);
void Peripheral_Init(void)
{
    MyRTC_Init();
    //MyRTC_SetTime();
    Button_Init();
    TIM2_Init();
    MPU6050_Init();
}
//-------------首页时钟-------------------
void Show_Clock_UI(void)
{

    OLED_Clear();
    
    MyRTC_ReadTime();
    OLED_Printf(0, 0, OLED_6X8, "%04d-%02d-%02d", MyRTC_Time[0], MyRTC_Time[1], MyRTC_Time[2]);

    OLED_Printf(16, 16, OLED_12X24, "%02d-%02d-%02d", MyRTC_Time[3], MyRTC_Time[4], MyRTC_Time[5]);

    
	OLED_ShowString(0,48,"哈基米菜单",OLED_8X16);
    OLED_ShowString(96,48,"设置",OLED_8X16);
}

//-------------首页按键选择-------------------
// @简介：在while循环中处理首页按键选择的动作，采取环形队列的方式切换按键位置,并且更新显示，只有按下确认键函数才退出
// @返回值：first_page_flag
uint8_t first_page_flag =1;//按键位置标志位，1表示菜单键，2表示设置键
uint8_t First_Page_Select()
{
    
   while(1)
   {	   
    uint8_t btn = Button_GetNum();
    if(btn == 3)//确认键
    {
        OLED_Clear();
        
        OLED_Update();
        return first_page_flag;
    }
    else if(btn == 1)//上一项   
    {
        first_page_flag--;
        if(first_page_flag < 1)
        {
            first_page_flag = 2;
        }
    }
    else if(btn == 2)//下一项   
    {
        first_page_flag++;
        if(first_page_flag > 2)
        {
            first_page_flag = 1;
        }
    }


    switch(first_page_flag)
    {
        case 1:
            Show_Clock_UI();
            OLED_ReverseArea(0,48,80,16);
            OLED_Update();
            break;
        case 2:
            Show_Clock_UI();
            OLED_ReverseArea(96,48,32,16);
            OLED_Update();
            break;
    }
    
	}
   //return first_page_flag;
}




//--------------------设置时间界面--------------------
//----------------------------------------------------
void Show_SettingPage_UI(void)
{
	

	OLED_ShowImage(0,0,16,16,Enter);
	OLED_ShowString(0,16,"日期时间设置",OLED_8X16);
	
	



}
//-------------设置界面按键选择---------------------
//--------------------------------------------------
uint8_t setting_page_flag = 1;//1是回车，2是设置时间
uint8_t SettingPage_Select(void)
{
	OLED_Clear();
	OLED_Update();
    
	while(1)
		
	{	    
    uint8_t btn = Button_GetNum();
	 if(btn == 3)//确认键
    {
        OLED_Clear();
        OLED_Update();
		if(setting_page_flag ==2)
		{
			
			SetTime_Select();
			SettingPage_Select();
			
			
		}
		if(setting_page_flag == 1)
        return setting_page_flag;
    }
    else if(btn == 1)//上一项       
    {
        setting_page_flag--;
        if(setting_page_flag < 1)
        {
            setting_page_flag = 2;
        }
    }
    else if(btn == 2)//下一项   
    {
        setting_page_flag++;
        if(setting_page_flag > 2)
        {
            setting_page_flag = 1;
        }
    }


    switch(setting_page_flag)
    {
        case 1:
            Show_SettingPage_UI();
            OLED_ReverseArea(0,0,16,16);
            OLED_Update();
            break;
        case 2:
            Show_SettingPage_UI();
            OLED_ReverseArea(0,16,96,16);
            OLED_Update();
            break;
    }
	
	}
   
	


}
//----------------滑动菜单界面------------------
//-----------------------------------------------
uint8_t cur_selection;//范围1~7
uint8_t target_selection;
uint8_t cur_x = 48;//当前选项的横坐标
uint8_t speed = 4;//滑动速度
uint8_t move_flag = 1; //是否移动中


uint8_t menu_flag = 1;//有7个功能 标志位是1~7，图像数组0~6

//实现菜单界面的完全滑动，并且可以改动move_flag
void Menu_slide(void)
{


	
	
while(1){
	
	
	
	//图标向左
	if(cur_selection < target_selection)
	{
	
			
			move_flag =1;
			
			
			cur_x-=speed;
				
				
			if(cur_x == 0)
			{
					
				
				cur_selection++;
				cur_x = 48;
					
			
			
			}
			
			
		
	}
	//图标向右
	else if(cur_selection > target_selection)
	{
	
			
			move_flag =1;
			
			cur_x+=speed;
				
				
			if(cur_x == 96)
			{
					
				
				cur_selection--;
				cur_x = 48;
					
			
			
			}
			
			
		
	}
	
	
	
	

		
	//oled显示部分
	OLED_Clear();
	OLED_ShowImage(42,10,44,44,Frame);
	//当前选项不是第一项才显示前一项
	if(cur_selection>=2)
	{
		
			OLED_ShowImage(cur_x-48,16,32,32,Menu_Graph[cur_selection-2]);
			
		
	}
		
			
	OLED_ShowImage(cur_x,16,32,32,Menu_Graph[cur_selection-1]);
	
	//当前选项不是最后项才显示后一项
	if(cur_selection<=6)
	{
		
			OLED_ShowImage(cur_x+48,16,32,32,Menu_Graph[cur_selection]);
			
		
	}
		
	
	OLED_Update();
	
	
	
	//图标不动,退出循环
	if(cur_selection == target_selection)
	{
		move_flag =0;
		break;
	
	}
}
	
}
//设置cur_selection和target_selcetion，并实现移动
void Set_Selection(uint8_t enable,uint8_t cur_selection1,uint8_t target_selection1)
{
	if(enable ==1)
	{
		cur_selection=cur_selection1;
		target_selection=target_selection1;
		Menu_slide();
	
	}

}





int MPU6050(void);
//实现菜单的按键选择

uint8_t Menu_Select(void)
{
	OLED_Clear();
	Set_Selection(1,1,1);
    
	while(1)
		
{	    
    
	uint8_t btn = Button_GetNum();	
	
	 if(btn == 3)//确认键
    {
        OLED_Clear();
        OLED_Update();
		
		if(menu_flag == 1)
			
			return menu_flag;
		
		
		else if(menu_flag == 2)
		{	
			SecTimer_Select();
			
		}
		else if(menu_flag == 3)
		{			
			Light();			
		}
		else if(menu_flag == 4)
		{
			MPU6050();		
		}
		else if(menu_flag == 5)
		{
			
		
			
		}
		else if(menu_flag == 6)
		{
			
			
			
			
		}
		else if(menu_flag == 7)
		{
			
			
			
			
		}
		
		
    }
   







	else if(btn == 1)//上一项       
    {
		
        menu_flag--;
        
		if(menu_flag < 1)
        {
            menu_flag =7;
        }
		
    }
    else if(btn == 2)//下一项   
    {
        
		menu_flag++;
        
		if(menu_flag > 7)
        {
            menu_flag = 1;
        }
		
    }


    switch(menu_flag)
    {
		

        case 1:
			if(btn ==1)//图像右滑
				Set_Selection(1,2,1);
			else if(btn ==2)
				Set_Selection(1,7,1);
            
				
            break;
        case 2:
			if(btn ==1)//图像右滑
				Set_Selection(1,3,2);
			else if(btn ==2)
				Set_Selection(1,1,2);
            else 
				Set_Selection(1,2,2);
            break;
		case 3:
			if(btn ==1)//图像右滑
				Set_Selection(1,4,3);
			else if(btn ==2)
				Set_Selection(1,2,3);
            else 
				Set_Selection(1,3,3);
            break;
		case 4:
			if(btn ==1)//图像右滑
				Set_Selection(1,5,4);
			else if(btn ==2)
				Set_Selection(1,3,4);
			else 
				Set_Selection(1,4,4);
            
            break;
		case 5:
			if(btn ==1)//图像右滑
				Set_Selection(1,6,5);
			else if(btn ==2)
				Set_Selection(1,4,5);
            
            break;
		case 6:
			if(btn ==1)//图像右滑
				Set_Selection(1,7,6);
			else if(btn ==2)
				Set_Selection(1,5,6);
            
            break;
		case 7:
			if(btn ==1)//图像右滑
				Set_Selection(1,1,7);
			else if(btn ==2)
				Set_Selection(1,6,7);
            
            break;
    }
	
}
   

}
//-------------------mpu6050-------------------没有单独写一个文件
//陀螺仪测出角度，但是由于设备原因，测出的是角速度，不断积分，会漂移
//加速度计，只能测出row和pitch（yaw根据原理无法测量），静态稳定时测量才准确，内部抖动有噪声
//然后互补滤波，
int16_t ax,ay,az,gx,gy,gz;
float roll_g,pitch_g,yaw_g;
float roll_a,pitch_a,yaw_a;
float Roll,Pitch,Yaw;
float a=0.9;
float Delta_t = 0.005;
double pi=3.1415927;
void MPU6050_Calculation(void)
{
	Delay(5);
	MPU6050_GetData(&ax,&ay,&az,&gx,&gy,&gz);
	
	roll_g = Roll+gx*Delta_t;
	pitch_g =Pitch+gy*Delta_t;
	yaw_g = Yaw+gz*Delta_t;
	
	
	pitch_a = atan2((-1)*ax,az)*180/pi;
	roll_a=atan2(ay,az)*180/pi;
	
	Roll =a*roll_g+(1-a)*roll_a;
	Pitch = a*pitch_g+(1-a)*roll_a;
	Yaw =a*yaw_g;


	
}
void Show_MPU6050_UI(void)
{
	OLED_Clear();
	OLED_ShowImage(0,0,16,16,Enter);
	OLED_Printf(0,16,OLED_8X16,"Roll:%.2f",Roll);
	OLED_Printf(0,32,OLED_8X16,"Pitch:%.2f",Pitch);
	OLED_Printf(0,48,OLED_8X16,"Yaw:%.2f",Yaw);


}
int MPU6050(void)
{

	while(1)
	{
		uint8_t but =Button_GetNum();
		
		if(but ==3 )
		{
			return 0;
		}
		OLED_Clear();
		MPU6050_Calculation();
		Show_MPU6050_UI();
		OLED_ReverseArea(0,0,16,16);
		OLED_Update();
		
	}

}
