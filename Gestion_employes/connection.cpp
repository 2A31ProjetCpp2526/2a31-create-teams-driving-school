#include "connection.h"
#include <QDebug>

Connection::Connection()
{
    db = QSqlDatabase::addDatabase("QODBC");
    db.setDatabaseName("drivingschool");          // nom du DSN Oracle
    db.setUserName("drivingschool");        // utilisateur
    db.setPassword("123456");               // mot de passe
}

bool Connection::createConnect()  // ✅ même casse que dans .h
{
    if (!db.open()) {
        qDebug() << "❌ Erreur de connexion:" << db.lastError().text();
        return false;
    }

    qDebug() << "✅ Connexion réussie à la base de données Oracle via ODBC!";
    return true;
}

void Connection::closeConnect()
{
    if (db.isOpen())
        db.close();
}
