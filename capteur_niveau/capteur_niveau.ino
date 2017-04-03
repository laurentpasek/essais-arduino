// Capteur de niveau d'eau
// letmeknow.fr

int capteur =0;// Le capteur est sur la pin A0
int val =0;// Variable de stockage de la valeur lue

void setup ()
{
Serial.begin (9600);// Démarrage de la liaison série
}

void loop ()
{
val = analogRead (capteur);// Lecture de la valeur du capteur
Serial.println (val);// Affichage de la valeur
delay (1000);// Attendre 1s avant la prochaine lecture
}
