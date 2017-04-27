// Capteur de niveau d'eau
// letmeknow.fr

int capteur =0;// Le capteur est sur la pin A0
float val =0;// Variable de stockage de la valeur lue
float voltagemap =6;  //valeur lue monvertie en volts
float voltcalc=6;

void setup ()
{
Serial.begin (9600);// Démarrage de la liaison série
}

void loop ()
{
val = analogRead (capteur);// Lecture de la valeur du capteur
//voltagemap = map (val, 0, 1023, 0.00, 5.00);  //convertit la valeur en volts et la stocke
voltcalc = (val * 5) / 1023;
Serial.println (val);// Affichage de la valeur
//Serial.println (voltagemap);  //affiche le voltage
Serial.println (voltcalc);
Serial.println ();
delay (500);// Attendre 1s avant la prochaine lecture
}
