/************************************************/
/*        Fonction qui cherche une issue        */
/*       Algorithme définit dans la vidéo.      */
/************************************************/
void ChercheUneIssue()
{
 Arret(); 
 ChercheUneIssueAGauche();
 if(LaVoieEstLibre()) 
 {                    
   PivoteAGauche(); 
 }
 else
 {
   ChercheUneIssueADroite();
   if(LaVoieEstLibre())
   {
      PivoteADroite();
   }
   else
   {
      DemiTour();
   }
 }
}
