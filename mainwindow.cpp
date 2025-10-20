#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "vehicule.h"
#include <QMessageBox>
#include <QDate>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableView->setModel(V.afficher());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_valider_clicked()
{
    QString imm = ui->lineEdit_immatricule->text();
    QString mod = ui->lineEdit_modele->text();
    QString typ = ui->comboBox_type->currentText();
    QString trans = ui->comboBox_transmission->currentText();
    QString et = ui->comboBox_etat->currentText();
    QDate date = ui->dateEdit_assurance->date();

    Vehicule v(imm, mod, typ, trans, et, date);
    bool test = v.ajouter();

    if (test) {
        ui->tableView->setModel(v.afficher());
        QMessageBox::information(this, tr("OK"), tr("Ajout effectué."));
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Échec de l'ajout."));
    }
}
void MainWindow::on_pushButton_supprimer_clicked()
{
    QString id = ui->lineEdit_immatricule->text();
    bool test = V.supprimer(id);

    if (test) {
        ui->tableView->setModel(V.afficher());
        QMessageBox::information(this, tr("OK"), tr("Suppression effectuée."));
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Échec de la suppression."));
    }
}

    void MainWindow::on_checkBox_modifier_clicked()
{
    QString id = ui->lineEdit_immatricule->text();
    QString mod = ui->lineEdit_modele->text();
    QString typ = ui->comboBox_type->currentText();
    QString trans = ui->comboBox_transmission->currentText();
    QString et = ui->comboBox_etat->currentText();
    QDate date = ui->dateEdit_assurance->date();

    Vehicule v(id, mod, typ, trans, et, date);

    bool test = v.modifier(id);
    if (test) {
        ui->tableView->setModel(v.afficher());
        QMessageBox::information(this, tr("OK"), tr("Modification effectuée ✅"));
    } else {
        QMessageBox::critical(this, tr("Erreur"), tr("Échec de la modification ❌"));
    }
}
