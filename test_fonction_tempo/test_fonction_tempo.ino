/* test tempo 2
 
Une temporisation qui permet d'allumer les relais par intermittence en choisissant le temps d'allumage et d'extinction
*/

const int relais = 6;  //le relais est sur le pin 6

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.

// On crée un tableau contenant : le temps allumé, le temps éteint, le moment du dernier changement, l'état (allumé ou éteint)
long circulation [] = {1000,3000,0,0};  

int etatcirculation = 1;      //une variable pour stocker l'état dans lequel doit etre le relais


void setup() {

  pinMode(relais, OUTPUT); //Définir la broche du relais comme sortie
  
    Serial.begin(9600); //initialiser la communication série

}

void loop()
{
  etatcirculation = clignotement (circulation);    //le retour de la fonction clignotement por circulation est stocké dans etat
  digitalWrite (relais , etatcirculation);        //on met physiquement le relais dans la bonne position
}



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




