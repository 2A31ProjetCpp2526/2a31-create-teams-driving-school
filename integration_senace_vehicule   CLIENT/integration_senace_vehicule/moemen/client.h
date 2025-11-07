#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>

class client
{
public:
    client();
    client(QString nom_client, QString prenom_client, QString cin_client,
           QDate date_naissance, QString adresse, int tel_client,
           QString code_qr = "", QString column8 = "");

    int getid_client() const;
    QString getnom_client() const;
    QString getprenom_client() const;
    QString getcin_client() const;
    QDate getdate_naissance() const;
    QString getadresse() const;
    int gettel_client() const;
    QString getcode_qr() const;
    QString getcolumn8() const;

    void setid_client(int id_client);
    void setnom_client(QString nom_client);
    void setprenom_client(QString prenom_client);
    void setcin_client(QString cin_client);
    void setdate_naissance(QDate date_naissance);
    void setadresse(QString adresse);
    void settel_client(int tel_client);
    void setcode_qr(QString code_qr);
    void setcolumn8(QString column8);

    bool ajouter();
    QSqlQueryModel* afficher();
    QSqlQueryModel* afficher(QString orderBy);
    bool supprimer(int id_client);
    bool checkIfClientExists(int id_client);
    bool checkIfCinExists(QString cin_client, int excludeId = 0);
    bool modifierE();
    QSqlQueryModel* rechercheParNom(QString nom);

private:
    bool validerDonnees() const;

    int id_client;
    QString nom_client;
    QString prenom_client;
    QString cin_client;
    QDate date_naissance;
    QString adresse;
    int tel_client;
    QString code_qr;
    QString column8;
};

#endif // CLIENT_H

