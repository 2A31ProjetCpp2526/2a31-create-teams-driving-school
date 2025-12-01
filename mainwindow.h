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
#include "moniteur.h"



class QTableWidget;
class QLabel;
class QChart;
class QChartView;
class QTimer;

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
    void onEmployeeFilterChanged(const QString &text);
    void applyEmployeeFilter();

    // Debug: afficher infos utilisateur (ID, poste, DB)
    void showUserInfo();


    void seanceclick();
    void employeclick();
    void clientclick();
    void vehiculeclick();
    void moniteurclick();
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
    void on_pushButton_29_clicked();  // Export PDF des employés (SEUL BOUTON ACTIF)


    // Slots pour les opérations CRUD des moniteurs
    void on_addContratBtn_clicked();  // Ajouter moniteur (tab Ajout)
    void on_updateContratBtn_clicked();  // Modifier moniteur (tab Ajout)
    void on_deleteContratBtn_clicked();  // Supprimer moniteur (tab Ajout)
    void on_Modifiermoniteur_clicked();  // Modifier depuis Afficher tab
    void on_supprimermoniteur_clicked();  // Supprimer depuis Afficher tab
    void on_moniteurTable_cellClicked(int row, int column);  // Sélectionner depuis la table

private:
    Ui::MainWindow *ui;
    Seance currentSeance;
    Vehicule V; // Instance de Vehicule
    Circuit C; // Instance de Circuit
    client C_Client; // Instance de Client
    Moniteur M; // Instance de Moniteur

    // Méthodes utilitaires
    void refreshTableSeance();
    void clearFieldsSeance();

    void refreshTableVehicule();
    void clearFieldsVehicule();

    void refreshTableCircuit();
    void clearFieldsCircuit();
    
    void refreshTableClient();
    void clearFieldsClient();
    
    void refreshTableMoniteur();
    void clearFieldsMoniteur();
    
    // Méthodes pour la gestion des employés
    bool initDatabaseEmploye();
    void loadEmployes();
    bool saveEmploye(const Employe &employe);
    bool updateEmploye(int ID_EMPLOYE, const Employe &emp);
    bool deleteEmploye(int ID_EMPLOYE);
    void updateTableEmploye();
    void updateTableEmploye(const QList<Employe> &employes);
    void clearFieldsEmploye();
    void refreshTableEmploye();
    int getNextIdEmploye() const;
    QString escapeCsvField(const QString &field);
    void updateStatsEmploye();  // Mise à jour des statistiques des employés
    void exportEmployesToPDF();  // Fonction principale d'export PDF des employés
    
    // Variables pour la gestion des employés
    QList<Employe> m_employes;
    bool m_isModifyModeEmploye;
    QTimer *m_employeeFilterTimer;
    
    // Variables pour les statistiques des employés
    QTableWidget *statsTableEmploye;
    QLabel *statsLabelEmploye;
    QChart *m_pieChartEmploye;
    QChartView *m_chartViewEmploye;
    QLabel *m_noEmployeesLabel;
    
};

#endif // MAINWINDOW_H
