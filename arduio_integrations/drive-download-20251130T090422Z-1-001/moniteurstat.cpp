#include "moniteurstat.h"
#include "connection.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QtCharts/QLegend>

MoniteurStatDialog::MoniteurStatDialog(QWidget *parent)
    : QDialog(parent), chartView(nullptr)
{
    setWindowTitle("📊 Statistiques Moniteurs par Permis");
    setMinimumSize(600, 500);
    setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Titre
    QLabel *titleLabel = new QLabel("📊 Répartition des Moniteurs par Type de Permis");
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; color: #2196F3; margin: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Chart View
    chartView = new QChartView();
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setStyleSheet("background-color: white; border-radius: 10px;");
    mainLayout->addWidget(chartView);

    // Boutons
    QHBoxLayout *btnLayout = new QHBoxLayout();

    QPushButton *btnRefresh = new QPushButton("🔄 Actualiser");
    btnRefresh->setStyleSheet("background-color: #4CAF50; color: white; padding: 10px 20px; border-radius: 5px; font-weight: bold;");
    connect(btnRefresh, &QPushButton::clicked, this, &MoniteurStatDialog::loadStatistics);
    btnLayout->addWidget(btnRefresh);

    QPushButton *btnClose = new QPushButton("❌ Fermer");
    btnClose->setStyleSheet("background-color: #f44336; color: white; padding: 10px 20px; border-radius: 5px; font-weight: bold;");
    connect(btnClose, &QPushButton::clicked, this, &QDialog::close);
    btnLayout->addWidget(btnClose);

    mainLayout->addLayout(btnLayout);

    // Charger les statistiques
    loadStatistics();
}

MoniteurStatDialog::~MoniteurStatDialog()
{
}

QMap<QString, int> MoniteurStatDialog::getPermisStatistics()
{
    QMap<QString, int> stats;

    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée pour statistiques";
        return stats;
    }

    QSqlQuery query;
    // Grouper par type de permis et compter
    if (!query.exec("SELECT PERMIS, COUNT(*) as NB FROM MONITEUR GROUP BY PERMIS ORDER BY NB DESC")) {
        qDebug() << "❌ Erreur requête statistiques:" << query.lastError().text();
        return stats;
    }

    while (query.next()) {
        QString permis = query.value(0).toString().trimmed();
        int count = query.value(1).toInt();
        
        if (!permis.isEmpty()) {
            stats[permis] = count;
        }
    }

    qDebug() << "📊 Statistiques moniteurs:" << stats;
    return stats;
}

void MoniteurStatDialog::loadStatistics()
{
    QMap<QString, int> stats = getPermisStatistics();

    if (stats.isEmpty()) {
        QMessageBox::information(this, "Information", 
            "Aucune donnée de moniteur disponible pour les statistiques.");
        return;
    }

    // Créer la série Pie
    QPieSeries *series = new QPieSeries();

    // Couleurs pour les différents types de permis
    QList<QColor> colors = {
        QColor("#FF6384"),  // Rouge/Rose
        QColor("#36A2EB"),  // Bleu
        QColor("#FFCE56"),  // Jaune
        QColor("#4BC0C0"),  // Turquoise
        QColor("#9966FF"),  // Violet
        QColor("#FF9F40"),  // Orange
        QColor("#7CFC00"),  // Vert clair
        QColor("#DC143C"),  // Crimson
        QColor("#00CED1"),  // Dark Turquoise
        QColor("#9400D3")   // Dark Violet
    };

    int colorIndex = 0;
    int total = 0;

    for (auto it = stats.begin(); it != stats.end(); ++it) {
        total += it.value();
    }

    for (auto it = stats.begin(); it != stats.end(); ++it) {
        QString permis = it.key();
        int count = it.value();
        double percentage = (total > 0) ? (count * 100.0 / total) : 0;

        QPieSlice *slice = series->append(
            QString("%1 (%2 - %3%)").arg(permis).arg(count).arg(percentage, 0, 'f', 1),
            count
        );

        slice->setColor(colors[colorIndex % colors.size()]);
        slice->setLabelVisible(true);
        slice->setLabelColor(Qt::black);

        // Effet d'explosion sur hover
        connect(slice, &QPieSlice::hovered, this, [slice](bool hovered) {
            slice->setExploded(hovered);
            slice->setExplodeDistanceFactor(hovered ? 0.1 : 0);
        });

        colorIndex++;
    }

    // Créer le graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(QString("Total: %1 moniteurs").arg(total));
    chart->setTitleFont(QFont("Arial", 14, QFont::Bold));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);
    chart->legend()->setFont(QFont("Arial", 10));
    chart->setBackgroundBrush(QBrush(QColor(250, 250, 250)));

    // Remplacer l'ancien graphique
    if (chartView->chart()) {
        delete chartView->chart();
    }
    chartView->setChart(chart);
}
