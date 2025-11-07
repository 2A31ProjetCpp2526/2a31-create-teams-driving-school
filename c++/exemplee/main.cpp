#include "Magasin.h"
#include "produitepicerie.h"
#include <iostream>

int main() {
    Magasin magasin("Mon Magasin");

    ProdEpicerie p1(1, "Pommes", 2.5, 10, 1.5);
    ProdEpicerie p2(2, "Bananes", 1.8, 0, 2.0);
    ProdEpicerie p3(3, "Oranges", 3.0, 5, 1.0);

    magasin.ajouterProdEpicerie(p1);
    magasin.ajouterProdEpicerie(p2);
    magasin.ajouterProdEpicerie(p3);

    cout << "Affichage de tous les produits:" <<endl;
    magasin.afficherTousLesProduits();

    cout << "Suppression des produits en rupture de stock..." << endl;
    magasin.supprimerProduitsEnRupture();

    cout << "Affichage apres suppression:" << endl;
    magasin.afficherTousLesProduits();

    cout << "Prix total des produits en stock: " << magasin.calculerPrixTotal() << endl;

    return 0;
}
