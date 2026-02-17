#include "mainwindowex3.h"
#include "ui_mainwindowex3.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


MainWindowEx3::MainWindowEx3(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindowEx3)
{
    ui->setupUi(this);
    int fd;
    fd = open("Trace.log",O_RDWR|O_CREAT|O_APPEND,0644);
    dup2(fd, 2);
    ::close(fd);
}

MainWindowEx3::~MainWindowEx3()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles : ne pas modifier /////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void MainWindowEx3::setGroupe1(const char* Text)
{
  fprintf(stderr,"---%s---\n",Text);
  if (strlen(Text) == 0 )
  {
    ui->lineEditGroupe1->clear();
    return;
  }
  ui->lineEditGroupe1->setText(Text);
}

void MainWindowEx3::setGroupe2(const char* Text)
{
  fprintf(stderr,"---%s---\n",Text);
  if (strlen(Text) == 0 )
  {
    ui->lineEditGroupe2->clear();
    return;
  }
  ui->lineEditGroupe2->setText(Text);
}

void MainWindowEx3::setGroupe3(const char* Text)
{
  fprintf(stderr,"---%s---\n",Text);
  if (strlen(Text) == 0 )
  {
    ui->lineEditGroupe3->clear();
    return;
  }
  ui->lineEditGroupe3->setText(Text);
}

void MainWindowEx3::setResultat1(int nb)
{
  char Text[20];
  sprintf(Text,"%d",nb);
  fprintf(stderr,"---%s---\n",Text);
  if (strlen(Text) == 0 )
  {
    ui->lineEditResultat1->clear();
    return;
  }
  ui->lineEditResultat1->setText(Text);
}

void MainWindowEx3::setResultat2(int nb)
{
  char Text[20];
  sprintf(Text,"%d",nb);
  fprintf(stderr,"---%s---\n",Text);
  if (strlen(Text) == 0 )
  {
    ui->lineEditResultat2->clear();
    return;
  }
  ui->lineEditResultat2->setText(Text);
}

void MainWindowEx3::setResultat3(int nb)
{
  char Text[20];
  sprintf(Text,"%d",nb);
  fprintf(stderr,"---%s---\n",Text);
  if (strlen(Text) == 0 )
  {
    ui->lineEditResultat3->clear();
    return;
  }
  ui->lineEditResultat3->setText(Text);
}

bool MainWindowEx3::recherche1Selectionnee()
{
  return ui->checkBoxRecherche1->isChecked();
}

bool MainWindowEx3::recherche2Selectionnee()
{
  return ui->checkBoxRecherche2->isChecked();
}

bool MainWindowEx3::recherche3Selectionnee()
{
  return ui->checkBoxRecherche3->isChecked();
}

const char* MainWindowEx3::getGroupe1()
{
  if (ui->lineEditGroupe1->text().size())
  { 
    strcpy(groupe1,ui->lineEditGroupe1->text().toStdString().c_str());
    return groupe1;
  }
  return NULL;
}

const char* MainWindowEx3::getGroupe2()
{
  if (ui->lineEditGroupe2->text().size())
  { 
    strcpy(groupe2,ui->lineEditGroupe2->text().toStdString().c_str());
    return groupe2;
  }
  return NULL;
}

const char* MainWindowEx3::getGroupe3()
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
void MainWindowEx3::on_pushButtonLancerRecherche_clicked()
{
  fprintf(stderr,"Clic sur le bouton Lancer Recherche\n");
  // TO DO
  int idFils1,idFils2,idFils3;
  int status;
  pid_t id;

  if (MainWindowEx3::recherche1Selectionnee())
  { //processus fils cree
     idFils1 = fork();

    if(idFils1 == 0){ //processus fils
      const char* groupe1temp = MainWindowEx3::getGroupe1();

      execl("./Lecture", "./Lecture ",groupe1temp, NULL);
      exit(1);
    }
  }

  if (MainWindowEx3::recherche2Selectionnee())
  { //processus fils cree
     idFils2 = fork();

    if(idFils2 == 0){ //processus fils
      const char* groupe2temp = MainWindowEx3::getGroupe2();
      execl("./Lecture", "./Lecture ",groupe2temp, NULL);
      exit(1);
    }
  }

  if (MainWindowEx3::recherche3Selectionnee())
  { //processus fils cree
     idFils3 = fork();

    if(idFils3 == 0){ //processus fils
      const char* groupe3temp = MainWindowEx3::getGroupe3();
      execl("./Lecture", "./Lecture ", groupe3temp , NULL);
      exit(1);
    }
  }

  while((id=wait(&status))!=-1 ){
    int result = WEXITSTATUS(status);
    
    if(idFils1 == id){
      MainWindowEx3::setResultat1(result);
    }
    else if(idFils2 == id){
      MainWindowEx3::setResultat2(result);
    }
    else if(idFils3 == id){
      MainWindowEx3::setResultat3(result);
    }
  }
}

void MainWindowEx3::on_pushButtonVider_clicked()
{
  fprintf(stderr,"Clic sur le bouton Vider\n");
    // Vider les groupes
    ui->lineEditGroupe1->clear();
    ui->lineEditGroupe2->clear();
    ui->lineEditGroupe3->clear();
 
    // Vider les resultats
    ui->lineEditResultat1->clear();
    ui->lineEditResultat2->clear();
    ui->lineEditResultat3->clear();
  
}

void MainWindowEx3::on_pushButtonQuitter_clicked()
{
  fprintf(stderr,"Clic sur le bouton Quitter\n");
  close();
}
