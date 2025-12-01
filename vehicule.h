#ifndef VEHICULE_H
#define VEHICULE_H

#include <QDate>
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableView>
#include <QPieSeries>
#include <QChart>
#include <QChartView>
#include <QStandardPaths>
#include <QWidget>
#include <QTextEdit>
#include <QSqlQuery>
#include <QStandardPaths>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>


class Vehicule
{
public:
    Vehicule();
    Vehicule(QString, QString, QString, QString, QString, QDate);
    bool ajouter();
    bool supprimer(QString);
    bool modifier(QString);
    QSqlQueryModel* afficher();
    QSqlQueryModel* rechercher(const QString &debut);
    QSqlQueryModel* trier(const QString& critere);
    bool exportV(const QString& filePath, QTableView* tableView);
    QChartView* statistiquesV();
    QWidget* afficherHistorique(const QString &immat);
    bool exportHistoriqueTxt(const QString &immat);


private:
    QString immatricule, modele, type_vehicule, transmission, etat;
    QDate date_assurence; // Gardez le nom interne comme vous voulez
};

#endif // VEHICULE_H
