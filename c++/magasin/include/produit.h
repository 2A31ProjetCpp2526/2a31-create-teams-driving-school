#ifndef PRODUIT_H
#define PRODUIT_H


class produit
{
   protected:
    int id,quantite;
    std::string libelle;
    double prix;

public:


produit( int id, int quantite,std::string& libelle,double prix)
{

}

~produit();
    int getId() const;
    std::string getLibelle() const;
    double getPrix() const;
    int getQty() const;

    void setLibelle(const std::string& libelle);
    void setPrix(double prix);
    void setQuantite(int quantite);

    void afficher() const;

    private:
};


