#ifndef PROEPICERIE_H
#define PROEPICERIE_H


class Proepicerie:public produit
{  protected:
        double poids;
    public:

        ProdEpicerie(int id, const std::string& libelle, double prix, int quantite, double poids)const;
        double getPoids()const;
        void setPoids(double poids);
        void afficher() const;

        ~Proepicerie();




};

#endif // PROEPICERIE_H
