#include "mainwindow.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection *c = Connection::instance();

    bool test = c->createConnect();

    if (test) {
        QMessageBox::information(nullptr, QObject::tr("Database is open"),
                                 QObject::tr("Connexion établie avec succès.\nClick Cancel to exit."),
                                 QMessageBox::Cancel);
        MainWindow w;
        w.show();
        return a.exec();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Database is not open"),
                              QObject::tr("Échec de connexion.\nClick Cancel to exit."),
                              QMessageBox::Cancel);
        return -1;
    }
}
