



// définition des pins sur lesquelles sont branchés les relais
/*
const int inter1 = 6;
const int inter2 = 7;
const int inter3 = 8;
const int inter4 = 9;
*/
char inter[] = {6,7,8,9};


//définition des pins sur lesquels sont branchés les boutons
/*
const int bouton1 = 2;
const int bouton2 = 3;
const int bouton3 = 4;
const int bouton4 = 5;
*/
char bouton[] = {2,3,4,5};



int pompecirculation = 0;  //variable pour stocker le mode de fonctionnement de la pompe de circulation (-1=off ; 0=auto ; 1=on )

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


// the setup routine runs once when you press reset:
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

  
  //activation de l'interface série
  Serial.begin(9600);
              
     
              }


void loop() {
  
  
  
  if ( etatbouton(databouton1)== 1 )    //si le bouton est réellement appuyé
  {
  
  switch (pompecirculation)  //tester la valeur du mode de fonctionnement de la pompe
  {
   case -1:  //si elle est éteinte
   pompecirculation=0;  //la passer en automatique
   break;
   
   case 0:  //si elle est en automatique 
   pompecirculation=1;  //la passer en allumé forçé
   break;
   
   case 1:  //si elle est forçée allumée
   pompecirculation=-1;  //l'éteindre
   break;
   }
  } 
  
  
  // écritures sur le moniteur série pour déboggage
  
  
  // on écrit le mode de fonctionnement de la pompe
  
  switch (pompecirculation)
{
  case -1:  //si elle est forcée éteinte
  Serial.println ("off");  //écrire "off"
  break;
  
  case 0:  //si elle est en automatique
  Serial.println ("automatique");  //écrire automatique
  break;
  
  case 1:  //si elle est forcée allumée
  Serial.println ("on");  //écrire "on"
  break;
}

// On écrit l'état des différents boutons
 Serial.print ("bouton1 : "); Serial.print ( etatbouton(databouton1));Serial.print ("    ");
 Serial.print ("bouton2 : "); Serial.print ( etatbouton(databouton2));Serial.print ("    ");
 Serial.print ("bouton3 : "); Serial.print ( etatbouton(databouton3));Serial.print ("    ");
 Serial.print ("bouton4 : "); Serial.print ( etatbouton(databouton4));Serial.print ("    ");
 Serial.println();
 Serial.println();
 
 delay (300);    //on attend 300ms


  
}
  
  
  
  
  
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
  




