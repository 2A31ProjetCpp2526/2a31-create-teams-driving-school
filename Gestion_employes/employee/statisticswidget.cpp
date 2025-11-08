#include "statisticswidget.h"

StatisticsWidget::StatisticsWidget(EmployeeService *service, QWidget *parent)
    : QWidget(parent)
    , m_service(service)
{
    setupUI();
    connectSignals();
    refreshStatistics();
}

void StatisticsWidget::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    
    // Titre
    m_titleLabel = new QLabel("<html><head/><body><p><span style=\" font-size:18pt; font-weight:700;\">📊 Statistiques des Employés</span></p></body></html>");
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_mainLayout->addWidget(m_titleLabel);
    
    // Layout pour les statistiques générales
    m_statsLayout = new QGridLayout();
    
    // Total des employés
    m_totalEmployeesLabel = new QLabel("<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">👥 Total des employés:</span></p></body></html>");
    m_totalEmployeesValue = new QLabel("0");
    m_totalEmployeesValue->setStyleSheet(
        "font-size: 24pt;"
        "font-weight: bold;"
        "color: #2E8B57;"
        "background-color: #F0F8FF;"
        "border: 2px solid #4682B4;"
        "border-radius: 10px;"
        "padding: 10px;"
    );
    m_totalEmployeesValue->setAlignment(Qt::AlignCenter);
    
    m_statsLayout->addWidget(m_totalEmployeesLabel, 0, 0);
    m_statsLayout->addWidget(m_totalEmployeesValue, 0, 1);
    
    // Frame pour les statistiques par poste
    m_posteFrame = new QFrame();
    m_posteFrame->setStyleSheet(
        "QFrame {"
        "    background-color: #F5F5F5;"
        "    border: 2px solid #4682B4;"
        "    border-radius: 10px;"
        "    margin: 10px;"
        "}"
    );
    
    m_posteLayout = new QVBoxLayout(m_posteFrame);
    m_posteTitleLabel = new QLabel("<html><head/><body><p><span style=\" font-size:14pt; font-weight:600;\">📈 Répartition par poste:</span></p></body></html>");
    m_posteLayout->addWidget(m_posteTitleLabel);
    
    m_statsLayout->addWidget(m_posteFrame, 1, 0, 1, 2);
    
    m_mainLayout->addLayout(m_statsLayout);
    
    // Ajouter un espacement
    m_mainLayout->addStretch();
}

void StatisticsWidget::connectSignals()
{
    connect(m_service, &EmployeeService::dataChanged, this, &StatisticsWidget::onDataChanged);
}

void StatisticsWidget::refreshStatistics()
{
    updateStatistics();
}

void StatisticsWidget::onDataChanged()
{
    refreshStatistics();
}

void StatisticsWidget::updateStatistics()
{
    // Mettre à jour le total des employés
    int totalEmployees = m_service->getTotalEmployees();
    m_totalEmployeesValue->setText(QString::number(totalEmployees));
    
    // Nettoyer les anciens widgets de poste
    for (QProgressBar *bar : m_posteProgressBars) {
        m_posteLayout->removeWidget(bar);
        bar->deleteLater();
    }
    for (QLabel *label : m_posteLabels) {
        m_posteLayout->removeWidget(label);
        label->deleteLater();
    }
    m_posteProgressBars.clear();
    m_posteLabels.clear();
    
    // Créer les nouvelles statistiques par poste
    QStringList postes = m_service->getUniquePostes();
    
    if (postes.isEmpty()) {
        QLabel *noDataLabel = new QLabel("Aucune donnée disponible");
        noDataLabel->setStyleSheet("color: gray; font-style: italic;");
        m_posteLayout->addWidget(noDataLabel);
        m_posteLabels.append(noDataLabel);
        return;
    }
    
    for (const QString &poste : postes) {
        int count = m_service->getEmployeesByPoste(poste);
        int percentage = totalEmployees > 0 ? (count * 100) / totalEmployees : 0;
        
        // Créer le label du poste
        QLabel *posteLabel = new QLabel(QString("%1: %2 employé(s) (%3%)")
                                       .arg(poste)
                                       .arg(count)
                                       .arg(percentage));
        posteLabel->setStyleSheet("font-weight: bold; margin: 5px 0px;");
        
        // Créer la barre de progression
        QProgressBar *progressBar = new QProgressBar();
        progressBar->setRange(0, totalEmployees);
        progressBar->setValue(count);
        progressBar->setStyleSheet(
            "QProgressBar {"
            "    border: 2px solid #4682B4;"
            "    border-radius: 5px;"
            "    text-align: center;"
            "    background-color: #E6E6FA;"
            "}"
            "QProgressBar::chunk {"
            "    background-color: #4682B4;"
            "    border-radius: 3px;"
            "}"
        );
        
        m_posteLayout->addWidget(posteLabel);
        m_posteLayout->addWidget(progressBar);
        
        m_posteLabels.append(posteLabel);
        m_posteProgressBars.append(progressBar);
    }
}
