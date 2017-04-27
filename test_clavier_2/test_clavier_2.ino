


// définition des pins sur lesquelles sont branchés les relais
const int inter1 = 6;
const int inter2 = 7;
const int inter3 = 8;
const int inter4 = 9;

//définition des pins sur lesquels sont branchés les boutons
const int bouton1 = 2;
const int bouton2 = 3;
const int bouton3 = 4;
const int bouton4 = 5;



int pompe1 = 0;  //variable pour stocker l'état de la pompe (-1=off ; 0=auto ; 1=on )

/*
la variable databouton1 stocke les données relatives au bouton 1 avec :
[0] : le délai entre deux conparaisons de son état
[1] : son état actuel
[2] : son dernier état
[3] : le temps (millis) de la dernière mesure de son état
*/
long databouton1 []={200,0,0,0};


long time;  // une variable pour stocker la valeur de millis

// the setup routine runs once when you press reset:
void setup() {                
  // Définition des pins D6 à D9 (les relais ) comme sortie
  pinMode(inter1, OUTPUT);
  pinMode(inter2, OUTPUT);
  pinMode(inter3, OUTPUT);
  pinMode(inter4, OUTPUT);
  
  //définition des pins D2 à D5 (les boutons) comme entrées
  pinMode(bouton1, INPUT);
  pinMode(bouton2, INPUT);
  pinMode(bouton3, INPUT);
  pinMode(bouton4, INPUT);
  
  // Définir tous les relais éteints pour commencer
  digitalWrite(inter1, HIGH);
  digitalWrite(inter2, HIGH);
  digitalWrite(inter3, HIGH);
  digitalWrite(inter4, HIGH);
  
  //activation de l'interface série
  Serial.begin(9600);
              
  databouton1 [1] = LOW;
              
              
              }


void loop() {
  
  databouton1 [1]=digitalRead(bouton1); //lire l'état du bouton 1 et le stocker dans databouton1 [1]
  
  if(databouton1 [1] == HIGH)  //si bouton 1 appuyé
  {
    digitalWrite (inter1,LOW);  //allumer le relais 1
  }
  else
  {
    digitalWrite(inter1,HIGH);  //sinon, l'éteindre
  }
  
  
  

databouton1 [1] = digitalRead(bouton1);  //lire l'état du bouton
time = millis();  //enregistrer la valeur de millis

if (databouton1 [1]==HIGH && databouton1 [2]==LOW)  //si bouton1 est appuyé et qu'il n'y était pas avant
{
  databouton1 [2]=HIGH;  //enregistrer son dernier état à appuyé
  databouton1 [3] = time;  //et enregistrer le temps (millis) de ce test
}
  
  
  if (databouton1 [1] == HIGH  &&  databouton1 [2]==HIGH   &&   time - databouton1 [3] >= 200)  //si le bouton était et est toujours appuyé et qu'il s'est écoulé plus de 200 millisecondes
  {
  

  switch (pompe1)  //tester la valeur du mode de fonctionnement de la pompe
  {
   case -1:  //si elle est éteinte
   pompe1=0;  //la passer en automatique
   break;
   
   case 0:  //si elle est en automatique 
   pompe1=1;  //la passer en allumé forçé
   break;
   
   case 1:  //si elle est forçée allumée
   pompe1=-1;  //l'éteindre
   break;
   } 
   databouton1 [2]=LOW;  //remettre le dernier état du bouton à relaché
  }



switch (pompe1)
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

  
}
