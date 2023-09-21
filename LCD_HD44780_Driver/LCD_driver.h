#include <stdint.h>

#ifndef HITACHI_DRIVER
#define HITACHI_DRIVER

#define BIT4_TRANSSMITION 0
#define BIT8_TRANSSMISION 1

#define CLEAR_CMD 0x01
#define POS_CMD 0x80

class LCD
{
public:
	LCD(uint8_t RS, uint8_t Enable, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7);
	void SendString(char* msg);
	void SendChar(uint8_t c);
	void Clear();
	void SetPos(uint8_t line, uint8_t pos);
private:
	void send4bits(uint8_t nibble, uint8_t rs);
	void sendCmd(uint8_t cmd);
	uint8_t rs, rw, enable;						//Holds reference to controlling pins
	uint8_t d0, d1, d2, d3, d4, d5, d6, d7;		//Holds reference to data pins

	uint8_t bitMode;							//4 or 8 bit mode
};

LCD::LCD(uint8_t RS, uint8_t Enable, uint8_t D4, uint8_t D5, uint8_t D6, uint8_t D7)
{
	rs = RS;
	enable = Enable;
	d4 = D4;
	d5 = D5;
	d6 = D6;
	d7 = D7;

	//Because we are using all pins as outputs, we need to change their modes
	uint8_t pins[] = { rs, enable, d4, d5, d6, d7 };
	for (int i = 0; i < 6; i++)
	{
		pinMode(pins[i], OUTPUT);
	}


	delay(50);					//wait > 40ms

	//Making sure the pins are in correct states
	digitalWrite(enable, 0);
	digitalWrite(rs, 0);

	send4bits(0x03, 0);
	delayMicroseconds(4500);	//wait > 4.1ms

	send4bits(0x03, 0);
	delayMicroseconds(4500);	//wait > 100us
	
	send4bits(0x03, 0);
	delayMicroseconds(200);		//undefined delay

	send4bits(0x02, 0);
	delayMicroseconds(50);		//undefined delay

	sendCmd(0x28);	//set 4-bit mode with 2 liness
	delay(1);
	sendCmd(0x08);	//display off cursor off
	delay(5);
	sendCmd(0x01);	//display clear
	delay(5);
	sendCmd(0x06);	//increment, no shift
	delay(1);

	//End of initialization
	sendCmd(0x0F);	//display+cursor+blinking on 
	delay(2);
	sendCmd(0x80);	//resets cursor position
	delay(2);

}

void LCD::send4bits(uint8_t nibble, uint8_t rs_value)
{
	uint8_t rs_state = rs_value ? HIGH : LOW; 
	digitalWrite(rs, rs_state);
  
	digitalWrite(d4, (nibble & 0x01));
	digitalWrite(d5, ((nibble >> 1) & 0x01));
	digitalWrite(d6, ((nibble >> 2) & 0x01));
	digitalWrite(d7, ((nibble >> 3) & 0x01));

	digitalWrite(enable, 0);
	delayMicroseconds(1);

	digitalWrite(enable, 1);
	delayMicroseconds(1);

	digitalWrite(enable, 0);
	delayMicroseconds(100);
}

void LCD::SendChar(uint8_t c)
{
	uint8_t nibble = (c >> 4) & 0x0F;
	send4bits(nibble, 1);

	delay(1);

	nibble = c & 0x0F;
	send4bits(nibble, 1);

}

void LCD::sendCmd(uint8_t c)
{
	uint8_t nibble = (c >> 4) & 0x0F;
	send4bits(nibble, 0);

	delay(1);

	nibble = c & 0x0F;
	send4bits(nibble, 0);

}

void LCD::SendString(char* msg)
{
	while(*msg)
	{
		SendChar(*msg);
		msg++; 
		delayMicroseconds(50);
	}
}

void LCD::Clear()
{
	sendCmd(CLEAR_CMD);
	delayMicroseconds(100);
}

void LCD::SetPos(uint8_t line, uint8_t pos)
{
	uint8_t bitPos = line & 0x02 ? pos + 0x20 : pos;

	//Now to make command we need to add the addres to cmd number
	bitPos = POS_CMD | (bitPos - 0x01);

	sendCmd(bitPos);
	delayMicroseconds(50); //wait at least 37us 
}
#endif