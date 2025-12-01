#include "seance.h"
#include "connection.h"
#include <QSqlQuery>
#include <QDebug>

Seance::Seance() : id_seance(0), duree(0), id_client(0), id_moniteur(0) {}

Seance::Seance(int id, QDate date, QTime heure, int duree, QString type, QString appareil,
               int idClient, QString immatricule, int idMoniteur)
    : id_seance(id), date_seance(date), heure_debut(heure), duree(duree), type(type),
    appareil(appareil), id_client(idClient), immatricule(immatricule), id_moniteur(idMoniteur)
{}

bool Seance::existeDeja(int id)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans existeDeja()";
        return false;
    }
    
    QSqlQuery query;
    query.prepare("SELECT ID_SEANCE FROM SEANCE WHERE ID_SEANCE = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return true;
    }
    return false;
}

bool Seance::ajouter()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans ajouter()";
        return false;
    }
    
    qDebug() << "=== TENTATIVE AJOUT SÉANCE ===";
    qDebug() << "ID:" << id_seance;
    qDebug() << "Date:" << date_seance;
    qDebug() << "Heure:" << heure_debut;
    qDebug() << "Durée:" << duree;
    qDebug() << "Type:" << type;
    qDebug() << "Appareil:" << appareil;
    qDebug() << "ID Client:" << id_client;
    qDebug() << "Immatricule:" << immatricule;
    qDebug() << "ID Moniteur:" << id_moniteur;

    // Validation des champs
    if (id_seance <= 0) {
        qDebug() << "❌ ID séance invalide:" << id_seance;
        return false;
    }
    
    if (duree <= 0) {
        qDebug() << "❌ Durée invalide:" << duree;
        return false;
    }
    
    if (appareil.isEmpty()) {
        qDebug() << "❌ Appareil vide";
        return false;
    }
    
    if (id_client <= 0) {
        qDebug() << "❌ ID client invalide:" << id_client;
        return false;
    }
    
    if (immatricule.isEmpty()) {
        qDebug() << "❌ Immatricule vide";
        return false;
    }
    
    if (id_moniteur <= 0) {
        qDebug() << "❌ ID moniteur invalide:" << id_moniteur;
        return false;
    }
    
    if (type.isEmpty()) {
        qDebug() << "❌ Type vide";
        return false;
    }
    
    if (!date_seance.isValid()) {
        qDebug() << "❌ Date invalide";
        return false;
    }
    
    if (!heure_debut.isValid()) {
        qDebug() << "❌ Heure invalide";
        return false;
    }

    // Vérifier si l'ID existe déjà
    if (existeDeja(id_seance)) {
        qDebug() << "❌ ID existe déjà:" << id_seance;
        return false;
    }

    QSqlQuery query;

    // REQUÊTE CORRIGÉE selon la structure réelle de ta table
    query.prepare("INSERT INTO SEANCE (ID_SEANCE, IMMATRICULE, ID_CLIENT, ID_MONITEUR, DATE_SEANCE, HEURE_DEBUT, TYPE, APPAREIL, DUREE) "
                  "VALUES (:id, :immatricule, :id_client, :id_moniteur, :date, :heure, :type, :appareil, :duree)");

    query.bindValue(":id", id_seance);
    query.bindValue(":immatricule", immatricule);
    query.bindValue(":id_client", id_client);
    query.bindValue(":id_moniteur", id_moniteur);
    query.bindValue(":date", date_seance);

    // Convertir QTime en QString pour HEURE_DEBUT (VARCHAR2 dans ta base)
    query.bindValue(":heure", heure_debut.toString("HH:mm"));

    query.bindValue(":type", type);
    query.bindValue(":appareil", appareil);
    query.bindValue(":duree", duree);

    bool success = query.exec();

    if (success) {
        qDebug() << "✅ AJOUT RÉUSSI !";
        return true;
    } else {
        qDebug() << "❌ ERREUR SQL:" << query.lastError().text();
        qDebug() << "Message driver:" << query.lastError().driverText();
        qDebug() << "Message database:" << query.lastError().databaseText();
        return false;
    }
}

bool Seance::modifier()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans modifier()";
        return false;
    }
    
    qDebug() << "=== DÉBUT MODIFICATION SÉANCE ===";

    // Validation des champs
    if (id_seance <= 0) {
        qDebug() << "❌ ID séance invalide:" << id_seance;
        return false;
    }
    
    if (duree <= 0) {
        qDebug() << "❌ Durée invalide:" << duree;
        return false;
    }
    
    if (appareil.isEmpty()) {
        qDebug() << "❌ Appareil vide";
        return false;
    }
    
    if (id_client <= 0) {
        qDebug() << "❌ ID client invalide:" << id_client;
        return false;
    }
    
    if (immatricule.isEmpty()) {
        qDebug() << "❌ Immatricule vide";
        return false;
    }
    
    if (id_moniteur <= 0) {
        qDebug() << "❌ ID moniteur invalide:" << id_moniteur;
        return false;
    }
    
    if (type.isEmpty()) {
        qDebug() << "❌ Type vide";
        return false;
    }
    
    if (!date_seance.isValid()) {
        qDebug() << "❌ Date invalide";
        return false;
    }
    
    if (!heure_debut.isValid()) {
        qDebug() << "❌ Heure invalide";
        return false;
    }

    // Vérifier si la séance existe
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT ID_SEANCE FROM SEANCE WHERE ID_SEANCE = :id");
    checkQuery.bindValue(":id", id_seance);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "❌ Séance non trouvée avec ID:" << id_seance;
        return false;
    }

    QSqlQuery query;

    // REQUÊTE CORRIGÉE
    query.prepare("UPDATE SEANCE SET IMMATRICULE=:immatricule, ID_CLIENT=:id_client, ID_MONITEUR=:id_moniteur, "
                  "DATE_SEANCE=:date, HEURE_DEBUT=:heure, TYPE=:type, APPAREIL=:appareil, DUREE=:duree "
                  "WHERE ID_SEANCE=:id");

    query.bindValue(":id", id_seance);
    query.bindValue(":immatricule", immatricule);
    query.bindValue(":id_client", id_client);
    query.bindValue(":id_moniteur", id_moniteur);
    query.bindValue(":date", date_seance);
    query.bindValue(":heure", heure_debut.toString("HH:mm"));  // Conversion en QString
    query.bindValue(":type", type);
    query.bindValue(":appareil", appareil);
    query.bindValue(":duree", duree);

    bool success = query.exec();
    if (!success) {
        qDebug() << "❌ Erreur lors de la modification:" << query.lastError().text();
    } else {
        qDebug() << "✅ Séance modifiée avec succès !";
    }

    return success;
}

bool Seance::supprimer(int id)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans supprimer()";
        return false;
    }
    
    qDebug() << "=== DÉBUT SUPPRESSION SÉANCE ===";
    qDebug() << "ID à supprimer:" << id;

    QSqlQuery query;
    query.prepare("DELETE FROM SEANCE WHERE ID_SEANCE = :id");
    query.bindValue(":id", id);

    bool success = query.exec();
    if (!success) {
        qDebug() << "❌ Erreur lors de la suppression:" << query.lastError().text();
    } else {
        qDebug() << "✅ Séance supprimée avec succès !";
    }

    return success;
}

QSqlQueryModel* Seance::afficher()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans afficher()";
        return new QSqlQueryModel();  // Retourner un modèle vide plutôt que nullptr
    }
    
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM SEANCE");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Séance"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Immatricule"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Moniteur"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Heure Début"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Appareil"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Durée"));

    return model;
}

QSqlQueryModel* Seance::rechercher(const QString& critere)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans rechercher()";
        return new QSqlQueryModel();
    }
    
    qDebug() << "🔍 Recherche de séance avec critère:" << critere;
    
    QSqlQueryModel* model = new QSqlQueryModel();
    
    // Utiliser la syntaxe Oracle correcte: TO_CHAR() au lieu de CAST AS TEXT
    // Échapper les caractères spéciaux pour éviter les injections SQL
    QString critereEscaped = critere;
    critereEscaped.replace("'", "''"); // Échapper les apostrophes
    
    QString queryStr = "SELECT * FROM SEANCE WHERE "
                       "TO_CHAR(ID_SEANCE) LIKE '%" + critereEscaped + "%' "
                       "OR UPPER(TYPE) LIKE UPPER('%" + critereEscaped + "%') "
                       "OR UPPER(APPAREIL) LIKE UPPER('%" + critereEscaped + "%') "
                       "OR TO_CHAR(ID_CLIENT) LIKE '%" + critereEscaped + "%' "
                       "OR UPPER(IMMATRICULE) LIKE UPPER('%" + critereEscaped + "%') "
                       "OR TO_CHAR(ID_MONITEUR) LIKE '%" + critereEscaped + "%'";

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

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Séance"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Immatricule"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Moniteur"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Heure Début"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Appareil"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Durée"));

    return model;
}

QSqlQueryModel* Seance::trier(const QString& critere)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans trier()";
        return new QSqlQueryModel();
    }
    
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT * FROM SEANCE ORDER BY " + critere;
    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Séance"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Immatricule"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Moniteur"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Heure Début"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Appareil"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Durée"));

    return model;
}
