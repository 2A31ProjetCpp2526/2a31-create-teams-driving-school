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
    Employe(int ID_EMPLOYE, const QString& NOM_EMPLOYE, const QString& PRENOM, const QString& POSTE,
            const QString& MOT_DE_PASSEE, const QString& TEL_EMPLOYE, const QDate& DATE_EMBAUCHE);

    // Getters
    int ID_EMPLOYE() const;
    QString NOM_EMPLOYE() const;
    QString PRENOM() const;
    QString POSTE() const;
    QString MOT_DE_PASSEE() const;
    QString TEL_EMPLOYE() const;
    QDate DATE_EMBAUCHE() const;

    // Setters
    void setID_EMPLOYE(int ID_EMPLOYE);
    void setNOM_EMPLOYE(const QString& NOM_EMPLOYE);
    void setPRENOM(const QString& PRENOM);
    void setPOSTE(const QString& POSTE);
    void setMOT_DE_PASSEE(const QString& MOT_DE_PASSEE);
    void setTEL_EMPLOYE(const QString& TEL_EMPLOYE);
    void setDATE_EMBAUCHE(const QDate& DATE_EMBAUCHE);

    // Méthode pour afficher les informations (utile pour debug ou affichage)
    QString toString() const;

private:
    int m_ID_EMPLOYE;
    QString m_NOM_EMPLOYE;
    QString m_PRENOM;
    QString m_POSTE;
    QString m_MOT_DE_PASSEE;
    QString m_TEL_EMPLOYE;
    QDate m_DATE_EMBAUCHE;
};

#endif // EMPLOYE_H

