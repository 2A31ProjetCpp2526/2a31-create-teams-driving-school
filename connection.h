
#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql/QSqlDatabase>

    class Connection
{
public:
    static Connection* instance();  // Accès à l’instance unique
    bool createConnect();           // Méthode pour établir la connexion

private:
    Connection();                   // Constructeur privé
    ~Connection();                  // Destructeur privé
    Connection(const Connection&) = delete;            // Suppression du constructeur de copie
    Connection& operator=(const Connection&) = delete; // Suppression de l’opérateur d’affectation

    static Connection* p_instance;  // Instance unique
};

#endif // CONNECTION_H
