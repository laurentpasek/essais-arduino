


/* Dépendance pour le bus 1-Wire */
#include <OneWire.h>
 
 
/* Broche du bus 1-Wire */
const byte BROCHE_ONEWIRE = 10;
/* Création de l'objet OneWire pour manipuler le bus 1-Wire */
OneWire ds(BROCHE_ONEWIRE);

//déclarer l'adresse du capteur (temprature d'eau)
byte addrtempeau [8] = {0x28, 0xFF, 0x60, 0x26, 0x92, 0x16, 0x05, 0x3E };


byte datatempeau[9];    //le tableau pour stocker les données envoyées par le capteur
float tempeau;    //un float pour stocker la température un fois convertie


void setup ()
{
  Serial.begin (9600);    //innitialiser la communication série
}

void loop ()
{
  ds.reset();    //reset du bus 1-wire
  ds.select (addrtempeau);    //selection de la sonde DS18B20 par son addresse
  ds.write (0x44, 1);    //demande à la sonde une prise de mesure (conversion)
  delay (800);      //attend que la sonde ait fait sa mesure
  
  ds.reset();    //reset du bus 1-wire
  ds.select (addrtempeau);    //selection de la sonde DS18B20 par son addresse
  ds.write (0xBE);    //demande à la sonde la lecture du scratchpad
  
  
  //on stocke chacun des octets dans le tableau data
  for (byte i=0; i<9; i++)
  {
    datatempeau[i] = ds.read();
  }
  
  
  
  //on reprend les deux premiers octets et on les convertit en une température en degrés celsius
  tempeau = ((datatempeau[1] << 8 | datatempeau[0]) * 0.0625);
  
  Serial.println (tempeau);    //on affiche la température sur le moniteur série
  
}




