#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QDate>
#include <QSqlQueryModel>
#include <QMap>

QT_BEGIN_NAMESPACE
class QWidget;
class QChart;
class QChartView;
class QBarSeries;
class QBarSet;
class QBarCategoryAxis;
class QValueAxis;
QT_END_NAMESPACE

class client
{
public:
    client();
    client(QString nom_client, QString prenom_client, QString cin_client,
           QDate date_naissance, QString adresse, int tel_client,
           QString email = "", double latitude = 0.0, double longitude = 0.0);

    int getid_client() const;
    QString getnom_client() const;
    QString getprenom_client() const;
    QString getcin_client() const;
    QDate getdate_naissance() const;
    QString getadresse() const;
    int gettel_client() const;
    QString getemail() const;
    double getlatitude() const;
    double getlongitude() const;

    void setid_client(int id_client);
    void setnom_client(QString nom_client);
    void setprenom_client(QString prenom_client);
    void setcin_client(QString cin_client);
    void setdate_naissance(QDate date_naissance);
    void setadresse(QString adresse);
    void settel_client(int tel_client);
    void setemail(QString email);
    void setlatitude(double latitude);
    void setlongitude(double longitude);

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
    QString email;
    double latitude;
    double longitude;
};

#endif // CLIENT_H



