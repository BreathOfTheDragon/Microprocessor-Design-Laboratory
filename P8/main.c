#include "LPC17xx.h"
#include "gpio.h"
#include "delay.h"
#include "timer.h"
#include "adc.h"
#include "uart.h"
#include "stdio.h"
#include "fat32.h"
#include "ks0108.h" 
#include "pwm.h"
#include "extintr.h"
#include "spi.h"
#include "sdcard.h"



int main()
{	
	SystemInit();
	
	UART2_Init(9600);
	
	GLCD_Initialize();
	GLCD_ClearScreen();
	
	//////////  Q1  //////////
	GLCD_GoTo(0,0);
	GLCD_WriteString("Milad Asgari");
	GLCD_GoTo(0,1);
	GLCD_WriteString("Mehdi Arjmand");
	GLCD_GoTo(0,2);
	GLCD_WriteString("Amir Radmehr");
	
	//////////  Q2   //////////
	uint8_t x=0, y=0;
	GLCD_GoTo(x,y);

	while(TRUE){
		char character;
		character = UART2_RxChar();
		if(x == 20){
			x = 0;
			y++;
			GLCD_GoTo(x,y);
		}
		if(character == '\n'){
			x = 0;
			y++;
			GLCD_GoTo(0,y);
			continue;
		}
		GLCD_WriteChar(character);
		x++;
	}

	//////////  Q3   //////////
	GLCD_Rectangle(2,2,126,62);
	GLCD_Line(30,55,100,55);
	GLCD_Line(30,55,45,35);
	GLCD_Line(45,35,60,45);
	GLCD_Line(60,45,80,20);
	GLCD_Line(80,20,100,55);
	GLCD_Circle(55,20,10);
	
	//////////  Q4   //////////
	char character;
	fileConfig_st *source_pointer;
	uint8_t status,sdcardType;
	
	status = SD_Init(&sdcardType);
	source_pointer = FILE_Open("mpl.img",READ,&status);		
	uint8_t x=0,byte_=0;
			
	while(TRUE){
		character = FILE_GetCh(source_pointer);

		for(uint8_t bit_=0;bit_<=7;bit_++){
			if(!(character&(1<<bit_)))
				GLCD_SetPixel(byte_*8+bit_,x,255);
		}
		byte_++;
		
		if(byte_ == 16){
			x++;
			byte_ = 0;
		}
	}
	FILE_Close(source_pointer);

}

