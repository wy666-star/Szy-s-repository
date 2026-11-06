#include "stm32f10x.h"
#include "menu.h"
#include "oled.h"
#include "MyButton.h"
//试用一下revert
volatile uint8_t Button_NUM = 0;


void TIM_init(void);

int main(void)
{
	OLED_Init();
	OLED_Clear();
	Peripheral_Init();
	TIM_init();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	uint8_t first_page_flag1 = 1;
	 
	
	while(1)
	{
		first_page_flag1 = First_Page_Select();
		if(first_page_flag1 == 1 )//菜单
		{
		
			Menu_Select();
		}
		else if(first_page_flag1 == 2 )
		{
			
			SettingPage_Select();
	
		}
		
	}
}
void TIM_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
	TIM_TimeBaseStructure.TIM_Period = 199;
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//20ms
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	TIM_ARRPreloadConfig(TIM1,ENABLE);
	
	TIM_Cmd(TIM1,ENABLE);
	
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	NVIC_InitTypeDef nvic_initstruct;
	nvic_initstruct.NVIC_IRQChannel = TIM1_UP_IRQn; /* TIM1 update interrupt on STM32F10x */
	nvic_initstruct.NVIC_IRQChannelPreemptionPriority = 1;
	nvic_initstruct.NVIC_IRQChannelSubPriority = 0;
	nvic_initstruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvic_initstruct);


	
}
void TIM1_UP_IRQHandler(void)
{

	static uint8_t pre_button_state = 0;
	static int8_t cur_button_state =0;
	
	if(TIM_GetFlagStatus(TIM1,TIM_FLAG_Update) == 1)
	{
		
		
		pre_button_state = cur_button_state;
		cur_button_state = Button_GetState();
		if(pre_button_state ==0 && cur_button_state !=0)
		Button_NUM = cur_button_state;
	
		
	
	}

	TIM_ClearFlag(TIM1,TIM_FLAG_Update);
}

