#include <Adafruit_Sensor.h>

// Test des capteurs d'humidité et de température DHT-11 / DHT-21 /  DHT-22
// Code adapté de l'exemple DHTTester de ladyada (Adafruit)
// Projets DIY - 22/01/2016

#include "DHT.h"   // Librairie des capteurs DHT


#define DHTPIN 11    // Changer le pin sur lequel est branché le DHT

// Dé-commentez la ligne qui correspond à votre capteur 
//#define DHTTYPE DHT11     // DHT 11 
#define DHTTYPE DHT22      // DHT 22  (AM2302)
//#define DHTTYPE DHT21     // DHT 21 (AM2301)

// Initialisation du capteur pour un Arduino à 16mhz par défaut
// Il faudra modifier le 3ème paramètres pour une autre carte (sinon le capteur renvoie 0). Quelques valeurs : 8mhz => 3, 16mhz => 6, 84mhz => 84

DHT dht(DHTPIN, DHTTYPE); 



float h = 20.5;
float t = 12.2;
float f = 22.6;
float hi = 18.4;





//communication serie
const byte buffSize = 40;
char inputBuffer[buffSize];
const char startMarker = '<';
const char endMarker = '>';
byte bytesRecvd = 0;
boolean readInProgress = false;
boolean newDataFromPC = false;

char messageFromPC[buffSize] = {0};

unsigned long curMillis;

unsigned long prevReplyToPCmillis = 0;
unsigned long replyToPCinterval = 1000;

//fin communication serie








void setup() {
  Serial.begin(9600); 
  Serial.println("DHTxx test!");
 
  dht.begin();
//}




//communication serie

    // tell the PC we are ready
    
  Serial.println("<Arduino is ready>");
  Serial.println ("Hygro pour Hygrometrie");
  Serial.println ("Tair pour temperature de l'air");
  Serial.println ();
}


//fin communication serie




void loop() {



  curMillis = millis();
  getDataFromPC();
  ChoixRequete ();



  /*
  // Délai de 2 secondes entre chaque mesure. La lecture prend 250 millisecondes
  delay(2000);


  ReadHumidity ();
  ReadCelcius ();
  ReadFahrenheit ();

  
  // Stop le programme et renvoie un message d'erreur si le capteur ne renvoie aucune mesure
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Echec de lecture !");
    return;
  }






  // Calcul la température ressentie. Il calcul est effectué à partir de la température en Fahrenheit
  // On fait la conversion en Celcius dans la foulée
  float hi = dht.computeHeatIndex(f, h);
  

  Serial.print("Humidite: "); 
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: "); 
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("Temperature ressentie: ");
  Serial.print(dht.convertFtoC(hi));
  Serial.println(" *C");



  */
}




void ReadHumidity () {
  h = dht.readHumidity();
}

void ReadCelcius() {
  t = dht.readTemperature();
}

void ReadFahrenheit () {
  f = dht.readTemperature(true);
}


// fonctions communication serie




//=============

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

//=============

 
void parseData() {

    // split the data into its parts
    
  char * strtokIndx; // this is used by strtok() as an index
  
  strtokIndx = strtok(inputBuffer,",");      // get the first part - the string
  strcpy(messageFromPC, strtokIndx); // copy it to messageFromPC




  /*
  strtokIndx = strtok(NULL, ","); // this continues where the previous call left off
  newFlashInterval = atoi(strtokIndx);     // convert this part to an integer
  
  strtokIndx = strtok(NULL, ","); 
  servoFraction = atof(strtokIndx);     // convert this part to a float
  */



}

//=============


void ChoixRequete() {

   // this illustrates using different inputs to call different functions
  if (strcmp(messageFromPC, "Hygro") == 0) {
     ReadHumidity();
     if (newDataFromPC) {
      newDataFromPC = false;
      Serial.print ("<Humidite : ");
      Serial.print (h);
      Serial.print (" %>");
      Serial.println ();
     }
  }

  if (strcmp(messageFromPC, "Tair") == 0) {
     ReadCelcius();
     if (newDataFromPC) {
      newDataFromPC = false;
      Serial.print ("<Temperature : ");
      Serial.print (t);
      Serial.print (" *C>");
      Serial.println ();
     }
  }



  /*
  if (strcmp(messageFromPC, "LED2") == 0) {
     updateLED2();
  }

  */

  
}







