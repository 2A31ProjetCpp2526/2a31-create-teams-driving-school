#include "mainwindow.h"
#include "login.h"
#include "seance.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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
}
