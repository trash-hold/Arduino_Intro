#include <LiquidCrystal.h>

//Defining Pins for generalizing the code
#define RS 3
#define ENABLE 2
#define D4 4
#define D5 5
#define D6 6
#define D7 7

//LCD characteristics
#define WORD_LENGTH 16
#define LCD_ROWS 2

//Global variables for easier access
LiquidCrystal lcd(RS, ENABLE, D4, D5, D6, D7);
String text = "Hello world!";
int count = 1;
int len;

//Functions to run text across lcd screen
void fillText();
void moveTextLeft();
void moveTextRight();


void setup() {
	//LCD startup
	lcd.begin(WORD_LENGTH, LCD_ROWS);
	lcd.clear();

	fillText();
	lcd.print(text);
}

void loop() {
	moveTextLeft();
}

//Functions' definitions
void fillText() {
	//Makes sure that the text is AT LEAST WORD_LENGTH -> only for aesthetical purposes
	len = text.length() - 1;

	if (len < WORD_LENGTH - 1) {
		for (int i = 1; i <= WORD_LENGTH - len - 1; i++)
		{
			text += " ";
		}

		len = text.length() - 1;
	}
}

void moveTextRight() {
	delay(750);   //So the text doesn't bug out
	lcd.clear();
	if (count < len)
	{
		//Printing letters from count to end of string
		for (int i = count; i >= 0; i--) {
			lcd.write(text[len - i]);
		}
		//Previous loop gets n characters, second loop fills it up by m (m = WORD_LENGTH - n) characters starting from text[0]
		for (int i = 0; i <= len - count; i++) {
			lcd.write(text[i]);
		}
		count++;
	}
	else
	{
		lcd.print(text);
		count = 1;
	}
}

void moveTextLeft() {
	delay(750);   //So the text doesn't bug out
	lcd.clear();
	if (count < len)
	{
		for (int i = 0; i < WORD_LENGTH; i++)
		{
			if (count + i < len) lcd.print(text[count + i]);
			else lcd.print(text[i + count - len]);
		}
		count++;
	}
	else
	{
		lcd.print(text);
		count = 1;
	}
}