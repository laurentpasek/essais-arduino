

// définition des pins sur lesquelles sont branchés les relais
/*
const int inter1 = 6;
const int inter2 = 7;
const int inter3 = 8;
const int inter4 = 9;
*/
char inter[] = {6,7,8,9};

//const int relaiscirculation = 6;  //le relais est sur le pin 6

//définition des pins sur lesquels sont branchés les boutons
/*
const int bouton1 = 2;
const int bouton2 = 3;
const int bouton3 = 4;
const int bouton4 = 5;
*/
char bouton[] = {2,3,4,5};


// On crée un tableau contenant : { le temps allumé, le temps éteint, le moment du dernier changement, l'état (allumé ou éteint) }
long circulation [] = {1000,3000,0,0};  

int etatcirculation = 1;      //une variable pour stocker l'état dans lequel doit etre le relais
int modecirculation = 0;  //variable pour stocker le mode de fonctionnement de la pompe de circulation (-1=off ; 0=auto ; 1=on )

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



void setup() {
  
  // Définition des pins D6 à D9 (les relais ) comme sortie et éteints au départ
  
  for ( char i=0; i < sizeof(inter) ; i++)
  {
    pinMode (inter[i], OUTPUT);    //on définit tous les relais comme sorties
    digitalWrite (inter[i], HIGH);    //on définit tous les relais éteints au départ
  }
  
// Définition des pins D2 à D5 (les bouton) comme entrées
  for (char i=0 ; i<sizeof (bouton); i++)
  {
    pinMode (bouton[i], INPUT);
  }


//  pinMode(inter[0], OUTPUT); //Définir la broche du relais comme sortie
  
    Serial.begin(9600); //initialiser la communication série

}

void loop()
{
  
// première partie permettant de selectionner le mode de fonctionnement de la pompe en fonction
// des appuis sur le bouton 1
    if ( etatbouton(databouton1)== 1 )    //si le bouton est réellement appuyé
  {
  
  switch (modecirculation)  //tester la valeur du mode de fonctionnement de la pompe
  {
   case -1:  //si elle est éteinte
   modecirculation=0;  //la passer en automatique
   break;
   
   case 0:  //si elle est en automatique 
   modecirculation=1;  //la passer en allumé forçé
   break;
   
   case 1:  //si elle est forçée allumée
   modecirculation=-1;  //l'éteindre
   break;
   }
  }
  //fin de la première partie
  
  //seconde partie : on allume ou éteint la pompe selon le mode de circulation choisi
  switch (modecirculation)    //selon le mode de fonctionnement choisi
  {
    case -1:    //si elle est forçée éteinte
    digitalWrite (inter[0], HIGH);    //on l'éteint
    break;
    
    case 0:    //si elle est en fonctionnement automatique
    etatcirculation = clignotement (circulation);    //le retour de la fonction clignotement por circulation est stocké dans etatcirculation
    digitalWrite (inter[0] , etatcirculation);        //on met physiquement le relais dans la bonne position
    break;
    
    case 1:    //si elle est forçée allumée
    digitalWrite (inter[0], LOW);    //on l'allume
    break;
  }
    
    
    
    
    
    
}





// début de la fonction de clignotement qui permet d'allumer et d'éteindre la pompe à intervalles réguliers
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



//début de la fonction permettant de lire l'état dun bouton en évitant le phénomène de rebond
long etatbouton (long databouton [])
{ 
   

databouton [2] = digitalRead(databouton [0]);  //lire l'état du bouton

if (databouton [2]==HIGH && databouton [3]==LOW)  //si bouton1 est appuyé et qu'il n'y était pas avant
{
  databouton [3]=HIGH;  //enregistrer son dernier état à appuyé

    databouton [4] = millis();  //et enregistrer le temps (millis) de ce test
}
  
  
  if (databouton [2] == HIGH  &&  databouton [3]==HIGH   &&   millis() - databouton [4] >= databouton [1] )  //si le bouton était et est toujours appuyé et qu'il s'est écoulé plus de 200 millisecondes
  {
  

   
   databouton [3]=LOW;  //remettre le dernier état du bouton à relaché
   return 1;    //on cosidère que le bouton est réellement appuyé donc on envoie 1
  }
  
  else
  {
  return 0;    //on considère que le bouton n'est pas réellement appuyé donc on envoie 0
  }
}
//Fin de la fonction de lecture des boutons




