#include "mainwindowex4.h"
#include "ui_mainwindowex4.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>


extern MainWindowEx4 *w;

int idFils1, idFils2, idFils3;
// TO DO : HandlerSIGCHLD

void HandlerSIGCHLD(int sig)
{

    int status;
    pid_t id;

    // On récupère le PID du fils terminé et son status
    // wait() permet de libérer les ressources du processus zombie
    id = wait(&status);

    // Si le fils s'est terminé normalement (via exit)
    if (WIFEXITED(status))
    {
        // On récupère la valeur passée à exit() (le nombre d'étudiants)
        int resultat = WEXITSTATUS(status);

        // On identifie quel fils a terminé pour l'affichage
        if (id == idFils1)
        {
            w->setResultat1(resultat);
            idFils1 = 0; // Optionnel : marque que ce fils n'existe plus
        }
        else if (id == idFils2)
        {
            w->setResultat2(resultat);
            idFils2 = 0;
        }
        else if (id == idFils3)
        {
            w->setResultat3(resultat);
            idFils3 = 0;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainWindowEx4::MainWindowEx4(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindowEx4)
{
  ui->setupUi(this);
  ui->pushButtonAnnulerTous->setVisible(false);

  int fdLog = open("Trace.log", O_CREAT | O_TRUNC | O_RDWR, 0600);
    if(fdLog == -1)
    {
        perror("Erreur ouverture Trace.log");
        exit(1);
    }
    dup2(fdLog, STDERR_FILENO);
    
  // armement de SIGCHLD
  struct sigaction A;
      A.sa_handler = HandlerSIGCHLD;  // On lie le signal à ta fonction
      sigemptyset(&A.sa_mask);        // On ne bloque rien d'autre
      A.sa_flags = 0;

      // Appel système indispensable pour activer l'interruption
      if (sigaction(SIGCHLD, &A, NULL) == -1) 
      {
          perror("Erreur armement SIGCHLD");
      }
}

MainWindowEx4::~MainWindowEx4()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles : ne pas modifier /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowEx4::setGroupe1(const char* Text)
{
  //fprintf(stderr,"---%s---\n",Text);
  if (strlen(Text) == 0 )
  {
    ui->lineEditGroupe1->clear();
    return;
  }
  ui->lineEditGroupe1->setText(Text);
}

void MainWindowEx4::setGroupe2(const char* Text)
{
  //fprintf(stderr,"---%s---\n",Text);
  if (strlen(Text) == 0 )
  {
    ui->lineEditGroupe2->clear();
    return;
  }
  ui->lineEditGroupe2->setText(Text);
}

void MainWindowEx4::setGroupe3(const char* Text)
{
  //fprintf(stderr,"---%s---\n",Text);
  if (strlen(Text) == 0 )
  {
    ui->lineEditGroupe3->clear();
    return;
  }
  ui->lineEditGroupe3->setText(Text);
}

void MainWindowEx4::setResultat1(int nb)
{
  char Text[20];
  sprintf(Text,"%d",nb);
  //fprintf(stderr,"---%s---\n",Text);
  if (strlen(Text) == 0 )
  {
    ui->lineEditResultat1->clear();
    return;
  }
  ui->lineEditResultat1->setText(Text);
}

void MainWindowEx4::setResultat2(int nb)
{
  char Text[20];
  sprintf(Text,"%d",nb);
  //fprintf(stderr,"---%s---\n",Text);
  if (strlen(Text) == 0 )
  {
    ui->lineEditResultat2->clear();
    return;
  }
  ui->lineEditResultat2->setText(Text);
}

void MainWindowEx4::setResultat3(int nb)
{
  char Text[20];
  sprintf(Text,"%d",nb);
  //fprintf(stderr,"---%s---\n",Text);
  if (strlen(Text) == 0 )
  {
    ui->lineEditResultat3->clear();
    return;
  }
  ui->lineEditResultat3->setText(Text);
}

bool MainWindowEx4::traitement1Selectionne()
{
  return ui->checkBoxTraitement1->isChecked();
}

bool MainWindowEx4::traitement2Selectionne()
{
  return ui->checkBoxTraitement2->isChecked();
}

bool MainWindowEx4::traitement3Selectionne()
{
  return ui->checkBoxTraitement3->isChecked();
}

const char* MainWindowEx4::getGroupe1()
{
  if (ui->lineEditGroupe1->text().size())
  { 
    strcpy(groupe1,ui->lineEditGroupe1->text().toStdString().c_str());
    return groupe1;
  }
  return NULL;
}

const char* MainWindowEx4::getGroupe2()
{
  if (ui->lineEditGroupe2->text().size())
  { 
    strcpy(groupe2,ui->lineEditGroupe2->text().toStdString().c_str());
    return groupe2;
  }
  return NULL;
}

const char* MainWindowEx4::getGroupe3()
{
  if (ui->lineEditGroupe3->text().size())
  { 
    strcpy(groupe3,ui->lineEditGroupe3->text().toStdString().c_str());
    return groupe3;
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions clics sur les boutons ////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowEx4::on_pushButtonDemarrerTraitements_clicked()
{
  if(MainWindowEx4::traitement1Selectionne())
  {
    idFils1= fork();
    if(idFils1 == 0)
    {
      //code du fils
      const char * donnee_groupe1 = MainWindowEx4::getGroupe1();
      execl("./Traitement", "./Traitement",donnee_groupe1, "200",NULL);
      exit(1);

    }
  }

  if(MainWindowEx4::traitement2Selectionne())
  {
    idFils2 = fork();
    if(idFils2==0)
    {
      //code fils

      const char * donnee_groupe2 = MainWindowEx4::getGroupe2();

      execl("./Traitement", "./Traitement",donnee_groupe2, "450",NULL);
      exit(1);
    }
  }  

  if(MainWindowEx4::traitement3Selectionne())
  {
    idFils3 = fork();
    if(idFils3==0)
    {
      //code fils
      const char * donnee_groupe3 = MainWindowEx4::getGroupe3();

      execl("./Traitement", "./Traitement",donnee_groupe3, "700",NULL);
      exit(1);
    }
  }  
}

void MainWindowEx4::on_pushButtonVider_clicked()
{
  fprintf(stderr,"Clic sur le bouton Vider\n");
    ui->lineEditGroupe1->clear();
    ui->lineEditGroupe2->clear();
    ui->lineEditGroupe3->clear();
 
    // Vider les resultats
    ui->lineEditResultat1->clear();
    ui->lineEditResultat2->clear();
    ui->lineEditResultat3->clear();
  
}

void MainWindowEx4::on_pushButtonQuitter_clicked()
{
  fprintf(stderr,"Clic sur le bouton Quitter\n");
  // On tue les processus fils s'ils sont encore en vie
    if (idFils1 > 0) kill(idFils1, SIGKILL); // SIGKILL est radical, ou SIGUSR1
    if (idFils2 > 0) kill(idFils2, SIGKILL);
    if (idFils3 > 0) kill(idFils3, SIGKILL);

    // On ferme l'application
    exit(0);
}

void MainWindowEx4::on_pushButtonAnnuler1_clicked()
{
  fprintf(stderr,"Clic sur le bouton Annuler1\n");
    // On vérifie si le fils existe encore
    if (idFils1 > 0) 
    {
        // On envoie le signal SIGUSR1 au processus dont l'ID est idFils1
        kill(idFils1, SIGUSR1);
    }
}

void MainWindowEx4::on_pushButtonAnnuler2_clicked()
{
  fprintf(stderr,"Clic sur le bouton Annuler2\n");
    // On vérifie si le fils existe encore
    if (idFils2 > 0) 
    {
        // On envoie le signal SIGUSR1 au processus dont l'ID est idFils1
        kill(idFils2, SIGUSR1);
    }
}

void MainWindowEx4::on_pushButtonAnnuler3_clicked()
{
  fprintf(stderr,"Clic sur le bouton Annuler3\n");
    // On vérifie si le fils existe encore
    if (idFils3 > 0) 
    {
        // On envoie le signal SIGUSR1 au processus dont l'ID est idFils1
        kill(idFils3, SIGUSR1);
    }
}

void MainWindowEx4::on_pushButtonAnnulerTous_clicked()
{
  // fprintf(stderr,"Clic sur le bouton Annuler tout\n");
  // NOTHING TO DO --> bouton supprimé
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////// Handlers de signaux //////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// TO DO : HandlerSIGCHLD
