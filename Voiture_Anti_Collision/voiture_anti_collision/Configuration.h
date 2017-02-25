/**************************************************/
/*   Variables à configurer selon votre voiture   */
/**************************************************/
const int potar = 0;  //la broche pour régler la vitesse


int completementAGauche = 135; // Position du Servo considérée comme complètement à gauche (pour chercher une issue).
int aGauche             = 120; // Position du Servo considérée comme à gauche: 120°.

int toutDroit           = 90;  // Position du Servo considérée comme en face: 90° <!> 60° si votre Servo n'est pas inversé mécaniquement.

int aDroite             = 60;  // Position du Servo considérée comme à gauche: 60° <!> 90° si votre Servo n'est pas inversé mécaniquement
int completementADroite = 45;  // Position du Servo considérée comme complètement à droite (pour chercher une issue).

int dureePostionnementServo = 250;// Durée d'attente en milliseconde pour considérer que le Servo à atteint sa position

int seuilDistance       = 30;    // Si un objet est à moins de 30cm alors c'est considéré comme un obstacle
int dureePourPivoter    = 200;   // Durée d'une rotation à gauche ou à droite en milliseconde
int dureeDunDemiTour    = 600;   // Durée d'un demi-tour en milliseconde
