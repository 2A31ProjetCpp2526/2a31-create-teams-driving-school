#ifndef PRODUIT_H
#define PRODUIT_H

#include <string>
using namespace std;


class Produit {
protected:
    int id;
    string libelle;
    double prix;
    int quantite;

public:
    Produit(int id,  string libelle, double prix, int quantite);
    ~Produit();

    int getId() const;
    string getLibelle() const;
    double getPrix() const;
    int getQuantite() const;

    void setLibelle( string libelle);
    void setPrix(double prix);
    void setQuantite(int quantite);

    void afficher() const;
};

#endif // PRODUIT_H
