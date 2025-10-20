#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "vehicule.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_valider_clicked();   // Ajouter
    void on_pushButton_supprimer_clicked(); // Supprimer
    void on_checkBox_modifier_clicked();

private:
    Ui::MainWindow *ui;
    Vehicule V; // Objet global

    void chargerTableau();
};

#endif // MAINWINDOW_H
