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
    qDebug() << "=== DÉBUT MODIFICATION SÉANCE ===";

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
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT * FROM SEANCE WHERE "
                       "CAST(ID_SEANCE AS TEXT) LIKE '%" + critere + "%' "
                                   "OR TYPE LIKE '%" + critere + "%' "
                                   "OR APPAREIL LIKE '%" + critere + "%' "
                                   "OR CAST(ID_CLIENT AS TEXT) LIKE '%" + critere + "%' "
                                   "OR IMMATRICULE LIKE '%" + critere + "%' "
                                   "OR CAST(ID_MONITEUR AS TEXT) LIKE '%" + critere + "%'";

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

QSqlQueryModel* Seance::trier(const QString& critere)
{
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
