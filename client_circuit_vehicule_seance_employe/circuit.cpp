#include "circuit.h"
#include "connection.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QObject>

Circuit::Circuit() : id_circuit(0), distance(0.0), duree_estimee(0) {}

Circuit::Circuit(int id, QString nom, QString desc, double dist, int duree, QString diff, QString immat)
    : id_circuit(id), nom_circuit(nom), description(desc), distance(dist),
      duree_estimee(duree), difficulte(diff), immatricule(immat.toUpper().trimmed())
{}

bool Circuit::existeDeja(int id)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans existeDeja()";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_CIRCUIT FROM CIRCUIT WHERE ID_CIRCUIT = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return true;
    }
    return false;
}

bool Circuit::ajouter()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans ajouter()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }

    qDebug() << "=== TENTATIVE AJOUT CIRCUIT ===";
    qDebug() << "ID:" << id_circuit;
    qDebug() << "Nom:" << nom_circuit;
    qDebug() << "Description:" << description;
    qDebug() << "Distance:" << distance;
    qDebug() << "Durée estimée:" << duree_estimee;
    qDebug() << "Difficulté:" << difficulte;
    qDebug() << "Immatricule:" << immatricule;

    // Vérifier si l'ID existe déjà
    if (existeDeja(id_circuit)) {
        qDebug() << "❌ ID existe déjà:" << id_circuit;
        QMessageBox::warning(nullptr, "Attention", "Un circuit avec cet ID existe déjà!");
        return false;
    }

    // Vérifier que tous les champs obligatoires sont remplis
    if (nom_circuit.isEmpty() || immatricule.isEmpty()) {
        QMessageBox::warning(nullptr, "Attention", "Le nom du circuit et l'immatricule sont obligatoires!");
        return false;
    }

    // Validation de la distance
    if (distance <= 0) {
        QMessageBox::warning(nullptr, "Attention", "La distance doit être un nombre positif!");
        return false;
    }

    // Validation de la durée estimée
    if (duree_estimee <= 0) {
        QMessageBox::warning(nullptr, "Attention", "La durée estimée doit être un nombre positif!");
        return false;
    }

    // Validation de la difficulté
    if (difficulte.isEmpty()) {
        QMessageBox::warning(nullptr, "Attention", "La difficulté est obligatoire!");
        return false;
    }

    // Validation du nom du circuit (longueur minimale)
    if (nom_circuit.length() < 2) {
        QMessageBox::warning(nullptr, "Attention", "Le nom du circuit doit contenir au moins 2 caractères!");
        return false;
    }

    // Vérifier que le véhicule existe
    QSqlQuery checkVehicule;
    checkVehicule.prepare("SELECT IMMATRICULE FROM VEHICULE WHERE IMMATRICULE = :immat");
    checkVehicule.bindValue(":immat", immatricule);
    if (!checkVehicule.exec() || !checkVehicule.next()) {
        QMessageBox::warning(nullptr, "Attention", "Le véhicule avec cette immatriculation n'existe pas!");
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO CIRCUIT (ID_CIRCUIT, NOM_CIRCUIT, DESCRIPTION, DISTANCE, DUREE_ESTIMEE, DIFFICULTE, IMMATRICULE) "
                  "VALUES (:id, :nom, :desc, :distance, :duree, :diff, :immat)");

    query.bindValue(":id", id_circuit);
    query.bindValue(":nom", nom_circuit);
    query.bindValue(":desc", description);
    query.bindValue(":distance", distance);
    query.bindValue(":duree", duree_estimee);
    query.bindValue(":diff", difficulte);
    query.bindValue(":immat", immatricule);

    bool success = query.exec();

    if (success) {
        qDebug() << "✅ AJOUT RÉUSSI !";
        QMessageBox::information(nullptr, "Succès", "Circuit ajouté avec succès ✓");
        return true;
    } else {
        qDebug() << "❌ ERREUR SQL:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de l'ajout:\n" + query.lastError().text());
        return false;
    }
}

bool Circuit::modifier()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans modifier()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }

    qDebug() << "=== DÉBUT MODIFICATION CIRCUIT ===";

    // Vérifier si le circuit existe
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT ID_CIRCUIT FROM CIRCUIT WHERE ID_CIRCUIT = :id");
    checkQuery.bindValue(":id", id_circuit);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "❌ Circuit non trouvé avec ID:" << id_circuit;
        QMessageBox::warning(nullptr, "Attention", "Circuit non trouvé!");
        return false;
    }

    // Vérifier que tous les champs obligatoires sont remplis
    if (nom_circuit.isEmpty() || immatricule.isEmpty()) {
        QMessageBox::warning(nullptr, "Attention", "Le nom du circuit et l'immatricule sont obligatoires!");
        return false;
    }

    // Validation de la distance
    if (distance <= 0) {
        QMessageBox::warning(nullptr, "Attention", "La distance doit être un nombre positif!");
        return false;
    }

    // Validation de la durée estimée
    if (duree_estimee <= 0) {
        QMessageBox::warning(nullptr, "Attention", "La durée estimée doit être un nombre positif!");
        return false;
    }

    // Validation de la difficulté
    if (difficulte.isEmpty()) {
        QMessageBox::warning(nullptr, "Attention", "La difficulté est obligatoire!");
        return false;
    }

    // Validation du nom du circuit (longueur minimale)
    if (nom_circuit.length() < 2) {
        QMessageBox::warning(nullptr, "Attention", "Le nom du circuit doit contenir au moins 2 caractères!");
        return false;
    }

    // Vérifier que le véhicule existe
    QSqlQuery checkVehicule;
    checkVehicule.prepare("SELECT IMMATRICULE FROM VEHICULE WHERE IMMATRICULE = :immat");
    checkVehicule.bindValue(":immat", immatricule);
    if (!checkVehicule.exec() || !checkVehicule.next()) {
        QMessageBox::warning(nullptr, "Attention", "Le véhicule avec cette immatriculation n'existe pas!");
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE CIRCUIT SET "
                  "NOM_CIRCUIT = :nom, "
                  "DESCRIPTION = :desc, "
                  "DISTANCE = :distance, "
                  "DUREE_ESTIMEE = :duree, "
                  "DIFFICULTE = :diff, "
                  "IMMATRICULE = :immat "
                  "WHERE ID_CIRCUIT = :id");

    query.bindValue(":id", id_circuit);
    query.bindValue(":nom", nom_circuit);
    query.bindValue(":desc", description);
    query.bindValue(":distance", distance);
    query.bindValue(":duree", duree_estimee);
    query.bindValue(":diff", difficulte);
    query.bindValue(":immat", immatricule);

    bool success = query.exec();
    if (!success) {
        qDebug() << "❌ Erreur lors de la modification:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la modification:\n" + query.lastError().text());
    } else {
        qDebug() << "✅ Circuit modifié avec succès !";
        QMessageBox::information(nullptr, "Succès", "Circuit modifié avec succès ✓");
    }

    return success;
}

bool Circuit::supprimer(int id)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans supprimer()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }

    qDebug() << "=== DÉBUT SUPPRESSION CIRCUIT ===";
    qDebug() << "ID à supprimer:" << id;

    QSqlQuery query;
    query.prepare("DELETE FROM CIRCUIT WHERE ID_CIRCUIT = :id");
    query.bindValue(":id", id);

    bool success = query.exec();
    if (!success) {
        qDebug() << "❌ Erreur lors de la suppression:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la suppression:\n" + query.lastError().text());
    } else {
        if (query.numRowsAffected() == 0) {
            QMessageBox::warning(nullptr, "Attention", "Aucun circuit trouvé avec cet ID!");
            return false;
        }
        qDebug() << "✅ Circuit supprimé avec succès !";
        QMessageBox::information(nullptr, "Succès", "Circuit supprimé avec succès ✓");
    }

    return success;
}

QSqlQueryModel* Circuit::afficher()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans afficher()";
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_CIRCUIT, NOM_CIRCUIT, DESCRIPTION, DISTANCE, DUREE_ESTIMEE, DIFFICULTE, IMMATRICULE FROM CIRCUIT ORDER BY ID_CIRCUIT");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Circuit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Circuit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Distance (km)"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Durée (min)"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Difficulté"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Immatricule"));

    return model;
}

QSqlQueryModel* Circuit::rechercher(const QString& critere)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans rechercher()";
        return new QSqlQueryModel();
    }

    qDebug() << "🔍 Recherche de circuit avec critère:" << critere;

    QSqlQueryModel* model = new QSqlQueryModel();

    // Utiliser la syntaxe Oracle correcte: TO_CHAR() au lieu de CAST AS TEXT
    // Échapper les caractères spéciaux pour éviter les injections SQL
    QString critereEscaped = critere;
    critereEscaped.replace("'", "''"); // Échapper les apostrophes

    QString queryStr = "SELECT ID_CIRCUIT, NOM_CIRCUIT, DESCRIPTION, DISTANCE, DUREE_ESTIMEE, DIFFICULTE, IMMATRICULE FROM CIRCUIT WHERE "
                       "TO_CHAR(ID_CIRCUIT) LIKE '%" + critereEscaped + "%' "
                       "OR UPPER(NOM_CIRCUIT) LIKE UPPER('%" + critereEscaped + "%') "
                       "OR UPPER(DESCRIPTION) LIKE UPPER('%" + critereEscaped + "%') "
                       "OR TO_CHAR(DISTANCE) LIKE '%" + critereEscaped + "%' "
                       "OR TO_CHAR(DUREE_ESTIMEE) LIKE '%" + critereEscaped + "%' "
                       "OR UPPER(DIFFICULTE) LIKE UPPER('%" + critereEscaped + "%') "
                       "OR UPPER(IMMATRICULE) LIKE UPPER('%" + critereEscaped + "%')";

    qDebug() << "📝 Requête SQL:" << queryStr;

    QSqlQuery query;
    if (!query.exec(queryStr)) {
        qDebug() << "❌ Erreur SQL lors de la recherche:" << query.lastError().text();
        qDebug() << "   Erreur détaillée:" << query.lastError().databaseText();
    }

    model->setQuery(queryStr);

    // Vérifier les erreurs du modèle
    if (model->lastError().isValid()) {
        qDebug() << "❌ Erreur du modèle:" << model->lastError().text();
    }

    int rowCount = model->rowCount();
    qDebug() << "✅ Résultats trouvés:" << rowCount;

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Circuit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Circuit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Distance (km)"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Durée (min)"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Difficulté"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Immatricule"));

    return model;
}

QSqlQueryModel* Circuit::trier(const QString& critere)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans trier()";
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT ID_CIRCUIT, NOM_CIRCUIT, DESCRIPTION, DISTANCE, DUREE_ESTIMEE, DIFFICULTE, IMMATRICULE FROM CIRCUIT ORDER BY " + critere;
    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Circuit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Circuit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Distance (km)"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Durée (min)"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Difficulté"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Immatricule"));

    return model;
}

