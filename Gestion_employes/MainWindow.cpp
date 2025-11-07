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
#include <QAbstractItemView>
#include "connection.h"
#include <QMainWindow>
#include <QTextDocument>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_isModifyMode(false)
    , statsTable(nullptr)
    , statsLabel(nullptr)
    , m_pieChart(new QChart())
    , m_chartView(nullptr)
{
    ui->setupUi(this);

    Connection c;
    bool connected = c.createConnect();
    if (!connected) {
        QMessageBox::critical(this, "Erreur de connexion", "Impossible de se connecter à la base Oracle !");
        return;
    }
    qDebug() << "✅ Connexion Oracle réussie via QODBC";

    setupStyle();

    if (!initDatabase()) {
        showError("Erreur d'initialisation de la base de données !");
        return;
    }

    loadEmployes();

    if (ui->tableWidget) {
        ui->tableWidget->setColumnCount(7);
        QStringList headers = {"ID", "Nom", "Prénom", "Poste", "Mot de passe", "Num Tel", "Date Embauche"};
        ui->tableWidget->setHorizontalHeaderLabels(headers);
        ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
        ui->tableWidget->setAlternatingRowColors(false);
        ui->tableWidget->setStyleSheet(
            "QTableWidget {"
            "background-color: #001B70;"   // bleu foncé uniforme
            "color: white;"                // texte blanc
            "gridline-color: #CCCCCC;"
            "selection-background-color: #0078D7;"
            "selection-color: white;"
            "}"
            "QHeaderView::section {"
            "background-color: #0078D7;"
            "color: white;"
            "font-weight: bold;"
            "}"
            );
    }

    if (ui->tabWidget)
        ui->tabWidget->setCurrentIndex(0);

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

        // ✅ ICI : créer le QChartView AVANT de l’utiliser
        m_chartView = new QChartView(m_pieChart, ui->tab_2);
        m_pieChart->setTitle("Répartition des employés par POSTE (%)");
        m_pieChart->setAnimationOptions(QChart::SeriesAnimations);
        m_chartView->setRenderHint(QPainter::Antialiasing);
        m_chartView->setMinimumSize(400, 300);

        statsLayout->addWidget(m_chartView);
    }

    updateTable();
    updateStats();
}
MainWindow::~MainWindow()
{
    if (m_db.isOpen()) m_db.close();
    delete ui;
}

void MainWindow::setupStyle()
{
    this->setStyleSheet(R"(
        QMainWindow { background-color: #f4f6f9; }
        QTableWidget { background-color: #ffffff; border: 1px solid #d0d0d0; font-size: 13px; }
        QHeaderView::section { background-color: #0078d7; color: white; font-weight: bold; border: none; }
        QPushButton { background-color: #0078d7; color: white; border-radius: 6px; padding: 6px 12px; font-weight: bold; }
        QPushButton:hover { background-color: #005fa3; }
        QLabel { font-size: 13px; }
    )");
}

bool MainWindow::initDatabase()
{
    m_db = QSqlDatabase::database();
    if (!m_db.isOpen()) {
        if (!m_db.open()) {
            qDebug() << "❌ Erreur ouverture base :" << m_db.lastError().text();
            return false;
        }
    }

    QSqlQuery checkQuery;
    if (!checkQuery.exec("SELECT COUNT(*) FROM user_tables WHERE table_name = 'EMPLOYE'")) {
        qDebug() << "❌ Erreur exécution SELECT user_tables :" << checkQuery.lastError().text();
        return false;
    }

    bool exists = false;
    if (checkQuery.next())
        exists = checkQuery.value(0).toInt() > 0;

    if (!exists) {
        QSqlQuery createQuery;
        QString sql = R"(
            CREATE TABLE EMPLOYE (
                ID_EMPLOYE NUMBER PRIMARY KEY,
                NOM_EMPLOYE VARCHAR2(100),
                PRENOM VARCHAR2(100),
                POSTE VARCHAR2(100),
                MOT_DE_PASSEE VARCHAR2(100),
                TEL_EMPLOYE VARCHAR2(20),
                DATE_EMBAUCHE DATE
            )
        )";
        if (!createQuery.exec(sql)) {
            qDebug() << "❌ Erreur création table :" << createQuery.lastError().text();
            return false;
        } else {
            qDebug() << "✅ Table 'EMPLOYE' créée avec succès.";
        }
    } else {
        qDebug() << "ℹ️ Table 'EMPLOYE' déjà existante.";
    }

    return true;
}


void MainWindow::loadEmployes()
{
    m_employes.clear();
    QSqlQuery query("SELECT * FROM EMPLOYE ORDER BY ID_EMPLOYE");
    while (query.next()) {
        int ID_EMPLOYE = query.value("ID_EMPLOYE").toInt();
        QString NOM_EMPLOYE = query.value("NOM_EMPLOYE").toString();
        QString PRENOM = query.value("PRENOM").toString();
        QString POSTE = query.value("POSTE").toString();
        QString MOT_DE_PASSEE = query.value("MOT_DE_PASSEE").toString();
        QString TEL_EMPLOYE = query.value("TEL_EMPLOYE").toString();
        QDate DATE_EMBAUCHE = query.value("DATE_EMBAUCHE").toDate();
        m_employes.append(Employe(ID_EMPLOYE, NOM_EMPLOYE, PRENOM, POSTE, MOT_DE_PASSEE, TEL_EMPLOYE, DATE_EMBAUCHE));
    }
    updateStats();
}
bool MainWindow::saveEmploye(const Employe& emp)
{
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYE (NOM_EMPLOYE, PRENOM, POSTE, MOT_DE_PASSEE, TEL_EMPLOYE, DATE_EMBAUCHE) "
                  "VALUES (:NOM_EMPLOYE, :PRENOM, :POSTE, :MOT_DE_PASSEE, :TEL_EMPLOYE, :DATE_EMBAUCHE)");
    query.bindValue(":NOM_EMPLOYE", emp.NOM_EMPLOYE());
    query.bindValue(":PRENOM", emp.PRENOM());
    query.bindValue(":POSTE", emp.POSTE());
    query.bindValue(":MOT_DE_PASSEE", emp.MOT_DE_PASSEE());
    query.bindValue(":TEL_EMPLOYE", emp.TEL_EMPLOYE());
    query.bindValue(":DATE_EMBAUCHE", emp.DATE_EMBAUCHE());

    if (!query.exec()) {
        qDebug() << "❌ Erreur insertion employé :" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Employé inséré avec succès :" << emp.NOM_EMPLOYE() << emp.PRENOM();
    return true;
}

bool MainWindow::updateEmploye(int ID_EMPLOYE, const Employe& emp)
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYE SET NOM_EMPLOYE=:NOM_EMPLOYE, PRENOM=:PRENOM, POSTE=:POSTE, MOT_DE_PASSEE=:MOT_DE_PASSEE, "
                  "TEL_EMPLOYE=:TEL_EMPLOYE, DATE_EMBAUCHE=:DATE_EMBAUCHE WHERE ID_EMPLOYE=:ID_EMPLOYE");
    query.bindValue(":NOM_EMPLOYE", emp.NOM_EMPLOYE());
    query.bindValue(":PRENOM", emp.PRENOM());
    query.bindValue(":POSTE", emp.POSTE());
    query.bindValue(":MOT_DE_PASSEE", emp.MOT_DE_PASSEE());
    query.bindValue(":TEL_EMPLOYE", emp.TEL_EMPLOYE());
    query.bindValue(":DATE_EMBAUCHE", emp.DATE_EMBAUCHE());
    query.bindValue(":ID_EMPLOYE", ID_EMPLOYE);
    return query.exec();
}

bool MainWindow::deleteEmploye(int ID_EMPLOYE)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYE WHERE ID_EMPLOYE=:ID_EMPLOYE");
    query.bindValue(":ID_EMPLOYE", ID_EMPLOYE);
    return query.exec();
}

int MainWindow::getNextId() const
{
    QSqlQuery query("SELECT NVL(MAX(ID_EMPLOYE),0)+1 FROM EMPLOYE");
    if (query.next())
        return query.value(0).toInt();
    return 1;
}

// ✅ Slots fonctionnels
void MainWindow::on_pushButton_clicked()
{
    QString NOM_EMPLOYE = ui->lineEdit_2->text();
    QString PRENOM = ui->lineEdit_3->text();
    QString POSTE = ui->lineEdit_4->text();
    QString MOT_DE_PASSEE = ui->lineEdit_5->text();
    QString TEL_EMPLOYE = "+216" + ui->lineEdit_6->text();
    QDate DATE_EMBAUCHE = ui->dateEdit->date();

    if (NOM_EMPLOYE.isEmpty() || PRENOM.isEmpty() || POSTE.isEmpty() || MOT_DE_PASSEE.isEmpty() || TEL_EMPLOYE.isEmpty()) {
        showError("Veuillez remplir tous les champs !");
        return;
    }

    // 🧩 Cas 1 : modification
    if (m_isModifyMode) {
        bool ok;
        int ID_EMPLOYE = ui->lineEdit->text().toInt(&ok);
        if (!ok || ID_EMPLOYE <= 0) {
            showError("ID d'employé invalide !");
            return;
        }

        Employe emp(ID_EMPLOYE, NOM_EMPLOYE, PRENOM, POSTE, MOT_DE_PASSEE, TEL_EMPLOYE, DATE_EMBAUCHE);

        if (updateEmploye(ID_EMPLOYE, emp)) {
            QMessageBox::information(this, "Succès", "Employé modifié avec succès !");
            loadEmployes();
            updateTable();
            updateStats();
            clearForm();
            ui->pushButton->setText("Ajouter");
            m_isModifyMode = false;
        } else {
            showError("Erreur lors de la modification de l'employé !");
        }
    }
    // 🧩 Cas 2 : ajout normal
    else {
        Employe emp(getNextId(), NOM_EMPLOYE, PRENOM, POSTE, MOT_DE_PASSEE, TEL_EMPLOYE, DATE_EMBAUCHE);

        if (saveEmploye(emp)) {
            QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
            loadEmployes();
            updateTable();
            updateStats();
            clearForm();
        } else {
            showError("Erreur lors de l'insertion de l'employé !");
        }
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::information(this, "Info", "Bouton Rechercher cliqué !");
}

void MainWindow::on_pushButton_4_clicked()
{
    QString idText = ui->lineEdit_14->text().trimmed();

    if (idText.isEmpty()) {
        showError("Veuillez entrer l'ID de l'employé à supprimer !");
        return;
    }

    bool ok;
    int ID_EMPLOYE = idText.toInt(&ok);
    if (!ok) {
        showError("L'ID doit être un nombre valide !");
        return;
    }

    // 🔹 Vérifie directement dans la base de données
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT ID_EMPLOYE FROM EMPLOYE WHERE ID_EMPLOYE = :ID_EMPLOYE");
    checkQuery.bindValue(":ID_EMPLOYE", ID_EMPLOYE);

    if (!checkQuery.exec()) {
        showError("Erreur lors de la vérification dans la base : " + checkQuery.lastError().text());
        return;
    }

    if (!checkQuery.next()) {
        showError("Aucun employé trouvé avec cet ID !");
        return;
    }

    // 🔹 Confirmation avant suppression
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirmation de suppression",
        QString("Voulez-vous vraiment supprimer l'employé #%1 ?").arg(ID_EMPLOYE),
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::No)
        return;

    // 🔹 Supprimer de la base
    if (deleteEmploye(ID_EMPLOYE)) {
        QMessageBox::information(this, "Succès", QString("Employé #%1 supprimé avec succès !").arg(ID_EMPLOYE));

        // 🔹 Recharge immédiatement la liste depuis la base
        loadEmployes();
        updateTable();
        updateStats();

        // 🔹 Vide la zone de saisie après suppression
        ui->lineEdit_14->clear();
    } else {
        showError("Erreur lors de la suppression de l'employé : " + m_db.lastError().text());
    }
}
// Button qui génére le PDF
void MainWindow::on_pushButton_5_clicked()
{
    // 🕒 Nom dynamique du fichier PDF
    QString dateStr = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    QString fileName = QString("employes_%1.pdf").arg(dateStr);

    QPdfWriter writer(fileName);
    writer.setPageSize(QPageSize::A4);
    writer.setResolution(300);

    QPainter painter(&writer);
    painter.setRenderHint(QPainter::Antialiasing);

    // 🧱 Dimensions de la page
    int pageWidth = writer.width();
    int pageHeight = writer.height();
    int margin = 50;
    int tableWidth = pageWidth - 2 * margin;

    // 🎨 Police
    QFont titleFont("Arial", 22, QFont::Bold);
    QFont headerFont("Arial",11, QFont::Bold);
    QFont dataFont("Arial", 26);

    // 🧾 Titre
    painter.setFont(titleFont);
    painter.drawText(QRect(margin, 80, tableWidth, 100), Qt::AlignCenter, "Liste des Employés");

    // 🧭 En-têtes
    QStringList headers = {"ID", "Nom", "Prénom", "Poste", "Mot de passe", "Numéro", "Date Embauche"};
    QFontMetrics fmHeader(headerFont);

    // 🔹 Calcul de la largeur des colonnes selon la taille du texte + marge
    QVector<int> colWidths;
    int totalWidth = 0;
    for (const QString &header : headers) {
        int w = fmHeader.horizontalAdvance(header) + 60; // marge confortable
        colWidths.append(w);
        totalWidth += w;
    }

    // 🔸 Ajustement proportionnel à la largeur totale de la page
    double scale = double(tableWidth) / totalWidth;
    for (int &w : colWidths)
        w = int(w * scale);

    // 🎨 Position initiale
    int startY = 250;
    int y = startY;
    int rowHeight = 0;

    // 🧱 En-tête du tableau
    painter.setFont(headerFont);
    painter.setBrush(QColor("#007bff"));
    painter.setPen(Qt::NoPen);
    painter.drawRect(margin, y, tableWidth, 50);

    painter.setPen(Qt::white);
    int x = margin;
    for (int i = 0; i < headers.size(); ++i) {
        painter.drawText(QRect(x, y, colWidths[i], 50), Qt::AlignCenter, headers[i]);
        x += colWidths[i];
    }

    y += 50;

    // 📄 Données
    painter.setFont(dataFont);
    QPen borderPen(QColor("#cccccc"), 0.5);
    QFontMetrics fmData(dataFont);

    for (int i = 0; i < m_employes.size(); ++i) {
        const Employe &emp = m_employes[i];
        QStringList data = {
            QString::number(emp.ID_EMPLOYE()),
            emp.NOM_EMPLOYE(),
            emp.PRENOM(),
            emp.POSTE(),
            emp.MOT_DE_PASSEE(),
            emp.TEL_EMPLOYE(),
            emp.DATE_EMBAUCHE().toString("yyyy-MM-dd")
        };

        // 🧩 Calcul dynamique de la hauteur de ligne selon le contenu le plus haut
        rowHeight = 0;
        for (int j = 0; j < data.size(); ++j) {
            QTextDocument doc;
            doc.setDefaultFont(dataFont);
            doc.setTextWidth(colWidths[j] - 12);
            doc.setPlainText(data[j]);
            rowHeight = std::max(rowHeight, int(doc.size().height()) + 10);
        }

        // 🟦 Alternance de couleur des lignes
        painter.setBrush((i % 2 == 0) ? QColor("#f8f9fa") : QColor("#ffffff"));
        painter.setPen(Qt::NoPen);
        painter.drawRect(margin, y, tableWidth, rowHeight);

        // 🖋️ Dessin du texte avec retour à la ligne et centrage
        x = margin;
        painter.setPen(Qt::black);
        for (int j = 0; j < data.size(); ++j) {
            QRect cellRect(x, y, colWidths[j], rowHeight);

            QTextDocument doc;
            doc.setDefaultFont(dataFont);
            doc.setTextWidth(colWidths[j] - 12);
            doc.setPlainText(data[j]);

            painter.save();
            painter.translate(cellRect.left() + 6, cellRect.top() + (rowHeight - doc.size().height()) / 2);
            doc.drawContents(&painter, QRectF(0, 0, colWidths[j] - 12, doc.size().height()));
            painter.restore();

            x += colWidths[j];
        }

        // 🧍 Bordure inférieure
        painter.setPen(borderPen);
        painter.drawLine(margin, y + rowHeight, margin + tableWidth, y + rowHeight);

        y += rowHeight;

        // 🧾 Nouvelle page si dépassement
        if (y + rowHeight > pageHeight - margin) {
            writer.newPage();
            y = startY;

            // 🔁 Redessine l'en-tête
            painter.setBrush(QColor("#007bff"));
            painter.setPen(Qt::NoPen);
            painter.drawRect(margin, y, tableWidth, 50);
            painter.setPen(Qt::white);
            x = margin;
            for (int k = 0; k < headers.size(); ++k) {
                painter.drawText(QRect(x, y, colWidths[k], 50), Qt::AlignCenter, headers[k]);
                x += colWidths[k];
            }
            y += 50;
        }
    }

    painter.end();
    QMessageBox::information(this, "Succès",
                             QString("Le fichier '%1' a été généré avec succès !").arg(fileName));
}



void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    if (index == 0) {
        std::sort(m_employes.begin(), m_employes.end(), [](const Employe& a, const Employe& b) { return a.POSTE() < b.POSTE(); });
    } else if (index == 1) {
        std::sort(m_employes.begin(), m_employes.end(), [](const Employe& a, const Employe& b) { return a.ID_EMPLOYE() < b.ID_EMPLOYE(); });
    }
    updateTable();
}

void MainWindow::on_tableWidget_itemSelectionChanged()
{
    // Si plus rien n’est sélectionné → retour au mode "ajouter"
    if (ui->tableWidget->selectedItems().isEmpty()) {
        m_isModifyMode = false;
        ui->pushButton->setText("Ajouter");
        clearForm(); // facultatif : efface le formulaire quand on désélectionne
        return;
    }

    // Sinon → on charge les infos pour modification
    int row = ui->tableWidget->currentRow();
    if (row < 0 || row >= m_employes.size())
        return;

    const Employe& emp = m_employes[row];
    ui->lineEdit->setText(QString::number(emp.ID_EMPLOYE()));
    ui->lineEdit_2->setText(emp.NOM_EMPLOYE());
    ui->lineEdit_3->setText(emp.PRENOM());
    ui->lineEdit_4->setText(emp.POSTE());
    ui->lineEdit_5->setText(emp.MOT_DE_PASSEE());
    ui->lineEdit_6->setText(emp.TEL_EMPLOYE().remove("+216"));
    ui->dateEdit->setDate(emp.DATE_EMBAUCHE());

    m_isModifyMode = true;
    ui->pushButton->setText("Modifier");
}

void MainWindow::updateTable(const QList<Employe>& employesToShow)
{
    if (!ui->tableWidget) return;
    const QList<Employe>& list = employesToShow.isEmpty() ? m_employes : employesToShow;
    ui->tableWidget->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        const Employe& emp = list[i];
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(emp.ID_EMPLOYE())));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(emp.NOM_EMPLOYE()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(emp.PRENOM()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(emp.POSTE()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(emp.MOT_DE_PASSEE()));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(emp.TEL_EMPLOYE()));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(emp.DATE_EMBAUCHE().toString("yyyy-MM-dd")));
    }
}
void MainWindow::updateStats()
{
    if (!statsTable || m_employes.isEmpty())
        return;

    int total = m_employes.size();
    statsTable->setRowCount(4);
    statsTable->setItem(0, 0, new QTableWidgetItem("Total employés"));
    statsTable->setItem(0, 1, new QTableWidgetItem(QString::number(total)));

    double totalSeniority = 0;
    for (const Employe &emp : m_employes)
        totalSeniority += emp.DATE_EMBAUCHE().daysTo(QDate::currentDate()) / 365.25;

    double avgSeniority = total > 0 ? totalSeniority / total : 0;

    statsTable->setItem(1, 0, new QTableWidgetItem("Ancienneté moyenne (ans)"));
    statsTable->setItem(1, 1, new QTableWidgetItem(QString::number(avgSeniority, 'f', 1)));

    QMap<QString, int> countByPoste;
    for (const Employe &emp : m_employes)
        countByPoste[emp.POSTE()]++;

    QPieSeries *series = new QPieSeries();
    for (auto it = countByPoste.constBegin(); it != countByPoste.constEnd(); ++it)
        series->append(it.key(), it.value());

    series->setLabelsVisible(true);
    series->setLabelsPosition(QPieSlice::LabelOutside);

    m_pieChart->removeAllSeries();
    m_pieChart->addSeries(series);
    m_pieChart->legend()->setVisible(true);
    m_pieChart->legend()->setAlignment(Qt::AlignBottom);

    statsTable->setItem(2, 0, new QTableWidgetItem("Postes différents"));
    statsTable->setItem(2, 1, new QTableWidgetItem(QString::number(countByPoste.size())));

    statsTable->setItem(3, 0, new QTableWidgetItem("Dernière mise à jour"));
    statsTable->setItem(3, 1, new QTableWidgetItem(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm")));
}

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

void MainWindow::showError(const QString& message)
{
    QMessageBox::warning(this, "Erreur", message);
}
