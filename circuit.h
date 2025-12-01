#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <QString>
#include <QSqlQueryModel>
#include <QDate>

class Circuit
{
private:
    int id_circuit;
    QString nom_circuit;
    QString description;
    double distance;  // en km
    int duree_estimee; // en minutes
    QString difficulte; // Facile, Moyen, Difficile
    QString immatricule; // Relation avec Véhicule

public:
    Circuit();
    Circuit(int id, QString nom, QString desc, double dist, int duree, QString diff, QString immat);

    // Getters
    int getId() const { return id_circuit; }
    QString getNom() const { return nom_circuit; }
    QString getDescription() const { return description; }
    double getDistance() const { return distance; }
    int getDureeEstimee() const { return duree_estimee; }
    QString getDifficulte() const { return difficulte; }
    QString getImmatricule() const { return immatricule; }

    // Setters
    void setId(int id) { id_circuit = id; }
    void setNom(QString nom) { nom_circuit = nom; }
    void setDescription(QString desc) { description = desc; }
    void setDistance(double dist) { distance = dist; }
    void setDureeEstimee(int duree) { duree_estimee = duree; }
    void setDifficulte(QString diff) { difficulte = diff; }
    void setImmatricule(QString immat) { immatricule = immat; }

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

#endif // CIRCUIT_H


