#include "gpio.h"
#include "LPC17xx.h"
#include "uart.h"
#include "timer.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"
#include "ks0108.h"
#include "delay.h"
#include "stdlib.h"
#include "string.h"


char player_line = 0;
char line1[16];
char line2[16];

TaskHandle_t screen_handler;
TaskHandle_t movement_handler;

void movement(void *pvParameters){
	char data;
	while(1){
		data = UART_RxChar(2);
		if(data == 'w' && player_line == 1){
			player_line = 0;
		}
		else if (data == 's' && player_line == 0)
		{
			player_line = 1;
		}
		vTaskDelay(100/portTICK_PERIOD_MS);
	}
}

void screen(void *pvParameters){
	int cntr = 0;
	int point = 0;
	while(1){

		int random_num1 = rand();
		for (int i = 0; i < 15; i++)
		{
			line1[i] = line1[i+1];
			line2[i] = line2[i+1];
		}
		
		if (cntr % 3 == 0)
		{
			if (rand() % 2 == 0)
			{
				line1[15] = '#';
				line2[15] = ' ';
			}
			else
			{
				line1[15] = ' ';
				line2[15] = '#';
			}
		}
		else
		{
			line1[15] = ' ';
			line2[15] = ' ';
		}
		if (player_line == 0)
		{
			if (line1[0] == '#')
				line1[0] = 'X';
			else
				line1[0] = '$';
		}
		else if (player_line == 1)
		{
			if (line2[0] == '#')
				line2[0] = 'X';
			else
				line2[0] = '$';
		}
		LCD_Clear();
		LCD_SetCursor(0, 0);
		LCD_Printf(line1);
		LCD_SetCursor(1, 0);
		LCD_Printf(line2);
		if ((line1[0] == '#' && line2[0] == '$') || (line2[0] == '#' && line1[0] == '$'))
		{
			point ++;
		}
		if ((line1[0] == 'X') || (line2[0] == 'X'))
		{
			GLCD_GoTo(30, 5);
			GLCD_WriteString("Game Over!");
			vTaskDelete(screen_handler);
			vTaskDelete(movement_handler);
		}
		GLCD_ClearScreen();
		GLCD_GoTo(20, 1);
		GLCD_WriteString("Your Score:");
		GLCD_GoTo(86, 3);
		GLCD_WriteChar((char) point/100 + 48);
		GLCD_GoTo(92, 3);
		GLCD_WriteChar((char)(point % 100) / 10 + 48);
		GLCD_GoTo(98, 3);
		GLCD_WriteChar((char)(point % 100) % 10 + 48);
		cntr ++;
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}

int main(){
	SystemInit();
	LCD_SetUp(P0_0,P0_1,P0_2,P_NC,P_NC,P_NC,P_NC,P0_4,P0_5,P0_6,P0_7);
	LCD_Init(2,16);
	LCD_Clear();
	LCD_SetCursor(0, 0);
	GLCD_Initialize();
  GLCD_ClearScreen();
	for (int i = 0; i < 16; i ++)
	{
		line1[i] = ' ';
		line2[i] = ' ';
	}
		
	LCD_Printf("Faraar Az Mane");
	LCD_SetCursor(1,3);
	LCD_Printf("By: Group 1");
	DELAY_ms(10000);
	LCD_Clear();
	LCD_SetCursor(0, 0);
	LCD_Printf("Press Enter");
	LCD_SetCursor(1, 7);
	LCD_Printf("to Start");
	UART2_Init(9600);
	char data = UART_RxChar(2);
	if (data == '\n')
	{
		xTaskCreate(screen, "screen", 128, NULL, 1, &screen_handler);
		xTaskCreate(movement, "movement", 128, NULL, 1, &movement_handler);
		vTaskStartScheduler();
	}
	while(1); 
}
