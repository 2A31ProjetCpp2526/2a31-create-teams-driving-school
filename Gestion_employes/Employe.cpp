// Employe.cpp
#pragma clang diagnostic ignored "-Winfinite-recursion"
#include "Employe.h"
#include <QDebug>  // Pour debug, optionnel

Employe::Employe()
    : m_ID_EMPLOYE(0), m_NOM_EMPLOYE(""), m_PRENOM(""), m_POSTE(""), m_MOT_DE_PASSEE(""), m_TEL_EMPLOYE(""), m_DATE_EMBAUCHE(QDate::currentDate())
{
}

Employe::Employe(int ID_EMPLOYE, const QString& NOM_EMPLOYE, const QString& PRENOM, const QString& POSTE,
                 const QString& MOT_DE_PASSEE, const QString& TEL_EMPLOYE, const QDate& DATE_EMBAUCHE)
    : m_ID_EMPLOYE(ID_EMPLOYE), m_NOM_EMPLOYE(NOM_EMPLOYE), m_PRENOM(PRENOM), m_POSTE(POSTE), m_MOT_DE_PASSEE(MOT_DE_PASSEE),
    m_TEL_EMPLOYE(TEL_EMPLOYE), m_DATE_EMBAUCHE(DATE_EMBAUCHE)
{
}

// Getters
int Employe::ID_EMPLOYE() const
{
    return m_ID_EMPLOYE;
}

QString Employe::NOM_EMPLOYE() const
{
    return m_NOM_EMPLOYE;
}

QString Employe::PRENOM() const
{
    return m_PRENOM;  // Pas "PRENOM()" !
}

QString Employe::POSTE() const
{
    return m_POSTE;
}

QString Employe::MOT_DE_PASSEE() const
{
    return m_MOT_DE_PASSEE;
}

QString Employe::TEL_EMPLOYE() const
{
    return m_TEL_EMPLOYE;
}

QDate Employe::DATE_EMBAUCHE() const
{
    return m_DATE_EMBAUCHE;
}

// Setters
void Employe::setID_EMPLOYE(int ID_EMPLOYE)
{
    m_ID_EMPLOYE = ID_EMPLOYE;
}

void Employe::setNOM_EMPLOYE(const QString& NOM_EMPLOYE)
{
    m_NOM_EMPLOYE = NOM_EMPLOYE;
}

void Employe::setPRENOM(const QString& PRENOM)
{
    m_PRENOM = PRENOM;
}

void Employe::setPOSTE(const QString& POSTE)
{
    m_POSTE = POSTE;
}

void Employe::setMOT_DE_PASSEE(const QString& MOT_DE_PASSEE)
{
    m_MOT_DE_PASSEE = MOT_DE_PASSEE;
}

void Employe::setTEL_EMPLOYE(const QString& TEL_EMPLOYE)
{
    m_TEL_EMPLOYE = TEL_EMPLOYE;
}

void Employe::setDATE_EMBAUCHE(const QDate& DATE_EMBAUCHE)
{
    m_DATE_EMBAUCHE = DATE_EMBAUCHE;
}

// Méthode toString pour représentation textuelle
QString Employe::toString() const
{
    return QString("ID_EMPLOYE: %1, NOM_EMPLOYE: %2 %3, POSTE: %4, Tel: %5, Embauche: %6")
    .arg(m_ID_EMPLOYE).arg(m_NOM_EMPLOYE).arg(m_PRENOM).arg(m_POSTE).arg(m_TEL_EMPLOYE).arg(m_DATE_EMBAUCHE.toString("yyyy-MM-dd"));
}
