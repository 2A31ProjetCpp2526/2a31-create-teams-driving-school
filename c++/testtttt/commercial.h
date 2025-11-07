
#ifndef COMMERCIAL_H
#define COMMERCIAL_H

#include "Employe.h"

class Commercial : public Employe {
private:
    double fixe;
    double interessement;

public:
    Commercial(const string nom, int matricule, double salaireBase, double fixe, double interessement);
    ~Commercial();

    double getFixe() const;
    double getInteressement() const;

    void setFixe(double fixe);
    void setInteressement(double interessement);

    void afficher() const;
};

#endif // COMMERCIAL_H
