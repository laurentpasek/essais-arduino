/********************************
* name:I2C LCD1602
* function:You should now see your I2C LCD1602 display the flowing characters: "SunFounder" and "hello, world".
********************************/
//Email:support@sunfounder.com
//Website:www.sunfounder.com

/********************************/
// include the library code
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
/**********************************************************/
char array1[]=" SunFounder               "; //the string to print on the LCD
char array2[]="hello, world!             "; //the string to print on the LCD
int tim = 500; //the value of delay time


#define I2C_ADDR 0x3F // <<—– Mettre votre adresse
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7
int n = 1;

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);
//LiquidCrystal_I2C lcd(0x3f,16,2); // set the LCD address to 0x27 for a 16 chars and 2 line display
/*********************************************************/
void setup()
{
lcd.begin (16,2);
  //  lcd.init(); //initialize the lcd
  lcd.backlight( ); //open the backlight 
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight (HIGH);
}
/*********************************************************/
void loop() 
{
  lcd.setCursor(15,0); // set the cursor to column 15, line 0
  for (int positionCounter1 = 0; positionCounter1 < 26; positionCounter1++)
  {
    lcd.scrollDisplayLeft(); //Scrolls the contents of the display one space to the left.
    lcd.print(array1[positionCounter1]); // Print a message to the LCD.
    delay(tim); //wait for 250 microseconds
  }
  lcd.clear(); //Clears the LCD screen and positions the cursor in the upper-left  corner.
  lcd.setCursor(15,1); // set the cursor to column 15, line 1
  for (int positionCounter = 0; positionCounter < 26; positionCounter++)
  {
    lcd.scrollDisplayLeft(); //Scrolls the contents of the display one space to the left.
    lcd.print(array2[positionCounter]); // Print a message to the LCD.
    delay(tim); //wait for 250 microseconds
  }
  lcd.clear(); //Clears the LCD screen and positions the cursor in the upper-left corner.
}
/************************************************************/
