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
    QString immatricule, modele, type, transmission, etat;
    QDate date_assurance;
};

#endif // VEHICULE_H
