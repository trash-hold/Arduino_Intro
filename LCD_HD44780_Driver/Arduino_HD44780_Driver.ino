/*
======================================================
Example of the library use. 
LCD cleared, cursor on, blinking on after init  
======================================================
*/

#include "LCD_driver.h"

//Personalize according to your pins
#define RS 3
#define ENABLE 8
#define D4 4
#define D5 5
#define D6 6
#define D7 7

//LCD characteristics
#define WORD_LENGTH 16
#define LCD_ROWS 2

void SerialTransmission(int baud);

void setup()
{	
	//For quick startup of serial monitor :)
	//SerialTransmission(9600);			

	LCD own_lcd(RS, ENABLE, D4, D5, D6, D7);
	delay(1000);
	own_lcd.SendString("Hello world!");
}

void loop()
{
	delay(1000);
}

void SerialTransmission(int baud)
{
	Serial.begin(baud);
	delay(1000);			//To make sure serial is ready to tx/rx 
	Serial.print("Start of transmission");
}