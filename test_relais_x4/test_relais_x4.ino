/*
  Test des relais
  Les relais sont tous éteints au démarrage
  puis s'allument l'un après l'autre alors que les précédents s'éteignent
 */
 
// définition des pins sur lesquelles sont branchés les relais
int inter1 = 6;
int inter2 = 7;
int inter3 = 8;
int inter4 = 9;

// the setup routine runs once when you press reset:
void setup() {                
  // Définition des pins D6 à D9 comme sortie
  pinMode(inter1, OUTPUT);
  pinMode(inter2, OUTPUT);
  pinMode(inter3, OUTPUT);
  pinMode(inter4, OUTPUT);
  
  // Définir tous les relais éteints pour commencer
    digitalWrite(inter1, HIGH);
    digitalWrite(inter2, HIGH);
    digitalWrite(inter3, HIGH);
    digitalWrite(inter4, HIGH);
  
}

// the loop routine runs over and over again forever:
void loop() {
  
  digitalWrite(inter1, LOW);   // allume le relais 1
  delay(1000);               // wait for a second
  digitalWrite(inter1, HIGH);  // éteint le relais 1
  digitalWrite(inter2, LOW);  // allume le relais 2
  delay(1000);               // wait for a second
    digitalWrite(inter2, HIGH);  // éteint le relais 2
  digitalWrite(inter3, LOW);  // allume le relais 3
  delay(1000);               // wait for a second
    digitalWrite(inter3, HIGH);  // éteint le relais 3
  digitalWrite(inter4, LOW);  // allume le relais 4
  delay(1000);               // wait for a second
    digitalWrite(inter4, HIGH);  // éteint le relais 4
  
}
