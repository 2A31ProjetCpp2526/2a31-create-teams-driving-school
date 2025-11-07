#ifndef MAGASIN_H
#define MAGASIN_H

#include <vector>
#include "produitepicerie.h"

class Magasin {
private:
    string nom;
    vector<ProdEpicerie> produits;

public:
    Magasin(const string nom);
    ~Magasin();

    void ajouterProdEpicerie(const ProdEpicerie& produit);
    void afficherTousLesProduits() const;
    void supprimerProduitsEnRupture();
    double calculerPrixTotal() const;
};

#endif // MAGASIN_H


