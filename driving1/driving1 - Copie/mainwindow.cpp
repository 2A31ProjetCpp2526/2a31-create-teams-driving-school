#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->seance, &QPushButton::clicked, this, &MainWindow::seanceclick);
    connect(ui->employe, &QPushButton::clicked, this, &MainWindow::employeclick);
    connect(ui->client, &QPushButton::clicked, this, &MainWindow::clientclick);
    connect(ui->vehicule, &QPushButton::clicked, this, &MainWindow::vehiculeclick);
    connect(ui->contrat, &QPushButton::clicked, this, &MainWindow::contratclick);

    for (int i = 1; i <= 4; ++i) {
        QWidget *page = new QWidget();
        QLabel *label = new QLabel("Page " + QString::number(i) + " - Contenu à ajouter", page);
        label->setGeometry(100, 100, 300, 50);
        ui->stack->addWidget(page);
    }

    ui->stack->setCurrentIndex(0);
}

void MainWindow::seanceclick() { ui->stack->setCurrentIndex(0); }
void MainWindow::employeclick() { ui->stack->setCurrentIndex(1); }
void MainWindow::clientclick() { ui->stack->setCurrentIndex(2); }
void MainWindow::vehiculeclick() { ui->stack->setCurrentIndex(3); }
void MainWindow::contratclick() { ui->stack->setCurrentIndex(4); }

MainWindow::~MainWindow()
{
    delete ui;
}
