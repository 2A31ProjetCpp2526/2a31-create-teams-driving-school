#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QList>
#include <QVBoxLayout>
#include <QSystemTrayIcon>
#include "seance.h"
#include "vehicule.h"
#include "circuit.h"
#include "client.h"
#include "Employe.h"
#include "moniteur.h"
#include <QDate>//a
#include <QSqlQueryModel>  // ✅ AJOUTER CETTE LIGNE
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ClientMapWidget;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    explicit MainWindow(const QString &userRole, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void seanceclick();
    void employeclick();
    void clientclick();
    void vehiculeclick();
    void moniteurclick();
    void cercuitclick();
    void calendrierclick();

    // Slots pour les opérations CRUD des séances
    // Séances
    void on_pushButton_valider_clicked();
    void on_pushButton_modifier_clicked();
    void on_pushButton_supprimer_clicked();
    void on_pushButton_recherche_clicked();
    void on_comboBox_trier_currentTextChanged(const QString &text);
    void on_pushButton_effacer_clicked();
    void on_pushButton_rafraichir_clicked();
    void on_pushButton_export_clicked();  // Export PDF
    void on_pushButton_statistiques_clicked();
    void on_tableView_seances_clicked(const QModelIndex &index);
    void on_pushButton_envoyer_clicked();

    void on_lineEdit_recherche_textChanged(const QString &text);
    // Calendrier
    void onCalendrierDateClicked(const QDate &date);
    void onMoisPrecedent();
    void onMoisSuivant();
    void onAfficherCalendrier();
    // Slots pour les opérations CRUD des véhicules
    void on_pushButton_validervehicule_clicked();
    void on_pushButton_supprimervehicule_clicked();
    void on_pushButton_modifiervehicule_clicked();
    void on_pushButton_rechercheVehicule_clicked();
    void on_lineEdit_rechercheVehicule_textChanged(const QString &text);
    void on_comboBox_trierVehicule_currentIndexChanged(int index);
    void on_pushButton_exporterVehicule_clicked();
    void on_pushButton_statistiques_2_clicked();
    void on_tableView_vehicules_clicked(const QModelIndex &index);
    void on_pushButton_historique_clicked();
    void updateAssuranceTable();
    void updateAssuranceAlertPanel();
    void updateMetiersBadge();
    void showAssuranceNotifications();
    void showNotification(const QString &title, const QString &message);
    void on_tabWidget_4_currentChanged(int index);


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
    void on_btnClear_clicked();  // Bouton Clear pour réinitialiser la recherche
    void on_tableWidget_6_itemClicked(QTableWidgetItem *item);
    void on_lineEditRecherche_textChanged(const QString &arg1);
    void on_comboBoxTrier_currentIndexChanged(int index);
    void on_btnExporterPDF_clicked();  // Exporter client sélectionné en PDF
    void on_btnGenererQRClient_clicked();  // Générer QR Code du client sélectionné
    void on_btnGenererQR_clicked();  // Générer QR Code pour tous les clients
    void on_pushButton_30_clicked();  // Ancien bouton Exporter QR (compatibilité)

    // Métiers / GPS client
    void on_btnShowClientOnMap_clicked();

    // Slots pour les opérations CRUD des employés
    void on_pushButton_26_clicked();  // Ajouter employé
    void on_pushButton_27_clicked();  // Afficher/Rechercher employé
    void on_pushButton_28_clicked();  // Supprimer employé
    void on_tableWidget_5_itemSelectionChanged();  // Sélection dans le tableau
    void on_comboBox_8_currentIndexChanged(int index);  // Trier employés
    void on_exportPdfBtn_clicked(); // Exporter employés en PDF

    // Slots pour les opérations CRUD des moniteurs
    void on_addContratBtn_clicked();  // Ajouter moniteur (tab Ajout)
    void on_updateContratBtn_clicked();  // Modifier moniteur (tab Ajout)
    void on_deleteContratBtn_clicked();  // Supprimer moniteur (tab Ajout)
    void on_Modifiermoniteur_clicked();  // Modifier depuis Afficher tab
    void on_supprimermoniteur_clicked();  // Supprimer depuis Afficher tab
    void on_moniteurTable_cellClicked(int row, int column);  // Sélectionner depuis la table

    // Slots pour les statistiques
    void afficherStatistiqueClient();  // Afficher statistiques clients par âge
    
    // Slots pour QR code et PDF (générés automatiquement par Qt)
    void selectClientByQRCode(const QString &clientId);  // Sélectionner client via QR code

private:
    Ui::MainWindow *ui;
    // --- Notification Windows ---
    QSystemTrayIcon *trayIcon;
    QString m_userRole;
    Seance currentSeance;
    Vehicule V; // Instance de Vehicule
    Circuit C; // Instance de Circuit
    client C_Client; // Instance de Client
    Moniteur M; // Instance de Moniteur

    ClientMapWidget *m_clientMapWidget = nullptr;

    // Méthodes utilitaires
    void refreshTableSeance();
    void clearFieldsSeance();

    void refreshTableVehicule();
    void clearFieldsVehicule();
    void enregistrerActionVehicule(const QString &immat, const QString &action);


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
    void afficherStatistiqueEmploye();
    
    // Variables pour la gestion des employés
    QList<Employe> m_employes;
    bool m_isModifyModeEmploye;
    
    // Stat client
    QVBoxLayout* layoutChartClient = nullptr;
    //seance
    void refreshTable();
    void clearFields();

    // Calendrier
    void setupCalendrier();
    void updateCalendrier();
    void afficherDetailsDate(const QDate &date);
    void marquerDatesAvecSeances();
    void applyRolePermissions();
    bool isClearingSearch = false;
    QDate m_dateCalendrier;
    QMap<QDate, int> m_seancesParDate;
    QString genererHTMLFromModel(QSqlQueryModel* model);
        bool canAccess(const QString &section) const;
        void showAccessDenied(const QString &section) const;
        bool canPerformAction(const QString &action) const;
};

#endif // MAINWINDOW_H
