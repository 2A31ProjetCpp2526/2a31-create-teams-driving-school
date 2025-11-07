#include "Magasin.h"
#include <iostream>

Magasin::Magasin(const string nom) : nom(nom) {}

Magasin::~Magasin() {}

void Magasin::ajouterProdEpicerie(const ProdEpicerie& produit) {
    for (size_t i = 0; i < produits.size(); ++i) {
        if (produits[i].getId() == produit.getId()) {
            cout << "Erreur: Un produit avec le meme ID existe deja." << endl;
            return;
        }
    }
    produits.push_back(produit);
}

void Magasin::afficherTousLesProduits() const {
    for (size_t i = 0; i < produits.size(); ++i) {
        produits[i].afficher();
    }
}

void Magasin::supprimerProduitsEnRupture() {
    for (size_t i = 0; i < produits.size(); ++i) {
        if (produits[i].getQuantite() == 0) {
            produits.erase(produits.begin() + i);
            --i; // Réajuster l'indice après suppression
        }
    }
}

double Magasin::calculerPrixTotal() const {
    double total = 0;
    for (size_t i = 0; i < produits.size(); ++i) {
        if (produits[i].getQuantite() > 0) {
            total += produits[i].getPrix() * produits[i].getQuantite();
        }
    }
    return total;
}

