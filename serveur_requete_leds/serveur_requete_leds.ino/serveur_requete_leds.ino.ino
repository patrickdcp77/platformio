
// --- Programme Arduino --- 
// Code généré par le générateur de code Arduino
// du site www.mon-club-elec.fr 
// Auteur : X. HINAULT - Tous droits réservés 
// 16 octobre 2010

// ------- Licence du code de ce programme ----- 
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License,
//  or any later version.
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <http://www.gnu.org/licenses/>.

//*************** PRESENTATION DU PROGRAMME *************** 

// -------- Que fait ce programme ? ---------
 /* Commande 7 LEDs sur une carte Arduino
couplée à un module Ethernet
configurée en serveur HTTP
à partir d'un navigateur client sur le PC

La connexion série est utilisée sur le PC connecté au serveur
pour visualiser le protocole HTTP utilisé
et les échanges Serveur/client

Les chaines de caractères constantes
sont stockées en mémoire flash programme
en raison du grand nombre de chaines utilisées

Programme très didactique pour comprendre
ce qui se passe entre le serveur et le client

 */ 

// --- Fonctionnalités utilisées --- 

// Utilise la connexion série vers le PC 
// Utilise le module Ethernet Arduino 
// en mode serveur HTTP 

// utilise la librairie Flash de stockage en mémoire programme FLASH

// -------- Circuit à réaliser --------- 

// La connexion série vers le PC utilise les broches 0 et 1 (via le câble USB) 

// Le module Ethernet est à enficher broche à broche sur la carte Arduino 

//****************** Entête déclarative **************** 
// A ce niveau sont déclarées les librairies incluses, les constantes, les variables...

// --- Déclaration des constantes ---

// --- Inclusion des librairies ---

//-- librairies utilisées pour le module Ethernet
#include <SPI.h>
#include <Ethernet.h>
#include <Server.h>
#include <Client.h>

// librairie pour stockage en mémoire flash programme
#include <Flash.h> 
// attention : l'utilisation de F("chaine") necessite modification
// du fichier print.h
// voir : www.mon-club-elec.fr/pmwiki_reference_arduino/pmwiki.php?n=Main.LibrairieFlashProgramme


// --- Déclaration des constantes des broches E/S numériques ---

const int LED[7]={2,3,5,6,7,8,9}; // Constante pour les broches
//--- la broche 4 est utilisée par le module ethernet

// --- Déclaration des constantes des broches analogiques ---


// --- Déclaration des variables globales ---

//--- déclaration du tableau d'adresse MAC ---
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // valeur arbitraire

//---- tableau de l'adresse IP de la carte Arduino
byte ip[] = { 192,168,1,30 }; // le PC a pour IP : 192.168.1.30

//----- tableau de l'adresse de la passerelle --- 
byte passerelle[] = { 192, 168, 1, 1 }; // l'adresse du PC de connexion ou de la Box

//----- tableau du masque de sous réseau 
byte masque[] = { 255, 255, 255, 0 }; // idem masque sous-réseau du PC : 255.255.255.0

// variables globales utiles 
String chaineRecue=""; // déclare un string vide global
int comptChar=0; // variable de comptage des caractères reçus 

//------ tableau de String ---- 
// variables globales de mémorisation de l'état d'une case à cocher "" ou "checked"
String etatCase[7]={ "","","","","","",""}; 

// --- Déclaration des objets utiles pour les fonctionnalités utilisées ---

//--- création de l'objet serveur ----
Server serveurHTTP(80); // crée un objet serveur utilisant le port 80 = port HTTP


//**************** FONCTION SETUP = Code d'initialisation *****
// La fonction setup() est exécutée en premier et 1 seule fois, au démarrage du programme

void setup()   { // debut de la fonction setup()

// --- ici instructions à exécuter 1 seule fois au démarrage du programme --- 

// ------- Initialisation fonctionnalités utilisées -------  

Serial.begin(115200); // initialise connexion série à 115200 bauds
// IMPORTANT : régler le terminal côté PC avec la même valeur de transmission 

//---- initialise la connexion Ethernet avec l'adresse MAC, l'adresse IP et le masque
Ethernet.begin(mac, ip, passerelle, masque);

//---- initialise le serveur HTTP----
serveurHTTP.begin(); //---- initialise le serveur HTTP


// ------- Broches en sorties numériques -------  
for (int i=0; i<=6; i++) {
   pinMode (LED[i],OUTPUT); // Broche LED configurée en sortie
}

// ------- Broches en entrées numériques -------  

// ------- Activation si besoin du rappel au + (pullup) des broches en entrées numériques -------  

} // fin de la fonction setup()
// ********************************************************************************

//*************** FONCTION LOOP = Boucle sans fin = coeur du programme *************
// la fonction loop() s'exécute sans fin en boucle aussi longtemps que l'Arduino est sous tension

void loop(){ // debut de la fonction loop()


// --- ici instructions à exécuter par le programme principal --- 

//---- Code type pour une connexion d'un client au serveur --- 

// crée un objet client basé sur le client connecté au serveur HTTP
  while(!serveurHTTP.available()); // attend qu'un client soit disponible

  // si un client est disponible, création de l'objet client correspondant
  Client client = serveurHTTP.available();

  if (client) { // si l'objet client n'est pas vide = si le client existe
    Serial.println(F("------------ Connexion Client ------------")); // message début de connexion
    Serial.println (F("")); 

     Serial.println(F("Detection client...")); // message debug 

    //-- initialisation des variables utilisées pour l'échange serveur/client
    chaineRecue=""; // vide le String de reception 
    comptChar=0; // compteur de caractères en réception à 0  

    if (client.connected()) { // si le client est connecté

         Serial.println(F("Connexion avec client OK ... ")); // message debug 
         Serial.println(F("")); // message debug 


//////////////// Reception de la requete envoyée par le client //////////////////
    Serial.println(F("------------ Reception de la requete Client ------------")); // affiche le String de la requete
    Serial.println (F("")); 

      while (client.available()) { // tant que des octets sont disponibles en lecture

                char c = client.read(); // lit l'octet suivant reçu du client (pour vider le buffer au fur à mesure !)
                comptChar=comptChar+1; // incrémente le compteur de caractère reçus

                //--- on ne mémorise que les n premiers caractères de la requete reçue
                //--- afin de ne pas surcharger la RAM et car cela suffit pour l'analyse de la requete
                if (comptChar<=100) chaineRecue=chaineRecue+c; // ajoute le caractère reçu au String pour les N premiers caractères 

                Serial.print(c); // message debug - affiche la requete entiere reçue

      } // --- fin while client.available


       // si plus de caractères disponibles = la requete client terminée
       Serial.println(F("Reception requete client terminee... ")); // message debug 
       Serial.println(F("")); // message debug 


/////////////////// Analyse de la requete reçue //////////////////////

    Serial.println(F("------------ Analyse de la requete recue ------------")); // affiche le String de la requete
    Serial.println (F("")); 

    Serial.print (F("Chaine prise en compte pour analyse : ")); 
    Serial.println(chaineRecue); // affiche le String de la requete pris en compte pour analyse
    Serial.println (""); 


    // variables pour analyse de la requete reçue 
    String chaineAnalyse=""; // crée un string utilisé pour analyser la requete
    int indexChaine=0; // variable index pour parcourir la chaine reçue

    //----- longeur de la chaine recue
    int longeurChaine=chaineRecue.length(); // variable de la longeur de la chaine reçue
    Serial.print(F("La longueur de la chaine est : ")), Serial.println(longeurChaine); 

    //------ type de la requete reçue --------
    if (chaineRecue.startsWith("GET")) { // si la chaine recue commence par "GET"
     Serial.println (F("La requete recue est une GET")); 
    }

    //------------- analyse si présence données formulaire --------- 

    //-- analyse la présence du ? => présent si appui sur bouton envoi coté client
    indexChaine=5; // analyse le 6ème caractère
    chaineAnalyse=chaineRecue.substring(indexChaine,indexChaine+1); // extrait le 6ème caractère 
    Serial.print(F("Le 6eme caractere est : ")), Serial.println(chaineAnalyse);

    if (chaineAnalyse == "?") { // test si le 6ème caractère est un ? 

      Serial.println(F("Donnees de formulaire disponibles !")); // si c'est le cas, des données de formulaire sont disponibles


      // *****************  analyse des données de formulaires disponibles ************
      // ---- l'analyse des éléments de formulaire reçus se base sur le formulaire HTML envoyé au client ci-dessous      


      // ------------- recherche du champ de case à cocher nommé case=----------------
      // le plus simple est de chercher la chaine "caseLED=ON" sans tenir compte de l'index
      // car cette chaine est fixe si le case est cochée

  for (int i=0; i<=6; i++) { // analyse pour les 6 LEDs

      chaineAnalyse="L"+String(i+1)+"=ON"; 

      //Serial.println(chaineRecue.indexOf(chaineAnalyse)); // affiche l'index de la chaine recherchée - debug

      if (chaineRecue.indexOf(chaineAnalyse)!=-1){ // si la chaine recherchee a été trouvée

        etatCase[i]="checked"; // mémorise valeur HTML de la case cochée
        Serial.print(F("Etat de la case a cocher ")), Serial.print(i+1),Serial.print(F(" : ON => ")); 

        digitalWrite(LED[i],HIGH); // allume la LED
        Serial.print(F("LED ")), Serial.print(i+1), Serial.println(F(" allumee")); 

      } // fin if chaineRecue.indexOf

      else { // debut else chaineRecue.indexOf

        etatCase[i] =""; // mémorise valeur HTML bouton non coché
        Serial.print(F("Etat de la case a cocher ")), Serial.print(i+1),Serial.print((" : OFF => ")); 

        digitalWrite(LED[i],LOW); // eteint la LED
        Serial.print(F("LED ")), Serial.print(i+1), Serial.println(F(" eteinte")); 

      } // fin else  chaineRecue.indexOf

  } // fin for i 


    } // fin if Chaineanalyse==?

    else { // debut chaineAnalyse == "?"

      Serial.println(F("Aucune donnee de formulaire disponible !"));

    } // fin else chaineAnalyse == "?"

       Serial.println(F("")); // message debug 


//////////// ENVOI DE LA REPONSE DU SERVEUR ///////////////

       Serial.println(F("Analyse requete terminee...")); // message debug 
       Serial.println(F("")); // message debug 

    Serial.println(F("------------ Envoi de la reponse au client ------------")); // affiche le String de la requete
    Serial.println (F("")); 

       Serial.println(F("Envoi de la reponse HTTP au client...")); // message debug 

        // envoi d'une entete standard de réponse http
          client.println(F("HTTP/1.1 200 OK"));
          client.println(F("Content-Type: text/html"));
          client.println(F("Connection: close")); // indique au client que la connexion est fermée après réponse
          // à noter que la connexion est persistante par défaut coté client en l'absence de cette ligne          
          client.println(); // ligne blanche obligatoire après l'entete HTTP envoyée par le serveur

       Serial.println(F("Envoi de la reponse HTML au client...")); // message debug 
       Serial.println(F("")); // message debug 

        // envoi du code HTML de la page 

             //---- debut de la page HTML --- 
             client.println(F("<html>")); 

             // ---- Entete de la page HTML ---- 
             client.println(F("<head>"));

               //client.println("<meta content=\"text/html; charset=ISO-8859-1\" http-equiv=\"Content-Type\">"); 
               client.println(F("<title>Connexion au Serveur Arduino</title>")); 

               // balise meta pour réactualisation automatique de la page Web toutes les n secondes
               //client.println("<META HTTP-EQUIV=\"Refresh\" CONTENT=\"1\">");

             client.println(F("</head>"));

             //----- Corps de la page HTML ---
             // body avec fond coloré
             client.println(F("<body style=\"color: rgb(0, 0, 255); background-color: rgb(255, 255, 255);\">")); 

             client.println(F("<br>"));

             client.println(F("<center>")); // pour centrer la page HTML
             client.println(F("************************************************* <br> ")); 
             client.println(F("Bienvenue sur le serveur Arduino <br>")); 
             client.println(F("************************************************* <br>")); 
             client.println(F("<br>"));

             // intègre une image - suppose connexion internet disponible 
             //client.println("<CENTER> <img src=\"http://www.arduino.cc/mes_images/clipart/logo_arduino_150.gif\"> </CENTER>");
             client.println(F("<CENTER> <img src=\"http://www.arduino.cc/mes_images/communs/led_rouge_5mm.gif\"> </CENTER>"));

             client.println(F("<br>"));


      for (int i=0; i<=6; i++) { // pour les 6 LEDs     

             if (etatCase[i]=="checked") client.print(F("LED ")), client.print(i+1), client.println(F(" allumee <br>")); 
             if (etatCase[i]=="")client.print(F("LED ")), client.print(i+1), client.println(F(" eteinte <br>"));  

      } // fin for i 

             //------------- génération du formulaire ---------------- 

             client.println(F("<FORM method=\"get\" action=\"http://192.168.1.30/\">")); // debut du formulaire - utilise méthode Get
             client.println(F("<P>")); // balise paragraphe

             // intégration d'un champ texte invisible pour obtenir un ? à l'appui sur le bouton submit
             client.println(F("<INPUT type=\"text\" style=\"display:none\" name=\"vide\" value=\"\" <br>"));


        for (int i=0; i<=6; i++) { // pour les 6 LEDs    

             client.print(F("<INPUT type=\"checkbox\" name=\"L")),client.print(i+1), client.print(F("\" value=\"ON\" "));
             client.print(etatCase[i]); // pour utiliser valeur courante de l'etat du bouton (mémorise valeur précédente)
             // etatCase vaut "" ou "checked" 
             client.print(F("> Allume/Eteint la LED ")),client.print(i+1), client.println(F(" connectee au serveur <BR>"));

        } // fin boucle for i 


             client.println(F("<INPUT type=\"submit\" value=\"envoi\"> "));
             client.println(F("</P>"));// balise fin paragraphe   
             client.println(F("<br>"));

             client.println(F("</center>"));

             client.println(F("</FORM>"));// ne pas oublier ++ 


             client.println(F("<br>"));


             client.println(F("</body>"));

             //---- fin de la page HTML
             client.println(F("</html>"));





    } // --- fin if client connected

    delay(1);
    // on donne au navigateur le temps de recevoir les données

    // fermeture de la connexion avec le client après envoi réponse
    client.stop(); 

    Serial.println(F("------------ Fermeture de la connexion avec le client ------------")); // affiche le String de la requete
    Serial.println (F("")); 

    Serial.println(F("")); // message debug 
    Serial.println(F("Fin de la connexion avec le client")); // message debug 


    Serial.println(F("Fin existence client")); // message debug 
    Serial.println(F("")); // message debug 

  } //---- fin if client existe ---- 



} // fin de la fonction loop() - le programme recommence au début de la fonction loop sans fin
// ********************************************************************************


 
