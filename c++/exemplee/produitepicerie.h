
#ifndef PRODEPICERIE_H
#define PRODEPICERIE_H

#include "Produit.h"

class ProdEpicerie : public Produit {
private:
    double poids;

public:
    ProdEpicerie(int id,  string libelle, double prix, int quantite, double poids);
    ~ProdEpicerie();

    double getPoids() const;
    void setPoids(double poids);

    void afficher() const;
};

#endif // PRODEPICERIE_H


