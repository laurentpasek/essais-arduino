#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DallasTemperature.h>
#include <OneWire.h>

// Data wire is plugged into pin 10 on the Arduino
#define ONE_WIRE_BUS 10

// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

//Définition des pins de l'écran sur I2C
#define I2C_ADDR 0x3F // <<—– Mettre votre adresse
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

int n = 1; //nécessaire à lécran...

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin); //définition de l'écran

float temperature = 0; //définition de la variable température

void setup()
{
 
  Serial.begin (9600); //port série de test
  
 lcd.begin (16, 2); // <<—– My LCD was 16×2
 // Switch on the backlight
 lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
 lcd.setBacklight(HIGH);
  
 // Start up the library température
 sensors.begin();

}


void loop(void)
{
 
 sensors.requestTemperatures (); //Envoie la commande pour récupérer les températures
 
 temperature = sensors.getTempCByIndex(0);
 
 lcd.home ();
 lcd.print ("temperature");
 lcd.setCursor (2,1);
 lcd.print (temperature);
 //lcd.print (sensors.getTempCByIndex(0));
 lcd.print ((char)223); //le signe °
 //lcd.print ((char)99); // le symbole c sur la table de caractères
 lcd.print("c");
 
 Serial.print ("temperature : ");
 Serial.print (temperature);
 Serial.write (176); // le signe °
 //Serial.write (99); // le symbole c sur la table de caractères ascii
 Serial.println ("c");
 delay (500);
}
