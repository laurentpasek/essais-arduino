// Capteur de niveau d'eau
// letmeknow.fr

int capteur =0;// Le capteur est sur la pin A0
float mesure =0;// Variable de stockage de la valeur lue
//float voltagemap =6;  //valeur lue monvertie en volts
float tension=6;  //valeur lue convertie en volts
float conductance=0;
float conductivite=0;

void setup ()
{
Serial.begin (9600);// Démarrage de la liaison série
}

void loop ()
{
mesure = analogRead (capteur);// Lecture de la valeur du capteur
//voltagemap = map (val, 0, 1023, 0.00, 5.00);  //convertit la valeur en volts et la stocke
tension = (mesure * 5) / 1023;
conductance= (5-tension)/(tension*4066);
conductivite= (conductance*1000000000);


Serial.println (mesure);// Affichage de la valeur
//Serial.println (voltagemap);  //affiche le voltage
Serial.println (tension);
Serial.println (conductance);
Serial.println (conductivite);
Serial.println ();
delay (500);// Attendre 1s avant la prochaine lecture
}
