#include "Produit.h"
#include <iostream>
Produit::Produit(int id, const string libelle, double prix, int quantite)
    : id(id), libelle(libelle), prix(prix), quantite(quantite) {}

Produit::~Produit() {}

int Produit::getId() const {
    return id;
}

string Produit::getLibelle() const {
    return libelle;
}

double Produit::getPrix() const {
    return prix;
}

int Produit::getQuantite() const {
    return quantite;
}

void Produit::setLibelle( string libelle) {
    this->libelle = libelle;
}

void Produit::setPrix(double prix) {
    this->prix = prix;
}

void Produit::setQuantite(int quantite) {
    this->quantite = quantite;
}

void Produit::afficher() const {
   cout << "ID: " << id << ", Libelle: " << libelle << ", Prix: " << prix << ", Quantite: " << quantite << endl;
}
