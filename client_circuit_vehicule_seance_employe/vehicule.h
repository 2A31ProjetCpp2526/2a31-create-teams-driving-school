#ifndef VEHICULE_H
#define VEHICULE_H

#include <QDate>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Vehicule
{
public:
    Vehicule();
    Vehicule(QString, QString, QString, QString, QString, QDate);
    bool ajouter();
    bool supprimer(QString);
    bool modifier(QString);
    QSqlQueryModel* afficher();

private:
    QString immatricule, modele, type_vehicule, transmission, etat;
    QDate date_assurence; // Gardez le nom interne comme vous voulez
};

#endif // VEHICULE_H
