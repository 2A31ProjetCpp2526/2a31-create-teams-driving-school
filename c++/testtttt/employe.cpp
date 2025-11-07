#include "Employe.h"
#include <iostream>

Employe::Employe(const string nom, int matricule, double salaireBase)
    : nom(nom), matricule(matricule), salaireBase(salaireBase) {}

Employe::~Employe() {}

string Employe::getNom() const {
    return nom;
}

int Employe::getMatricule() const {
    return matricule;
}

double Employe::getSalaireBase() const {
    return salaireBase;
}

void Employe::setNom(const string nom) {
    this->nom = nom;
}

void Employe::setSalaireBase(double salaireBase) {
    this->salaireBase = salaireBase;
}

void Employe::afficher() const {
    cout << "Nom: " << nom << ", Matricule: " << matricule << ", Salaire de base: " << salaireBase << endl;
}
