#include "produitepicerie.h"
#include <iostream>

ProdEpicerie::ProdEpicerie(int id,  string libelle, double prix, int quantite, double poids)
    : Produit(id, libelle, prix, quantite), poids(poids) {}

ProdEpicerie::~ProdEpicerie() {}

double ProdEpicerie::getPoids() const {
    return poids;
}

void ProdEpicerie::setPoids(double poids) {
    this->poids = poids;
}

void ProdEpicerie::afficher() const {
    Produit::afficher();
    cout << "Poids: " << poids << " kg" << endl;
}
