#ifndef SEANCE_H
#define SEANCE_H

#include <QString>
#include <QDate>
#include <QTime>
#include <QSqlQueryModel>
#include <QMap>
#include <QList>

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
    // Constructeurs
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
    void setDate(const QDate& date) { date_seance = date; }
    void setHeure(const QTime& heure) { heure_debut = heure; }
    void setDuree(int d) { duree = d; }
    void setType(const QString& t) { type = t; }
    void setAppareil(const QString& a) { appareil = a; }
    void setIdClient(int id) { id_client = id; }
    void setImmatricule(const QString& immat) { immatricule = immat; }
    void setIdMoniteur(int id) { id_moniteur = id; }

    // Méthodes CRUD
    bool ajouter();
    bool modifier();
    bool supprimer(int id);
    static QSqlQueryModel* afficher();
    static QSqlQueryModel* rechercher(const QString& critere);

    static QSqlQueryModel* trier(const QString& critere);
    static bool existeDeja(int id);

    // Méthodes statistiques et emails

    static bool envoyerRappelEmail(int idSeance);
    static QMap<QString, int> getStatistiquesParType();

    // Méthodes pour le calendrier
    static QList<Seance> getSeancesParDate(const QDate& date);
    static QMap<QDate, int> getSeancesParMois(int mois, int annee);
    static bool exporterPDF(const QString& fileName, QSqlQueryModel* model = nullptr);
    static QList<Seance> getAllSeances();
    static QString genererHTMLSeances(const QList<Seance>& seances);
};

#endif // SEANCE_H
