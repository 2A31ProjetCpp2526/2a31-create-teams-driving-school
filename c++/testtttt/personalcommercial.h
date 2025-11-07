#ifndef PERSONNELCOMMERCIAL_H
#define PERSONNELCOMMERCIAL_H

#include <vector>
#include "Commercial.h"

class PersonnelCommercial {
private:
    vector<Commercial> commerciaux;

public:
    PersonnelCommercial();
    ~PersonnelCommercial();

    void ajouterCommercial(const Commercial& commercial);
    void afficherCommerciaux() const;
    void supprimerCommerciauxFaibleVente(double seuil);
};

#endif // PERSONNELCOMMERCIAL_H
