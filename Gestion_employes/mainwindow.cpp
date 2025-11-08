#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QPdfWriter>
#include <QPainter>
#include <QMap>
#include <QLabel>
#include <QVBoxLayout>
#include <QtCharts>
#include <QPieSeries>
#include <QChart>
#include <QChartView>
#include <QAbstractItemView>  // Pour NoEditTriggers
#include "connection.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_isModifyMode(false)
    , statsTable(nullptr)
    , statsLabel(nullptr)
    , m_pieChart(new QChart())
    , m_chartView(new QChartView(m_pieChart, ui->tab_2))
{
    ui->setupUi(this);
    Connection c;
    bool connected = c.createconnect();
    if (!connected){
        QMessageBox::critical(this, "Erreur de connexion", "Impossible de se connecter à Oracle !");
        return;
    }
    setupStyle(); // ✅ Appliquer le style général
    if (!initDatabase()) {
        showError("Erreur d'initialisation de la base de données !");
        return;
    }
    loadEmployes();
    // Table des employés
    if (ui->tableWidget) {
        ui->tableWidget->setColumnCount(7);
        QStringList headers = {"ID", "Nom", "Prénom", "Poste", "Mot de passe", "Num Tel", "Date Embauche"};
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        ui->tableWidget->setAlternatingRowColors(true);
        // ✅ Ajustement clair des colonnes
        ui->tableWidget->setColumnWidth(0, 60);
        ui->tableWidget->setColumnWidth(1, 150);
        ui->tableWidget->setColumnWidth(2, 150);
        ui->tableWidget->setColumnWidth(3, 150);
        ui->tableWidget->setColumnWidth(4, 120);
        ui->tableWidget->setColumnWidth(5, 150);
        ui->tableWidget->setColumnWidth(6, 130);
    }
    // ✅ Mettre l’onglet "Ajouter" en premier
    if (ui->tabWidget)
        ui->tabWidget->setCurrentIndex(0);
    // Statistiques
    if (ui->tab_2) {
        QVBoxLayout *statsLayout = new QVBoxLayout(ui->tab_2);
        statsLayout->setContentsMargins(10, 10, 10, 10);
        statsLabel = new QLabel("Statistiques des Employés", ui->tab_2);
        statsLabel->setStyleSheet("font-weight: bold; font-size: 14px;");
        statsLayout->addWidget(statsLabel);
        statsTable = new QTableWidget(ui->tab_2);
        statsTable->setColumnCount(2);
        statsTable->setHorizontalHeaderLabels({"Statistique", "Valeur"});
        statsTable->horizontalHeader()->setStretchLastSection(true);
        statsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        statsTable->setAlternatingRowColors(true);
        statsLayout->addWidget(statsTable);
        m_pieChart->setTitle("Répartition des employés par poste (%)");
        m_pieChart->setAnimationOptions(QChart::SeriesAnimations);
        m_chartView->setRenderHint(QPainter::Antialiasing);
        m_chartView->setMinimumSize(400, 300);
        statsLayout->addWidget(m_chartView);
    }
    // Connexions
    if (ui->pushButton) connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::on_pushButton_clicked);
    if (ui->pushButton_2) connect(ui->pushButton_2, &QPushButton::clicked, this, &MainWindow::on_pushButton_2_clicked);
    if (ui->pushButton_4) connect(ui->pushButton_4, &QPushButton::clicked, this, &MainWindow::on_pushButton_4_clicked);
    if (ui->pushButton_5) connect(ui->pushButton_5, &QPushButton::clicked, this, &MainWindow::on_pushButton_5_clicked);
    if (ui->comboBox) connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::on_comboBox_currentIndexChanged);
    if (ui->tableWidget) connect(ui->tableWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::on_tableWidget_itemSelectionChanged);
    updateTable();
    updateStats();
}

MainWindow::~MainWindow()
{
    if (m_db.isOpen()) m_db.close();
    delete ui;
}

// ✅ Style global QSS
void MainWindow::setupStyle()
{
    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #f4f6f9;
        }
        QTableWidget {
            background-color: #ffffff;
            border: 1px solid #d0d0d0;
            gridline-color: #e0e0e0;
            font-size: 13px;
        }
        QHeaderView::section {
            background-color: #0078d7;
            color: white;
            font-weight: bold;
            border: none;
        }
        QPushButton {
            background-color: #0078d7;
            color: white;
            border-radius: 6px;
            padding: 6px 12px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #005fa3;
        }
        QLabel {
            font-size: 13px;
        }
    )");
}

// 🧮 Mise à jour des stats
void MainWindow::updateStats()
{
    if (!statsTable || m_employes.isEmpty()) return;
    int total = m_employes.size();
    statsTable->setRowCount(4);
    statsTable->setItem(0, 0, new QTableWidgetItem("Total employés"));
    statsTable->setItem(0, 1, new QTableWidgetItem(QString::number(total)));
    double totalSeniority = 0;
    for (const Employe &emp : m_employes)
        totalSeniority += emp.dateEmbauche().daysTo(QDate::currentDate()) / 365.25;
    double avgSeniority = total > 0 ? totalSeniority / total : 0;
    statsTable->setItem(1, 0, new QTableWidgetItem("Ancienneté moyenne (ans)"));
    statsTable->setItem(1, 1, new QTableWidgetItem(QString::number(avgSeniority, 'f', 1)));
    QMap<QString, int> countByPoste;
    for (const Employe &emp : m_employes)
        countByPoste[emp.poste()]++;
    QPieSeries *series = new QPieSeries();
    double totalPct = 0;
    for (auto it = countByPoste.constBegin(); it != countByPoste.constEnd(); ++it) {
        double pct = (static_cast<double>(it.value()) / total) * 100;
        series->append(it.key(), it.value());
        totalPct += pct;
    }
    series->setLabelsVisible(true);
    series->setLabelsPosition(QPieSlice::LabelOutside);
    m_pieChart->removeAllSeries();
    m_pieChart->addSeries(series);
    m_pieChart->legend()->setVisible(true);
    m_pieChart->legend()->setAlignment(Qt::AlignBottom);
    statsTable->setItem(2, 0, new QTableWidgetItem("Nombre de postes uniques"));
    statsTable->setItem(2, 1, new QTableWidgetItem(QString::number(countByPoste.size())));
    statsTable->setItem(3, 0, new QTableWidgetItem("Total % (diagramme)"));
    statsTable->setItem(3, 1, new QTableWidgetItem(QString::number(totalPct, 'f', 1) + "%"));
}

bool MainWindow::initDatabase()
{
    // ✅ Passage à Oracle
    m_db = QSqlDatabase::addDatabase("QODBC");
    m_db.setDatabaseName("drivingschool");  // Nom de ta base Oracle (SID ou service name)
    m_db.setUserName("drivingschool");   // Ton utilisateur Oracle
    m_db.setPassword("123456"); // Ton mot de passe Oracle

    if (!m_db.open()) {
        qDebug() << "Erreur DB:" << m_db.lastError().text();
        return false;
    }

    QSqlQuery query;
    // ✅ CREATE TABLE version Oracle
    query.exec(R"(
        BEGIN
            EXECUTE IMMEDIATE '
            CREATE TABLE employes (
                id NUMBER GENERATED BY DEFAULT ON NULL AS IDENTITY PRIMARY KEY,
                nom VARCHAR2(100),
                prenom VARCHAR2(100),
                poste VARCHAR2(100),
                motDePasse VARCHAR2(100),
                numTel VARCHAR2(20),
                dateEmbauche DATE
            )';
        EXCEPTION
            WHEN OTHERS THEN
                IF SQLCODE != -955 THEN
                    RAISE;
                END IF;
        END;
    )");
    return true;
}

// Load Employes from DB
void MainWindow::loadEmployes()
{
    m_employes.clear();
    QSqlQuery query("SELECT * FROM employes ORDER BY id");
    while (query.next()) {
        int id = query.value("id").toInt();
        QString nom = query.value("nom").toString();
        QString prenom = query.value("prenom").toString();
        QString poste = query.value("poste").toString();
        QString mdp = query.value("motDePasse").toString();
        QString tel = query.value("numTel").toString();
        QDate dateEmb = query.value("dateEmbauche").toDate();
        m_employes.append(Employe(id, nom, prenom, poste, mdp, tel, dateEmb));
    }
    updateStats(); // Mise à jour stats après load
}

// Save Employe to DB
bool MainWindow::saveEmploye(const Employe& emp)
{
    QSqlQuery query;
    query.prepare("INSERT INTO employes (nom, prenom, poste, motDePasse, numTel, dateEmbauche) VALUES (:nom, :prenom, :poste, :mdp, :tel, :dateE)");
    query.bindValue(":nom", emp.nom());
    query.bindValue(":prenom", emp.prenom());
    query.bindValue(":poste", emp.poste());
    query.bindValue(":mdp", emp.motDePasse());
    query.bindValue(":tel", emp.numTel());
    query.bindValue(":dateE", emp.dateEmbauche());
    return query.exec();
}

// Update Employe in DB
bool MainWindow::updateEmploye(int id, const Employe& emp)
{
    QSqlQuery query;
    query.prepare("UPDATE employes SET nom=:nom, prenom=:prenom, poste=:poste, motDePasse=:mdp, numTel=:tel, dateEmbauche=:dateE WHERE id=:id");
    query.bindValue(":nom", emp.nom());
    query.bindValue(":prenom", emp.prenom());
    query.bindValue(":poste", emp.poste());
    query.bindValue(":mdp", emp.motDePasse());
    query.bindValue(":tel", emp.numTel());
    query.bindValue(":dateE", emp.dateEmbauche());
    query.bindValue(":id", id);
    return query.exec();
}

// Delete Employe from DB
bool MainWindow::deleteEmploye(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM employes WHERE id=:id");
    query.bindValue(":id", id);
    return query.exec();
}

int MainWindow::getNextId() const
{
    QSqlQuery query("SELECT NVL(MAX(id),0)+1 FROM employes");
    if (query.next()) {
        int maxId = query.value(0).toInt();
        return maxId;
    }
    return 1;
}

// ✅ Toutes les autres fonctions restent inchangées (ajout, suppression, stats, export PDF, etc.)
// Slot: Ajouter ou Modifier
void MainWindow::on_pushButton_clicked()
{
    if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty() || ui->lineEdit_3->text().isEmpty() ||
        ui->lineEdit_4->text().isEmpty() || ui->lineEdit_5->text().isEmpty() || ui->lineEdit_6->text().isEmpty()) {
        showError("Tous les champs obligatoires doivent être remplis !");
        return;
    }
    QString numTel = "+216" + ui->lineEdit_6->text();
    QDate dateEmb = ui->dateEdit->date();
    int id = m_isModifyMode ? ui->lineEdit->text().toInt() : getNextId();
    Employe emp(id, ui->lineEdit_2->text(), ui->lineEdit_3->text(), ui->lineEdit_4->text(),
                ui->lineEdit_5->text(), numTel, dateEmb);
    bool success;
    if (m_isModifyMode) {
        success = updateEmploye(id, emp);
        if (success) {
            for (int i = 0; i < m_employes.size(); ++i) {
                if (m_employes[i].id() == id) {
                    m_employes[i] = emp;
                    break;
                }
            }
            ui->pushButton->setText("Ajouter");
            m_isModifyMode = false;
        }
    } else {
        success = saveEmploye(emp);
        if (success) m_employes.append(emp);
    }
    if (!success) {
        showError("Erreur lors de la sauvegarde !");
        return;
    }
    updateTable();
    updateStats(); // Mise à jour stats et diagramme après modification
    clearForm();
    QMessageBox::information(this, "Succès", m_isModifyMode ? "Employé modifié !" : "Employé ajouté !");
}

// Slot: Afficher/Rechercher
void MainWindow::on_pushButton_2_clicked()
{
    QString searchText = ui->lineEdit_13->text().toLower();
    QList<Employe> filtered;
    if (searchText.isEmpty()) {
        filtered = m_employes;
    } else {
        for (const Employe& emp : m_employes) {
            if (QString::number(emp.id()).contains(searchText) || emp.nom().toLower().contains(searchText)) {
                filtered.append(emp);
            }
        }
    }
    updateTable(filtered);
}

// Slot: Supprimer
void MainWindow::on_pushButton_4_clicked()
{
    bool ok;
    int idToDelete = ui->lineEdit_14->text().toInt(&ok);
    if (!ok || idToDelete <= 0) {
        showError("ID invalide pour suppression !");
        return;
    }
    if (deleteEmploye(idToDelete)) {
        for (int i = 0; i < m_employes.size(); ++i) {
            if (m_employes[i].id() == idToDelete) {
                m_employes.removeAt(i);
                break;
            }
        }
        updateTable();
        updateStats(); // Mise à jour après suppression
        ui->lineEdit_14->clear();
        QMessageBox::information(this, "Succès", "Employé supprimé !");
    } else {
        showError("Employé non trouvé pour suppression !");
    }
}

// Slot: Exporter PDF (valeurs dynamiques basées sur m_employes actuelle)
void MainWindow::on_pushButton_5_clicked()
{
    QPdfWriter writer("employes.pdf");
    writer.setPageSize(QPageSize::A4);
    writer.setResolution(300);
    QPainter painter(&writer);
    painter.setFont(QFont("Arial", 10)); // Police petite (10pt)
    // Titre centré
    painter.setFont(QFont("Arial", 14, QFont::Bold));
    painter.drawText(50, 80, "Liste des Employés");
    painter.setFont(QFont("Arial", 10, QFont::Bold)); // Bold pour en-tête
    // Bordure du tableau (plein page)
    int startY = 120;
    int rowHeight = 60; // Hauteur grande
    // Ajustements largeurs : Num Tel doublé (200 -> 400), Date doublée (300 -> 600)
    int colWidths[7] = {80, 300, 300, 250, 200, 400, 600}; // Nouvelles largeurs
    int totalWidth = 80 + 300 + 300 + 250 + 200 + 400 + 600; // Recalculé = 2130px (adapté A4 ~2100px, légère compression si besoin)
    int xPos = 10;
    int yPos = startY;
    // En-têtes avec bordure
    painter.setPen(QPen(Qt::black, 2)); // Bordure épaisse
    painter.drawRect(xPos, yPos, totalWidth, rowHeight); // Bordure haut
    painter.drawText(xPos + 30, yPos + 40, "ID");
    xPos += 80; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); painter.drawText(xPos + 30, yPos + 40, "Nom");
    xPos += 300; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); painter.drawText(xPos + 30, yPos + 40, "Prénom");
    xPos += 300; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); painter.drawText(xPos + 30, yPos + 40, "Poste");
    xPos += 250; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); painter.drawText(xPos + 30, yPos + 40, "MDP");
    xPos += 200; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); painter.drawText(xPos + 30, yPos + 40, "Num Tel");
    xPos += 400; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); painter.drawText(xPos + 30, yPos + 40, "Date E");
    xPos += 600; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); // Bordure droite
    xPos = 10; yPos += rowHeight;
    painter.drawLine(10, yPos, 10 + totalWidth, yPos); // Bordure après en-tête
    painter.setPen(QPen(Qt::black, 1)); // Bordure fine pour données
    painter.setFont(QFont("Arial", 10)); // Police petite pour données
    // Données dynamiques (basées sur m_employes actuelle, avec boucle pour n'importe quel nombre d'employés)
    for (const Employe& emp : m_employes) {
        xPos = 10;
        painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); // Bordure gauche
        painter.drawText(xPos + 30, yPos + 40, QString::number(emp.id()));
        xPos += 80; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); painter.drawText(xPos + 30, yPos + 40, emp.nom().left(40) + (emp.nom().length() > 40 ? "..." : ""));
        xPos += 300; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); painter.drawText(xPos + 30, yPos + 40, emp.prenom().left(45) + (emp.prenom().length() > 45 ? "..." : ""));
        xPos += 300; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); painter.drawText(xPos + 30, yPos + 40, emp.poste().left(35) + (emp.poste().length() > 35 ? "..." : ""));
        xPos += 250; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); painter.drawText(xPos + 30, yPos + 40, emp.motDePasse().left(30) + (emp.motDePasse().length() > 30 ? "..." : ""));
        xPos += 200; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); painter.drawText(xPos + 30, yPos + 40, emp.numTel().left(50) + (emp.numTel().length() > 50 ? "..." : "")); // Tronquage adapté à nouvelle largeur
        xPos += 400; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); painter.drawText(xPos + 30, yPos + 40, emp.dateEmbauche().toString("yyyy-MM-dd")); // Pas de tronquage pour date
        xPos += 600; painter.drawLine(xPos, yPos, xPos, yPos + rowHeight); // Bordure droite
        yPos += rowHeight;
        painter.drawLine(10, yPos, 10 + totalWidth, yPos); // Bordure bas ligne
        if (yPos > 650) { // Multi-page dynamique
            writer.newPage();
            yPos = startY;
        }
    }
    painter.drawLine(10, yPos, 10 + totalWidth, yPos); // Bordure bas final
    QMessageBox::information(this, "Succès", "Exporté vers employes.pdf !");
}

// Slot: Trier
void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if (index == 0) { // Poste
        std::sort(m_employes.begin(), m_employes.end(), [](const Employe& a, const Employe& b) {
            return a.poste() < b.poste();
        });
    } else if (index == 1) { // ID
        std::sort(m_employes.begin(), m_employes.end(), [](const Employe& a, const Employe& b) {
            return a.id() < b.id();
        });
    }
    updateTable();
}

// Slot: Charger sélection pour modification
void MainWindow::on_tableWidget_itemSelectionChanged()
{
    if (ui->tableWidget->selectedItems().isEmpty()) return;
    int row = ui->tableWidget->currentRow();
    if (row < 0 || row >= m_employes.size()) return;
    const Employe& emp = m_employes[row];
    ui->lineEdit->setText(QString::number(emp.id()));
    ui->lineEdit_2->setText(emp.nom());
    ui->lineEdit_3->setText(emp.prenom());
    ui->lineEdit_4->setText(emp.poste());
    ui->lineEdit_5->setText(emp.motDePasse());
    ui->lineEdit_6->setText(emp.numTel().remove("+216"));
    ui->dateEdit->setDate(emp.dateEmbauche());
    m_isModifyMode = true;
    ui->pushButton->setText("Modifier");
}

// Met à jour le tableau
void MainWindow::updateTable(const QList<Employe>& employesToShow)
{
    if (!ui->tableWidget) return;
    const QList<Employe>& list = employesToShow.isEmpty() ? m_employes : employesToShow;
    ui->tableWidget->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        const Employe& emp = list[i];
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(emp.id())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(emp.nom()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(emp.prenom()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(emp.poste()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(emp.motDePasse()));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(emp.numTel()));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(emp.dateEmbauche().toString("yyyy-MM-dd")));
    }
}

// Vide le formulaire
void MainWindow::clearForm()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->lineEdit_13->clear();
    ui->lineEdit_14->clear();
}

// Affiche erreur
void MainWindow::showError(const QString& message)
{
    QMessageBox::warning(this, "Erreur", message);
}
