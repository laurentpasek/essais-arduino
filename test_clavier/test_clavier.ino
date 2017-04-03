


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
int etat1;  //variable pour stocker l'état du bouton 1 (appuyé ou non )


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
              
              etat1 = LOW;
              
              
              }


void loop() {
  
  etat1=digitalRead(bouton1); //lire l'état du bouton 1 et le stocker dans etat1
  
  if(etat1 == HIGH)  //si bouton 1 appuyé
  {
    digitalWrite (inter1,LOW);  //allumer le relais 1
  }
  else
  {
    digitalWrite(inter1,HIGH);  //sinon, l'éteindre
  }
  
  
  

//  etat1 = digitalRead(bouton1);
 
  if (etat1==HIGH)  //si bouton 1 appuyé
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
       
  }

    
  delay(700);
Serial.print("etat : ");  //afficher l'état de la pompe1
Serial.println(pompe1);

Serial.print ("bouton : ");  //afficher la valeur de etat1
Serial.println (etat1);

  Serial.print (digitalRead(2));  //afficher l'état des 4 pins correspondant aux boutons
  Serial.print (digitalRead(3));
  Serial.print (digitalRead(4));
  Serial.println (digitalRead(5));
  
Serial.print (digitalRead(bouton1));  //afficher l'état des 4 boutons
Serial.print (digitalRead(bouton2));
Serial.print (digitalRead(bouton3));
Serial.print (digitalRead(bouton4));
  
  
  Serial.println();
  
  
}
