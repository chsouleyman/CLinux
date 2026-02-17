#include "FichierUtilisateur.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int estPresent(const char* nom)
{
  int fd;
  UTILISATEUR user;
  int position=1;

  if((fd = open(FICHIER_UTILISATEURS,O_RDONLY))==-1){
    return -1;
  }

  while (read(fd, &user, sizeof(UTILISATEUR)) == sizeof(UTILISATEUR)) {
        if (strcmp(user.nom, nom)==0){
          close(fd);
          return position;  
        }
        position++;
  }
  
  close(fd);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////
int hash(const char* motDePasse)
{
  int sum;
  int i=0;
  if(motDePasse == NULL) return 0;
  for(i=0 ; i< strlen(motDePasse) ; i++){
    sum += (i+1)* motDePasse[i];
  }return sum % 97;
}

////////////////////////////////////////////////////////////////////////////////////
void ajouteUtilisateur(const char* nom, const char* motDePasse)
{
  //etape 1
  int fd = open(FICHIER_UTILISATEURS,O_WRONLY | O_APPEND| O_CREAT, 0644);
  //ouverture fichier ecriture seule modifier en fin de fichier et la cree si il nexiste pas
  if(fd == -1){
    perror("erreur d'ouverture");
    return;
  }
  //etape 2
  UTILISATEUR user; //on cree un obj de type UTILISATEUR
  strncpy(user.nom, nom, sizeof(user.nom) - 1); //comparaison du nom stocké et celui entré
  user.nom[sizeof(user.nom) - 1] = '\0';//user.nom est une chaine de caractere, 
  //on mesure sa taille et on place \0 qui indique la fin d'une chaine de caractere
 
  // Calculer le hash du mot de passe
  user.hash = hash(motDePasse); //on récupère le hash retourné par la fct mdp
  if (write(fd, &user, sizeof(UTILISATEUR)) != sizeof(UTILISATEUR)) {
  //ecriture de user de type UTILISATEUR dans le fichier
  //on verifie que l'ecriture fais la taille attentue donc de la structure
        perror("Erreur écriture fichier");
        close(fd);
        return;
    }
  close(fd);
}

////////////////////////////////////////////////////////////////////////////////////
int verifieMotDePasse(int pos, const char* motDePasse)
{
  // TO DO
  UTILISATEUR user;

  int fd = open(FICHIER_UTILISATEURS,O_RDONLY);
  //ouverture fichier ecriture seule modifier en fin de fichier et la cree s'il nexiste pas
  if(fd == -1){
    return fd;
  }

  
  lseek(fd, ((pos - 1)* sizeof(user)),SEEK_SET);
//read(..&user...)
  read(fd, &user,sizeof(UTILISATEUR));
  
  if(hash(motDePasse) == user.hash)
    return 1;
  else
    return 0;

  close(fd);
}

////////////////////////////////////////////////////////////////////////////////////
int listeUtilisateurs(UTILISATEUR *vecteur) // le vecteur doit etre suffisamment grand
{
// Reçoit en paramètre l’adresse d’un vecteur d’utilisateurs suffisamment grand
// pour contenir tous les utilisateurs contenus dans le fichier.
// o Lit l’ensemble des utilisateurs du fichier et les place dans le vecteur.
// o Retourne le nombre d’utilisateurs lus.

  int fd = open(FICHIER_UTILISATEURS,O_RDONLY);
  if(fd == -1){
    return -1;
  }

  int nbutilisateur = 0;

  UTILISATEUR user;
 
    while(read(fd,&user,sizeof(UTILISATEUR)) == sizeof(UTILISATEUR)){
      vecteur[nbutilisateur] = user;
      nbutilisateur++;
    }
    close(fd);

    return nbutilisateur;



  return 0;
}