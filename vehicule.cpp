#include "vehicule.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>


Vehicule::Vehicule(){}

// Constructeur
Vehicule::Vehicule(QString imm, QString mod, QString typ, QString trans, QString et, QDate date)
{
    this->immatricule = imm.toUpper();
    this->modele = mod;
    this->type = typ;
    this->transmission = trans;
    this->etat = et;
    this->date_assurance = date;
}

// Ajouter
bool Vehicule::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO VEHICULES (IMMATRICULE, MODELE, TYPE, TRANSMISSION,  ETAT, DATE_ASSURANCE) "
                  "VALUES (:immat, :modele, :type, :transmission, :etat, :date_assurance)");

    query.bindValue(":immat", immatricule);
    query.bindValue(":modele", modele);
    query.bindValue(":type", type);
    query.bindValue(":transmission", transmission);
    query.bindValue(":etat", etat);
    query.bindValue(":date_assurance", date_assurance);

    if (!query.exec()) {
        qDebug() << "❌ Erreur ajout véhicule:" << query.lastError().text();
        QMessageBox::critical(nullptr, QObject::tr("Erreur SQL"), query.lastError().text());
        return false;
    }

    QMessageBox::information(nullptr, QObject::tr("Succès"), QObject::tr("Véhicule ajouté avec succès ✅"));
    return true;
}

// Supprimer
bool Vehicule::supprimer(QString id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM VEHICULES WHERE IMMATRICULE = :id");
    query.bindValue(":id", id.toUpper());

    qDebug() << "Suppression SQL =" << query.lastQuery();
    qDebug() << "id =" << id;

    if (!query.exec()) {
        qDebug() << "Erreur suppression:" << query.lastError().text();
        QMessageBox::critical(nullptr, QObject::tr("Erreur"), QObject::tr("Échec de la suppression : %1").arg(query.lastError().text()));
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, QObject::tr("Aucune suppression"),
                             QObject::tr("Aucun véhicule trouvé avec cette immatricule ❌"));
        return false;
    }

    QMessageBox::information(nullptr, QObject::tr("Succès"), QObject::tr("Véhicule supprimé avec succès ✅"));
    return true;
}

// ===== MODIFIER =====
bool Vehicule::modifier(QString id)
{
    QSqlQuery query;

    // NB: colonne PK s’appelle bien IMMATRICULE (avec U)
    query.prepare(
        "UPDATE VEHICULES "
        "SET MODELE = :mod, "
        "    TYPE = :typ, "
        "    TRANSMISSION = :trans, "
        "    ETAT = :et, "
        "    DATE_ASSURANCE = TO_DATE(:date,'DD/MM/YYYY') "
        "WHERE IMMATRICULE = :id"
        );

    query.bindValue(":mod",  modele);
    query.bindValue(":typ",  type);
    query.bindValue(":trans", transmission);
    query.bindValue(":et",   etat);
    // Oracle attend une chaîne formatée si vous passez par ODBC
    query.bindValue(":date", date_assurance.toString("dd/MM/yyyy"));
    query.bindValue(":id",   id);

    if (!query.exec()) {
        qDebug() << "Erreur modification:" << query.lastError().text();
        return false;
    }
    return true;
}

// Afficher
QSqlQueryModel* Vehicule::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT IMMATRICULE, MODELE, TYPE, TRANSMISSION,  ETAT, DATE_ASSURANCE FROM VEHICULES");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Immatricule"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Modèle"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Transmission"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Assurance"));

    return model;
}
