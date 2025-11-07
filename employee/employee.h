#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QDate>

class Employee
{
public:
    Employee();
    Employee(const QString &id, const QString &nom, const QString &prenom, 
             const QString &poste, const QString &motDePasse, 
             const QString &telephone, const QDate &dateEmbauche);

    // Getters
    QString getId() const { return m_id; }
    QString getNom() const { return m_nom; }
    QString getPrenom() const { return m_prenom; }
    QString getPoste() const { return m_poste; }
    QString getMotDePasse() const { return m_motDePasse; }
    QString getTelephone() const { return m_telephone; }
    QDate getDateEmbauche() const { return m_dateEmbauche; }

    // Setters
    void setId(const QString &id) { m_id = id; }
    void setNom(const QString &nom) { m_nom = nom; }
    void setPrenom(const QString &prenom) { m_prenom = prenom; }
    void setPoste(const QString &poste) { m_poste = poste; }
    void setMotDePasse(const QString &motDePasse) { m_motDePasse = motDePasse; }
    void setTelephone(const QString &telephone) { m_telephone = telephone; }
    void setDateEmbauche(const QDate &dateEmbauche) { m_dateEmbauche = dateEmbauche; }

    // Méthodes utilitaires
    QString getNomComplet() const { return m_prenom + " " + m_nom; }
    bool isValid() const;

private:
    QString m_id;
    QString m_nom;
    QString m_prenom;
    QString m_poste;
    QString m_motDePasse;
    QString m_telephone;
    QDate m_dateEmbauche;
};

#endif // EMPLOYEE_H
