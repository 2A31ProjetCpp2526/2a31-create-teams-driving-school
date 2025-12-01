#include "connection.h"
#include <QDebug>
#include <QSqlError>

Connection::Connection()
{
    // Vérifier si le driver QODBC est disponible
    if (!QSqlDatabase::isDriverAvailable("QODBC")) {
        qDebug() << "❌ Le driver QODBC n'est pas disponible!";
        qDebug() << "Drivers disponibles:" << QSqlDatabase::drivers();
        return;
    }

    db = QSqlDatabase::addDatabase("QODBC");
    // Note: Ne pas utiliser setUserName/setPassword ici car on utilise UID/PWD dans la chaîne de connexion
}

bool Connection::createConnect()  // ✅ même casse que dans .h
{
    // Vérifier si le driver est disponible
    if (!QSqlDatabase::isDriverAvailable("QODBC")) {
        QString errorMsg = "Le driver QODBC n'est pas disponible!\n\n"
                         "Drivers disponibles: " + QSqlDatabase::drivers().join(", ") + "\n\n"
                         "Veuillez installer le driver Oracle ODBC.";
        qDebug() << "❌" << errorMsg;
        return false;
    }

    // Configuration de la connexion
    QString driverName = "Oracle in XE";              // Nom du driver ODBC
    QString host = "localhost";                        // Adresse du serveur
    QString port = "1521";                            // Port Oracle
    QString serviceName = "XE";                       // Nom du service (XE, ORCL, etc.)
    QString username = "drivingschool";                // Nom d'utilisateur (Oracle le convertit en majuscules)
    QString password = "123456";                       // Mot de passe

    // Format: Driver={NomDriver};DBQ=host:port/service;UID=user;PWD=pass
    // Note: Oracle stocke les usernames en MAJUSCULES, donc on utilise toUpper()
    QString connectionString = QString("Driver={%1};DBQ=%2:%3/%4;UID=%5;PWD=%6")
                               .arg(driverName)
                               .arg(host)
                               .arg(port)
                               .arg(serviceName)
                               .arg(username.toUpper())  // Oracle stocke les usernames en majuscules
                               .arg(password);

    // Liste des méthodes de connexion à essayer (par ordre de préférence)
    QStringList connectionMethods;

    // Méthode 1: DSN (si configuré)
    connectionMethods << QString("DSN=drivingschool;UID=%1;PWD=%2").arg(username.toUpper()).arg(password);

    // Méthode 2: Chaîne de connexion directe avec le driver "Oracle in XE" (format principal)
    connectionMethods << connectionString;

    // Méthode 3: Format alternatif avec DBQ simplifié (si TNS est configuré)
    connectionMethods << QString("Driver={%1};DBQ=%2;UID=%3;PWD=%4")
                          .arg(driverName)
                          .arg(serviceName)
                          .arg(username.toUpper())
                          .arg(password);

    // Essayer chaque méthode de connexion
    for (const QString &connectionString : connectionMethods) {
        db.setDatabaseName(connectionString);

        qDebug() << "🔄 Tentative de connexion avec:" << connectionString.split(";").first();

        if (db.open()) {
            qDebug() << "✅ Connexion réussie à la base de données Oracle via ODBC!";
            qDebug() << "Méthode utilisée:" << connectionString.split(";").first();
            return true;
        } else {
            QSqlError error = db.lastError();
            qDebug() << "⚠️ Échec avec:" << connectionString.split(";").first();
            qDebug() << "   Erreur:" << error.text();
            qDebug() << "   Erreur driver:" << error.driverText();
            qDebug() << "   Erreur database:" << error.databaseText();

            // Fermer la connexion pour réessayer avec la suivante
            if (db.isOpen()) {
                db.close();
            }
        }
    }

    // Si toutes les méthodes ont échoué
    QString errorText = db.lastError().text();
    QString errorDatabase = db.lastError().databaseText();

    // Détecter le type d'erreur
    QString errorType = "";
    QString solution = "";

    if (errorDatabase.contains("ORA-01017") || errorText.contains("invalid username/password")) {
        errorType = "❌ ERREUR D'IDENTIFIANTS";
        solution = QString(
            "Le driver Oracle fonctionne correctement, mais les identifiants sont incorrects.\n\n"
            "Identifiants actuellement utilisés:\n"
            "  - Utilisateur: %1 (converti en majuscules: %2)\n"
            "  - Mot de passe: %3\n\n"
            "Solutions possibles:\n"
            "1. Vérifiez que l'utilisateur '%2' existe dans Oracle XE (Oracle stocke en MAJUSCULES)\n"
            "2. Vérifiez que le mot de passe est correct\n"
            "3. Vérifiez si l'utilisateur est verrouillé (connectez-vous en SYSTEM):\n"
            "   SELECT username, account_status FROM dba_users WHERE username = '%2';\n"
            "   Si verrouillé: ALTER USER %1 ACCOUNT UNLOCK;\n"
            "4. Si l'utilisateur n'existe pas, créez-le avec SQL*Plus ou SQL Developer:\n"
            "   CREATE USER %1 IDENTIFIED BY %3;\n"
            "   GRANT CONNECT, RESOURCE TO %1;\n"
            "   GRANT CREATE SESSION TO %1;\n"
            "   GRANT CREATE TABLE TO %1;\n"
            "   GRANT UNLIMITED TABLESPACE TO %1;\n\n"
            "5. Testez la connexion dans SQL*Plus avec:\n"
            "   CONNECT %2/%3@localhost:1521/XE;")
            .arg("drivingschool")
            .arg("DRIVINGSCHOOL")
            .arg("123456");
    } else if (errorDatabase.contains("ORA-12154") || errorText.contains("could not resolve")) {
        errorType = "❌ ERREUR DE CONNEXION TNS";
        solution = "Le format de connexion TNS n'est pas reconnu. Utilisez le format host:port/service.";
    } else {
        errorType = "❌ ERREUR DE CONNEXION";
        solution = "Vérifiez que le service Oracle XE est démarré et accessible.";
    }

    QString detailedError = QString("%1\n\n"
                                   "Erreur détaillée:\n%2\n\n"
                                   "%3")
                                   .arg(errorType)
                                   .arg(errorText)
                                   .arg(solution);
    qDebug() << detailedError;

    return false;
}

void Connection::closeConnect()
{
    if (db.isOpen())
        db.close();
}

bool Connection::isDatabaseOpen()
{
    QSqlDatabase database = QSqlDatabase::database();
    if (!database.isValid()) {
        return false;
    }
    return database.isOpen();
}

bool Connection::testConnection()
{
    if (!isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée";
        return false;
    }

    QSqlQuery query;
    // Test simple avec une requête Oracle
    if (query.exec("SELECT 1 FROM DUAL")) {
        qDebug() << "✅ Test de connexion réussi - La base de données répond";
        return true;
    } else {
        qDebug() << "❌ Test de connexion échoué:" << query.lastError().text();
        return false;
    }
}

QString Connection::getConnectionStatus()
{
    QSqlDatabase database = QSqlDatabase::database();

    if (!database.isValid()) {
        return "❌ Base de données non valide (driver non disponible)";
    }

    if (!database.isOpen()) {
        return "❌ Base de données fermée";
    }

    // Tester avec une requête simple
    QSqlQuery query;
    if (!query.exec("SELECT 1 FROM DUAL")) {
        return QString("⚠️ Base de données ouverte mais requête échouée: %1").arg(query.lastError().text());
    }

    // Obtenir des informations sur la connexion
    QString driverName = database.driverName();
    QString databaseName = database.databaseName();
    QString hostName = database.hostName();
    QString userName = database.userName();

    return QString("✅ Connexion active\n"
                   "   Driver: %1\n"
                   "   Base: %2\n"
                   "   Host: %3\n"
                   "   User: %4")
            .arg(driverName)
            .arg(databaseName.split(";").first()) // Prendre seulement la première partie
            .arg(hostName.isEmpty() ? "localhost" : hostName)
            .arg(userName.isEmpty() ? "drivingschool" : userName);
}
