    #include <Wire.h>
    // Adresse de l'esclave
    #define SLAVE_ADDRESS 0x44
    int number = 0;
    int state = 0;
    void setup() {
      pinMode(13, OUTPUT);
      // Ouverture du port série
      Serial.begin(115200);
      // Initialisation du bus i2c en mode esclave
      Wire.begin(SLAVE_ADDRESS);
      // Définition des méthodes de Callback pour la communication i2c
      Wire.onReceive(receiveData);
      Wire.onRequest(sendData);
      Serial.println("Ready!");
    }
    void loop() {
      delay(100);
    }
    // Callback en réception de données
    void receiveData(int byteCount)
    {
      while(Wire.available())
      {
        number = Wire.read();
        Serial.print("data received: ");
        Serial.println(number);
      // On utilise le chiffre 1 pour switcher l'allumage de la LED
        if (number == 1)
        {
          if (state == 0)
          {
            digitalWrite(13, HIGH); // Allume la LED
            state = 1;
          }
          else
          {
            digitalWrite(13, LOW); // Eteint la LED
            state = 0;
          }
        }
      }
    }
    // Callback pour l'envoi de données
    void sendData()
    {
      // Envoie le dernier octet reçu
      Wire.write(number);
    }
