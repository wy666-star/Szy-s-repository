#include "stm32f10x.h"
#include "MyButton.h"
#include "OLED.h"
uint8_t light_flag=0;
void Light()
{

	while(1)
	{
		uint8_t but =Button_GetNum();
		if(but==3)
		{
			light_flag++;
			light_flag%=2;
		}
		else if(but==1||but==2)
			return;
	
		switch(light_flag)
		{
		
			case 1:
				OLED_ShowString(0,0,"นþป๙รืฃก",OLED_8X16);
				OLED_Update();
				break;
			case 0:
				OLED_Clear();
				OLED_Update();
				break;
		
		
		
		
		}
	
	
	}




}
