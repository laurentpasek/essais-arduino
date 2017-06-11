

int capteur =0;// Le capteur est sur la pin A0
// définition des pins sur lesquelles sont branchés les relais
char inter[] = {6, 7, 8, 9};
int mesures [10];    //un tableau por stocker les valeurs lues
int total;    //le total des mesures
float moyenne;  //la valeur moyenne

void setup ()
{
    // Initialisation des relais
  for ( char i = 0; i < sizeof(inter) ; i++ ) {
    // en sortie
    pinMode(inter[i], OUTPUT);
    // éteint au départ
    digitalWrite(inter[i], HIGH);
    
  }

  Serial.begin (9600);// Démarrage de la liaison série


  
}

void loop ()
{
  
  digitalWrite(inter[2],LOW);  //on alimente le module d'ec
  delay (3000);    //on attend 3 secondes
  
  //Serial.println (sizeof(mesures));
  
  // on prend 10 mesures à 200ms d'intervalle
  for (char i =0; i < 10; i++)    
  {
   mesures [i] = analogRead (capteur);
   delay (200);    
   
   Serial.println(mesures[i]);
  }
  

  
  digitalWrite(inter[2],HIGH);  //on eteint le module d'ec
  
  Serial.println();
  
  // calcul du total des mesures
  for ( char i=0; i < (sizeof(mesures)/2) ; i++)    //(sizeof(mesures)/2) car mesures est un tableau composé de valeurs int qui prennent chacun 2 octets
  {
   total = total + mesures [i];
   Serial.println (total);
  }
  
  
  //calcul de la moyenne des mesures
  moyenne = total / (sizeof(mesures)/2);
  total = 0;    // On remet le total à zero pour le prochain calcul
  
  //affichage de toutes les mesures
  for ( char i =0; i < (sizeof(mesures)/2); i++)
  {
   Serial.print (mesures[i]); Serial.print("  ");    
  }
  
    
  // affichage de la moyenne
  Serial.println();
  Serial.print ("moyenne : ");
  Serial.println (moyenne);
  Serial.println();
  
  
  
  delay (10000);    //on attent 10 secondes avant de recommencer
  
  
  
}
