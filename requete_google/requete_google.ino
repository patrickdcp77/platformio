/*
  Nom sketch : phm_WebClient.ino
  Description : Exemple de Web client qui se connecte au site Google pour faire par exemple dans
  le cas présent une recherche sur le mot clé Arduino. Ce sketch s'inspire de l'exemple
  standard de David A. Mellis
  Auteur : PHMARDUINO
  Date modification : 30 07 2013
*/

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x65, 0x6F }; // MAC adresse de mon arduino ethernet
IPAddress server(173,194,67,147); // Initialisation de l'objet server avec l'adresse IP du site
                                  // Google (à vérifier et à modifier éventuellement car 
                                  // car celle-ci peut evoluer)

// Initialisation objet client
EthernetClient client;

void setup() {
  // Initialisation communication série
  Serial.begin(9600);
  Serial.println("Envoi demande de configuration DHCP\n");
  // Initialisation communication Ethernet
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Echec demande configuration DHCP\n");
    // Fin des traitements
    for(;;)
      ;
  }
  Serial.println("Configuration DHCP OK\n");
  // Affichage adresse IP Arduino
  Serial.print("Adresse IP obtenue : ");
  Serial.println(Ethernet.localIP());
  Serial.println();
  // Laisse une seconde au shield Ethernet pour initialisation
  delay(1000);
  Serial.println("Connexion en cours...\n");

  // Connexion obtenue
  if (client.connect(server, 80)) {
    Serial.println("Connexion OK\n");
    // Envoi de la requete HTTP
   Serial.println("Connection à l'api Yahoo Weather...");
    client.println("GET /weather.yahoo.com/france/ile-de-france/cesson-583732 HTTP/1.0");
    //client.println("HOST:weather.yahooapis.com\n\n");
    client.println();
    Serial.println("Connection OK...");
  } 
  else {
    // Echec connexion au serveur Google
    Serial.println("Echec connexion");
  }
}

void loop()
{
  // Lecture resultat requete
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // Arret client a la deconnexion du serveur
  if (!client.connected()) {
    Serial.println();
    Serial.println("Deconnexion\n");
    client.stop();

    // Arret des traitements
    for(;;)
      ;
  }
}

