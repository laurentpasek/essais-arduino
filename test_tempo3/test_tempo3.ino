/* test tempo 2
 
Une temporisation qui permet d'allumer les relais par intermittence en choisissant le temps d'allumage et d'extinction
*/

const int relais = 6;  //le relais est sur le pin 6

boolean etatRelais = 0; //Créer une variable pour savoir si le relais doit tre allumé ou éteint

long previousMillis;         // Temps du dernier changement

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long intervalOn = 1000;           // Temps d'allumage
long intervalOff = 3000;          //Temps d'extinction

// On crée un tableau contenant : le temps allumé, le temps éteint, le moment du dernier changement, l'état (allumé ou éteint)
long circulation [] = {1000,3000,0,0}  

void setup() {

  pinMode(relais, OUTPUT); //Définir la broche du relais comme sortie
  
    Serial.begin(9600); //initialiser la communication série
  
}

void loop()
{

  unsigned long currentMillis = millis();  //crée une variable ou sera stockée millis ()
  
  
  if (etatRelais == 1)  //si le relais est éteint
  {
    if (currentMillis - previousMillis > intervalOff)  //et que le délai d'extinction est dépassé
    {
      previousMillis = currentMillis;  //mettre à jour previousMillis
      etatRelais = 0;  //allumer le relais
    }
  }
  
    else      //Sinon, le relais est allumé
    {
     if (currentMillis - previousMillis > intervalOn)  //et si le délai d'allumage est dépassé
      {
        previousMillis = currentMillis;  //mettre à jour PreviousMillis
        etatRelais = 1;  //éteindre le relais
      }
    }
    
digitalWrite (relais, etatRelais);  //On met physiquement le relais à sa bonne position


// affichages de déboggage
 Serial.print ("millis : ");   Serial.println (currentMillis);
 Serial.print ("etat : ");   Serial.println (etatRelais);
 Serial.print ("previousMillis : ");  Serial.println (previousMillis);
 Serial.println();

}

