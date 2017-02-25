//application : Ethernet
//version : 0_02
//créé 2014/09/07 - maj 2014/09/08

//Cette application est basée sur Ethernet V0.01

//imports
#include <SPI.h>
#include <Ethernet.h>


//constantes globales
const int pinLed = 13;
const String appNom = "Ethernet V0_02";
const int nbParam = 5;


//variables globales
//pour les paramètres
String tabParamCode[nbParam];
String tabParamValeur[nbParam];
//pour le découpage des chaînes
String tab[10];
int nbTab = 0;
//pour debug
String tabDebug[6];
int nbTabDebug = 0;


String chaineRecue = "";
int etatChaineRecue = 0;
boolean echappe = false;

byte mac[] = {0x90, 0xA2, 0xDA, 0x07, 0x00, 0xA1};
EthernetServer server(80);


void setup()
{
    Serial.begin(9600);
    
    Serial.println(F("Oh yeah !!"));
    pinMode(pinLed, OUTPUT);
    chargerJeuParamDefaut();
    
    Ethernet.begin(mac);    //DHCP
    server.begin();
    Serial.println(F("Le serveur est a l'adresse : "));
    Serial.print(Ethernet.localIP());
    
    //TabDebug
    //initialiser le tableau (variable globale) qui contiendra toutes les valeurs
    TabDebugIni();
}


void loop() {
    if ( Serial.available()) {
        char entree = Serial.read();
        //Serial.println("donnee recue : " + String(entree));
        
        if (entree == '<' && etatChaineRecue == 0) {
            //chaineRecue = String(entree);
            etatChaineRecue = 1;
        } else if (etatChaineRecue == 1) {
            if (entree == '/') {
                //le caractère actuel est un slash, donc le prochain sera échappé
                echappe = true;
            } else {
                if ((entree != '<' && entree != '>') && echappe == true) {
                    //le caractère est échappé et n'est ni < ni > : mettre le slash d'échappement (car. préc.)
                    chaineRecue += "/" + String(entree);
                } else if ((entree == '<' || entree == '>') && echappe == true) {
                    //le caractère est échappé et est < ou > : zapper le slash d'échappement (car. préc.)
                    chaineRecue += String(entree);
                } else if (entree == '<' && echappe == false) {
                    //le caractère est < et n'est pas échappé : début de la chaîne suivante, ne pas traiter la chaîne actuelle
                    chaineRecue = "";
                    etatChaineRecue = 0;
                } else if (entree == '>' && echappe == false) {
                    //le caractère est > et n'est pas échappé : fin de la chaîne
                    etatChaineRecue = 2;
                } else {
                    chaineRecue += String(entree);
                }
                //le caractère actuel n'est pas un slash, donc le prochain ne sera pas échappé
                echappe = false;
            }
        }
        
        if (etatChaineRecue == 2) {
            //si on a reçu une instruction entière, la traiter et réinitialiser var. globales
            Serial.println("appel lireChaineRecue, chaineRecue : " + chaineRecue);
            
            //TabDebug
            TabDebugAjout(chaineRecue);
            
            lireChaineRecue(chaineRecue);
            //Serial.println("chaineRecue : " + chaineRecue);
            chaineRecue = "";
            etatChaineRecue = 0;
        }

    }
    
    
    EthernetClient client = server.available();
    if (client) {
        Serial.println(F("Nouveau client."));
        boolean currentLineIsBlank = true;
        //lire la requête
        while (client.connected()) {
            if (client.available()) {
                char c = client.read();
                Serial.print(String(c));
                if (c == '\n' && currentLineIsBlank) break;  
                if (c == '\n') {
                    currentLineIsBlank = true;
                } else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            }
            
        }
        //servir la page
        Serial.println(F("Servir la page web"));
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println();
        Serial.println(F("header envoye"));
        client.println(F("<!DOCTYPE HTML>"));
        client.println(F("<html>"));
        client.print("ceci est la page web...");
        
        //TabDebug
        for (int cpt = 0; cpt < 6; cpt++) {
            client.print("valeur indice " + String(cpt) + " : ");
            client.print(String(tabDebug[cpt]));
            client.print("<br />");
        }
        
        client.println(F("</html>"));
        Serial.println(F("contenu page envoye"));
        //laisser au navigateur le temps de recevoir les données
        delay(1);
        client.stop();
        Serial.println(F("client stoppe"));
    }
}


//TabDebug
void TabDebugIni() {
    nbTabDebug = 0;
    for (int cpt = 0; cpt < 6; cpt++) {
        tabDebug[cpt] = "";
    }
}

//TabDebug
void TabDebugAjout(String chaine) {
    if (nbTabDebug == 6) {
        TabDebugIni();
    }
    nbTabDebug++;
    tabDebug[nbTabDebug - 1] = chaine;
}



void lireChaineRecue(String chaine) {
    
    //Maj20140901 : on reçoit désormais la chaîne sans les car. de début et fin < et >
    //supprimer les caractères '<' et '>'
    //chaine = chaine.substring(1, chaine.length() - 1);
    
    Serial.println(F("Dans lireChaineRecue..."));
    Serial.println("chaine : " + chaine);
    String retour = splitterChaine(chaine, ";", "/");
    Serial.println("retour fonction splitterChaine : " + retour);
    
    if (tab[0] == "Test") {
        if (char(tab[1][0]) >= 'a' && char(tab[1][0]) <= 'd') {
            Serial.println(F("instruction de Test a-d"));
            testSplitterChaine(char(tab[1][0]));
        } else if (char(tab[1][0]) >= 'e' && char(tab[1][0]) <= 'h') {
            Serial.println(F("instruction de Test e-h"));
            lancerTestParam(char(tab[1][0]));
        } else {
            Serial.println(F("instruction de Test, valeur non connue"));
        }
    } else if (tab[0] == "Param") {
        Serial.println(F("instruction de Param"));
        modifierValeurParam(tab[1], tab[2]);
    }
}


void clignoterLed(int pinLed, int nbClignote, int dureeOn, int dureeOff) {
    for(int cpt=1; cpt<=nbClignote; cpt++) {
        digitalWrite(pinLed, HIGH);
        delay(dureeOn);
        digitalWrite(pinLed, LOW);
        delay(dureeOff);
    }
}

