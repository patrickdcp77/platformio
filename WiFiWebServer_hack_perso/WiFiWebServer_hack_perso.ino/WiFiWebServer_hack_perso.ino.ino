#include <ESP8266WiFi.h>

const char* ssid = "maison";
const char* password = "b5c84f1cec";

// Créé le serveur Web en spécifiant le port TCP/IP
// 80 est le port par défaut pour HTTP
WiFiServer server(80);

// démarrage
void setup() {

pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output

  
  // communication série 115200
  Serial.begin(115200);
  // petit pause
  delay(10);

  // GPIO2 en sortie
  pinMode(2, OUTPUT);
  // GPIO2 à la masse
  digitalWrite(2, 0);
  
  // Deux sauts de ligne pour faire le ménage car
  // le module au démarrage envoi des caractères sur le port série
  Serial.println();
  Serial.println();
  Serial.print("Connexion a : ");
  Serial.println(ssid);
  Serial.print("ceci est ma box !!!");
  
  // Connexion au point d'accès
  WiFi.begin(ssid, password);
  
  // On boucle en attendant une connexion
  // Si l'état est WL_CONNECTED la connexion est acceptée
  // et on a obtenu une adresse IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Démarrage du serveur Web
  server.begin();
  Serial.println("Server started");

  // On affiche notre adresse IP
  Serial.println(WiFi.localIP());
}

// boucle principale
void loop() {
  // Est-ce qu'un client Web est connecté ?
  WiFiClient client = server.available();
  if (!client) {
    // non, on abandonne ici et on repart dans un tour de loop
    return;
  }
  
  // Un client est connecté
  Serial.println("new client");
  // On attend qu'il envoi des données
  while(!client.available()){
    delay(1);
  }
  
  // On récupère la ligne qu'il envoi jusqu'au premier retour chariot (CR)
  String req = client.readStringUntil('\r');
  // On affiche la ligne obtenue pour information
  Serial.println(req);
  // La ligne est récupérée, on purge 
  client.flush();
  
  // Test de la requête
  int val;
  // est-ce que le chemin dans la requête est "/gpio/0" ?
  if (req.indexOf("/gpio/0") != -1)
    // oui, ceci correspond à "0" (OFF)
    val = 0;
  // non, est-ce que le chemin est "/gpio/1" ?
  else if (req.indexOf("/gpio/1") != -1)
    // oui, ceci correspond à "1" (ON)
    val = 1;


//clignotant_bleu()


    
  // C'est un autre chemin qui est demandé et il ne correspond à rien
  else {
    // On signal que cette demande est invalide
    Serial.println("invalid request");
    // et on déconnecte le client du serveur Web
    client.stop();
    // et on repart dans un tour de boucle
    return;
  }

  // Nous avons obtenu l'état demandé pour la sortie GPIO2
  // Nous pouvons écrire l'état de la sortie
  digitalWrite(2, val);

  // Peut-être le client a-t-il envoyé d'autres données,
  // mais nous purgeons la mémoire avant d'envoyer notre réponse
  client.flush();

  // Composition de la réponse à envoyer au client
  // On commence avec le texte d'une réponse HTTP 200 standard et un début de page HTML
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nLa sortie est a l'etat ";
  // puis on complète avec "haut" ou "bas" en fonction de la la valeur de val
  s += (val)?"haut":"bas";
  // et on termine la page HTML
  s += "</html>\n";

  // On envoi la réponse au client
  client.print(s);
  // petite pause
  delay(1);
  // et on le déconnecte du serveur Web
  Serial.println("Client disonnected");

  // client.stop() n'est pas utilisé ici,
  // le client est automatiquement déconnecté à la fin de la fonction loop(),
  // lorsque l'objet "client" est détruit, car la déclaration est locale.
}


void clignotant_bleu() {
  digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
                                    // but actually the LED is on; this is because 
                                    // it is acive low on the ESP-01)
  delay(1000);                      // Wait for a second
  digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)
}
