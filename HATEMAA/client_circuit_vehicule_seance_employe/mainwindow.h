#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QList>
#include "seance.h"
#include "vehicule.h"
#include "circuit.h"
#include "client.h"
#include "Employe.h"

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
    void cercuitclick();

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

    // Slots pour les opérations CRUD des circuits
    void on_pushButton_validercircuit_clicked();
    void on_pushButton_modifiercircuit_clicked();
    void on_pushButton_supprimercircuit_clicked();
    void on_pushButton_recherchecircuit_clicked();
    void on_tableView_circuits_clicked(const QModelIndex &index);
    void on_tableView_circuits_recherche_clicked(const QModelIndex &index);
    void on_comboBox_trier_circuit_currentIndexChanged(int index);
    void on_pushButton_supprimercircuit_bas_clicked();
    void on_pushButton_export_circuit_clicked();
    
    // Slots pour les opérations CRUD des clients
    void on_btnAjouter_clicked();
    void on_btnModifier_clicked();
    void on_btnSupprimer_clicked();
    void on_btnChercher_clicked();
    void on_tableWidget_6_itemClicked(QTableWidgetItem *item);
    void on_lineEditRecherche_textChanged(const QString &arg1);
    void on_comboBoxTrier_currentIndexChanged(int index);

    // Slots pour les opérations CRUD des employés
    void on_pushButton_26_clicked();  // Ajouter employé
    void on_pushButton_27_clicked();  // Afficher/Rechercher employé
    void on_pushButton_28_clicked();  // Supprimer employé
    void on_tableWidget_5_itemSelectionChanged();  // Sélection dans le tableau
    void on_comboBox_8_currentIndexChanged(int index);  // Trier employés

private:
    Ui::MainWindow *ui;
    Seance currentSeance;
    Vehicule V; // Instance de Vehicule
    Circuit C; // Instance de Circuit
    client C_Client; // Instance de Client

    // Méthodes utilitaires
    void refreshTableSeance();
    void clearFieldsSeance();

    void refreshTableVehicule();
    void clearFieldsVehicule();

    void refreshTableCircuit();
    void clearFieldsCircuit();
    
    void refreshTableClient();
    void clearFieldsClient();
    
    // Méthodes pour la gestion des employés
    void refreshTableEmploye();
    void clearFieldsEmploye();
    bool initDatabaseEmploye();
    void loadEmployes();
    bool saveEmploye(const Employe& emp);
    bool updateEmploye(int ID_EMPLOYE, const Employe& emp);
    bool deleteEmploye(int ID_EMPLOYE);
    int getNextIdEmploye() const;
    void updateTableEmploye(const QList<Employe>& employesToShow = {});
    
    // Fonction utilitaire pour l'exportation CSV
    QString escapeCsvField(const QString& field);
    
    // Données pour les employés
    QList<Employe> m_employes;
    bool m_isModifyModeEmploye;
};

#endif // MAINWINDOW_H
