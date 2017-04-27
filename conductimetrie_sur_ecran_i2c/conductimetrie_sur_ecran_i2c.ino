
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

/*
** Example Arduino sketch for SainSmart I2C LCD Screen 16×2
** based on https://bitbucket.org/celem/sainsmart-i2c-lcd/src/3adf8e0d2443/sainlcdtest.ino
** by
** Edward Comer
** LICENSE: GNU General Public License, version 3 (GPL-3.0)

** This example uses F Malpartida’s NewLiquidCrystal library. Obtain from:
** https://bitbucket.org/fmalpartida/new-liquidcrystal

** Modified – Ian Brennan ianbren at hotmail.com 23-10-2012 to support Tutorial posted to Arduino.cc

** Written for and tested with Arduino 1.0
**
** NOTE: Tested on Arduino Uno whose I2C pins are A4==SDA, A5==SCL

*/


// configuration de l'écran

#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>


#define I2C_ADDR 0x3F // <<—– Mettre votre adresse
#define BACKLIGHT_PIN 3
#define En_pin 2
#define Rw_pin 1
#define Rs_pin 0
#define D4_pin 4
#define D5_pin 5
#define D6_pin 6
#define D7_pin 7

int n = 1;  //sert au fonctionnement de l'écran..

LiquidCrystal_I2C lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);


// définitions des variables pour le calcul de la conductimétrie
int capteur =0;// Le capteur est sur la pin A0
float mesure =0;// Variable de stockage de la valeur lue
//float voltagemap =6;  //valeur lue monvertie en volts
float tension=6;  //valeur lue convertie en volts
float conductance=0;
float conductivite=0;



void setup()
{
Serial.begin (9600);// Démarrage de la liaison série

lcd.begin (16, 2); // <<—– My LCD was 16×2
// Switch on the backlight
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
lcd.setBacklight(HIGH);
}

void loop()
{

mesure = analogRead (capteur);// Lecture de la valeur du capteur
tension = (mesure * 5) / 1023;
conductance= (5-tension)/(tension*4066);
//conductivite= conductance*1000000000;
conductivite= ( (5-tension)/(tension*4066) *1278)-0.0;

Serial.println (mesure);// Affichage de la valeur
//Serial.println (voltagemap);  //affiche le voltage
Serial.println (tension*1000);
//Serial.println (conductance);
//Serial.println (conductivite);
Serial.println ();
delay (1000);// Attendre 1s avant la prochaine lecture 
  
  
  
lcd.home (); // go home
lcd.print(tension*1000); lcd.print ("mV");
//lcd.setCursor (10,0); lcd.print (conductance*100000);
lcd.setCursor (0,1);
//lcd.print(conductivite);
lcd.print (mesure);
}
