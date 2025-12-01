#ifndef MONITEUR_H
#define MONITEUR_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Moniteur
{
public:
    Moniteur();
    Moniteur(QString id, QString cin, QString nom, QString prenom, QString tel, QString email, QString permis);
    
    bool ajouter();
    bool supprimer(QString id);
    bool modifier(QString id);
    QSqlQueryModel* afficher();
    
    // Getters
    QString getId() const { return id_moniteur; }
    QString getCin() const { return cin_moniteur; }
    QString getNom() const { return nom_moniteur; }
    QString getPrenom() const { return prenom_moniteur; }
    QString getTel() const { return tel_moniteur; }
    QString getEmail() const { return email; }
    QString getPermis() const { return permis; }

private:
    QString id_moniteur;
    QString cin_moniteur;
    QString nom_moniteur;
    QString prenom_moniteur;
    QString tel_moniteur;
    QString email;
    QString permis;
};

#endif // MONITEUR_H
