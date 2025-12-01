// Employe.cpp
#pragma clang diagnostic ignored "-Winfinite-recursion"
#include "Employe.h"
#include <QDebug>  // Pour debug, optionnel

Employe::Employe()
    : m_id(0), m_nom(""), m_prenom(""), m_poste(""), m_motDePasse(""), m_numTel(""), m_dateEmbauche(QDate::currentDate())
{
}

Employe::Employe(int id, const QString& nom, const QString& prenom, const QString& poste,
                 const QString& motDePasse, const QString& numTel, const QDate& dateEmbauche)
    : m_id(id), m_nom(nom), m_prenom(prenom), m_poste(poste), m_motDePasse(motDePasse),
    m_numTel(numTel), m_dateEmbauche(dateEmbauche)
{
}

// Getters
int Employe::id() const
{
    return m_id;
}

QString Employe::nom() const
{
    return m_nom;
}

QString Employe::prenom() const
{
    return m_prenom;  // Pas "prenom()" !
}

QString Employe::poste() const
{
    return m_poste;
}

QString Employe::motDePasse() const
{
    return m_motDePasse;
}

QString Employe::numTel() const
{
    return m_numTel;
}

QDate Employe::dateEmbauche() const
{
    return m_dateEmbauche;
}

// Setters
void Employe::setId(int id)
{
    m_id = id;
}

void Employe::setNom(const QString& nom)
{
    m_nom = nom;
}

void Employe::setPrenom(const QString& prenom)
{
    m_prenom = prenom;
}

void Employe::setPoste(const QString& poste)
{
    m_poste = poste;
}

void Employe::setMotDePasse(const QString& motDePasse)
{
    m_motDePasse = motDePasse;
}

void Employe::setNumTel(const QString& numTel)
{
    m_numTel = numTel;
}

void Employe::setDateEmbauche(const QDate& dateEmbauche)
{
    m_dateEmbauche = dateEmbauche;
}

// Méthode toString pour représentation textuelle
QString Employe::toString() const
{
    return QString("ID: %1, Nom: %2 %3, Poste: %4, Tel: %5, Embauche: %6")
    .arg(m_id).arg(m_nom).arg(m_prenom).arg(m_poste).arg(m_numTel).arg(m_dateEmbauche.toString("yyyy-MM-dd"));
}
