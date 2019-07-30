// inclure les librairies nécessaires au fonctionnement
#include <Wire.h>    //du 1-wire et de l'I2C
#include <LiquidCrystal_I2C.h>    //de l'écran I2C
#include <OneWire.h>    //du bus 1-wire
#include "DHT.h"   // Librairie des capteurs DHT
#include <Adafruit_Sensor.h>  // la librairie adafruit sensor est nécessaire à la librairie DHT.h
#include "SparkFunBME280.h"   //la librairie permettant l'utilisation BME280.
#include <SPI.h>  // Cette librairie est nécessaire au fonctionnement de la librairie SparkfunBME280.
#include <Adafruit_TSL2561_U.h>  // La librairie nécessaire au capteur de luminosité




//définition du bus 1-wire---------------------------------------------------

/* Broche du bus 1-Wire */
const byte BROCHE_ONEWIRE = 10;
/* Création de l'objet OneWire pour manipuler le bus 1-Wire */
OneWire ds(BROCHE_ONEWIRE);

//---------------------------------------------------------------------------

// définition du capteur DHT22-------------------------------------------------------------------------------

#define DHTPIN 11    // Changer le pin sur lequel est branché le DHT
#define DHTTYPE DHT22      // Il s'agit d'un capteur DHT 22  (AM2302) et non DHT11 ou DHT21
// Initialisation du capteur pour un Arduino à 16mhz par défaut
// Il faudra modifier le 3ème paramètres pour une autre carte (sinon le capteur renvoie 0). Quelques valeurs : 8mhz => 3, 16mhz => 6, 84mhz => 84
DHT dht(DHTPIN, DHTTYPE); 

// Variables nécessaites au capteur DHT22---------------------------------------------------------------------------------

float tempair;    //un float pour stocker la température de l'air
float humidite;    //un float pour stocker le taux d'humidité
//-------------------------------------------------------------------------------------------------------------------



//Définition de l'écran I2C--------------------------------------------------------------------------------------------

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


//----------------------------------------------------------------------------------------------------------------------



/* Définition des pins sur lesquelles sont branchés les relais
const int inter1 = 6;
const int inter2 = 7;
const int inter3 = 8;
const int inter4 = 9;
*/
char inter[] = {6,7,8,9};



/* Définition des pins sur lesquels sont branchés les boutons
const int bouton1 = 2;
const int bouton2 = 3;
const int bouton3 = 4;
const int bouton4 = 5;
*/
char bouton[] = {2,3,4,5};



//variables nécessaires au capteur de température d'eau (DS18B20)---------------------------------------------------------

//déclarer l'adresse du capteur (temprature d'eau)
byte addrtempeau [8] = {0x28, 0xFF, 0x60, 0x26, 0x92, 0x16, 0x05, 0x3E };

byte datatempeau[9];    //le tableau pour stocker les données envoyées par le capteur
float tempeau;    //un float pour stocker la température un fois convertie

long delaitempeau = millis();    //une variable pour enregistrer le temps de départ de mesure de la temperature d'eau
byte tempeauencours = 1;    //une variable pour savoir si une mesure de la temperature d'eau est en cours
//------------------------------------------------------------------------------------------------------------------------

// Variables nécessaires à la pompe de circulation-------------------------------------------------------------------------

// On crée un tableau contenant : { le temps allumé, le temps éteint, le moment du dernier changement, l'état (allumé ou éteint) }
// et qui sera utilisé par la fonction "clignotement"
// long circulation [] = {1000,3000,0,0};
long circulation [] = {4294967290,0,0,0};

//      int etatcirculation = 1;      //une variable pour stocker l'état dans lequel doit etre le relais
    int modecirculation [2]= {0,0};  //variable pour stocker le mode de fonctionnement de la pompe de circulation (-1=off ; 0=auto ; 1=on )en premier et le mode précédent ensuite

//--------------------------------------------------------------------------------------------------------------------------

//variables nécessaires à la pompe à air----------------------------------------------------------------------------------------

// On crée un tableau contenant : { le temps allumé, le temps éteint, le moment du dernier changement, l'état (allumé ou éteint) }
// et qui sera utilisé par la fonction "clignotement"
//long air [] = {1000,3000,0,0};
long air [] = {4294967290,0,0,0};

int modeair [2]= {0,0};  //variable pour stocker le mode de fonctionnement de la pompe à air (-1=off ; 0=auto ; 1=on )en premier et le mode précédent ensuite


//Variables nécessaires à l'écran------------------------------------------------------------------------------------------

// On crée une variable qui sera utilisée par la fonction "clignotement" et qui premettra de faire clignotter l'écran
long blinkecran [] = {1000, 1000, 0, 0};    //soit une seconde allumé, une seconde éteint, millis() au dernier changement, l'état (allumé ou éteint)
//On crée une variable qui contient {le temps d'allumage du rétroéclairage, le temps (millis) depuis l'appui du bouton, et 0 ou 1 si le rétroéclairage est allumé ou éteint}
long retroeclairage []= {4000, 0, 0};

//-------------------------------------------------------------------------------------------------------------------------


// Variables nécessaires au fonctionnement des boutons----------------------------------------------------------------------
/*
la variable databouton1 stocke les données relatives au bouton 1 avec :
[0] : le pin sur lequel il est branché
[1] : le délai entre deux conparaisons de son état
[2] : son état actuel
[3] : son dernier état
[4] : le temps (millis) de la dernière mesure de son état
*/
long databouton1 []={(bouton[0]),300,0,0,0};
long databouton2 []={(bouton[1]),300,0,0,0};
long databouton3 []={(bouton[2]),300,0,0,0};
long databouton4 []={(bouton[3]),300,0,0,0};
//---------------------------------------------------------------------------------------------------------------------------



// variables necessaires à la communication serie---------------------------
const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;

char messageFromPC[buffSize] = {0};
int NewData = 0;

unsigned long curMillis;

unsigned long prevReplyToPCmillis = 0;
unsigned long replyToPCinterval = 1000;

//---------------------------------------------------------------------------

// Définition du capteur de pression BME280
BME280 CapteurPression; //Uses I2C address 0x76 (jumper closed). On pourra aussi l'appeler CapteurPression
float Pression; //Une variable pour stocker la pression atmospherique.
float TempElec; //Une variable pour stocker la température dans le boitier electronique.

//------------------------------------------------------------------------------------------


// Définition du capteur TSL2561 ( Luxmètre )

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_FLOAT, 12345);
// ADDR_FLOAT définit l'adresse du capteur à 0x39 soit le pin d'adresse sur le capteur
//n'est pas connecté
float Luminosite; //Une variable pour stocker la valeur lue par le capteur

//-------------------------------------------------------------------------------




void setup() {
  
  // Définition des pins D6 à D9 (les relais ) comme sortie et éteints au départ
  for ( char i=0; i < sizeof(inter) ; i++)
  {
    pinMode (inter[i], OUTPUT);    //on définit tous les relais comme sorties
    digitalWrite (inter[i], HIGH);    //on définit tous les relais éteints au départ
  }
  
//--------------------------------------------------------------------------------------------------------------------------  
  
// Définition des pins D2 à D5 (les bouton) comme entrées et activation des résistances de pull-up
  for (char i=0 ; i<sizeof (bouton); i++)
  {
    pinMode (bouton[i], INPUT);
//    digitalWrite (bouton [i], HIGH);  // mettre une broche en entrée à l'état haut permet d'activer les résistances de pull-up
  }
  
//--------------------------------------------------------------------------------------------------------------------------  

//Paramétrage de l'écran
lcd.begin (16, 2); // <<—– My LCD was 16×2
// Switch on the backlight
lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
lcd.setBacklight(HIGH);
//-------------------------------------------------------------------------------------------------------------------------

dht.begin();    // initialisation du capteur DHT22

//----------------------------------------------------------------------------------------------------------------------


  
  Serial.begin(9600); //initialiser la communication série ( pour déboggage)
//  Serial.println("<Arduino is ready>"); //permet au raspberry de savoir que l'arduino est pret à recevoir les données

// Initialisation du capteur de pression BME280
CapteurPression.setI2CAddress(0x76); //Le capteur a pour adresse 0x76.
CapteurPression.beginI2C();  //Démarrage du capteur.
//-----------------------------------------------------------------------------------

//Initialisation du capteur de luminosité TSL2561
tsl.begin();

//sensor_t sensor;
//tsl.getSensor(&sensor);

tsl.enableAutoRange(true); // Choisit seul le gain du capteur selon la luminosité pour éviter la saturation
tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* Résolution moyenne pour une vitesse acceptable */

//------------------------------------------------


}



void loop()
{
  
// première partie permettant de selectionner le mode de fonctionnement de la pompe de circulation en fonction
// des appuis sur le bouton 1
    if ( etatbouton(databouton1)== 1 )    //si le bouton 1 est réellement appuyé
  {
  
  switch (modecirculation [0])  //tester la valeur du mode de fonctionnement de la pompe
  {
   case -1:  //si elle est éteinte
   modecirculation [0]=0;  //la passer en automatique
   lcd.clear();    //on en profite pour effacer l'écran qui devra maintenant afficher : circulation OFF
   break;
   
   case 0:  //si elle est en automatique 
   modecirculation [0]=1;  //la passer en allumé forçé
   lcd.clear();    //on en profite pour effacer l'écran qui devra maintenant afficher les températures
   break;
   
   case 1:  //si elle est forçée allumée
   modecirculation [0]=-1;  //l'éteindre
   lcd.clear();    //on en profite pour effacer l'écran qui devra maintenant afficher : circulation ON
   break;
   }
  }
  //fin de la première partie
  
  //----------------------------------------------------------------------------------------------------------------------------------------
  
  //seconde partie : on allume ou éteint la pompe de circulation selon le mode de circulation choisi
  switch (modecirculation [0])    //selon le mode de fonctionnement choisi
  {
    case -1:    //si elle est forçée éteinte
    digitalWrite (inter[0], HIGH);    //on l'éteint
    lcd.setCursor (0,0);    //on se met sur le premier caractère de la première ligne
    lcd.print ("circulation OFF");    //on affiche le texte : circulation OFF
    lcd.setBacklight (clignotement(blinkecran));    //on fait clignoter le rétroéclairage de l'écran
    break;
    
    case 0:    //si elle est en fonctionnement automatique
    digitalWrite (inter[0] , clignotement (circulation));  //on met physiquement le relais dans la bonne position en fonction du retour de la fonction "clignotement" pour la variable "circulation"
    if (modeair[0] ==0 && retroeclairage[2] ==0 )    //si la pompe à air est aussi en mode automatique et que l'écren ne doit pas etre allumé
    {
    lcd.setBacklight(LOW);    //on allume le rétroéclairage de l'écran
    }
    break;
    
    case 1:    //si elle est forçée allumée
    digitalWrite (inter[0], LOW);    //on l'allume
    lcd.setCursor (0,0);    //on se met sur le premier caractère sur la première ligne
    lcd.print ("circulation ON");    //on affiche le texte : circulation ON
    lcd.setBacklight (clignotement(blinkecran));    //on fait clignoter le rétroéclairage de l'écran
    break;
  }
  //fin de la seconde partie
  
  //-------------------------------------------------------------------------------------------------------------------------------------------
  
  //troisième partie : permettant de selectionner le mode de fonctionnement de la pompe à air en ----------------------------------------------
  // fonction de appuis sur le bouton 2
  
      if ( etatbouton(databouton2)== 1 )    //si le bouton 2 est réellement appuyé
  {
  
  switch (modeair [0])  //tester la valeur du mode de fonctionnement de la pompe à air
  {
   case -1:  //si elle est éteinte
   modeair [0]=0;  //la passer en automatique
   lcd.clear();    //on en profite pour effacer l'écran qui devra maintenant afficher : air OFF
   break;
   
   case 0:  //si elle est en automatique 
   modeair [0]=1;  //la passer en allumé forçé
   lcd.clear();    //on en profite pour effacer l'écran qui devra maintenant afficher les températures
   break;
   
   case 1:  //si elle est forçée allumée
   modeair [0]=-1;  //l'éteindre
   lcd.clear();    //on en profite pour effacer l'écran qui devra maintenant afficher : air ON
   break;
   }
  }

  
  
  // fin de la troisième partie--------------------------------------------------------------------------------------------------------------
  
  // quatrième partie : on allume ou on éteint la pompe à air selon le mode choisi-------------------------------------------------------------
  
  switch (modeair [0])    //selon le mode de fonctionnement choisi
  {
    case -1:    //si elle est forçée éteinte
    digitalWrite (inter[1], HIGH);    //on l'éteint
    lcd.setCursor (0,1);    //on se met sur le premier caractère de la seconde ligne
    lcd.print ("air OFF");    //on affiche le texte : air OFF
    lcd.setBacklight (clignotement(blinkecran));    //on fait clignoter le rétroéclairage de l'écran
    break;
    
    case 0:    //si elle est en fonctionnement automatique
    digitalWrite (inter[1] , clignotement (air));  //on met physiquement le relais dans la bonne position en fonction du retour de la fonction "clignotement" pour la variable "air"
    if (modecirculation[0] ==0 && retroeclairage[2] == 0 )    //si la pompe de circulation est aussi en automatique et que l'écran ne doit pas etre allumé
    {
    lcd.setBacklight(LOW);    //on eteint le rétroéclairage de l'écran
    }
    break;
    
    case 1:    //si elle est forçée allumée
    digitalWrite (inter[1], LOW);    //on l'allume
    lcd.setCursor (0,1);    //on se met sur le premier caractère sur la seconde ligne
    lcd.print ("air ON");    //on affiche le texte : air ON
    lcd.setBacklight (clignotement(blinkecran));    //on fait clignoter le rétroéclairage de l'écran
    break;
  }
  
  // fin de la quatrième partie---------------------------------------------------------------------------------------------------------------
  
  // cinquième partie : gestion du rétroéclairage de l'écran juste le temps d'une lecture-----------------------------------------------------
  
  if (etatbouton(databouton4) == 1 )    //si le bouton 4 est réellement appuyé
  {
  retroeclairage[2] = 1;    //on déclare que le rétroéclairage doit etre allumé
  retroeclairage[1] = millis();    //on enregistre le millis au moment de l'appui sur le bouton
  lcd.setBacklight (HIGH);    //on allume le rétroéclairage
  }
  
  if ( millis() - retroeclairage[1] > retroeclairage[0] )    //si le temps de rétroéclairage est écoulé
  {
   retroeclairage[2] = 0;    //on déclare que le rétroéclairage doit etre éteint. (il sera éteint physiquement dans les boucles sur le fonctionnement des pompes).
   
  }
  
  // fin de la cinquième partie--------------------------------------------------------------------------------------------------------------
  
  
  
  //sixième partie : lecture de la sonde de température d'eau---------------------------------------------------
  
  LectureTempEau ();
  
  /*
  if (tempeauencours ==0)    //si il n'y a pas de mesure en cours
  {
  ds.reset();    //reset du bus 1-wire
  ds.select (addrtempeau);    //selection de la sonde DS18B20 par son addresse
  ds.write (0x44, 1);    //demande à la sonde une prise de mesure (conversion)
  delaitempeau = millis();    //enregistre le temps de départ
  tempeauencours = 1;    //on signale qu'il y a une mesure en cours
  }
    
  if (millis() - delaitempeau > 800 && tempeauencours == 1)    //si une mesure est en cours et ce depuis plus de 800 millisecondes (temps nécessaire au D18B20)
  {
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
  tempeauencours = 0;
  }  //fin de la lecture de la temperature de l'eau
  */
  
   
//fin de la sixième partie-----------------------------------------------------------------------------------------------------------------------------  

// septième partie : lecture de la temperature de l'air et du taux d'humidité-----------------------------------------------------------------



// LectureTempAir ();
// LectureHumidite ();




/*
tempair = dht.readTemperature();    //lecture de la temperature sur la DHT22 en degrés celcius
humidite = dht.readHumidity();    //lecture du taux d'humidité sur la DHT22
*/

// fin de la septième partie------------------------------------------------------------------------------------------------------------



  //affichage de la température d'eau sur l'écran
  if (modecirculation [0] ==0 && modeair [0] ==0 )    //si la pompe de circulation et la pompe à air sont en mode automatique
  {
    AffichageEcran ();
    
    
    /*
     lcd.home ();    //on se met sur le premier caractère de la première ligne
     lcd.print (tempeau, 1);    //on affiche la température de l'eau avec un seul chiffre derrière la virgule
     lcd.print ((char)223); //le signe °
     lcd.print (" ");    //on laisse un espace
     lcd.print (tempair, 1);    //on affiche la température de l'air avec un seul chiffre derrière la virgule
     lcd.print ((char)223); //le signe °
     lcd.print (" ");    //on {laisse un espace
     lcd.print (humidite, 1);    //on affiche la l'humidité de l'air avec un seul chiffre derrière la virgule
     //lcd.print ((char)37); //le signe %
     */
     
    
    
    /*
 lcd.home ();    //on se met sur le premier caractère de la première ligne
 lcd.print ("temperature");    //on affiche le texte : temperature
 lcd.setCursor (2,1);    //on se met au troisième caractère sur la seconde ligne
 lcd.print (tempeau, 1=c);    //on affiche la température avec un seul chiffre derrière la virgule
 lcd.print ((char)223); //le signe °
 //lcd.print ((char)99); // le symbole c sur la table de caractères
 lcd.print("c");
 */
  }
  
  // fonction de lecture sur le port série--------------------------------
  
  curMillis = millis();
  getDataFromPC();
  ChoixRequete ();
  
  //-----------------------------------------------------------------------
  
    
    
    
}

//FIN DE VOID LOOP-----------------------------------------------------------------------------------------------------------------------------------




//---------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------


// début de la fonction de clignotement qui permet d'allumer et d'éteindre la pompe à intervalles réguliers ---------------------------------------------------------------
long clignotement ( long delais[4] )  
{
  
 
  if (delais[3] == 1)  //si le relais est éteint
  {
    if (millis () - delais[2] > delais[1])  //et que le délai d'extinction est dépassé
    {
      delais[2] = millis ();  //mettre à jour previousMillis
      delais[3] = 0;  //allumer le relais
    }
  }
  
    else      //Sinon, le relais est allumé
    {
     if (millis () - delais[2] > delais[0])  //et si le délai d'allumage est dépassé
      {
        delais[2] = millis ();  //mettre à jour PreviousMillis
        delais[3] = 1;  //éteindre le relais
      }
    }

return delais[3];    //retourne la position dans laquelle doit etre le relais

}

// fin de la fonction de clignotement



//début de la fonction permettant de lire l'état dun bouton en évitant le phénomène de rebond --------------------------------------------------------------------------

long etatbouton (long databouton [])
{ 
   

databouton [2] = digitalRead(databouton [0]);  //lire l'état du bouton

if (databouton [2]==LOW && databouton [3]==HIGH)  //si bouton1 est appuyé et qu'il n'y était pas avant
{
  databouton [3]=LOW;  //enregistrer son dernier état à appuyé

    databouton [4] = millis();  //et enregistrer le temps (millis) de ce test
}
  
  
  if (databouton [2] == LOW  &&  databouton [3]==LOW   &&   millis() - databouton [4] >= databouton [1] )  //si le bouton était et est toujours appuyé et qu'il s'est écoulé plus de 200 millisecondes
  {
  

   
   databouton [3]=HIGH;  //remettre le dernier état du bouton à relaché
   return 1;    //on cosidère que le bouton est réellement appuyé donc on envoie 1
  }
  
  else
  {
  return 0;    //on considère que le bouton n'est pas réellement appuyé donc on envoie 0
  }
}

//Fin de la fonction de lecture des boutons



// Début de la fonction de lecture de la température de l'eau -----------------------------------------------------------------------------------------------

void LectureTempEau ()
{
    if (tempeauencours ==0)    //si il n'y a pas de mesure en cours
  {
  ds.reset();    //reset du bus 1-wire
  ds.select (addrtempeau);    //selection de la sonde DS18B20 par son addresse
  ds.write (0x44, 1);    //demande à la sonde une prise de mesure (conversion)
  delaitempeau = millis();    //enregistre le temps de départ
  tempeauencours = 1;    //on signale qu'il y a une mesure en cours
  }
    
  if (millis() - delaitempeau > 800 && tempeauencours == 1)    //si une mesure est en cours et ce depuis plus de 800 millisecondes (temps nécessaire au D18B20)
  {
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
  tempeauencours = 0;
  }  //fin de la lecture de la temperature de l'eau
}

// Fin de la lecture de la Température de l'eau -------------------------------------------------------------------------------------------------------


// Début de la fonction permettant d'afficher les données sur l'écran ---------------------------------------------------------------------------------

void AffichageEcran ()
{
     lcd.home ();    //on se met sur le premier caractère de la première ligne
     lcd.print (tempeau, 1);    //on affiche la température de l'eau avec un seul chiffre derrière la virgule
     lcd.print ((char)223); //le signe °
     lcd.print (" ");    //on laisse un espace
     lcd.print (tempair, 1);    //on affiche la température de l'air avec un seul chiffre derrière la virgule
     lcd.print ((char)223); //le signe °
     lcd.print (" ");    //on laisse un espace
     lcd.print (humidite, 1);    //on affiche la l'humidité de l'air avec un seul chiffre derrière la virgule
     //lcd.print ((char)37); //le signe %
}

// Fin de la fonction d'affichage des valeurs sur l'écran---------------------------------------------------------------------------------------------------


// Dédut de la fonction de la lecture de la température de l'air.---------------------------------------------------------------------------------------------

void LectureTempAir ()
{
  tempair = dht.readTemperature();    //lecture de la temperature sur la DHT22 en degrés celcius et stockage dans tempair
}

// fin de la fonction de lecture de la température de l'air----------------------------------------------------------------------------------------------


// Début de la fonction de lecture de l'hygrométrie------------------------------------------------------------------------------------------------------------

void LectureHumidite ()
{
  humidite = dht.readHumidity();    //lecture du taux d'humidité sur la DHT22 et stockage dans humidite
}

// fin de la fonction de lecture de l'hygrométrie


// Début de la fonction de lecture de la pression atmosphérique

void LecturePression ()
{
  Pression = CapteurPression.readFloatPressure()/100, 2;
}

// fin de la fonction de lecture de la pression atmosphérique.

// début de la fonction de lecture de la température du boitier électronique

void LectureTempElec ()
{
  TempElec = CapteurPression.readTempC(), 2;
}

// fin de la fonction de lecture de la température dans le boitier electronique

//Début de la fonction de lecture de la luminosité

void LectureLuminosite ()
{
  //On crée un nouvel évènement sur le capteur
  sensors_event_t event;
  tsl.getEvent(&event);
  
  //Puis on prends la mesure et on l'enregistre
  if (event.light)
  {
    Luminosite = (event.light);
  }
}

// fin de la fonction de lecture de la luminosite



// Début de la fonction de lecture du port série--------------------------

void getDataFromPC() {

    // receive data from PC and save it into inputBuffer
    
  if(Serial.available() > 0) {

    char x = Serial.read();

      // the order of these IF clauses is significant
      
    if (x == endMarker) {
      readInProgress = false;
      newDataFromPC = true;
      inputBuffer[bytesRecvd] = 0;
      parseData();
    }
    
    if(readInProgress) {
      inputBuffer[bytesRecvd] = x;
      bytesRecvd ++;
      if (bytesRecvd == buffSize) {
        bytesRecvd = buffSize - 1;
      }
    }

    if (x == startMarker) { 
      bytesRecvd = 0; 
      readInProgress = true;
    }
  }
}



// fin de la fonction de lecture sur le port série-------------------------


// début de la fonction permettant de séparer les commandes recues par le raspberry

void parseData() {

    // split the data into its parts
    
  char * strtokIndx; // this is used by strtok() as an index
  
  strtokIndx = strtok(inputBuffer,",");      // get the first part - the string
  strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC

  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  NewData = atoi(strtokIndx);     // convert this part to an integer
  
  /*
  
  strtokIndx = strtok(NULL, ","); 
  servoFraction = atof(strtokIndx);     // convert this part to a float
  */



}

// fin de la foncion de séparation des informations-----------------------

// Début de la fonction de traitement des requetes du raspberry----------

void ChoixRequete() {

   // this illustrates using different inputs to call different functions
  if (strcmp(messageFromPC, "LireHygro") == 0) {
     LectureHumidite();
     if (newDataFromPC) {
      newDataFromPC = false;
      }
  }

  if (strcmp(messageFromPC, "SendHygro") == 0) {
     if (newDataFromPC) {
      newDataFromPC = false;
      Serial.print ("<");
      Serial.print (humidite);
      Serial.print (">");
      }
  }

  if (strcmp(messageFromPC, "LireTempAir") == 0) {
     LectureTempAir();
     if (newDataFromPC) {
       newDataFromPC = false;
       }
  }
  
  if (strcmp(messageFromPC, "SendTempAir") == 0) {
      if (newDataFromPC) {
      newDataFromPC = false;
      Serial.print ("<");
      Serial.print (tempair);
      Serial.print (">");
      }
  }
  
  if  (strcmp(messageFromPC, "LireTempEau") == 0) {
    LectureTempEau ();
    if (newDataFromPC) {
      newDataFromPC = false;
    }
  }
  
    if  (strcmp(messageFromPC, "SendTempEau") == 0) {
    if (newDataFromPC) {
      newDataFromPC = false;
      Serial.print ("<");
      Serial.print (tempeau);
      Serial.print (">");
    }
  }
  
  
    if (strcmp(messageFromPC, "LirePression") == 0) {
     LecturePression();
     if (newDataFromPC) {
      newDataFromPC = false;
      }
  }

  if (strcmp(messageFromPC, "SendPression") == 0) {
     if (newDataFromPC) {
      newDataFromPC = false;
      Serial.print ("<");
      Serial.print (Pression);
      Serial.print (">");
      }
  }
  
  
  if (strcmp(messageFromPC, "LireTempElec") == 0) {
     LectureTempElec();
     if (newDataFromPC) {
      newDataFromPC = false;
      }
  }

  if (strcmp(messageFromPC, "SendTempElec") == 0) {
     if (newDataFromPC) {
      newDataFromPC = false;
      Serial.print ("<");
      Serial.print (TempElec);
      Serial.print (">");
      }
  }
  
  if (strcmp(messageFromPC, "LireLuminosite") == 0) {
     LectureLuminosite();
     if (newDataFromPC) {
      newDataFromPC = false;
      }
  }

  if (strcmp(messageFromPC, "SendLuminosite") == 0) {
     if (newDataFromPC) {
      newDataFromPC = false;
      Serial.print ("<");
      Serial.print (Luminosite);
      Serial.print (">");
      }
  }

  
  
    
  //essai de modifier un paramètre en mémoire dans l'arduino ici le mode de circulation
   if  (strcmp(messageFromPC, "UMC") == 0) {
   if (newDataFromPC) {
     newDataFromPC = false;
     modecirculation [0] = NewData;
     Serial.print ("<Nouveau mode de Circulation: ");
     Serial.print (modecirculation [0]);
     Serial.println ();
   }
 }



  /*
  if (strcmp(messageFromPC, "LED2") == 0) {
     updateLED2();
  }

  */

  
}

// fin de la fonction de traitement des requetes-------------------------


