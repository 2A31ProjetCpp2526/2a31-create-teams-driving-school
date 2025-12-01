#include "circuitrating.h"
#include "connection.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>

CircuitRating::CircuitRating() : id_rating(0), id_circuit(0), id_client(0), rating(0) {}

CircuitRating::CircuitRating(int id, int circuitId, int clientId, int rating, QString comment)
    : id_rating(id), id_circuit(circuitId), id_client(clientId), rating(rating), commentaire(comment)
{
    date_rating = QDateTime::currentDateTime();
}

bool CircuitRating::ajouter()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans CircuitRating::ajouter()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }

    qDebug() << "=== TENTATIVE AJOUT RATING ===";
    qDebug() << "Circuit ID:" << id_circuit;
    qDebug() << "Client ID:" << id_client;
    qDebug() << "Rating:" << rating;
    qDebug() << "Commentaire:" << commentaire;

    // Vérifier si le client a déjà noté ce circuit
    if (existeDeja(id_circuit, id_client)) {
        qDebug() << "❌ Rating existe déjà pour ce client/circuit";
        QMessageBox::warning(nullptr, "Attention", "Vous avez déjà noté ce circuit!");
        return false;
    }

    // Validation du rating (1-5)
    if (rating < 1 || rating > 5) {
        QMessageBox::warning(nullptr, "Attention", "Le rating doit être entre 1 et 5 étoiles!");
        return false;
    }

    // Validation du commentaire (longueur maximale)
    if (commentaire.length() > 500) {
        QMessageBox::warning(nullptr, "Attention", "Le commentaire ne peut pas dépasser 500 caractères!");
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO CIRCUIT_RATING (ID_CIRCUIT, ID_CLIENT, RATING, COMMENTAIRE, DATE_RATING) "
                  "VALUES (:circuitId, :clientId, :rating, :comment, :dateRating)");

    query.bindValue(":circuitId", id_circuit);
    query.bindValue(":clientId", id_client);
    query.bindValue(":rating", rating);
    query.bindValue(":comment", commentaire);
    query.bindValue(":dateRating", date_rating);

    bool success = query.exec();

    if (success) {
        qDebug() << "✅ RATING AJOUTÉ AVEC SUCCÈS !";
        QMessageBox::information(nullptr, "Succès", "Votre note a été enregistrée ✓");
        return true;
    } else {
        qDebug() << "❌ ERREUR SQL:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de l'ajout de la note:\n" + query.lastError().text());
        return false;
    }
}

bool CircuitRating::modifier()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans CircuitRating::modifier()";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE CIRCUIT_RATING SET "
                  "RATING = :rating, COMMENTAIRE = :comment "
                  "WHERE ID_RATING = :id");

    query.bindValue(":id", id_rating);
    query.bindValue(":rating", rating);
    query.bindValue(":comment", commentaire);

    bool success = query.exec();
    if (!success) {
        qDebug() << "❌ Erreur lors de la modification:" << query.lastError().text();
    } else {
        qDebug() << "✅ Rating modifié avec succès !";
    }

    return success;
}

bool CircuitRating::supprimer(int id)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans CircuitRating::supprimer()";
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM CIRCUIT_RATING WHERE ID_RATING = :id");
    query.bindValue(":id", id);

    bool success = query.exec();
    if (!success) {
        qDebug() << "❌ Erreur lors de la suppression:" << query.lastError().text();
    } else {
        if (query.numRowsAffected() == 0) {
            QMessageBox::warning(nullptr, "Attention", "Aucune note trouvée avec cet ID!");
            return false;
        }
        qDebug() << "✅ Rating supprimé avec succès !";
    }

    return success;
}

bool CircuitRating::existeDeja(int circuitId, int clientId)
{
    if (!Connection::isDatabaseOpen()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_RATING FROM CIRCUIT_RATING WHERE ID_CIRCUIT = :circuitId AND ID_CLIENT = :clientId");
    query.bindValue(":circuitId", circuitId);
    query.bindValue(":clientId", clientId);

    if (query.exec() && query.next()) {
        return true;
    }
    return false;
}

QSqlQueryModel* CircuitRating::afficherTous()
{
    if (!Connection::isDatabaseOpen()) {
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT cr.ID_RATING, c.NOM_CIRCUIT, cl.NOM || ' ' || cl.PRENOM as CLIENT, cr.RATING, cr.COMMENTAIRE, cr.DATE_RATING "
                    "FROM CIRCUIT_RATING cr "
                    "JOIN CIRCUIT c ON cr.ID_CIRCUIT = c.ID_CIRCUIT "
                    "JOIN CLIENT cl ON cr.ID_CLIENT = cl.ID_CLIENT "
                    "ORDER BY cr.DATE_RATING DESC");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Rating"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Circuit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Rating"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Commentaire"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date"));

    return model;
}

QSqlQueryModel* CircuitRating::afficherParCircuit(int circuitId)
{
    if (!Connection::isDatabaseOpen()) {
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT cr.ID_RATING, cl.NOM || ' ' || cl.PRENOM as CLIENT, cr.RATING, cr.COMMENTAIRE, cr.DATE_RATING "
                  "FROM CIRCUIT_RATING cr "
                  "JOIN CLIENT cl ON cr.ID_CLIENT = cl.ID_CLIENT "
                  "WHERE cr.ID_CIRCUIT = :circuitId "
                  "ORDER BY cr.DATE_RATING DESC");
    query.bindValue(":circuitId", circuitId);
    query.exec();

    model->setQuery(query);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Rating"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Client"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Rating"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Commentaire"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date"));

    return model;
}

double CircuitRating::getAverageRating(int circuitId)
{
    if (!Connection::isDatabaseOpen()) {
        return 0.0;
    }

    QSqlQuery query;
    query.prepare("SELECT AVG(RATING) as avg_rating FROM CIRCUIT_RATING WHERE ID_CIRCUIT = :circuitId");
    query.bindValue(":circuitId", circuitId);

    if (query.exec() && query.next()) {
        return query.value(0).toDouble();
    }
    return 0.0;
}

int CircuitRating::getTotalRatings(int circuitId)
{
    if (!Connection::isDatabaseOpen()) {
        return 0;
    }

    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM CIRCUIT_RATING WHERE ID_CIRCUIT = :circuitId");
    query.bindValue(":circuitId", circuitId);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

QSqlQueryModel* CircuitRating::getCircuitRatingsSummary()
{
    if (!Connection::isDatabaseOpen()) {
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT c.ID_CIRCUIT, c.NOM_CIRCUIT, "
                    "COALESCE(AVG(cr.RATING), 0) as MOYENNE_RATING, "
                    "COUNT(cr.ID_RATING) as TOTAL_RATINGS "
                    "FROM CIRCUIT c "
                    "LEFT JOIN CIRCUIT_RATING cr ON c.ID_CIRCUIT = cr.ID_CIRCUIT "
                    "GROUP BY c.ID_CIRCUIT, c.NOM_CIRCUIT "
                    "ORDER BY c.ID_CIRCUIT");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Circuit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Circuit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Moyenne Rating"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Total Ratings"));

    return model;
}

QSqlQueryModel* CircuitRating::getRatingDistribution(int circuitId)
{
    if (!Connection::isDatabaseOpen()) {
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT RATING, COUNT(*) as COUNT "
                  "FROM CIRCUIT_RATING "
                  "WHERE ID_CIRCUIT = :circuitId "
                  "GROUP BY RATING "
                  "ORDER BY RATING");
    query.bindValue(":circuitId", circuitId);
    query.exec();

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("Rating"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Count"));

    return model;
}

QSqlQueryModel* CircuitRating::getTopRatedCircuits(int limit)
{
    if (!Connection::isDatabaseOpen()) {
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT c.ID_CIRCUIT, c.NOM_CIRCUIT, "
                  "AVG(cr.RATING) as MOYENNE_RATING, COUNT(cr.ID_RATING) as TOTAL_RATINGS "
                  "FROM CIRCUIT c "
                  "LEFT JOIN CIRCUIT_RATING cr ON c.ID_CIRCUIT = cr.ID_CIRCUIT "
                  "GROUP BY c.ID_CIRCUIT, c.NOM_CIRCUIT "
                  "HAVING COUNT(cr.ID_RATING) > 0 "
                  "ORDER BY AVG(cr.RATING) DESC, COUNT(cr.ID_RATING) DESC "
                  "FETCH FIRST :limit ROWS ONLY");
    query.bindValue(":limit", limit);
    query.exec();

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Circuit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Circuit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Moyenne"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Total Ratings"));

    return model;
}

QSqlQueryModel* CircuitRating::getRecentRatings(int limit)
{
    if (!Connection::isDatabaseOpen()) {
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT cr.ID_RATING, c.NOM_CIRCUIT, cl.NOM || ' ' || cl.PRENOM as CLIENT, "
                  "cr.RATING, cr.COMMENTAIRE, cr.DATE_RATING "
                  "FROM CIRCUIT_RATING cr "
                  "JOIN CIRCUIT c ON cr.ID_CIRCUIT = c.ID_CIRCUIT "
                  "JOIN CLIENT cl ON cr.ID_CLIENT = cl.ID_CLIENT "
                  "ORDER BY cr.DATE_RATING DESC "
                  "FETCH FIRST :limit ROWS ONLY");
    query.bindValue(":limit", limit);
    query.exec();

    model->setQuery(query);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Circuit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Rating"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Commentaire"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date"));

    return model;
}

QMap<int, int> CircuitRating::getOverallRatingDistribution()
{
    QMap<int, int> distribution;
    if (!Connection::isDatabaseOpen()) {
        return distribution;
    }

    QSqlQuery query("SELECT RATING, COUNT(*) as COUNT FROM CIRCUIT_RATING GROUP BY RATING ORDER BY RATING");

    while (query.next()) {
        int rating = query.value(0).toInt();
        int count = query.value(1).toInt();
        distribution[rating] = count;
    }

    return distribution;
}