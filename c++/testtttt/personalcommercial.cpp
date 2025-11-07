#include "Personalcommercial.h"
#include <iostream>

PersonnelCommercial::PersonnelCommercial() {}

PersonnelCommercial::~PersonnelCommercial() {}

void PersonnelCommercial::ajouterCommercial(const Commercial& commercial) {
    commerciaux.push_back(commercial);
}

void PersonnelCommercial::afficherCommerciaux() const {
    for (size_t i = 0; i < commerciaux.size(); ++i) {
        commerciaux[i].afficher();
    }
}

void PersonnelCommercial::supprimerCommerciauxFaibleVente(double seuil) {
    for (size_t i = 0; i < commerciaux.size(); ++i) {
        if (commerciaux[i].getInteressement() < seuil) {
            commerciaux.erase(commerciaux.begin() + i);
            --i; // Réajuster l'indice après suppression
        }
    }
}

