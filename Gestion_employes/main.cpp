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
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
>>>>>>> 55e7350 (Ajout initial du dossier gestion_employes avec CRUD et stats)
}
