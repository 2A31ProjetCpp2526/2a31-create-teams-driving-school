#include "employeewidget.h"

EmployeeWidget::EmployeeWidget(EmployeeService *service, QWidget *parent)
    : QWidget(parent)
    , m_service(service)
{
    setupUI();
    connectSignals();
}

void EmployeeWidget::setupUI()
{
    // Créer le layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Créer le GroupBox
    m_groupBox = new QGroupBox("➕Ajouter employé:", this);
    m_groupBox->setStyleSheet(
        "QGroupBox {"
        "    font: 900 9pt \"Segoe UI\";"
        "    font: 16pt \"Segoe UI\";"
        "    background-color: rgb(135, 206, 235);"
        "    border: 2px solid #4682B4;"
        "    border-radius: 8px;"
        "    margin-top: 10px;"
        "}"
        "QPushButton {"
        "    background-color: #4682B4;"
        "    color: white;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #5A9BD5;"
        "}"
    );
    
    // Créer le layout du formulaire
    QFormLayout *formLayout = new QFormLayout(m_groupBox);
    
    // ID Employé
    QLabel *idLabel = new QLabel("👨‍💻ID Employé:");
    m_idEdit = new QLineEdit();
    formLayout->addRow(idLabel, m_idEdit);
    
    // Nom
    QLabel *nomLabel = new QLabel("🔠Nom:");
    m_nomEdit = new QLineEdit();
    formLayout->addRow(nomLabel, m_nomEdit);
    
    // Prénom
    QLabel *prenomLabel = new QLabel("🔠Prénom:");
    m_prenomEdit = new QLineEdit();
    formLayout->addRow(prenomLabel, m_prenomEdit);
    
    // Poste
    QLabel *posteLabel = new QLabel("💼Poste:");
    m_posteEdit = new QLineEdit();
    formLayout->addRow(posteLabel, m_posteEdit);
    
    // Mot de passe
    QLabel *motDePasseLabel = new QLabel("🔣Mot de passe:");
    m_motDePasseEdit = new QLineEdit();
    m_motDePasseEdit->setEchoMode(QLineEdit::Password);
    formLayout->addRow(motDePasseLabel, m_motDePasseEdit);
    
    // Téléphone
    QLabel *telephoneLabel = new QLabel("📞Num téléphonique:");
    QHBoxLayout *telephoneLayout = new QHBoxLayout();
    QLabel *prefixLabel = new QLabel("+216");
    m_telephoneEdit = new QLineEdit();
    telephoneLayout->addWidget(prefixLabel);
    telephoneLayout->addWidget(m_telephoneEdit);
    formLayout->addRow(telephoneLabel, telephoneLayout);
    
    // Date d'embauche
    QLabel *dateLabel = new QLabel("📆Date d'embauche:");
    m_dateEmbaucheEdit = new QDateEdit();
    m_dateEmbaucheEdit->setDate(QDate::currentDate());
    m_dateEmbaucheEdit->setCalendarPopup(true);
    formLayout->addRow(dateLabel, m_dateEmbaucheEdit);
    
    // Boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_addButton = new QPushButton("Ajouter");
    m_clearButton = new QPushButton("Effacer");
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_clearButton);
    formLayout->addRow(buttonLayout);
    
    mainLayout->addWidget(m_groupBox);
}

void EmployeeWidget::connectSignals()
{
    connect(m_addButton, &QPushButton::clicked, this, &EmployeeWidget::onAddEmployee);
    connect(m_clearButton, &QPushButton::clicked, this, &EmployeeWidget::clearForm);
}

void EmployeeWidget::onAddEmployee()
{
    if (!validateForm()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs obligatoires.");
        return;
    }
    
    Employee employee = createEmployeeFromForm();
    
    if (m_service->addEmployee(employee)) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès!");
        clearForm();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible d'ajouter l'employé. Vérifiez que l'ID n'existe pas déjà.");
    }
}

void EmployeeWidget::clearForm()
{
    m_idEdit->clear();
    m_nomEdit->clear();
    m_prenomEdit->clear();
    m_posteEdit->clear();
    m_motDePasseEdit->clear();
    m_telephoneEdit->clear();
    m_dateEmbaucheEdit->setDate(QDate::currentDate());
}

Employee EmployeeWidget::createEmployeeFromForm() const
{
    QString telephone = "+216" + m_telephoneEdit->text();
    
    return Employee(
        m_idEdit->text().trimmed(),
        m_nomEdit->text().trimmed(),
        m_prenomEdit->text().trimmed(),
        m_posteEdit->text().trimmed(),
        m_motDePasseEdit->text(),
        telephone,
        m_dateEmbaucheEdit->date()
    );
}

bool EmployeeWidget::validateForm() const
{
    return !m_idEdit->text().trimmed().isEmpty() &&
           !m_nomEdit->text().trimmed().isEmpty() &&
           !m_prenomEdit->text().trimmed().isEmpty() &&
           !m_posteEdit->text().trimmed().isEmpty() &&
           !m_motDePasseEdit->text().isEmpty() &&
           !m_telephoneEdit->text().trimmed().isEmpty();
}
