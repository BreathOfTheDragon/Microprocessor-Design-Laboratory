#include "LPC17xx.h"
#include "gpio.h"
#include "delay.h"
#include "timer.h"
#include "adc.h"
#include "uart.h"
#include "stdio.h"
#include "pwm.h"
#include "extintr.h"
#include "sdcard.h"
#include "fat32.h"
#include "spi.h"
#include "ks0108.h" 
#include <FreeRTOS.h>
#include <task.h>
#include "lcd.h"



uint32_t FirstCounter,SecondCounter;

TaskHandle_t lcd_update_handel;

struct LCD_data {
   char  character;
   uint8_t  x_pos;
   uint8_t  y_pos;
}; 


void init_lcd(){
	LCD_SetUp(P0_0,P0_1,P0_2,P_NC,P_NC,P_NC,P_NC,P0_4,P0_5,P0_6,P0_7);
	LCD_Init(2,16);
}


void setup(void){
	SystemInit();
	GPIO_PinDirection(P1_0,OUTPUT);
	GPIO_PinDirection(P1_1,OUTPUT);
	GPIO_PinDirection(P1_4,OUTPUT);
	GPIO_PinDirection(P1_8,OUTPUT);

	
	
	GPIO_PinDirection(P0_0,OUTPUT);	
	GPIO_PinDirection(P0_1,OUTPUT);	
	GPIO_PinDirection(P0_2,OUTPUT);
	GPIO_PinDirection(P0_3,OUTPUT);
	GPIO_PinDirection(P0_4,OUTPUT);
	GPIO_PinDirection(P0_5,OUTPUT);
	GPIO_PinDirection(P0_6,OUTPUT);
	GPIO_PinDirection(P0_7,OUTPUT);
	
}

void LED1(void *pvParameters){
	while(1){
		GPIO_PinToggle(P1_0);
		vTaskDelay(1000/portTICK_PERIOD_MS);
	}
}
void LED2(void *pvParameters){
	while(1){
		GPIO_PinToggle(P1_1);
		vTaskDelay(500/portTICK_PERIOD_MS);
	}
}
void LED3(void *pvParameters){
	while(1){
		GPIO_PinToggle(P1_4);
		vTaskDelay(200/portTICK_PERIOD_MS);
	}
}
void LED4(void *pvParameters){
	while(1){
		GPIO_PinToggle(P1_8);
		vTaskDelay(100/portTICK_PERIOD_MS);
	}
}



void first_counter(void *pvParameters){
	while(1){
		FirstCounter++;
		vTaskDelay(100/portTICK_PERIOD_MS);
	}
}

void second_counter(void *pvParameters){
	while(1){
		SecondCounter++;
		vTaskDelay(160/portTICK_PERIOD_MS);
	}
}


void update_lcd_q2(void *pvParameters){
	while(1){
		LCD_Clear();
		LCD_GoToLine(0);
		LCD_Printf("First : %d",FirstCounter);
		LCD_GoToLine(1);
		LCD_Printf("Second : %d",SecondCounter);
		vTaskDelay(20/portTICK_PERIOD_MS);
	}
}

void update_lcd_q3(void *pvParameters){
	struct LCD_data _lcd_data;
	_lcd_data = *((struct LCD_data *) pvParameters);
	LCD_SetCursor(_lcd_data.y_pos,_lcd_data.x_pos);
	LCD_Printf("%c",_lcd_data.character);
	vTaskDelete(lcd_update_handel);
}

void rec_uart(void *pvParameters){
	uint8_t cur_x = 0,cur_y = 0;
	while(1){
		char data = UART2_RxChar();

		struct LCD_data lcd_data;
		lcd_data.character = data;
		lcd_data.x_pos = cur_x;
		lcd_data.y_pos = cur_y;
		xTaskCreate(update_lcd_q3, "DICK", ARM_MPU_REGION_SIZE_128MB,( void * ) &lcd_data, 2, &lcd_update_handel);
		
		cur_x++;
		if(cur_x == 16 ){
			cur_x = 0;
			cur_y++;
			if(cur_y == 2)
				cur_y = 0; 
		}
		
	}
}




int main()
{	
	setup();
	//q1
//	xTaskCreate(LED1, "DICK", 128,NULL, 10, NULL);
//	xTaskCreate(LED2, "DICK", 128,NULL, 9, NULL);
//	xTaskCreate(LED3, "DICK", 128,NULL, 8, NULL);
//	xTaskCreate(LED4, "DICK", 128,NULL, 7, NULL);
	
	//q2
//	xTaskCreate(first_counter, "DICK", 128,NULL, 9, NULL);
//	xTaskCreate(second_counter, "DICK", 128,NULL, 8, NULL);
//	xTaskCreate(update_lcd_q2, "DICK", 128,NULL, 7, NULL);
	
	//q3
	xTaskCreate(rec_uart, "DICK", 128,NULL, 1, NULL);
	UART2_Init(9600);
	
	
	init_lcd();
	LCD_Clear();
	
	
	vTaskStartScheduler();
}

