


// définition des pins sur lesquelles sont branchés les relais
int inter1 = 6;
int inter2 = 7;
int inter3 = 8;
int inter4 = 9;

//définition des pins sur lesquels sont branchés les boutons
int bouton1 = 2;
int bouton2 = 3;
int bouton3 = 4;
int bouton4 = 5;


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
              
              }


void loop() {
  
  
}
