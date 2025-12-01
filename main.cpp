<<<<<<< HEAD
#include <iostream>
#include <string>

using namespace std {


    int main(){

        cout<<"Hello World!!"<<end;
        return 0;
    }

=======
#include "mainwindow.h"
#include "login.h"
#include "seance.h"
#include <QApplication>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Initialiser la connexion à la base de données AVANT la boîte de dialogue de connexion
    // (le login vérifie la table EMPLOYE)
    Connection::instance()->createConnect();

    // Afficher la boîte de dialogue de connexion
    LoginDialog loginDialog;

    // Si l'utilisateur se connecte avec succès, afficher la fenêtre principale
    if (loginDialog.exec() == QDialog::Accepted) {
        MainWindow w;
        w.show();
        return a.exec();
    }

    // Si l'utilisateur annule la connexion, fermer l'application
    return 0;
>>>>>>> 21cd1a8 (Ajout du projet HATEMAA)
}
