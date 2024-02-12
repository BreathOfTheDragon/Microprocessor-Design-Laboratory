#include "extintr.h"
#include "gpio.h"
#include "lcd.h"
#include "pwm.h"
#include "stdutils.h"
#include "timer.h"
#include "LPC17xx.h"
#include "delay.h"

int cntr = 0;
int flag;
uint32_t motor_speed = 200;

void set_DC_motor(uint32_t speed)
{
		GPIO_PinDirection(P2_2, OUTPUT);
    GPIO_PinWrite(P2_2, HIGH);
		GPIO_PinDirection(P2_11, INPUT);
		GPIO_PinDirection(P2_10, INPUT);
    PWM_SetDutyCycle(PWM_1, speed);
}

void timer(void)
{
		if (cntr % 2 == 1)
		{
			TIMER_Stop(0);
			LCD_SetCursor(0,0);
			double t = 1000000 / TIMER_GetTime(0);
			LCD_Printf("Freq = ");
			LCD_DisplayFloatNumber(t);
			LCD_SetCursor(1,0);
			LCD_Printf("Group 01");
			cntr ++;
		}
		else
		{
			TIMER_Init(0, 1000000);
			TIMER_Start(0);
			cntr ++;
		}
}

void timer1(void)
{
		if (cntr % 2 == 1)
		{
			TIMER_Stop(0);
			LCD_SetCursor(0,0);
			double t = 1000000 / TIMER_GetTime(0);
			t = t*60;
			LCD_Printf("Speed = ");
			LCD_DisplayFloatNumber(t);
			LCD_SetCursor(0,12);
			LCD_Printf(" RPM");
			LCD_SetCursor(1,0);
			LCD_Printf("Targ = 3000 RPM");
			cntr ++;
			if (t < 2900)
			{
					flag = 0;
			}
			else if (t > 3100)
			{
					flag = 1;
			}
			else
			{
					flag = 2;
			}
		}
		else
		{
			TIMER_Init(0, 10000000);
			TIMER_Start(0);
			cntr ++;
		}
}

void set_speed(void)
{
		if (flag == 0)
		{
				motor_speed ++;
		}
		else if (flag == 1)
		{
				motor_speed --;
		}
		set_DC_motor(motor_speed);
}

int main()
{
			SystemInit();
			PWM_Init(255);
			LCD_SetUp(P0_0,P0_1,P0_2,P_NC,P_NC,P_NC,P_NC,P0_4,P0_5,P0_6,P0_7);
			LCD_Init(2,16);
			LCD_Clear();
	
			/*PWM_Start(PWM_1);
			set_DC_motor(127);*/ // Q1

			/*PWM_Stop(PWM_1);
			cntr = 0;
			TIMER_Init(0, 1000000);
			EINT_AttachInterrupt(EINT1, timer, FALLING);*/ // Q2
	
			PWM_Start(PWM_1);
			set_DC_motor(motor_speed);
			cntr = 0;
			TIMER_Init(0, 10000000);
			TIMER_Init(1, 500000);
			EINT_AttachInterrupt(EINT0, timer1, FALLING);
			TIMER_Start(1);
			TIMER_AttachInterrupt(1, set_speed); // Q4


			while(1){
			}
}