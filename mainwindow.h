#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "Employe.h"  // Inclusion de la classe Employe

QT_BEGIN_NAMESPACE
namespace Ui { class employe; }
QT_END_NAMESPACE

class employe : public QMainWindow
{
    Q_OBJECT

public:
    employe(QWidget *parent = nullptr);
    ~employe();

private slots:
    void on_pushButton_clicked();  // Ajouter/Modifier
    void on_pushButton_2_clicked();  // Afficher/Rechercher
    void on_pushButton_4_clicked();  // Supprimer
    void on_pushButton_5_clicked();  // Exporter
    void on_comboBox_currentIndexChanged(int index);  // Trier
    void on_tableWidget_itemSelectionChanged();  // Charger sélection pour modification

private:
    Ui::employe *ui;
    QList<Employe> m_employes;  // Liste des employés (stockage en mémoire)
    bool m_isModifyMode;  // Drapeau pour mode modification
    int getNextId() const;  // Génère ID auto-incrémenté
    void updateTable(const QList<Employe>& employesToShow = {});  // Met à jour le tableau
    void clearForm();  // Vide le formulaire
    void showError(const QString& message);  // Affiche erreur via QMessageBox
};

#endif // MAINWINDOW_H
