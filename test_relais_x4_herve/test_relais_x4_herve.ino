/*
  Test des relais
  Les relais sont tous éteints au démarrage
  puis s'allument l'un après l'autre alors que les précédents s'éteignent
 */
 
// définition des pins sur lesquelles sont branchés les relais
char inter[] = {6, 7, 8, 9};

// the setup routine runs once when you press reset:
void setup() {
  char i;  
  // Initialisation des relais
  for ( i = 0; i < sizeof(inter) ; i++ ) {
    // en sortie
    pinMode(inter[i], OUTPUT);
    // éteint au départ
    digitalWrite(inter[i], HIGH);
  }
  Serial.begin (9600);// Démarrage de la liaison série
}

/*
** Fonction one_up
**  éteint toutes les sorties, en allume UNE
**  paramètres:
**  - inter_on = le numéro de la sortie à allumer
*/
void one_up(char inter_on) {
  char i;
  // on éteint tous les relais
  for ( i = 0; i < sizeof(inter) ; i++ ) {
    digitalWrite(inter[i], HIGH);
  }
  // on allume le relais demandé
  digitalWrite(inter_on, LOW);
}

// the loop routine runs over and over again forever:
void loop() {
  char i;
  // définit la séquence d'allumage des sorties
  char chenillard[] = {0, 1, 2, 3, 2, 1, 0, 2, 3, 1};
  
  // on parcourt le tableau chenillard
  for ( i = 0; i < sizeof(chenillard) ; i++ ) {
    // allumer la sortie indiquée dans le tableau chenillard
    one_up(inter[chenillard[i]]);
    delay(1000);
  }
}
