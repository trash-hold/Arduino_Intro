#include "LCD_driver.h"
#include <stdio.h> 
#include <stdlib.h>

//Pin definitions
//==========================================================================================
//LCD pins
#define RS 3
#define ENABLE 8
#define D4 4
#define D5 5
#define D6 6
#define D7 7

//Encoder pins
#define PINA 2			//Encoder interrupt pin         
#define PINB 13



//Constants
//==========================================================================================
//LCD constants
#define WORD_LENGTH 16
#define LCD_ROWS 2

//Encoder constants
#define INCREMENT 1
#define DECREMENT 1
#define MAX_VALUE 360
#define MAX_VALUE_LEN 8		//3 digits + sign



//Public objects/variables
//==========================================================================================
enum transmit{ Lcd, SM, BOTH, NONE };		//used to manage displaying functions better
transmit Transmit_Mode;

LCD* own_lcd;
int count;
uint8_t change;



//Function prototypes
//==========================================================================================
void encoderSetup();
void LCDSetup();
void SerialSetup(unsigned int baud = 9600);

void encIntrr();

void countInfo(int transmit_mode);


//Program start
//==========================================================================================
void setup()
{
	Transmit_Mode = transmit::SM;
	
	SerialSetup();
	//LCDSetup();
	encoderSetup();
}


void loop()
{
	//To not overload the interrupt we use signaling variable change to trace changes of value
	if (change)
	{
		delay(1);
		change = 0x00;
		countInfo(Transmit_Mode);
	}
}


//==========================================================================================

void encoderSetup()
{
	pinMode(PINA, INPUT_PULLUP);
	pinMode(PINB, INPUT_PULLUP);

	attachInterrupt(digitalPinToInterrupt(PINA), encIntrr, FALLING);
}

void LCDSetup()
{
	LCD lcd(RS, ENABLE, D4, D5, D6, D7);
	own_lcd = &lcd;

	delay(1000);
	own_lcd->SendString("Count 0");
}

void SerialSetup(unsigned int baud = 9600)
{
	Serial.begin(baud);
	delay(100);
	Serial.println("Start");
}

//==========================================================================================

void encIntrr()
{
	uint8_t pinB_val = digitalRead(PINB);
	count = pinB_val ? (count - DECREMENT) : (count + INCREMENT);

	if (count > MAX_VALUE) count = MAX_VALUE;
	else if (count < -MAX_VALUE) count = -MAX_VALUE;

	change = 0x01;
}

void countInfo(int transmit_mode)
{
	char num[MAX_VALUE_LEN];
	sprintf(num, "%d", count);

	switch (transmit_mode)
	{
	case Lcd:

		own_lcd->SetPos(1, 7);
		own_lcd->SendString(num);
		break;

	case SM:
		Serial.println(count);
		break;

	case BOTH:

		own_lcd->SetPos(1, 7);
		own_lcd->SendString(num);

		Serial.println(count);
		break;

	case NONE:
		break;
	}
}