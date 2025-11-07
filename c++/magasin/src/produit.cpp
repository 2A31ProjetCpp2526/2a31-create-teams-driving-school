#include "produit.h"
#include <iostream>


Produit::Produit(int id, const std::string& libelle, double prix, int quantite):
    id(id),
    libelle(libelle),
    prix(prix),
    quantite(quantite) {}

produit::~produit()
{
    //dtor
}


int Produit::getId() const {
    return id;
}

std::string Produit::getLibelle() const {
    return libelle;
}

double Produit::getPrix() const {
    return prix;
}

int Produit::getQuantite() const {
    return quantite;
}

void Produit::setLibelle(const std::string& libelle) {
    this->libelle = libelle;
}

void Produit::setPrix(double prix) {
    this->prix = prix;
}

void Produit::setQuantite(int quantite) {
    this->quantite = quantite;
}

void Produit::afficher() const {
    std::cout << "ID: " << id << ", Libelle: " << libelle << ", Prix: " << prix << ", Quantite: " << quantite << std::endl;
}
