
#include "Personalcommercial.h"
#include "Commercial.h"
#include <iostream>
using namespace std;


int main() {
    PersonnelCommercial personnel;

    Commercial c1("kinza chaouachi", 101, 2000, 500, 300);
    Commercial c2("israa brini", 102, 2500, 600, 200);
    Commercial c3("nour farhat", 103, 2200, 550, 400);

    personnel.ajouterCommercial(c1);
    personnel.ajouterCommercial(c2);
    personnel.ajouterCommercial(c3);

    cout << "Affichage de tous les commerciaux:" << endl;
    personnel.afficherCommerciaux();

    cout << "Suppression des commerciaux avec un interessement inferieur a 250..." <<endl;
    personnel.supprimerCommerciauxFaibleVente(250);

    cout << "Affichage apres suppression:" << endl;
    personnel.afficherCommerciaux();

    return 0;
}

