#include "employee.h"

Employee::Employee()
    : m_id("")
    , m_nom("")
    , m_prenom("")
    , m_poste("")
    , m_motDePasse("")
    , m_telephone("")
    , m_dateEmbauche(QDate::currentDate())
{
}

Employee::Employee(const QString &id, const QString &nom, const QString &prenom, 
                   const QString &poste, const QString &motDePasse, 
                   const QString &telephone, const QDate &dateEmbauche)
    : m_id(id)
    , m_nom(nom)
    , m_prenom(prenom)
    , m_poste(poste)
    , m_motDePasse(motDePasse)
    , m_telephone(telephone)
    , m_dateEmbauche(dateEmbauche)
{
}

bool Employee::isValid() const
{
    return !m_id.isEmpty() && 
           !m_nom.isEmpty() && 
           !m_prenom.isEmpty() && 
           !m_poste.isEmpty() && 
           !m_motDePasse.isEmpty() && 
           !m_telephone.isEmpty() &&
           m_dateEmbauche.isValid();
}
