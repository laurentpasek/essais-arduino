 
// définition des pins sur lesquelles sont branchés les relais
char inter[] = {6, 7, 8, 9};


boolean circuetat;  //pompe allumée ou éteinte
long circurepos;  //durée pompe de circulation arretée
long circufonction;  //durée pompe de circulation allumée
long circureposlast;  //temps de départ de circurepos
long circufonctionlast;  //temps de départ de circufonction

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

circurepos = 3000;  //durée au repos
circufonction = 1000;  //durée de circulation
circuetat = 0; //on déclare la pompe allumée au départ
circureposlast = millis();  //on initialise le temps de repos
circufonctionlast = millis();  //on initialise le temps de fonctionnement
}

//définition de la fonction de calcul de temps écoulé
long delai ( long delaiacalculer )
{
  delai = millis() - delaiacalculer;
  return delai;
}
 

// the loop routine runs over and over again forever:
void loop() {

  switch (circuetat)  //on teste si la pompe est en fonction
  {
    case 0:  //si elle est allumée
    if (delai (circufonctionlast) >= circufonction)  //et que le délai de fonctionnement est écoulé
    {
      circuetat = 1;  //on eteint la pompe
      digitalWrite (inter[0] , circuetat);  //en éteignant le relais correspondant
    }
    
    case 1:  //si elle est éteinte
    if (delai (circureposlast) >= circurepos)  //et que le délais de repos est écoulé
    {
      circuetat = 0;  //on éteint la pompe
      digitalWrite (inter[0] , circuetat);
    }
  }
    
      
  
  
}
