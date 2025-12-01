#ifndef CONNECTION_H
#define CONNECTION_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

class Connection
{
private:
    static Connection * p_instance;
    QSqlDatabase db;

    // Private constructor and destructor for singleton
    Connection();
    ~Connection();

public:
    // Static method to get singleton instance
    static Connection* instance();

    // Connection management
    bool createConnect();
    void closeConnect();

    // Database checks (static methods for easy access)
    static bool isDatabaseOpen();
    static bool testConnection();
    static QString getConnectionStatus();
};

#endif // CONNECTION_H
