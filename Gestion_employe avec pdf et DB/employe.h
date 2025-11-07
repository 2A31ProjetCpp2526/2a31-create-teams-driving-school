// Employe.h
#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QDate>

class Employe
{
public:
    // Constructeur par défaut
    Employe();

    // Constructeur avec paramètres
    Employe(int id, const QString& nom, const QString& prenom, const QString& poste,
            const QString& motDePasse, const QString& numTel, const QDate& dateEmbauche);

    // Getters
    int id() const;
    QString nom() const;
    QString prenom() const;
    QString poste() const;
    QString motDePasse() const;
    QString numTel() const;
    QDate dateEmbauche() const;

    // Setters
    void setId(int id);
    void setNom(const QString& nom);
    void setPrenom(const QString& prenom);
    void setPoste(const QString& poste);
    void setMotDePasse(const QString& motDePasse);
    void setNumTel(const QString& numTel);
    void setDateEmbauche(const QDate& dateEmbauche);

    // Méthode pour afficher les informations (utile pour debug ou affichage)
    QString toString() const;

private:
    int m_id;
    QString m_nom;
    QString m_prenom;
    QString m_poste;
    QString m_motDePasse;
    QString m_numTel;
    QDate m_dateEmbauche;
};

#endif // EMPLOYE_H
