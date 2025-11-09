#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class Connection
{
private:
    QSqlDatabase db;

public:
    Connection();
    bool createConnect();   // ✅ majuscule cohérente
    void closeConnect();
    static bool isDatabaseOpen();  // Vérifier si la base de données est ouverte
    static bool testConnection();  // Tester la connexion avec une requête simple
    static QString getConnectionStatus();  // Obtenir le statut détaillé de la connexion
};

#endif // CONNECTION_H
