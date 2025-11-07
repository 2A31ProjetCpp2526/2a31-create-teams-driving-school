#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QSqlDatabase>
#include <QSqlQuery>

class QTableWidget;
class QLabel;
class QChart;
class QChartView;
class QPieSeries;

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
    void on_pushButton_clicked();      // Ajouter/Modifier
    void on_pushButton_2_clicked();    // Rechercher/Afficher
    void on_pushButton_4_clicked();    // Supprimer
    void on_pushButton_5_clicked();    // Export PDF
    void on_comboBox_currentIndexChanged(int index); // Trier
    void on_tableWidget_itemSelectionChanged();      // Sélection

private:
    Ui::MainWindow *ui;
    QList<Employe> m_employes;
    bool m_isModifyMode;
    QSqlDatabase m_db;

    QTableWidget *statsTable;
    QLabel *statsLabel;

    QChart *m_pieChart;
    QChartView *m_chartView;

    // Fonctions internes
    bool initDatabase();
    void loadEmployes();
    bool saveEmploye(const Employe& emp);
    bool updateEmploye(int id, const Employe& emp);
    bool deleteEmploye(int id);
    int getNextId() const;
    void updateTable(const QList<Employe>& employesToShow = {});
    void updateStats();
    void clearForm();
    void showError(const QString& message);
    void setupStyle(); // ✅ Nouveau : appliquer le style général
};

#endif // MAINWINDOW_H
