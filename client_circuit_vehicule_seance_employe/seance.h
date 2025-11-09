#ifndef SEANCE_H
#define SEANCE_H

#include <QDate>
#include <QTime>
#include <QString>
#include <QSqlQueryModel>

class Seance
{
private:
    int id_seance;
    QDate date_seance;
    QTime heure_debut;
    int duree;
    QString type;
    QString appareil;
    int id_client;
    QString immatricule;
    int id_moniteur;

public:
    Seance();
    Seance(int id, QDate date, QTime heure, int duree, QString type, QString appareil,
           int idClient, QString immatricule, int idMoniteur);

    // Getters
    int getId() const { return id_seance; }
    QDate getDate() const { return date_seance; }
    QTime getHeure() const { return heure_debut; }
    int getDuree() const { return duree; }
    QString getType() const { return type; }
    QString getAppareil() const { return appareil; }
    int getIdClient() const { return id_client; }
    QString getImmatricule() const { return immatricule; }
    int getIdMoniteur() const { return id_moniteur; }

    // Setters
    void setId(int id) { id_seance = id; }
    void setDate(QDate date) { date_seance = date; }
    void setHeure(QTime heure) { heure_debut = heure; }
    void setDuree(int d) { duree = d; }
    void setType(QString t) { type = t; }
    void setAppareil(QString a) { appareil = a; }
    void setIdClient(int id) { id_client = id; }
    void setImmatricule(QString immat) { immatricule = immat; }
    void setIdMoniteur(int id) { id_moniteur = id; }

    // Méthodes CRUD
    bool ajouter();
    bool modifier();
    static bool supprimer(int id);


    // Méthodes statiques
    static bool existeDeja(int id);
    static QSqlQueryModel* afficher();
    static QSqlQueryModel* rechercher(const QString& critere);
    static QSqlQueryModel* trier(const QString& critere);
};

#endif // SEANCE_H
