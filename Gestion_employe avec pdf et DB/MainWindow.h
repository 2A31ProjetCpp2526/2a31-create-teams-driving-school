#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "Employe.h"  // Inclusion de la classe Employe

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
    void on_pushButton_clicked();  // Ajouter/Modifier
    void on_pushButton_2_clicked();  // Afficher/Rechercher
    void on_pushButton_4_clicked();  // Supprimer
    void on_pushButton_5_clicked();  // Exporter PDF
    void on_comboBox_currentIndexChanged(int index);  // Trier
    void on_tableWidget_itemSelectionChanged();  // Charger sélection pour modification

private:
    Ui::MainWindow *ui;
    QList<Employe> m_employes;  // Liste en mémoire (rechargée du DB)
    bool m_isModifyMode;  // Drapeau pour mode modification
    QSqlDatabase m_db;  // Base de données SQLite

    // DB Functions
    bool initDatabase();  // Init DB + table
    void loadEmployes();  // Charger du DB
    bool saveEmploye(const Employe& emp);  // Sauvegarder un employé
    bool updateEmploye(int id, const Employe& emp);  // Update
    bool deleteEmploye(int id);  // Supprimer

    int getNextId() const;  // Génère ID auto-incrémenté (du DB)
    void updateTable(const QList<Employe>& employesToShow = {});  // Met à jour le tableau
    void clearForm();  // Vide le formulaire
    void showError(const QString& message);  // Affiche erreur via QMessageBox
};

#endif // MAINWINDOW_H
