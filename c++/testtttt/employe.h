#ifndef EMPLOYE_H
#define EMPLOYE_H
using namespace std;


#include <string>
#include <iostream>
class Employe {
protected:
    string nom;
    int matricule;
    double salaireBase;

public:
    Employe(const string nom, int matricule, double salaireBase);
    ~Employe();

    string getNom() const;
    int getMatricule() const;
    double getSalaireBase() const;

    void setNom(const string nom);
    void setSalaireBase(double salaireBase);

    void afficher() const;
};

#endif // EMPLOYE_H

