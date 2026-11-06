#ifndef __MENU_H__
#define __MENU_H__
#include "stm32f10x.h"
#include "MyRTC.h"
void Peripheral_Init(void);
void Show_Clock_UI(void);
uint8_t First_Page_Select(void);
uint8_t Menu_Select(void);
uint8_t SettingPage_Select(void);
void TIM2_IRQHandler(void);
int MPU6050(void);
#endif
