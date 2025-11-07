#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "seance.h"
#include "vehicule.h"
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void seanceclick();
    void employeclick();
    void clientclick();
    void vehiculeclick();
    void contratclick();

    // Slots pour les opérations CRUD des séances
    void on_pushButton_valider_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_recherche_clicked();
    void on_tableView_seances_clicked(const QModelIndex &index);

    // Slots pour les opérations CRUD des véhicules
    void on_pushButton_validervehicule_clicked();
    void on_pushButton_supprimervehicule_clicked();
    void on_pushButton_modifiervehicule_clicked();
    void on_tableView_vehicules_clicked(const QModelIndex &index);

    // Slots pour les opérations CRUD des clients
    void on_btnAjouter_clicked();
    void on_btnModifier_clicked();
    void on_btnSupprimer_clicked();
    void on_btnChercher_clicked();
    void on_tableWidget_6_itemClicked(QTableWidgetItem *item);
    void on_lineEditRecherche_textChanged(const QString &arg1);
    void on_comboBoxTrier_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    Seance currentSeance;
    Vehicule V; // Instance de Vehicule
    client C; // Instance de Client

    // Méthodes utilitaires
    void refreshTableSeance();
    void clearFieldsSeance();

    void refreshTableVehicule();
    void clearFieldsVehicule();

    void refreshTableClient();
    void clearFieldsClient();
};

#endif // MAINWINDOW_H
