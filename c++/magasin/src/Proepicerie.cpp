#include "Proepicerie.h"

Proepicerie::Proepicerie(int id, int qty,string libelle,double prix,double poids):
{
    Produit(id, libelle, prix, quantite), poids(poids) {}


}

Proepicerie::~Proepicerie()
{
    //dtor
}

double getPoids()const
{
return poids;
}
void setPoids(double poids){
            this->poids=poids;
}
void afficher() const{
Produit::afficher();
std::cout << "Poids: " << poids << " kg" << std::endl;
}
