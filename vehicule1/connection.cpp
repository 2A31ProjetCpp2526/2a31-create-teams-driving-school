#include "connection.h"
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QDebug>

Connection* Connection::p_instance = nullptr;

Connection::Connection() {}

Connection* Connection::instance()
{
    if (!p_instance)
        p_instance = new Connection();
    return p_instance;
}

bool Connection::createConnect()
{
    bool test = false;
    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("Source_Projet2A");
    db.setUserName("chaima");
    db.setPassword("esprit21");

    if (db.open()) {
        qDebug() << "Connexion réussie";
        test = true;
    } else {
        qDebug() << "Erreur de connexion :" << db.lastError().text();
    }

    return test;
}
