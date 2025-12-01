#include "circuit.h"
#include "connection.h"
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QObject>

Circuit::Circuit() : id_circuit(0), distance(0.0), duree_estimee(0) {}

Circuit::Circuit(int id, QString nom, QString desc, double dist, int duree, QString diff, QString immat)
    : id_circuit(id), nom_circuit(nom), description(desc), distance(dist),
      duree_estimee(duree), difficulte(diff), immatricule(immat.toUpper().trimmed())
{}

bool Circuit::existeDeja(int id)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans existeDeja()";
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_CIRCUIT FROM CIRCUIT WHERE ID_CIRCUIT = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return true;
    }
    return false;
}

bool Circuit::ajouter()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans ajouter()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }

    qDebug() << "=== TENTATIVE AJOUT CIRCUIT ===";
    qDebug() << "ID:" << id_circuit;
    qDebug() << "Nom:" << nom_circuit;
    qDebug() << "Description:" << description;
    qDebug() << "Distance:" << distance;
    qDebug() << "Durée estimée:" << duree_estimee;
    qDebug() << "Difficulté:" << difficulte;
    qDebug() << "Immatricule:" << immatricule;

    // Vérifier si l'ID existe déjà
    if (existeDeja(id_circuit)) {
        qDebug() << "❌ ID existe déjà:" << id_circuit;
        QMessageBox::warning(nullptr, "Attention", "Un circuit avec cet ID existe déjà!");
        return false;
    }

    // Vérifier que tous les champs obligatoires sont remplis
    if (nom_circuit.isEmpty() || immatricule.isEmpty()) {
        QMessageBox::warning(nullptr, "Attention", "Le nom du circuit et l'immatricule sont obligatoires!");
        return false;
    }

    // Validation de la distance
    if (distance <= 0) {
        QMessageBox::warning(nullptr, "Attention", "La distance doit être un nombre positif!");
        return false;
    }

    // Validation de la durée estimée
    if (duree_estimee <= 0) {
        QMessageBox::warning(nullptr, "Attention", "La durée estimée doit être un nombre positif!");
        return false;
    }

    // Validation de la difficulté
    if (difficulte.isEmpty()) {
        QMessageBox::warning(nullptr, "Attention", "La difficulté est obligatoire!");
        return false;
    }

    // Validation du nom du circuit (longueur minimale)
    if (nom_circuit.length() < 2) {
        QMessageBox::warning(nullptr, "Attention", "Le nom du circuit doit contenir au moins 2 caractères!");
        return false;
    }

    // Vérifier que le véhicule existe
    QSqlQuery checkVehicule;
    checkVehicule.prepare("SELECT IMMATRICULE FROM VEHICULE WHERE IMMATRICULE = :immat");
    checkVehicule.bindValue(":immat", immatricule);
    if (!checkVehicule.exec() || !checkVehicule.next()) {
        QMessageBox::warning(nullptr, "Attention", "Le véhicule avec cette immatriculation n'existe pas!");
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO CIRCUIT (ID_CIRCUIT, NOM_CIRCUIT, DESCRIPTION, DISTANCE, DUREE_ESTIMEE, DIFFICULTE, IMMATRICULE) "
                  "VALUES (:id, :nom, :desc, :distance, :duree, :diff, :immat)");

    query.bindValue(":id", id_circuit);
    query.bindValue(":nom", nom_circuit);
    query.bindValue(":desc", description);
    query.bindValue(":distance", distance);
    query.bindValue(":duree", duree_estimee);
    query.bindValue(":diff", difficulte);
    query.bindValue(":immat", immatricule);

    bool success = query.exec();

    if (success) {
        qDebug() << "✅ AJOUT RÉUSSI !";
        QMessageBox::information(nullptr, "Succès", "Circuit ajouté avec succès ✓");
        return true;
    } else {
        qDebug() << "❌ ERREUR SQL:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de l'ajout:\n" + query.lastError().text());
        return false;
    }
}

bool Circuit::modifier()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans modifier()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }

    qDebug() << "=== DÉBUT MODIFICATION CIRCUIT ===";

    // Vérifier si le circuit existe
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT ID_CIRCUIT FROM CIRCUIT WHERE ID_CIRCUIT = :id");
    checkQuery.bindValue(":id", id_circuit);

    if (!checkQuery.exec() || !checkQuery.next()) {
        qDebug() << "❌ Circuit non trouvé avec ID:" << id_circuit;
        QMessageBox::warning(nullptr, "Attention", "Circuit non trouvé!");
        return false;
    }

    // Vérifier que tous les champs obligatoires sont remplis
    if (nom_circuit.isEmpty() || immatricule.isEmpty()) {
        QMessageBox::warning(nullptr, "Attention", "Le nom du circuit et l'immatricule sont obligatoires!");
        return false;
    }

    // Validation de la distance
    if (distance <= 0) {
        QMessageBox::warning(nullptr, "Attention", "La distance doit être un nombre positif!");
        return false;
    }

    // Validation de la durée estimée
    if (duree_estimee <= 0) {
        QMessageBox::warning(nullptr, "Attention", "La durée estimée doit être un nombre positif!");
        return false;
    }

    // Validation de la difficulté
    if (difficulte.isEmpty()) {
        QMessageBox::warning(nullptr, "Attention", "La difficulté est obligatoire!");
        return false;
    }

    // Validation du nom du circuit (longueur minimale)
    if (nom_circuit.length() < 2) {
        QMessageBox::warning(nullptr, "Attention", "Le nom du circuit doit contenir au moins 2 caractères!");
        return false;
    }

    // Vérifier que le véhicule existe
    QSqlQuery checkVehicule;
    checkVehicule.prepare("SELECT IMMATRICULE FROM VEHICULE WHERE IMMATRICULE = :immat");
    checkVehicule.bindValue(":immat", immatricule);
    if (!checkVehicule.exec() || !checkVehicule.next()) {
        QMessageBox::warning(nullptr, "Attention", "Le véhicule avec cette immatriculation n'existe pas!");
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE CIRCUIT SET "
                  "NOM_CIRCUIT = :nom, "
                  "DESCRIPTION = :desc, "
                  "DISTANCE = :distance, "
                  "DUREE_ESTIMEE = :duree, "
                  "DIFFICULTE = :diff, "
                  "IMMATRICULE = :immat "
                  "WHERE ID_CIRCUIT = :id");

    query.bindValue(":id", id_circuit);
    query.bindValue(":nom", nom_circuit);
    query.bindValue(":desc", description);
    query.bindValue(":distance", distance);
    query.bindValue(":duree", duree_estimee);
    query.bindValue(":diff", difficulte);
    query.bindValue(":immat", immatricule);

    bool success = query.exec();
    if (!success) {
        qDebug() << "❌ Erreur lors de la modification:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la modification:\n" + query.lastError().text());
    } else {
        qDebug() << "✅ Circuit modifié avec succès !";
        QMessageBox::information(nullptr, "Succès", "Circuit modifié avec succès ✓");
    }

    return success;
}

bool Circuit::supprimer(int id)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans supprimer()";
        QMessageBox::critical(nullptr, "Erreur", "Base de données non connectée!");
        return false;
    }

    qDebug() << "=== DÉBUT SUPPRESSION CIRCUIT ===";
    qDebug() << "ID à supprimer:" << id;

    QSqlQuery query;
    query.prepare("DELETE FROM CIRCUIT WHERE ID_CIRCUIT = :id");
    query.bindValue(":id", id);

    bool success = query.exec();
    if (!success) {
        qDebug() << "❌ Erreur lors de la suppression:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la suppression:\n" + query.lastError().text());
    } else {
        if (query.numRowsAffected() == 0) {
            QMessageBox::warning(nullptr, "Attention", "Aucun circuit trouvé avec cet ID!");
            return false;
        }
        qDebug() << "✅ Circuit supprimé avec succès !";
        QMessageBox::information(nullptr, "Succès", "Circuit supprimé avec succès ✓");
    }

    return success;
}

QSqlQueryModel* Circuit::afficher()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans afficher()";
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_CIRCUIT, NOM_CIRCUIT, DESCRIPTION, DISTANCE, DUREE_ESTIMEE, DIFFICULTE, IMMATRICULE FROM CIRCUIT ORDER BY ID_CIRCUIT");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Circuit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Circuit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Distance (km)"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Durée (min)"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Difficulté"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Immatricule"));

    return model;
}

QSqlQueryModel* Circuit::rechercher(const QString& critere)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans rechercher()";
        return new QSqlQueryModel();
    }

    qDebug() << "🔍 Recherche de circuit avec critère:" << critere;

    QSqlQueryModel* model = new QSqlQueryModel();

    // Utiliser la syntaxe Oracle correcte: TO_CHAR() au lieu de CAST AS TEXT
    // Échapper les caractères spéciaux pour éviter les injections SQL
    QString critereEscaped = critere;
    critereEscaped.replace("'", "''"); // Échapper les apostrophes

    QString queryStr = "SELECT ID_CIRCUIT, NOM_CIRCUIT, DESCRIPTION, DISTANCE, DUREE_ESTIMEE, DIFFICULTE, IMMATRICULE FROM CIRCUIT WHERE "
                       "TO_CHAR(ID_CIRCUIT) LIKE '%" + critereEscaped + "%' "
                       "OR UPPER(NOM_CIRCUIT) LIKE UPPER('%" + critereEscaped + "%') "
                       "OR UPPER(DESCRIPTION) LIKE UPPER('%" + critereEscaped + "%') "
                       "OR TO_CHAR(DISTANCE) LIKE '%" + critereEscaped + "%' "
                       "OR TO_CHAR(DUREE_ESTIMEE) LIKE '%" + critereEscaped + "%' "
                       "OR UPPER(DIFFICULTE) LIKE UPPER('%" + critereEscaped + "%') "
                       "OR UPPER(IMMATRICULE) LIKE UPPER('%" + critereEscaped + "%')";

    qDebug() << "📝 Requête SQL:" << queryStr;

    QSqlQuery query;
    if (!query.exec(queryStr)) {
        qDebug() << "❌ Erreur SQL lors de la recherche:" << query.lastError().text();
        qDebug() << "   Erreur détaillée:" << query.lastError().databaseText();
    }

    model->setQuery(queryStr);

    // Vérifier les erreurs du modèle
    if (model->lastError().isValid()) {
        qDebug() << "❌ Erreur du modèle:" << model->lastError().text();
    }

    int rowCount = model->rowCount();
    qDebug() << "✅ Résultats trouvés:" << rowCount;

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Circuit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Circuit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Distance (km)"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Durée (min)"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Difficulté"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Immatricule"));

    return model;
}

QSqlQueryModel* Circuit::trier(const QString& critere)
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée dans trier()";
        return new QSqlQueryModel();
    }

    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT ID_CIRCUIT, NOM_CIRCUIT, DESCRIPTION, DISTANCE, DUREE_ESTIMEE, DIFFICULTE, IMMATRICULE FROM CIRCUIT ORDER BY " + critere;
    model->setQuery(queryStr);

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Circuit"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom Circuit"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Description"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Distance (km)"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Durée (min)"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Difficulté"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Immatricule"));

    return model;
}
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QTableWidget>
#include <QHeaderView>
#include <QChart>
#include <QSplineSeries>
#include <QPieSeries>
#include <QChartView>
#include <QValueAxis>
#include <QBarCategoryAxis>

CircuitStatisticsPage::CircuitStatisticsPage(QWidget *parent)
    : QWidget(parent), ui(nullptr), splineChartView(nullptr), pieChartView(nullptr), tableWidget(nullptr)
{
    setupKPICards();
    setupCharts();
    setupTable();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Header
    QLabel *headerLabel = new QLabel("📊 Circuit Statistics", this);
    headerLabel->setStyleSheet(
        "font-family: 'Segoe UI'; "
        "font-size: 24px; "
        "font-weight: bold; "
        "color: #ffffff; "
        "background-color: #2c3e50; "
        "padding: 15px; "
        "border-radius: 8px; "
        "margin-bottom: 10px;"
    );
    headerLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(headerLabel);

    // KPI Cards Row
    QHBoxLayout *kpiLayout = new QHBoxLayout();
    kpiLayout->setSpacing(15);

    // KPI Card 1: Total Circuits
    QFrame *totalCard = new QFrame(this);
    totalCard->setStyleSheet(
        "QFrame { "
        "background-color: #34495e; "
        "border: 2px solid #4682b4; "
        "border-radius: 10px; "
        "color: white; "
        "}"
    );
    totalCard->setFixedHeight(120);

    QVBoxLayout *totalLayout = new QVBoxLayout(totalCard);
    QLabel *totalIcon = new QLabel("🔢", totalCard);
    totalIcon->setStyleSheet("font-size: 32px; margin-bottom: 5px;");
    totalIcon->setAlignment(Qt::AlignCenter);

    QLabel *totalValue = new QLabel("15", totalCard);
    totalValue->setStyleSheet("font-family: 'Segoe UI'; font-size: 28px; font-weight: bold; color: #3498db; margin-bottom: 5px;");
    totalValue->setAlignment(Qt::AlignCenter);

    QLabel *totalLabel = new QLabel("Total Circuits", totalCard);
    totalLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 14px; color: #bdc3c7;");
    totalLabel->setAlignment(Qt::AlignCenter);

    totalLayout->addWidget(totalIcon);
    totalLayout->addWidget(totalValue);
    totalLayout->addWidget(totalLabel);
    kpiLayout->addWidget(totalCard);

    // KPI Card 2: Active Circuits
    QFrame *activeCard = new QFrame(this);
    activeCard->setStyleSheet(
        "QFrame { "
        "background-color: #34495e; "
        "border: 2px solid #27ae60; "
        "border-radius: 10px; "
        "color: white; "
        "}"
    );
    activeCard->setFixedHeight(120);

    QVBoxLayout *activeLayout = new QVBoxLayout(activeCard);
    QLabel *activeIcon = new QLabel("✅", activeCard);
    activeIcon->setStyleSheet("font-size: 32px; margin-bottom: 5px;");
    activeIcon->setAlignment(Qt::AlignCenter);

    QLabel *activeValue = new QLabel("12", activeCard);
    activeValue->setStyleSheet("font-family: 'Segoe UI'; font-size: 28px; font-weight: bold; color: #2ecc71; margin-bottom: 5px;");
    activeValue->setAlignment(Qt::AlignCenter);

    QLabel *activeLabel = new QLabel("Active Circuits", activeCard);
    activeLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 14px; color: #bdc3c7;");
    activeLabel->setAlignment(Qt::AlignCenter);

    activeLayout->addWidget(activeIcon);
    activeLayout->addWidget(activeValue);
    activeLayout->addWidget(activeLabel);
    kpiLayout->addWidget(activeCard);

    // KPI Card 3: Deleted Circuits
    QFrame *deletedCard = new QFrame(this);
    deletedCard->setStyleSheet(
        "QFrame { "
        "background-color: #34495e; "
        "border: 2px solid #e74c3c; "
        "border-radius: 10px; "
        "color: white; "
        "}"
    );
    deletedCard->setFixedHeight(120);

    QVBoxLayout *deletedLayout = new QVBoxLayout(deletedCard);
    QLabel *deletedIcon = new QLabel("🗑️", deletedCard);
    deletedIcon->setStyleSheet("font-size: 32px; margin-bottom: 5px;");
    deletedIcon->setAlignment(Qt::AlignCenter);

    QLabel *deletedValue = new QLabel("2", deletedCard);
    deletedValue->setStyleSheet("font-family: 'Segoe UI'; font-size: 28px; font-weight: bold; color: #e74c3c; margin-bottom: 5px;");
    deletedValue->setAlignment(Qt::AlignCenter);

    QLabel *deletedLabel = new QLabel("Deleted Circuits", deletedCard);
    deletedLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 14px; color: #bdc3c7;");
    deletedLabel->setAlignment(Qt::AlignCenter);

    deletedLayout->addWidget(deletedIcon);
    deletedLayout->addWidget(deletedValue);
    deletedLayout->addWidget(deletedLabel);
    kpiLayout->addWidget(deletedCard);

    // KPI Card 4: Growth Percentage
    QFrame *growthCard = new QFrame(this);
    growthCard->setStyleSheet(
        "QFrame { "
        "background-color: #34495e; "
        "border: 2px solid #f39c12; "
        "border-radius: 10px; "
        "color: white; "
        "}"
    );
    growthCard->setFixedHeight(120);

    QVBoxLayout *growthLayout = new QVBoxLayout(growthCard);
    QLabel *growthIcon = new QLabel("📈", growthCard);
    growthIcon->setStyleSheet("font-size: 32px; margin-bottom: 5px;");
    growthIcon->setAlignment(Qt::AlignCenter);

    QLabel *growthValue = new QLabel("+8.5%", growthCard);
    growthValue->setStyleSheet("font-family: 'Segoe UI'; font-size: 28px; font-weight: bold; color: #f1c40f; margin-bottom: 5px;");
    growthValue->setAlignment(Qt::AlignCenter);

    QLabel *growthLabel = new QLabel("Growth Rate", growthCard);
    growthLabel->setStyleSheet("font-family: 'Segoe UI'; font-size: 14px; color: #bdc3c7;");
    growthLabel->setAlignment(Qt::AlignCenter);

    growthLayout->addWidget(growthIcon);
    growthLayout->addWidget(growthValue);
    growthLayout->addWidget(growthLabel);
    kpiLayout->addWidget(growthCard);

    mainLayout->addLayout(kpiLayout);

    // Charts Row
    QHBoxLayout *chartsLayout = new QHBoxLayout();
    chartsLayout->setSpacing(20);

    // Spline Chart
    QFrame *splineFrame = new QFrame(this);
    splineFrame->setStyleSheet(
        "QFrame { "
        "background-color: #34495e; "
        "border: 2px solid #4682b4; "
        "border-radius: 10px; "
        "}"
    );
    splineFrame->setFixedSize(400, 300);

    QVBoxLayout *splineLayout = new QVBoxLayout(splineFrame);
    QLabel *splineTitle = new QLabel("Circuit Activity Over Time", splineFrame);
    splineTitle->setStyleSheet("font-family: 'Segoe UI'; font-size: 16px; font-weight: bold; color: white; margin-bottom: 10px;");
    splineTitle->setAlignment(Qt::AlignCenter);

    // Create spline series with dummy data
    QSplineSeries *splineSeries = new QSplineSeries();
    splineSeries->setName("Circuit Activity");
    splineSeries->setColor(QColor(52, 152, 219));

    // Add dummy data points (last 7 days)
    splineSeries->append(0, 5);
    splineSeries->append(1, 8);
    splineSeries->append(2, 12);
    splineSeries->append(3, 7);
    splineSeries->append(4, 15);
    splineSeries->append(5, 10);
    splineSeries->append(6, 18);

    QChart *splineChart = new QChart();
    splineChart->addSeries(splineSeries);
    splineChart->setTitle("");
    splineChart->setBackgroundBrush(QBrush(QColor(52, 73, 94)));
    splineChart->legend()->setVisible(false);

    QValueAxis *axisX = new QValueAxis();
    axisX->setRange(0, 6);
    axisX->setLabelFormat("%d");
    axisX->setTitleText("Days");
    axisX->setTitleBrush(QBrush(Qt::white));
    axisX->setLabelsBrush(QBrush(Qt::white));

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 20);
    axisY->setTitleText("Activity");
    axisY->setTitleBrush(QBrush(Qt::white));
    axisY->setLabelsBrush(QBrush(Qt::white));

    splineChart->addAxis(axisX, Qt::AlignBottom);
    splineChart->addAxis(axisY, Qt::AlignLeft);
    splineSeries->attachAxis(axisX);
    splineSeries->attachAxis(axisY);

    splineChartView = new QChartView(splineChart, splineFrame);
    splineChartView->setRenderHint(QPainter::Antialiasing);
    splineLayout->addWidget(splineTitle);
    splineLayout->addWidget(splineChartView);

    chartsLayout->addWidget(splineFrame);

    // Pie Chart
    QFrame *pieFrame = new QFrame(this);
    pieFrame->setStyleSheet(
        "QFrame { "
        "background-color: #34495e; "
        "border: 2px solid #4682b4; "
        "border-radius: 10px; "
        "}"
    );
    pieFrame->setFixedSize(400, 300);

    QVBoxLayout *pieLayout = new QVBoxLayout(pieFrame);
    QLabel *pieTitle = new QLabel("Rating Distribution", pieFrame);
    pieTitle->setStyleSheet("font-family: 'Segoe UI'; font-size: 16px; font-weight: bold; color: white; margin-bottom: 10px;");
    pieTitle->setAlignment(Qt::AlignCenter);

    // Create pie series with dummy data for ratings
    QPieSeries *pieSeries = new QPieSeries();

    QPieSlice *slice1 = pieSeries->append("5 Stars", 8);
    slice1->setColor(QColor(46, 204, 113)); // Green

    QPieSlice *slice2 = pieSeries->append("4 Stars", 12);
    slice2->setColor(QColor(241, 196, 15)); // Yellow

    QPieSlice *slice3 = pieSeries->append("3 Stars", 5);
    slice3->setColor(QColor(231, 76, 60)); // Red

    QPieSlice *slice4 = pieSeries->append("1-2 Stars", 2);
    slice4->setColor(QColor(149, 165, 166)); // Gray

    // Make slices explode slightly for better visibility
    slice1->setExploded(true);
    slice2->setExploded(false);
    slice3->setExploded(false);
    slice4->setExploded(false);

    // Set labels visible
    slice1->setLabelVisible(true);
    slice2->setLabelVisible(true);
    slice3->setLabelVisible(true);
    slice4->setLabelVisible(true);

    QChart *pieChart = new QChart();
    pieChart->addSeries(pieSeries);
    pieChart->setTitle("");
    pieChart->setBackgroundBrush(QBrush(QColor(52, 73, 94)));
    pieChart->legend()->setVisible(true);
    pieChart->legend()->setAlignment(Qt::AlignRight);
    pieChart->legend()->setLabelColor(Qt::white);

    pieChartView = new QChartView(pieChart, pieFrame);
    pieChartView->setRenderHint(QPainter::Antialiasing);
    pieLayout->addWidget(pieTitle);
    pieLayout->addWidget(pieChartView);

    chartsLayout->addWidget(pieFrame);
    mainLayout->addLayout(chartsLayout);

    // Table
    QLabel *tableTitle = new QLabel("Recent Circuit Data", this);
    tableTitle->setStyleSheet(
        "font-family: 'Segoe UI'; "
        "font-size: 18px; "
        "font-weight: bold; "
        "color: #ffffff; "
        "margin-top: 20px; "
        "margin-bottom: 10px;"
    );
    tableTitle->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(tableTitle);

    tableWidget = new QTableWidget(this);
    tableWidget->setColumnCount(6);
    tableWidget->setHorizontalHeaderLabels(QStringList() << "ID" << "Name" << "Difficulty" << "Distance" << "Duration" << "Status");

    // Set table style
    tableWidget->setStyleSheet(
        "QTableWidget { "
        "background-color: #34495e; "
        "color: white; "
        "gridline-color: #4682b4; "
        "border: 2px solid #2c3e50; "
        "border-radius: 8px; "
        "}"
        "QHeaderView::section { "
        "background-color: #2c3e50; "
        "color: white; "
        "padding: 8px; "
        "border: 1px solid #34495e; "
        "font-weight: bold; "
        "}"
        "QTableWidget::item { "
        "padding: 5px; "
        "border-bottom: 1px solid #4a6274; "
        "}"
        "QTableWidget::item:selected { "
        "background-color: #4682b4; "
        "}"
    );

    // Add dummy data
    tableWidget->setRowCount(5);
    tableWidget->setItem(0, 0, new QTableWidgetItem("1"));
    tableWidget->setItem(0, 1, new QTableWidgetItem("Circuit Ville"));
    tableWidget->setItem(0, 2, new QTableWidgetItem("Facile"));
    tableWidget->setItem(0, 3, new QTableWidgetItem("15.5 km"));
    tableWidget->setItem(0, 4, new QTableWidgetItem("45 min"));
    tableWidget->setItem(0, 5, new QTableWidgetItem("Active"));

    tableWidget->setItem(1, 0, new QTableWidgetItem("2"));
    tableWidget->setItem(1, 1, new QTableWidgetItem("Circuit Montagne"));
    tableWidget->setItem(1, 2, new QTableWidgetItem("Difficile"));
    tableWidget->setItem(1, 3, new QTableWidgetItem("35.2 km"));
    tableWidget->setItem(1, 4, new QTableWidgetItem("90 min"));
    tableWidget->setItem(1, 5, new QTableWidgetItem("Active"));

    tableWidget->setItem(2, 0, new QTableWidgetItem("3"));
    tableWidget->setItem(2, 1, new QTableWidgetItem("Circuit Désert"));
    tableWidget->setItem(2, 2, new QTableWidgetItem("Moyen"));
    tableWidget->setItem(2, 3, new QTableWidgetItem("28.7 km"));
    tableWidget->setItem(2, 4, new QTableWidgetItem("75 min"));
    tableWidget->setItem(2, 5, new QTableWidgetItem("Active"));

    tableWidget->setItem(3, 0, new QTableWidgetItem("4"));
    tableWidget->setItem(3, 1, new QTableWidgetItem("Circuit Littoral"));
    tableWidget->setItem(3, 2, new QTableWidgetItem("Facile"));
    tableWidget->setItem(3, 3, new QTableWidgetItem("12.3 km"));
    tableWidget->setItem(3, 4, new QTableWidgetItem("35 min"));
    tableWidget->setItem(3, 5, new QTableWidgetItem("Active"));

    tableWidget->setItem(4, 0, new QTableWidgetItem("5"));
    tableWidget->setItem(4, 1, new QTableWidgetItem("Circuit Forêt"));
    tableWidget->setItem(4, 2, new QTableWidgetItem("Moyen"));
    tableWidget->setItem(4, 3, new QTableWidgetItem("22.1 km"));
    tableWidget->setItem(4, 4, new QTableWidgetItem("65 min"));
    tableWidget->setItem(4, 5, new QTableWidgetItem("Active"));

    tableWidget->resizeColumnsToContents();
    tableWidget->setMaximumHeight(250);

    mainLayout->addWidget(tableWidget);
    mainLayout->addStretch();

    setLayout(mainLayout);
}

CircuitStatisticsPage::~CircuitStatisticsPage()
{
    delete ui;
    delete splineChartView;
    delete pieChartView;
    delete tableWidget;
}

void CircuitStatisticsPage::setupKPICards()
{
    // Implementation moved to constructor
}

void CircuitStatisticsPage::setupCharts()
{
    // Implementation moved to constructor
}

void CircuitStatisticsPage::setupTable()
{
    // Implementation moved to constructor
}

