#include "employeetable.h"

EmployeeTable::EmployeeTable(EmployeeService *service, QWidget *parent)
    : QWidget(parent)
    , m_service(service)
{
    setupUI();
    connectSignals();
    refreshTable();
}

void EmployeeTable::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // Titre
    m_titleLabel = new QLabel("<html><head/><body><p><span style=\" font-size:14pt; font-weight:700;\">Gestion employés</span></p></body></html>");
    m_mainLayout->addWidget(m_titleLabel);
    
    // Layout de recherche
    m_searchLayout = new QHBoxLayout();
    m_searchLabel = new QLabel("<html><head/><body><p><span style=\" font-size:12pt;\">Afficher employé:</span></p></body></html>");
    m_searchEdit = new QLineEdit();
    m_searchButton = new QPushButton("Afficher 🔍");
    m_searchButton->setStyleSheet(
        "background-color: rgb(70, 130, 180);"
        "font: 900 12pt \"Segoe UI\";"
        "color: white;"
        "border-radius: 12px;"
        "padding: 8px 16px;"
    );
    
    m_searchLayout->addWidget(m_searchLabel);
    m_searchLayout->addWidget(m_searchEdit);
    m_searchLayout->addWidget(m_searchButton);
    m_searchLayout->addStretch();
    
    // Layout de tri
    m_sortLayout = new QHBoxLayout();
    m_sortLabel = new QLabel("<html><head/><body><p><span style=\" font-size:12pt;\">Trier par:</span></p></body></html>");
    m_sortComboBox = new QComboBox();
    m_sortComboBox->addItems({"ID", "Nom", "Poste"});
    
    m_sortLayout->addWidget(m_sortLabel);
    m_sortLayout->addWidget(m_sortComboBox);
    m_sortLayout->addStretch();
    
    // Tableau
    m_tableWidget = new QTableWidget();
    m_tableWidget->setStyleSheet(
        "background-color: #00008B;"
        "color: white;"
        "gridline-color: white;"
        "border-radius: 8px;"
        "QHeaderView::section {"
        "    background-color: #34495e;"
        "    color: white;"
        "    padding: 4px;"
        "    border: 1px solid #2c3e50;"
        "}"
    );
    
    setupTableHeaders();
    
    // Layout de suppression
    m_deleteLayout = new QHBoxLayout();
    m_deleteLabel = new QLabel("<html><head/><body><p><span style=\" font-size:12pt;\">Supprimer employé:</span></p></body></html>");
    m_deleteEdit = new QLineEdit();
    m_deleteButton = new QPushButton("Supprimer 🗑");
    m_deleteButton->setStyleSheet(
        "background-color: rgb(70, 130, 180);"
        "font: 900 12pt \"Segoe UI\";"
        "color: white;"
        "border-radius: 12px;"
        "padding: 8px 16px;"
    );
    
    m_deleteLayout->addWidget(m_deleteLabel);
    m_deleteLayout->addWidget(m_deleteEdit);
    m_deleteLayout->addWidget(m_deleteButton);
    m_deleteLayout->addStretch();
    
    // Layout d'export
    m_exportLayout = new QHBoxLayout();
    m_exportButton = new QPushButton("Exporter📑");
    m_exportButton->setStyleSheet(
        "background-color: rgb(70, 130, 180);"
        "font: 900 12pt \"Segoe UI\";"
        "color: white;"
        "border-radius: 12px;"
        "padding: 8px 16px;"
    );
    
    m_exportLayout->addStretch();
    m_exportLayout->addWidget(m_exportButton);
    
    // Ajouter tous les layouts
    m_mainLayout->addLayout(m_searchLayout);
    m_mainLayout->addLayout(m_sortLayout);
    m_mainLayout->addWidget(m_tableWidget);
    m_mainLayout->addLayout(m_deleteLayout);
    m_mainLayout->addLayout(m_exportLayout);
}

void EmployeeTable::setupTableHeaders()
{
    QStringList headers = {"ID", "Nom", "Prenom", "Poste", "Mot de passe", "Num téléphonique", "Date d'embauche"};
    m_tableWidget->setColumnCount(headers.size());
    m_tableWidget->setHorizontalHeaderLabels(headers);
    
    // Ajuster la largeur des colonnes
    m_tableWidget->horizontalHeader()->setStretchLastSection(true);
    m_tableWidget->setAlternatingRowColors(true);
    m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void EmployeeTable::connectSignals()
{
    connect(m_searchButton, &QPushButton::clicked, this, &EmployeeTable::onSearchEmployee);
    connect(m_deleteButton, &QPushButton::clicked, this, &EmployeeTable::onDeleteEmployee);
    connect(m_exportButton, &QPushButton::clicked, this, &EmployeeTable::onExportData);
    connect(m_sortComboBox, QOverload<const QString &>::of(&QComboBox::currentTextChanged), 
            this, &EmployeeTable::onSortChanged);
    
    // Connecter aux signaux du service
    connect(m_service, &EmployeeService::dataChanged, this, &EmployeeTable::onDataChanged);
}

void EmployeeTable::onSearchEmployee()
{
    QString searchTerm = m_searchEdit->text().trimmed();
    
    if (searchTerm.isEmpty()) {
        refreshTable();
        return;
    }
    
    QList<Employee> results = m_service->searchEmployee(searchTerm);
    populateTable(results);
}

void EmployeeTable::onDeleteEmployee()
{
    QString id = m_deleteEdit->text().trimmed();
    
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID d'employé.");
        return;
    }
    
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, 
        "Confirmation", 
        QString("Êtes-vous sûr de vouloir supprimer l'employé avec l'ID: %1?").arg(id),
        QMessageBox::Yes | QMessageBox::No
    );
    
    if (reply == QMessageBox::Yes) {
        if (m_service->removeEmployee(id)) {
            QMessageBox::information(this, "Succès", "Employé supprimé avec succès!");
            m_deleteEdit->clear();
            refreshTable();
        } else {
            QMessageBox::critical(this, "Erreur", "Employé non trouvé ou erreur lors de la suppression.");
        }
    }
}

void EmployeeTable::onExportData()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        "Exporter les données",
        "employees.csv",
        "Fichiers CSV (*.csv);;Fichiers JSON (*.json)"
    );
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier.");
        return;
    }
    
    QTextStream out(&file);
    
    if (fileName.endsWith(".json")) {
        out << m_service->exportToJSON();
    } else {
        out << m_service->exportToCSV();
    }
    
    file.close();
    QMessageBox::information(this, "Succès", "Données exportées avec succès!");
}

void EmployeeTable::onSortChanged()
{
    QString sortBy = m_sortComboBox->currentText();
    QList<Employee> sortedEmployees = m_service->sortEmployees(sortBy);
    populateTable(sortedEmployees);
}

void EmployeeTable::refreshTable()
{
    QList<Employee> employees = m_service->getAllEmployees();
    populateTable(employees);
}

void EmployeeTable::onDataChanged()
{
    refreshTable();
}

void EmployeeTable::populateTable(const QList<Employee> &employees)
{
    m_tableWidget->setRowCount(employees.size());
    
    for (int i = 0; i < employees.size(); ++i) {
        const Employee &employee = employees[i];
        
        m_tableWidget->setItem(i, 0, new QTableWidgetItem(employee.getId()));
        m_tableWidget->setItem(i, 1, new QTableWidgetItem(employee.getNom()));
        m_tableWidget->setItem(i, 2, new QTableWidgetItem(employee.getPrenom()));
        m_tableWidget->setItem(i, 3, new QTableWidgetItem(employee.getPoste()));
        m_tableWidget->setItem(i, 4, new QTableWidgetItem(employee.getMotDePasse()));
        m_tableWidget->setItem(i, 5, new QTableWidgetItem(employee.getTelephone()));
        m_tableWidget->setItem(i, 6, new QTableWidgetItem(employee.getDateEmbauche().toString("yyyy-MM-dd")));
    }
    
    // Ajuster la largeur des colonnes
    m_tableWidget->resizeColumnsToContents();
}
