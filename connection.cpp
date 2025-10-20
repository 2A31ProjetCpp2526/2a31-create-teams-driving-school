#include "connection.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>

Connection* Connection::p_instance = nullptr;

// Constructeur privé
Connection::Connection() {}

// Méthode d’accès à l’unique instance
Connection* Connection::instance()
{
    if (!p_instance)
        p_instance = new Connection();
    return p_instance;
}

// Création de la connexion à la BD
bool Connection::createConnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");   // nom de ta source ODBC
    db.setUserName("chaima");                 // utilisateur
    db.setPassword("esprit21");          // mot de passe

    if (db.open()) {
        qDebug() << "Connexion réussie";
        test = true;
    } else {
        qDebug() << "Erreur de connexion :" << db.lastError().text();
    }

    return test;
}
