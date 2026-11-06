#include "stm32f10x.h"

//按键初始化 k1->pb1,k2->pb0,k3->pa5
// @简介：初始化按键，static函数只能在本文件中使用
void Button_Init(void)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStructure);



}
// @简介：获取按键状态，在tim1中断里调用。
// @返回值：0 - 无按键按下  1 - 上一项  2 - 下一项  3 - 确认键
uint8_t Button_GetState(void)
{
    uint8_t num = 0;
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_RESET)//上一项
    {
       
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == Bit_RESET)
        {
            num = 1;
        }
    }
    else if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == Bit_RESET)//下一项
    {
        
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == Bit_RESET)
        {
            num = 2;
        }
    }
    else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET)//确认
    {
        
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == Bit_RESET)
        {
            num = 3;
        }
    }




    return num;
}
//@简介:得到按键按下的编号
//
extern uint8_t Button_NUM;
 uint8_t Button_GetNum(void)
 {
 
	
	 uint8_t temp_button_num = Button_NUM;
	 Button_NUM = 0;
    return  temp_button_num;

 }
 

 