#include "Commercial.h"
#include <iostream>

Commercial::Commercial(const string nom, int matricule, double salaireBase, double fixe, double interessement)
    : Employe(nom, matricule, salaireBase), fixe(fixe), interessement(interessement) {}

Commercial::~Commercial() {}

double Commercial::getFixe() const {
    return fixe;
}

double Commercial::getInteressement() const {
    return interessement;
}

void Commercial::setFixe(double fixe) {
    this->fixe = fixe;
}

void Commercial::setInteressement(double interessement) {
    this->interessement = interessement;
}

void Commercial::afficher() const {
    Employe::afficher();
    cout << "Fixe: " << fixe << ", Interessement: " << interessement << endl;
}
