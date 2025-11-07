#include "connection.h"

Connection::Connection()
{
}

bool Connection::createconnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("drivingschool"); // Insérer le nom de la source de données
    db.setUserName("drivingschool"); // Insérer nom de l'utilisateur
    db.setPassword("123456"); // Insérer mot de passe de cet utilisateur
    if (db.open())
        test = true;
    return test;
}
