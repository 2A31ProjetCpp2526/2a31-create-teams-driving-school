#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    // Initialiser le service
    m_employeeService = new EmployeeService(this);
    
    // Configurer l'interface utilisateur modulaire
    setupUI();
    setupModules();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupUI()
{
    // Créer le widget central
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Créer le layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Créer le TabWidget
    m_tabWidget = new QTabWidget();
    mainLayout->addWidget(m_tabWidget);
    
    // Créer les onglets
    m_managementTab = new QWidget();
    m_statisticsTab = new QWidget();
    
    m_tabWidget->addTab(m_managementTab, "Gestion des Employés");
    m_tabWidget->addTab(m_statisticsTab, "Statistiques");
    
    // Configurer le layout de l'onglet de gestion
    m_managementLayout = new QHBoxLayout(m_managementTab);
    
    // Configurer le layout de l'onglet de statistiques
    QVBoxLayout *statisticsLayout = new QVBoxLayout(m_statisticsTab);
    
    // Ajouter une image ou logo si nécessaire
    QLabel *logoLabel = new QLabel();
    logoLabel->setPixmap(QPixmap("../../Capture d'écran 2025-09-29 142122.png"));
    logoLabel->setAlignment(Qt::AlignCenter);
    statisticsLayout->addWidget(logoLabel);
    
    // Définir la taille de la fenêtre
    resize(1460, 896);
    setWindowTitle("Système de Gestion des Employés");
}

void MainWindow::setupModules()
{
    // Créer les widgets modulaires
    m_employeeWidget = new EmployeeWidget(m_employeeService, this);
    m_employeeTable = new EmployeeTable(m_employeeService, this);
    m_statisticsWidget = new StatisticsWidget(m_employeeService, this);
    
    // Ajouter les widgets au layout de gestion
    m_managementLayout->addWidget(m_employeeWidget);
    m_managementLayout->addWidget(m_employeeTable);
    
    // Ajouter le widget de statistiques à l'onglet de statistiques
    QVBoxLayout *statisticsLayout = qobject_cast<QVBoxLayout*>(m_statisticsTab->layout());
    if (statisticsLayout) {
        statisticsLayout->addWidget(m_statisticsWidget);
    }
}
