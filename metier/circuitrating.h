#ifndef CIRCUITRATING_H
#define CIRCUITRATING_H

#include <QString>
#include <QSqlQueryModel>
#include <QDate>

class CircuitRating
{
private:
    int id_rating;
    int id_circuit;
    int id_client;
    int rating; // 1-5 stars
    QString commentaire;
    QDate date_rating;

public:
    CircuitRating();
    CircuitRating(int id, int circuitId, int clientId, int rate, QString comment, QDate date = QDate::currentDate());

    // Getters
    int getId() const { return id_rating; }
    int getCircuitId() const { return id_circuit; }
    int getClientId() const { return id_client; }
    int getRating() const { return rating; }
    QString getCommentaire() const { return commentaire; }
    QDate getDateRating() const { return date_rating; }

    // Setters
    void setId(int id) { id_rating = id; }
    void setCircuitId(int circuitId) { id_circuit = circuitId; }
    void setClientId(int clientId) { id_client = clientId; }
    void setRating(int rate) { rating = rate; }
    void setCommentaire(QString comment) { commentaire = comment; }
    void setDateRating(QDate date) { date_rating = date; }

    // CRUD Methods
    bool ajouter();
    bool modifier();
    static bool supprimer(int id);

    // Static Methods
    static bool existeDeja(int circuitId, int clientId);
    static QSqlQueryModel* afficherParCircuit(int circuitId);
    static QSqlQueryModel* afficherTous();
    static double getAverageRating(int circuitId);
    static int getTotalRatings(int circuitId);
};

#endif // CIRCUITRATING_H