#include "vehicule.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

Vehicule::Vehicule(){}

Vehicule::Vehicule(QString imm, QString mod, QString typ, QString trans, QString et, QDate date)
{
    this->immatricule = imm.toUpper().trimmed();
    this->modele = mod.trimmed();
    this->type_vehicule = typ;
    this->transmission = trans;
    this->etat = et;
    this->date_assurence = date;
}

bool Vehicule::ajouter()
{
    QSqlQuery query;

    // Vérifier si le véhicule existe déjà
    query.prepare("SELECT COUNT(*) FROM VEHICULE WHERE IMMATRICULE = :immat");
    query.bindValue(":immat", immatricule);
    if (query.exec() && query.next() && query.value(0).toInt() > 0) {
        QMessageBox::warning(nullptr, "Attention", "Un véhicule avec cette immatriculation existe déjà!");
        return false;
    }

    // Vérifier que tous les champs sont remplis
    if (immatricule.isEmpty() || modele.isEmpty() || type_vehicule.isEmpty() ||
        transmission.isEmpty() || etat.isEmpty() || !date_assurence.isValid()) {
        QMessageBox::warning(nullptr, "Attention", "Tous les champs doivent être remplis!");
        return false;
    }

    // INSERTION avec les bons noms de colonnes
    query.prepare("INSERT INTO VEHICULE (IMMATRICULE, MODELE, TYPE_VEHICULE, TRANSMISSION, ETAT, DATE_ASSURENCE) "
                  "VALUES (:immat, :modele, :type, :transmission, :etat, TO_DATE(:date_assurence, 'DD/MM/YYYY'))");

    query.bindValue(":immat", immatricule);
    query.bindValue(":modele", modele);
    query.bindValue(":type", type_vehicule);
    query.bindValue(":transmission", transmission);
    query.bindValue(":etat", etat);
    query.bindValue(":date_assurence", date_assurence.toString("dd/MM/yyyy"));

    qDebug() << "🔧 Tentative d'ajout véhicule - Immat:" << immatricule;
    qDebug() << "📅 Date envoyée:" << date_assurence.toString("dd/MM/yyyy");

    if (!query.exec()) {
        qDebug() << "❌ Erreur ajout véhicule:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de l'ajout:\n" + query.lastError().text());
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Véhicule ajouté avec succès ✓");
    return true;
}

bool Vehicule::supprimer(QString id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM VEHICULE WHERE IMMATRICULE = :id");
    query.bindValue(":id", id.toUpper());

    if (!query.exec()) {
        qDebug() << "Erreur suppression:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la suppression:\n" + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, "Attention", "Aucun véhicule trouvé avec cette immatriculation!");
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Véhicule supprimé avec succès ✓");
    return true;
}

bool Vehicule::modifier(QString id)
{
    QSqlQuery query;

    // Vérifier que tous les champs sont remplis
    if (modele.isEmpty() || type_vehicule.isEmpty() || transmission.isEmpty() ||
        etat.isEmpty() || !date_assurence.isValid()) {
        QMessageBox::warning(nullptr, "Attention", "Tous les champs doivent être remplis!");
        return false;
    }

    // MODIFICATION avec les bons noms de colonnes
    query.prepare("UPDATE VEHICULE "
                  "SET MODELE = :mod, "
                  "TYPE_VEHICULE = :typ, "
                  "TRANSMISSION = :trans, "
                  "ETAT = :et, "
                  "DATE_ASSURENCE = TO_DATE(:date, 'DD/MM/YYYY') "
                  "WHERE IMMATRICULE = :id");

    query.bindValue(":mod", modele);
    query.bindValue(":typ", type_vehicule);
    query.bindValue(":trans", transmission);
    query.bindValue(":et", etat);
    query.bindValue(":date", date_assurence.toString("dd/MM/yyyy"));
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur SQL modification:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la modification:\n" + query.lastError().text());
        return false;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(nullptr, "Attention", "Aucun véhicule trouvé avec cette immatriculation!");
        return false;
    }

    QMessageBox::information(nullptr, "Succès", "Véhicule modifié avec succès ✓");
    return true;
}

QSqlQueryModel* Vehicule::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    // AFFICHAGE avec les bons noms de colonnes
    QString queryStr = "SELECT IMMATRICULE, MODELE, TYPE_VEHICULE, TRANSMISSION, ETAT, "
                       "TO_CHAR(DATE_ASSURENCE, 'DD/MM/YYYY') AS DATE_ASSURENCE "
                       "FROM VEHICULE ORDER BY IMMATRICULE";

    model->setQuery(queryStr);

    // Définir les en-têtes de colonnes
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Immatricule"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Modele"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Transmission"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Etat"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Assurance"));

    return model;
}
