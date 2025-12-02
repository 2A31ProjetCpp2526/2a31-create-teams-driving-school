#include "arduino.h"
#include "mainwindow.h"
#include "seance.h"
#include "vehicule.h"
#include "circuit.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include "notification.h"
#include "moniteurstat.h"
#include "voicechat.h"
#include "voicechatdialog.h"
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QFileDialog>
#include <QTextStream>
#include <QStandardPaths>
#include <QDateTime>
#include <QTableView>
#include <QStringConverter>
#include <QFile>
#include <QAbstractItemModel>
#include <QTableWidgetItem>
#include <QRegularExpression>
#include <QComboBox>
#include <QVariant>
#include <QMetaType>
#include <algorithm>
#include "client.h"
#include <QPdfWriter>
#include <QPainter>
#include <QDir>
#include "qrcode.h"
#include "clientmapwidget.h"
#include <QLabel>
#include <QDialog>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QDesktopServices>
#include <QUrl>
#include <QProcess>
#include <QFile>
#include <QDebug>
#include <QChart>
#include <QChartView>
#include <QTimer>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QVBoxLayout>
#include <QTextDocument>//a
#include <QPainter>//a
#include <QPdfWriter>//a
#include <QDir>//a
#include <QPageSize>//
#include <QPushButton>//
#include <QGridLayout>//
#include <QVBoxLayout>//
#include <QHBoxLayout>//
#include <QTextEdit>//
#include <QDialog>//
#include <QSqlRecord>//
#include <QList>//
#include <QPrinter>//
#include <QTextDocument>//
#include <QDesktopServices>//
#include <QtCharts/QPieSeries>//
#include <QtCharts/QPieSlice>//
#include <QtCharts/QChartView>//
#include <QtCharts/QChart>//
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_isModifyModeEmploye(false)
{
    ui->setupUi(this);
    // Arduino debug dock (visible so user can see incoming messages)
    m_arduinoDock = new QDockWidget(tr("Arduino Debug"), this);
    QWidget *dockContainer = new QWidget(m_arduinoDock);
    QVBoxLayout *dockLayout = new QVBoxLayout(dockContainer);
    QHBoxLayout *topLayout = new QHBoxLayout();
    QLabel *lbl = new QLabel(tr("Port COM:"), dockContainer);
    m_comCombo = new QComboBox(dockContainer);
    m_refreshBtn = new QPushButton(tr("Refresh"), dockContainer);
    m_comBtn = new QPushButton(tr("Force COM"), dockContainer);
    topLayout->addWidget(lbl);
    topLayout->addWidget(m_comCombo);
    topLayout->addWidget(m_refreshBtn);
    topLayout->addWidget(m_comBtn);
    dockLayout->addLayout(topLayout);
    m_arduinoDebug = new QTextEdit(dockContainer);
    m_arduinoDebug->setReadOnly(true);
    dockLayout->addWidget(m_arduinoDebug);
    m_arduinoDock->setWidget(dockContainer);
    addDockWidget(Qt::BottomDockWidgetArea, m_arduinoDock);
    connect(m_comBtn, &QPushButton::clicked, this, &MainWindow::on_forceComBtn_clicked);
    connect(m_refreshBtn, &QPushButton::clicked, this, &MainWindow::on_refreshComBtn_clicked);

    // Populate COM combo initially
    on_refreshComBtn_clicked();
    // Initialiser Arduino
    int ret = A.connect_arduino();  // A est ton objet Arduino
    switch(ret){
        case 0: qDebug() << "Arduino is available and connected to:" << A.getarduino_port_name(); break;
        case 1: qDebug() << "Arduino is available but not connected to:" << A.getarduino_port_name(); break;
        case -1: qDebug() << "Arduino is not available"; break;
    }

    if (m_arduinoDebug) m_arduinoDebug->append(QString("Connect result: %1, port=%2").arg(ret).arg(A.getarduino_port_name()));

    // connecter readyRead à update_label
    QObject::connect(A.getserial(),SIGNAL(readyRead()),this,SLOT(update_label())); // permet de lancer
    //le slot update_label suite à la reception du signal readyRead (reception des données).


    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/shell-1x.png"));   // Mets n'importe quel icône de ton projet
    trayIcon->setVisible(true);

    // Ensure export buttons are connected (some UI names differ between forms/builds)
    const QStringList exportButtonNames = {"exportPdfBtn", "btnExporterPDF", "pushButtonExporter", "pushButton_29"};
    for (const QString &name : exportButtonNames) {
        QPushButton *b = this->findChild<QPushButton*>(name);
        if (b) connect(b, &QPushButton::clicked, this, &MainWindow::on_exportPdfBtn_clicked);
    }

    // Initialiser le widget de carte client (GPS offline)
    QWidget *mapContainer = this->findChild<QWidget*>("clientMapContainer");
    if (mapContainer) {
        QVBoxLayout *layout = new QVBoxLayout(mapContainer);
        layout->setContentsMargins(0, 0, 0, 0);
        m_clientMapWidget = new ClientMapWidget(mapContainer);
        layout->addWidget(m_clientMapWidget);

        // Charger explicitement l'image de fond de la carte (chemin absolu fourni par l'utilisateur)
        // Remarque : Qt accepte les slashs "/" même sous Windows.
        m_clientMapWidget->setBackgroundImage("C:/Users/XVI/Desktop/Client_circuit_vehicule_seance_employe--MONITEUR/Client_circuit_vehicule_seance_employe--MONITEUR/map_background.png");

        m_clientMapWidget->setClientInfo("", "", 0.0, 0.0, false);
    }

    // Initialisation de la base de données
    if (Connection::instance()->createConnect()) {
        QSqlQuery query;

        // Insertion de données de test pour séances
        query.exec("INSERT INTO SEANCE (ID_SEANCE, IMMATRICULE, ID_CLIENT, ID_MONITEUR, DATE_SEANCE, HEURE_DEBUT, TYPE, APPAREIL, DUREE) "
                   "SELECT 1, '123TU1234', 1, 1, SYSDATE, '08:00', 'code', 'PC', 2 FROM DUAL "
                   "WHERE NOT EXISTS (SELECT 1 FROM SEANCE WHERE ID_SEANCE = 1)");

        query.exec("INSERT INTO SEANCE (ID_SEANCE, IMMATRICULE, ID_CLIENT, ID_MONITEUR, DATE_SEANCE, HEURE_DEBUT, TYPE, APPAREIL, DUREE) "
                   "SELECT 2, '200TU5678', 2, 2, SYSDATE, '10:00', 'conduite', 'Voiture', 1 FROM DUAL "
                   "WHERE NOT EXISTS (SELECT 1 FROM SEANCE WHERE ID_SEANCE = 2)");

        // Insertion de données de test pour véhicules
        query.exec("INSERT INTO VEHICULE (IMMATRICULE, MODELE, TYPE_VEHICULE, TRANSMISSION, ETAT, DATE_ASSURANCE) "
                   "SELECT '123TU1234', 'Peugeot 208', 'Voiture', 'Manuelle', 'Disponible', TO_DATE('01/01/2025', 'DD/MM/YYYY') FROM DUAL "
                   "WHERE NOT EXISTS (SELECT 1 FROM VEHICULE WHERE IMMATRICULE = '123TU1234')");

        query.exec("INSERT INTO VEHICULE (IMMATRICULE, MODELE, TYPE_VEHICULE, TRANSMISSION, ETAT, DATE_ASSURANCE) "
                   "SELECT '200TU5678', 'Renault Clio', 'Voiture', 'Automatique', 'En maintenance', TO_DATE('01/06/2024', 'DD/MM/YYYY') FROM DUAL "
                   "WHERE NOT EXISTS (SELECT 1 FROM VEHICULE WHERE IMMATRICULE = '200TU5678')");

        // Insertion de données de test pour circuits
        query.exec("INSERT INTO CIRCUIT (ID_CIRCUIT, NOM_CIRCUIT, DESCRIPTION, DISTANCE, DUREE_ESTIMEE, DIFFICULTE, IMMATRICULE) "
                   "SELECT 1, 'Circuit Ville', 'Parcours en centre-ville', 15.5, 45, 'Facile', '123TU1234' FROM DUAL "
                   "WHERE NOT EXISTS (SELECT 1 FROM CIRCUIT WHERE ID_CIRCUIT = 1)");

        query.exec("INSERT INTO CIRCUIT (ID_CIRCUIT, NOM_CIRCUIT, DESCRIPTION, DISTANCE, DUREE_ESTIMEE, DIFFICULTE, IMMATRICULE) "
                   "SELECT 2, 'Circuit Montagne', 'Parcours en altitude', 35.2, 90, 'Difficile', '200TU5678' FROM DUAL "
                   "WHERE NOT EXISTS (SELECT 1 FROM CIRCUIT WHERE ID_CIRCUIT = 2)");

        refreshTableSeance();
        refreshTableVehicule();
        refreshTableClient();
        refreshTableCircuit();

        // Initialisation de la base de données pour les employés
        if (initDatabaseEmploye()) {
            loadEmployes();
            refreshTableEmploye();
        }

        // Insertion de données de test pour moniteurs
        query.exec("INSERT INTO MONITEUR (ID_MONITEUR, CIN_MONITEUR, NOM_MONITEUR, PRENOM_MONITEUR, TEL_MONITEUR, EMAIL, PERMIS) "
                   "SELECT '1', '12345678', 'Ben Ali', 'Mohamed', '98765432', 'benali@email.com', 'B' FROM DUAL "
                   "WHERE NOT EXISTS (SELECT 1 FROM MONITEUR WHERE ID_MONITEUR = '1')");

        query.exec("INSERT INTO MONITEUR (ID_MONITEUR, CIN_MONITEUR, NOM_MONITEUR, PRENOM_MONITEUR, TEL_MONITEUR, EMAIL, PERMIS) "
                   "SELECT '2', '87654321', 'Khalil', 'Fatima', '92345678', 'khalil@email.com', 'B' FROM DUAL "
                   "WHERE NOT EXISTS (SELECT 1 FROM MONITEUR WHERE ID_MONITEUR = '2')");

        refreshTableMoniteur();

        QMessageBox::information(this, "Succès", "Base de données initialisée avec succès!");
    } else {
        QMessageBox::critical(this, "Erreur", "Connexion à la base de données échouée!");
    }

    // Connexions pour la navigation entre pages
    connect(ui->seance, &QPushButton::clicked, this, &MainWindow::seanceclick);
    connect(ui->employe, &QPushButton::clicked, this, &MainWindow::employeclick);
    connect(ui->pushButton_exporterVehicule, &QPushButton::clicked,
                this, &MainWindow::on_pushButton_exporterVehicule_clicked);
    connect(ui->client, &QPushButton::clicked, this, &MainWindow::clientclick);
    connect(ui->vehicule, &QPushButton::clicked, this, &MainWindow::vehiculeclick);
    connect(ui->moniteur, &QPushButton::clicked, this, &MainWindow::moniteurclick);
    connect(ui->cercuit, &QPushButton::clicked, this, &MainWindow::cercuitclick);
    connect(ui->calendrier, &QPushButton::clicked, this, &MainWindow::calendrierclick);
    // Connect to tab change event for statistics (Client page uses tabWidget_3)
    if (ui->tabWidget_3) {
        connect(ui->tabWidget_3, &QTabWidget::currentChanged, this, [this](int index) {
            QString tabName = ui->tabWidget_3->tabText(index);
            qDebug() << "Tab changed to:" << index << "-" << tabName;
            if (tabName == "Statistiques" || tabName.contains("Stat", Qt::CaseInsensitive)) {
                qDebug() << "Statistiques tab activated, calling afficherStatistiqueClient()";
                afficherStatistiqueClient();
            }
        });
    }

    // Protéger l'accès au Chatbot pour les rôles restreints (ex: employé)
    if (ui->contratsTabWidget) {
        connect(ui->contratsTabWidget, &QTabWidget::currentChanged, this, [this](int index) {
            QString tabName = ui->contratsTabWidget->tabText(index);
            if (m_userRole.toLower().contains("employe") && (tabName.contains("Chatbot", Qt::CaseInsensitive) || tabName.contains("Chat"))) {
                showAccessDenied("Chatbot");
                // Revenir à l'onglet d'accueil (index 0 est généralement l'affichage)
                ui->contratsTabWidget->setCurrentIndex(0);
            }
        });
    }

    // Connexion pour le tableau de recherche des circuits
    connect(ui->tableView_circuits_recherche, &QTableView::clicked, this, &MainWindow::on_tableView_circuits_recherche_clicked);

    // Connexion pour le tableau des moniteurs (Affichage)
    connect(ui->moniteurTable, QOverload<int, int>::of(&QTableWidget::cellClicked), this, &MainWindow::on_moniteurTable_cellClicked);

    // Connexions pour notification, voicechat et statistiques moniteur
    connect(ui->notification, &QPushButton::clicked, this, &MainWindow::on_notification_clicked);
    connect(ui->voicechat, &QPushButton::clicked, this, &MainWindow::on_voicechat_clicked);
    connect(ui->stat, &QPushButton::clicked, this, &MainWindow::on_stat_clicked);
    // Connect export PDF for moniteurs if present
    if (ui->exportPdfBtnMoniteur) connect(ui->exportPdfBtnMoniteur, &QPushButton::clicked, this, &MainWindow::on_exportPdfBtnMoniteur_clicked);
    // Connect sort button for moniteurs
    if (ui->sortBtn) connect(ui->sortBtn, &QPushButton::clicked, this, &MainWindow::on_sortBtn_clicked);

    // Configuration des pages
    for (int i = 1; i <= 4; ++i) {
        QWidget *page = new QWidget();
        QLabel *label = new QLabel("Page " + QString::number(i) + " - Contenu à ajouter", page);
        label->setGeometry(100, 100, 300, 50);
        ui->stack->addWidget(page);
    }
    setupCalendrier();
    ui->stack->setCurrentIndex(0);

    // Removed - layout will be initialized when afficherStatistiquesEnfants is called



}
//arduino

// Fonction pour initialiser Arduino

void MainWindow::update_label()
{
    QByteArray data = A.read_from_arduino();
    if(data.isEmpty()) return;

    QString msg = QString::fromUtf8(data).trimmed();
    qDebug() << "Message Arduino :" << msg;
    if (m_arduinoDebug) m_arduinoDebug->append(msg);

    // Extraire la touche envoyée par l'Arduino.
    // Stratégie : parcourir la chaîne depuis la fin et choisir le premier caractère utile rencontré,
    // en donnant la priorité aux chiffres. Cela évite de capturer des lettres dans les messages
    // de debug (par ex. "Scan actif...") ou des caractères accentués.
    QString keyChar;
    for (int i = msg.length() - 1; i >= 0; --i) {
        QChar c = msg.at(i);
        if (c.isSpace()) continue;
        // priorité aux chiffres
        if (c.isDigit()) { keyChar = c; break; }
        // accepter *, #
        if (c == '*' || c == '#') { keyChar = c; break; }
        // accepter A-D ou a-d (touches spéciales du keypad)
        QChar up = c.toUpper();
        if (up == 'A' || up == 'B' || up == 'C' || up == 'D') { keyChar = up; break; }
        // sinon continuer à chercher vers la gauche
    }
    if (!keyChar.isEmpty()) {
        qDebug() << "Key extracted from Arduino message:" << keyChar;
        if (m_arduinoDebug) m_arduinoDebug->append(QString("Key extracted: %1").arg(keyChar));
    }

    // Gestion des commandes textuelles existantes (CODE, COND, PARC)
    QString upper = msg.toUpper();
    if (upper.contains("CODE")) {
        selectionnerComboBox("CODE");
        return;
    } else if (upper.contains("COND")) {
        selectionnerComboBox("COND");
        return;
    } else if (upper.contains("PARC")) {
        selectionnerComboBox("PARC");
        return;
    }

    // Si le message contient des chiffres du keypad, insérer dans le champ téléphone
    // Champ employé : ui->lineEdit_29 (stocke uniquement les chiffres, le préfixe +216 est en label séparé)
    if (!ui->lineEdit_29) return;

    // Ne rien insérer sauf si le champ téléphone a le focus ou si l'onglet "Ajouter employés" est actif
    bool allowInput = ui->lineEdit_29->hasFocus();
    if (!allowInput) {
        // si le projet utilise `tabWidget_2` pour les employés et que l'index 0 est l'onglet d'ajout
        if (ui->tabWidget_2 && ui->tabWidget_2->currentIndex() == 0) allowInput = true;
    }
    if (!allowInput) return;

    // Normaliser: prendre seulement les caractères utiles (0-9, *, #, B/D pour backspace)
    if (!keyChar.isEmpty()) {
        // traiter un seul caractère envoyé (ex: "1" ou "D")
        QChar c = keyChar.at(0);
        if (c.isDigit()) {
            QString cur = ui->lineEdit_29->text();
            if (cur.length() < 8) {
                ui->lineEdit_29->setText(cur + c);
                qDebug() << "Inserted digit from keypad:" << c << "->" << ui->lineEdit_29->text();
                if (m_arduinoDebug) m_arduinoDebug->append(QString("Inserted digit: %1 -> %2").arg(c).arg(ui->lineEdit_29->text()));
            }
        } else if (c == 'B' || c == 'b' || c == '\b' || c == 'D' || c == 'd') {
            QString cur = ui->lineEdit_29->text();
            if (!cur.isEmpty()) cur.chop(1);
            ui->lineEdit_29->setText(cur);
            qDebug() << "Backspace from keypad ->" << ui->lineEdit_29->text();
            if (m_arduinoDebug) m_arduinoDebug->append(QString("Backspace -> %1").arg(ui->lineEdit_29->text()));
        } else if (c == '*') {
            ui->lineEdit_29->clear();
            qDebug() << "Clear from keypad";
            if (m_arduinoDebug) m_arduinoDebug->append("Clear field from keypad");
        } else if (c == '#') {
            // Submit
            if (ui->stack->currentIndex() == 0 || (ui->tabWidget_2 && ui->tabWidget_2->currentIndex() == 0)) {
                if (ui->pushButton_26 && ui->pushButton_26->isEnabled())
                    QMetaObject::invokeMethod(ui->pushButton_26, "click", Qt::QueuedConnection);
                if (m_arduinoDebug) m_arduinoDebug->append("Submit requested via keypad (#)");
            }
        }
        return;
    }

    // Fallback: si aucune extraction, parcourir la chaîne entière et prendre les digits seuls
    for (QChar c : msg) {
        if (c.isDigit()) {
            // Limiter à 8 chiffres (format local tunisien après +216)
            QString cur = ui->lineEdit_29->text();
            if (cur.length() < 8) {
                ui->lineEdit_29->setText(cur + c);
            }
        } else if (c == 'B' || c == 'b' || c == '\b' || c == 'D' || c == 'd') {
            // Backspace demandé depuis l'Arduino (Keypad 4x4 envoie souvent 'D' pour la touche de droite)
            QString cur = ui->lineEdit_29->text();
            if (!cur.isEmpty()) cur.chop(1);
            ui->lineEdit_29->setText(cur);
        } else if (c == '*') {
            // Clear field
            ui->lineEdit_29->clear();
        } else if (c == '#') {
            // Optionnel: submit/trigger add button when # reçu
            // Si le focus est sur la zone des employés, déclencher l'ajout
            if (ui->stack->currentIndex() == 0 || ui->tabWidget_2->currentIndex() == 0) {
                // Appuyer sur le bouton Ajouter si disponible
                if (ui->pushButton_26 && ui->pushButton_26->isEnabled())
                    QMetaObject::invokeMethod(ui->pushButton_26, "click", Qt::QueuedConnection);
            }
        }
    }
}

void MainWindow::selectionnerComboBox(const QString &type)
{
    int index = ui->comboBox_type->findText(type, Qt::MatchFixedString);
    if(index != -1){
        ui->comboBox_type->setCurrentIndex(index);
        qDebug() << "ComboBox sélectionné :" << ui->comboBox_type->currentText();
    } else {
        qDebug() << "⚠️ Type non trouvé dans comboBox :" << type;
    }
}



void MainWindow::lireDonneesArduino() {
    if (!arduinoSerial || !arduinoSerial->isOpen())
        return;

    QByteArray data = arduinoSerial->readAll();
    if (data.isEmpty()) return;

    QString msg = QString::fromUtf8(data);

    qDebug() << "📩 Message reçu :" << msg;

    // Normalisation (minuscule pour éviter les problèmes)
    QString clean = msg.toLower();

    if (clean.contains("CODE"))
        traiterSelectionArduino("CODE");

    if (clean.contains("COND"))
        traiterSelectionArduino("COND");

    if (clean.contains("PARC"))
        traiterSelectionArduino("PARC");
}

void MainWindow::on_forceComBtn_clicked()
{
    QString port;
    if (m_comCombo && m_comCombo->currentText().trimmed().isEmpty() == false) {
        port = m_comCombo->currentText().trimmed();
    } else if (m_comLine) {
        port = m_comLine->text().trimmed();
    }
    if (port.isEmpty()) {
        if (m_arduinoDebug) m_arduinoDebug->append("No port entered");
        return;
    }

    QSerialPort *s = A.getserial();
    if (!s) {
        if (m_arduinoDebug) m_arduinoDebug->append("Arduino serial pointer is null");
        return;
    }

    if (s->isOpen()) s->close();
    s->setPortName(port);
    if (s->open(QSerialPort::ReadWrite)) {
        s->setBaudRate(QSerialPort::Baud9600);
        s->setDataBits(QSerialPort::Data8);
        s->setParity(QSerialPort::NoParity);
        s->setStopBits(QSerialPort::OneStop);
        s->setFlowControl(QSerialPort::NoFlowControl);
        if (m_arduinoDebug) m_arduinoDebug->append(QString("Forced open port %1").arg(port));
        qDebug() << "Forced open port" << port;
    } else {
        if (m_arduinoDebug) m_arduinoDebug->append(QString("Failed to open forced port %1").arg(port));
        qDebug() << "Failed to open forced port" << port << s->error();
    }
}

void MainWindow::on_refreshComBtn_clicked()
{
    if (!m_comCombo) return;
    m_comCombo->clear();
    const auto ports = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : ports) {
        m_comCombo->addItem(info.portName());
    }
    if (m_arduinoDebug) m_arduinoDebug->append(QString("Found %1 serial port(s)").arg(ports.count()));
}


// Fonction pour traiter la sélection Arduino
void MainWindow::traiterSelectionArduino(const QString &type) {
    int index = ui->comboBox_type->findText(type, Qt::MatchContains | Qt::MatchCaseSensitive);
    if (index == -1) {
        // essayer en ignorant la casse
        index = ui->comboBox_type->findText(type, Qt::MatchContains | Qt::MatchFixedString | Qt::MatchCaseSensitive);
    }

    if (index != -1) {
        ui->comboBox_type->setCurrentIndex(index);
        qDebug() << "ComboBox selectionné:" << ui->comboBox_type->currentText();
    } else {
        qDebug() << "Erreur: texte non trouvé dans comboBox:" << type;
    }
}



// Constructeur qui accepte le rôle de l'utilisateur et délègue à l'autre constructeur
MainWindow::MainWindow(const QString &userRole, QWidget *parent)
    : MainWindow(parent)
{
    m_userRole = userRole;
    applyRolePermissions();
}

void MainWindow::applyRolePermissions()
{
    QString role = m_userRole.toLower();

    // Si l'utilisateur est un moniteur, limiter l'accès aux sections Moniteur et Séances
    if (role.contains("moniteur")) {
        // Ne pas masquer les boutons mais ajouter un tooltip indiquant l'accès restreint
        QString tip = "Accès restreint: vous n'êtes pas autorisé à accéder à cette section.";
        if (ui->employe) ui->employe->setToolTip(tip);
        if (ui->client) ui->client->setToolTip(tip);
        if (ui->vehicule) ui->vehicule->setToolTip(tip);
        if (ui->cercuit) ui->cercuit->setToolTip(tip);
        if (ui->calendrier) ui->calendrier->setToolTip(tip);
        if (ui->seance) ui->seance->setToolTip("Accès: Séances");
        if (ui->moniteur) ui->moniteur->setToolTip("Accès: Moniteur");
        // Aller directement sur la page Séances par défaut
        ui->stack->setCurrentIndex(0);

        // Additionally, disable specific tabs inside the Seances area (Statistiques, Autres Métiers)
        if (ui->tabWidget) {
            for (int i = 0; i < ui->tabWidget->count(); ++i) {
                QString t = ui->tabWidget->tabText(i);
                if (t.compare("Statistiques", Qt::CaseInsensitive) == 0 || t.compare("Autres Métiers", Qt::CaseInsensitive) == 0) {
                    ui->tabWidget->setTabEnabled(i, false);
                }
            }
        }

        // Disable Moniteur-specific actions: ajout / modifier / supprimer / exporter / notification / chatbot
        if (ui->addContratBtn) ui->addContratBtn->setEnabled(false);
        if (ui->updateContratBtn) ui->updateContratBtn->setEnabled(false);
        if (ui->deleteContratBtn) ui->deleteContratBtn->setEnabled(false);

        // Disable notification/send button (mail) if present
        if (ui->pushButton_envoyer) ui->pushButton_envoyer->setEnabled(false);

        // Disable chatbot tab(s) inside contratsTabWidget if present
        if (ui->contratsTabWidget) {
            for (int i = 0; i < ui->contratsTabWidget->count(); ++i) {
                QString t = ui->contratsTabWidget->tabText(i);
                if (t.contains("Chatbot", Qt::CaseInsensitive) || t.contains("Chat", Qt::CaseInsensitive)) {
                    ui->contratsTabWidget->setTabEnabled(i, false);
                }
            }
        }

        // Disable common export buttons (names vary across UI versions)
        const QStringList exportButtonNames = {"exportPdfBtn", "btnExporterPDF", "pushButtonExporter", "pushButton_29", "pushButton_export", "pushButton_export_circuit"};
        for (const QString &name : exportButtonNames) {
            QPushButton *b = this->findChild<QPushButton*>(name);
            if (b) b->setEnabled(false);
        }

        // Disable seance modify/delete UI buttons if present
        if (ui->pushButton_modifier) ui->pushButton_modifier->setEnabled(false);
        if (ui->pushButton_supprimer) ui->pushButton_supprimer->setEnabled(false);
    }

    // Additional safety: scan all buttons and tabwidgets and disable action-like controls
    if (role.contains("moniteur")) {
        // Whitelist navigation buttons that must remain enabled
        const QStringList navWhitelist = {"seance", "employe", "client", "vehicule", "moniteur", "cercuit", "calendrier", "employe_2"};

        // Disable buttons whose object name or text suggests add/modify/delete/export/chat/notify
        const QStringList actionKeywords = {"add", "ajout", "ajouter", "supprimer", "delete", "del", "modifier", "update", "export", "envoyer", "chat", "notif", "notification"};

        QList<QPushButton*> allButtons = this->findChildren<QPushButton*>();
        for (QPushButton *b : allButtons) {
            if (!b) continue;
            QString obj = b->objectName().toLower();
            QString text = b->text().toLower();
            if (navWhitelist.contains(obj)) continue;

            bool shouldDisable = false;
            for (const QString &kw : actionKeywords) {
                if (obj.contains(kw) || text.contains(kw)) { shouldDisable = true; break; }
            }
            if (shouldDisable) b->setEnabled(false);
        }

        // Also disable any tab whose text indicates Chatbot, Statistiques, Autres Métiers, Ajout or Notification across all tab widgets
        QList<QTabWidget*> allTabs = this->findChildren<QTabWidget*>();
        for (QTabWidget *tw : allTabs) {
            if (!tw) continue;
            for (int i = 0; i < tw->count(); ++i) {
                QString t = tw->tabText(i).toLower();
                if (t.contains("chat") || t.contains("stat") || t.contains("metier") || t.contains("autres") || t.contains("ajout") || t.contains("notification")) {
                    tw->setTabEnabled(i, false);
                }
            }

            // Intercept clicks on tabs so we can show an error message when a moniteur attempts to open Ajout/Notification
            QTabBar *bar = tw->tabBar();
            if (bar) {
                connect(bar, &QTabBar::tabBarClicked, this, [this, tw](int index) {
                    QString t = tw->tabText(index).toLower();
                    if (t.contains("ajout") || t.contains("notification")) {
                        showAccessDenied("Accès restreint : " + tw->tabText(index));
                        // revert to previous tab
                        int prev = tw->currentIndex();
                        QTimer::singleShot(0, [tw, prev]() { tw->setCurrentIndex(prev); });
                    }
                });
            }
        }
    }
    // Si l'utilisateur est un employé, restreindre certaines sections (statistiques, autres métiers, chatbot)
    if (role.contains("employe")) {
        QString tip = "Accès restreint: vous n'êtes pas autorisé à accéder à cette section.";
        // Garder les éléments visibles mais indiquer qu'ils sont restreints
        if (ui->tabWidget_3) ui->tabWidget_3->setToolTip(tip); // statistiques (client page)
        if (ui->contratsTabWidget) ui->contratsTabWidget->setToolTip(tip); // inclut Chatbot
        if (ui->pushButton_statistiques) ui->pushButton_statistiques->setToolTip(tip);
        // Aller sur la page d'accueil par défaut
        ui->stack->setCurrentIndex(0);
    }
    // Si le rôle est secrétaire (ou autre), garder l'accès complet (ne rien masquer)
}

// ==================== NAVIGATION ====================
void MainWindow::seanceclick() {
    ui->stack->setCurrentIndex(0);
    refreshTableSeance();
}
void MainWindow::calendrierclick() { ui->stack->setCurrentIndex(5);
    if (!canAccess("calendrier")) { showAccessDenied("Calendrier"); return; }
    updateCalendrier();
}
void MainWindow::vehiculeclick(){
    if (!canAccess("vehicule")) { showAccessDenied("Véhicule"); return; }
    ui->stack->setCurrentIndex(3);
    refreshTableVehicule();
}

void MainWindow::employeclick() {
    if (!canAccess("employe")) { showAccessDenied("Employé"); return; }
    ui->stack->setCurrentIndex(1);
    refreshTableEmploye();
    // Update employee statistics whenever we open the employee section
    afficherStatistiqueEmploye();
}

void MainWindow::clientclick() {
    if (!canAccess("client")) { showAccessDenied("Client"); return; }
    ui->stack->setCurrentIndex(2);
    refreshTableClient();
    // Initialiser le comboBox de tri à ID par défaut
    if (ui->comboBoxTrier) {
        ui->comboBoxTrier->setCurrentIndex(0);
    }
    // Statistics will be loaded when user clicks on Statistique tab
}

void MainWindow::cercuitclick() {
    // Trouver l'index de la page circuit dans le stack
    if (!canAccess("circuit")) { showAccessDenied("Circuit"); return; }
    for (int i = 0; i < ui->stack->count(); i++) {
        QWidget *widget = ui->stack->widget(i);
        if (widget && widget->objectName() == "page_circuit") {
            ui->stack->setCurrentIndex(i);
            refreshTableCircuit();
            return;
        }
    }
    // Si la page n'est pas trouvée, utiliser l'index 4 par défaut
    ui->stack->setCurrentIndex(4);
    refreshTableCircuit();
}

void MainWindow::moniteurclick() {
    ui->stack->setCurrentIndex(4);
    refreshTableMoniteur();
}

bool MainWindow::canAccess(const QString &section) const
{
    QString role = m_userRole.toLower();
    if (role.contains("moniteur")) {
        QString s = section.toLower();
        if (s == "seance" || s == "moniteur") return true;
        return false;
    }
    // Employé: empêcher l'accès à certaines sections (statistiques, autres metiers, chatbot)
    if (role.contains("employe")) {
        QString s = section.toLower();
        if (s == "statistiques" || s == "stat" || s == "autres_metiers" || s == "metiers" || s == "chatbot") return false;
        return true;
    }
    return true;
}

void MainWindow::showAccessDenied(const QString &section) const
{
    QMessageBox::warning(const_cast<MainWindow*>(this), "Accès refusé",
                         QString("Vous n'avez pas accès à la section %1.").arg(section));
}

bool MainWindow::canPerformAction(const QString &action) const
{
    QString role = m_userRole.toLower();

    // Moniteur: accès en lecture seulement sur Séances et Moniteur
    if (role.contains("moniteur")) {
        // actions autorisées pour moniteur (lecture seule)
        QStringList allowed = {"view_seance", "view_moniteur", "open_seance", "open_moniteur"};
        if (allowed.contains(action)) return true;
        return false;
    }

    // Employé: lecture seulement + pas d'ajout/modification/suppression/export/chatbot/statistiques
    if (role.contains("employe")) {
        QString a = action.toLower();
        // autoriser uniquement les actions de view/open
        if (a.startsWith("view_") || a.startsWith("open_")) return true;
        // refuser actions d'ajout/modif/suppression/export/statistiques/chatbot
        if (a.startsWith("add_") || a.startsWith("modify_") || a.startsWith("delete_") || a.contains("export") || a == "chatbot" || a == "statistics") return false;
        // Par défaut, refuser les actions inconnues
        return false;
    }

    // autres rôles: tout autorisé
    return true;
}

// ==================== GESTION SÉANCES ====================
void MainWindow::refreshTableSeance()
{
    QSqlQueryModel *model = Seance::afficher();
    ui->tableView_seances->setModel(model);
}

void MainWindow::clearFieldsSeance()
{
    ui->lineEdit_id->clear();
    ui->dateEdit_date->setDate(QDate::currentDate());
    ui->timeEdit_heure->setTime(QTime::currentTime());
    ui->lineEdit_duree->clear();
    ui->comboBox_type->setCurrentIndex(0);
    ui->lineEdit_appareil->clear();
    ui->lineEdit_id_client->clear();
    ui->lineEdit_immatricule->clear();
    ui->lineEdit_id_moniteur->clear();
}

void MainWindow::on_pushButton_valider_clicked()
{
    if (!canPerformAction("add_seance")) { showAccessDenied("Ajouter séance"); return; }
    int id = ui->lineEdit_id->text().toInt();
    QDate date = ui->dateEdit_date->date();
    QTime heure = ui->timeEdit_heure->time();
    int duree = ui->lineEdit_duree->text().toInt();
    QString type = ui->comboBox_type->currentText();
    QString appareil = ui->lineEdit_appareil->text();
    int id_client = ui->lineEdit_id_client->text().toInt();
    QString immatricule = ui->lineEdit_immatricule->text();
    int id_moniteur = ui->lineEdit_id_moniteur->text().toInt();

    // Validation des champs
    if (id <= 0) {
        QMessageBox::warning(this, "Attention", "ID séance doit être supérieur à 0!");
        return;
    }
    if (duree <= 0) {
        QMessageBox::warning(this, "Attention", "Durée doit être supérieure à 0!");
        return;
    }
    if (appareil.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Appareil ne peut pas être vide!");
        return;
    }
    if (id_client <= 0) {
        QMessageBox::warning(this, "Attention", "ID client doit être supérieur à 0!");
        return;
    }
    if (immatricule.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Immatricule ne peut pas être vide!");
        return;
    }
    if (id_moniteur <= 0) {
        QMessageBox::warning(this, "Attention", "ID moniteur doit être supérieur à 0!");
        return;
    }

    // Vérifier si l'ID existe déjà
    if (Seance::existeDeja(id)) {
        QMessageBox::warning(this, "Attention", "Cet ID existe déjà! Veuillez utiliser un ID différent.");
        return;
    }

    Seance seance(id, date, heure, duree, type, appareil, id_client, immatricule, id_moniteur);

    if (seance.ajouter()) {

        // --- 1) Afficher message sur LCD ---
        QString msgID = QString::number(id);
        A.write_to_arduino(msgID.toUtf8() + "\n");

        // --- 2) Effacer LCD après 3 secondes ---
        QTimer::singleShot(10000, this, [=]() {
            A.write_to_arduino("CLEAR_LCD\n");
        });

        QMessageBox::information(this, "Succès", "Séance ajoutée avec succès!");
        refreshTableSeance();
        clearFieldsSeance();
    }


     else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout!");
    }
}

void MainWindow::on_pushButton_modifier_clicked()
{
    if (!canPerformAction("modify_seance")) { showAccessDenied("Modifier séance"); return; }
    int id = ui->lineEdit_id->text().toInt();
    if (id <= 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner une séance à modifier!");
        return;
    }

    QDate date = ui->dateEdit_date->date();
    QTime heure = ui->timeEdit_heure->time();
    int duree = ui->lineEdit_duree->text().toInt();
    QString type = ui->comboBox_type->currentText();
    QString appareil = ui->lineEdit_appareil->text();
    int id_client = ui->lineEdit_id_client->text().toInt();
    QString immatricule = ui->lineEdit_immatricule->text();
    int id_moniteur = ui->lineEdit_id_moniteur->text().toInt();

    // Validation
    if (duree <= 0 || appareil.isEmpty() || id_client <= 0 || immatricule.isEmpty() || id_moniteur <= 0) {
        QMessageBox::warning(this, "Attention", "Veuillez remplir tous les champs correctement!");
        return;
    }

    Seance seance(id, date, heure, duree, type, appareil, id_client, immatricule, id_moniteur);

    if (seance.modifier()) {
        QMessageBox::information(this, "Succès", "Séance modifiée avec succès!");
        refreshTableSeance();
        clearFieldsSeance();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification!");
    }
}

void MainWindow::on_pushButton_supprimer_clicked()
{
    qDebug() << "=== BOUTON SUPPRIMER CLIQUE ===";

    if (!canPerformAction("delete_seance")) { showAccessDenied("Supprimer séance"); return; }

    int id = ui->lineEdit_id->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Attention", "⚠️ Veuillez sélectionner une séance à supprimer !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation",
                              "Voulez-vous vraiment supprimer la séance ID " + QString::number(id) + " ?",
                              QMessageBox::Yes | QMessageBox::No)
        == QMessageBox::Yes)
    {
        Seance s;
        if (s.supprimer(id)) {
            QMessageBox::information(this, "Succès", "✅ Séance supprimée avec succès !");
            //refreshTable();
            //clearFields();
            updateCalendrier();
        } else {
            QMessageBox::critical(this, "Erreur", "❌ Erreur lors de la suppression !");
        }
    }
}
void MainWindow::on_pushButton_recherche_clicked()
{
    QString txt = ui->lineEdit_recherche->text().trimmed();
    if (txt.isEmpty())
        return;

    // Le tableau est déjà filtré → on récupère le modèle actuel
    QAbstractItemModel *model = ui->tableView_seances->model();

    for (int i = 0; i < model->rowCount(); i++) {
        QString type = model->index(i, 6).data().toString(); // Colonne TYPE
        QString immatricule = model->index(i, 1).data().toString(); // Colonne IMMATRICULE
        QString appareil = model->index(i, 7).data().toString(); // Colonne APPAREIL

        if (type.startsWith(txt, Qt::CaseInsensitive) ||
            immatricule.startsWith(txt, Qt::CaseInsensitive) ||
            appareil.startsWith(txt, Qt::CaseInsensitive)) {
            ui->tableView_seances->selectRow(i);
            break;
        }
    }
    // Empêche le reload automatique
    isClearingSearch = true;
    ui->lineEdit_recherche->clear();
    isClearingSearch = false;
}

void MainWindow::on_lineEdit_recherche_textChanged(const QString &text)
{
    if (isClearingSearch) return; // NE RIEN FAIRE quand c'est le bouton qui efface

    if (text.isEmpty()) {
        ui->tableView_seances->setModel(Seance::afficher());
        return;
    }

    ui->tableView_seances->setModel(Seance::rechercher(text));
}

void MainWindow::on_comboBox_trier_currentTextChanged(const QString &text)
{
    if (text.isEmpty()) {
        qDebug() << "⚠️ Texte vide dans le comboBox";
        return;
    }

    qDebug() << "🎯 Tri demandé:" << text;

    // Utiliser directement le texte du ComboBox
    QSqlQueryModel* model = Seance::trier(text);

    if (model) {
        ui->tableView_seances->setModel(model);
        qDebug() << "✅ Modèle appliqué au tableView";

        // Message de statut
        statusBar()->showMessage("Trié par: " + text, 3000);
    } else {
        qDebug() << "❌ Modèle null après tri";
    }
}

void MainWindow::on_pushButton_effacer_clicked()
{
    //clearFields();
}

void MainWindow::on_pushButton_rafraichir_clicked()
{
    //refreshTable();
}


void MainWindow::on_pushButton_export_clicked()
{
    if (!canPerformAction("export_seance")) { showAccessDenied("Exporter séances"); return; }
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter les séances en PDF",
                                                    QDir::homePath() + "/liste_seances.pdf",
                                                    "Fichiers PDF (*.pdf)");

    if (fileName.isEmpty()) return;

    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }

    // Récupérer le modèle actuel (avec tri/recherche appliqué)
    QSqlQueryModel* model = qobject_cast<QSqlQueryModel*>(ui->tableView_seances->model());

    if (!model || model->rowCount() == 0) {
        QMessageBox::information(this, "Export PDF", "Aucune séance à exporter !");
        return;
    }

    // Générer le HTML à partir du modèle
    QString html = genererHTMLFromModel(model);

    QTextDocument document;
    document.setHtml(html);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(fileName);
    printer.setPageSize(QPageSize(QPageSize::A4));
    //printer.setPageOrientation(QPageSize::Landscape);
    printer.setPageMargins(QMarginsF(10, 10, 10, 10));

    document.print(&printer);

    QMessageBox::information(this, "✅ Succès",
                             QString("PDF exporté!\n\nFichier: %1\nSéances: %2").arg(fileName).arg(model->rowCount()));
}

QString MainWindow::genererHTMLFromModel(QSqlQueryModel* model)
{
    QString html = R"(
    <html>
    <head>
    <style>
        body { font-family: Arial, sans-serif; margin: 15px; }
        h1 { color: #2c3e50; text-align: center; }
        table { width: 100%; border-collapse: collapse; margin-top: 15px; }
        th { background-color: #3498db; color: white; padding: 10px; }
        td { padding: 8px; border-bottom: 1px solid #ddd; }
        tr:nth-child(even) { background-color: #f9f9f9; }
    </style>
    </head>
    <body>
    <h1>RoadMaster - Séances</h1>
    <table>
    )";

    // En-têtes
    html += "<tr>";
    for (int col = 0; col < model->columnCount(); ++col) {
        QString header = model->headerData(col, Qt::Horizontal).toString();
        html += "<th>" + header + "</th>";
    }
    html += "</tr>";

    // Données
    for (int row = 0; row < model->rowCount(); ++row) {
        html += "<tr>";
        for (int col = 0; col < model->columnCount(); ++col) {
            QModelIndex index = model->index(row, col);
            QString data = model->data(index).toString();
            html += "<td>" + data + "</td>";
        }
        html += "</tr>";
    }

    html += "</table></body></html>";
    return html;
}
void MainWindow::on_pushButton_statistiques_clicked()
{
    if (!canPerformAction("statistics")) { showAccessDenied("Statistiques"); return; }
    // Créer une nouvelle fenêtre pour les statistiques
    QWidget *statsWindow = new QWidget();
    statsWindow->setWindowTitle("🚗 Statistiques des Séances par Type");
    statsWindow->setMinimumSize(900, 700);
    statsWindow->setStyleSheet("background-color: #f8f9fa;");

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(statsWindow);

    // Titre
    QLabel *title = new QLabel("📊 STATISTIQUES DES SÉANCES PAR TYPE");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #2c3e50; padding: 20px; background-color: white; border-radius: 10px; margin: 10px;");
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // Récupérer les statistiques
    QMap<QString, int> stats = Seance::getStatistiquesParType();
    int total = 0;
    for (int count : stats) {
        total += count;
    }

    if (total == 0) {
        QLabel *noDataLabel = new QLabel("Aucune séance dans la base de données !");
        noDataLabel->setStyleSheet("font-size: 18px; color: #7f8c8d; padding: 50px; background-color: white; border-radius: 10px; margin: 10px;");
        noDataLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(noDataLabel);
    } else {
        // Widget pour graphique + légende
        QHBoxLayout *chartLayout = new QHBoxLayout();

        // ==================== CAMEMBERT ====================
        QPieSeries *series = new QPieSeries();

        // Couleurs pour les types de séances
        QList<QColor> couleurs = {
            QColor(41, 128, 185),   // Bleu - Conduite
            QColor(39, 174, 96),    // Vert - Code
            QColor(231, 76, 60),    // Rouge - Examen
            QColor(155, 89, 182),   // Violet - Perfectionnement
            QColor(243, 156, 18),   // Orange - Recyclage
            QColor(52, 73, 94)      // Gris foncé - Autre
        };

        int colorIndex = 0;
        for (auto it = stats.begin(); it != stats.end(); ++it) {
            QString type = it.key();
            int count = it.value();
            double percentage = (count * 100.0) / total;

            // Créer la tranche
            QPieSlice *slice = series->append(type, count);
            slice->setColor(couleurs[colorIndex % couleurs.size()]);
            slice->setLabelVisible(true);
            slice->setExploded(false);

            // Formater le label
            QString label = QString("%1\n%2 séances\n%3%")
                                .arg(type)
                                .arg(count)
                                .arg(QString::number(percentage, 'f', 1));
            slice->setLabel(label);
            slice->setLabelArmLengthFactor(0.1);

            colorIndex++;
        }

        // Configurer le graphique
        QChart *chart = new QChart();
        chart->addSeries(series);
        chart->setTitle("Répartition des Séances par Type");
        chart->setTitleFont(QFont("Arial", 14, QFont::Bold));
        chart->setAnimationOptions(QChart::AllAnimations);
        chart->setBackgroundBrush(QBrush(QColor(248, 249, 250)));
        chart->legend()->setVisible(false); // On cache la légende car on va la créer manuellement

        QChartView *chartView = new QChartView(chart);
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->setFixedSize(500, 400);

        // ==================== LÉGENDE DÉTAILLÉE ====================
        QWidget *legendWidget = new QWidget();
        legendWidget->setFixedWidth(300);
        QVBoxLayout *legendLayout = new QVBoxLayout(legendWidget);

        QLabel *legendTitle = new QLabel("📋 DÉTAIL PAR TYPE");
        legendTitle->setStyleSheet("font-size: 16px; font-weight: bold; color: #2c3e50; padding: 10px;");
        legendLayout->addWidget(legendTitle);

        colorIndex = 0;
        for (auto it = stats.begin(); it != stats.end(); ++it) {
            QString type = it.key();
            int count = it.value();
            double percentage = (count * 100.0) / total;

            QHBoxLayout *itemLayout = new QHBoxLayout();

            // Carré de couleur
            QLabel *colorLabel = new QLabel();
            colorLabel->setFixedSize(20, 20);
            colorLabel->setStyleSheet(QString("background-color: %1; border-radius: 3px; border: 1px solid #ddd;")
                                          .arg(couleurs[colorIndex % couleurs.size()].name()));

            // Texte détaillé
            QLabel *textLabel = new QLabel(
                QString("<b>%1</b><br>%2 séances (%3%)")
                    .arg(type)
                    .arg(count)
                    .arg(QString::number(percentage, 'f', 1))
                );
            textLabel->setStyleSheet("font-size: 12px; color: #2c3e50;");
            textLabel->setWordWrap(true);

            itemLayout->addWidget(colorLabel);
            itemLayout->addWidget(textLabel);
            itemLayout->addStretch();

            legendLayout->addLayout(itemLayout);
            colorIndex++;
        }

        // Ajouter graphique et légende au layout horizontal
        chartLayout->addWidget(chartView);
        chartLayout->addWidget(legendWidget);

        mainLayout->addLayout(chartLayout);

        // ==================== STATISTIQUES GÉNÉRALES ====================
        QLabel *totalLabel = new QLabel(
            QString("📈 TOTAL GÉNÉRAL: <b>%1 séances</b> réparties sur <b>%2 types</b> différents")
                .arg(total)
                .arg(stats.size())
            );
        totalLabel->setStyleSheet("font-size: 16px; color: #27ae60; background-color: white; padding: 15px; border-radius: 10px; margin: 10px;");
        totalLabel->setAlignment(Qt::AlignCenter);
        mainLayout->addWidget(totalLabel);
    }

    // ==================== BOUTON FERMER ====================
    QPushButton *closeButton = new QPushButton("🗙 Fermer");
    closeButton->setFixedSize(120, 45);
    closeButton->setStyleSheet(
        "QPushButton {"
        "background-color: #e74c3c;"
        "color: white;"
        "border: none;"
        "border-radius: 8px;"
        "font-size: 14px;"
        "font-weight: bold;"
        "padding: 10px;"
        "}"
        "QPushButton:hover {"
        "background-color: #c0392b;"
        "}"
        );
    connect(closeButton, &QPushButton::clicked, statsWindow, &QWidget::close);

    mainLayout->addWidget(closeButton, 0, Qt::AlignCenter);

    // Afficher la fenêtre
    statsWindow->show();
}

void MainWindow::on_tableView_seances_clicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    int id = index.sibling(index.row(), 0).data().toInt();
    QString immatricule = index.sibling(index.row(), 1).data().toString();
    int id_client = index.sibling(index.row(), 2).data().toInt();
    int id_moniteur = index.sibling(index.row(), 3).data().toInt();
    QDate date = index.sibling(index.row(), 4).data().toDate();
    QString heure_str = index.sibling(index.row(), 5).data().toString();
    QString type = index.sibling(index.row(), 6).data().toString();
    QString appareil = index.sibling(index.row(), 7).data().toString();
    int duree = index.sibling(index.row(), 8).data().toInt();

    QTime heure = QTime::fromString(heure_str, "HH:mm");

    ui->lineEdit_id->setText(QString::number(id));
    ui->dateEdit_date->setDate(date);
    ui->timeEdit_heure->setTime(heure);
    ui->lineEdit_duree->setText(QString::number(duree));

    int indexType = ui->comboBox_type->findText(type);
    if (indexType != -1)
        ui->comboBox_type->setCurrentIndex(indexType);

    ui->lineEdit_appareil->setText(appareil);
    ui->lineEdit_id_client->setText(QString::number(id_client));
    ui->lineEdit_immatricule->setText(immatricule);
    ui->lineEdit_id_moniteur->setText(QString::number(id_moniteur));
}

void MainWindow::on_pushButton_envoyer_clicked()
{
    qDebug() << "=== BOUTON ENVOYER RAPPEL CLIQUE ===";

    int idSeance = ui->lineEdit_id_2->text().toInt();

    if (idSeance <= 0) {
        QMessageBox::warning(this, "Attention", "⚠️ Veuillez sélectionner une séance à rappeler !");
        return;
    }

    qDebug() << "ID Séance saisi:" << idSeance;

    // VÉRIFICATION RAPIDE DE LA SÉANCE
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM SEANCE WHERE ID_SEANCE = :id");
    checkQuery.bindValue(":id", idSeance);

    if (!checkQuery.exec() || !checkQuery.next() || checkQuery.value(0).toInt() == 0) {
        QMessageBox::warning(this, "Erreur", "❌ Cette séance n'existe pas !");
        return;
    }

    /// RÉCUPÉRATION DES INFOS POUR CONFIRMATION - CORRECTION: COLUMN8 et NOM_CLIENT/PRENOM_CLIENT
    QSqlQuery query;
    query.prepare("SELECT s.ID_CLIENT, s.DATE_SEANCE, s.HEURE_DEBUT, c.NOM_CLIENT, c.PRENOM_CLIENT, c.COLUMN8 as EMAIL "
                  "FROM SEANCE s "
                  "LEFT JOIN CLIENT c ON s.ID_CLIENT = c.ID_CLIENT "
                  "WHERE s.ID_SEANCE = :id");
    query.bindValue(":id", idSeance);

    int idClient = 0;
    QString clientInfo = "Client inconnu";
    QString dateInfo = "Date inconnue";
    QString email = "Email non trouvé";

    if (query.exec()) {
        if (query.next()) {
            idClient = query.value("ID_CLIENT").toInt();

            // Vérifier si le client existe dans la jointure
            if (query.value("NOM_CLIENT").isNull()) {
                qDebug() << "❌ Client non trouvé pour ID:" << idClient;
                QMessageBox::critical(this, "Erreur",
                                      "❌ Client non trouvé !\n\n"
                                      "ID Client: " + QString::number(idClient) + "\n"
                                                                        "La séance existe mais le client associé n'a pas été trouvé.");
                return;
            }

            QString nom = query.value("NOM_CLIENT").toString();
            QString prenom = query.value("PRENOM_CLIENT").toString();
            email = query.value("EMAIL").toString();

            QDate dateSeance = query.value("DATE_SEANCE").toDate();
            QTime heureSeance = query.value("HEURE_DEBUT").toTime();

            // Construction des infos avec vérifications
            if (!nom.isEmpty() || !prenom.isEmpty()) {
                clientInfo = prenom + " " + nom + " (ID: " + QString::number(idClient) + ")";
            } else {
                clientInfo = "Client ID: " + QString::number(idClient);
            }

            if (dateSeance.isValid() && heureSeance.isValid()) {
                dateInfo = dateSeance.toString("dd/MM/yyyy") + " à " + heureSeance.toString("HH:mm");
            }

            qDebug() << "✅ Infos récupérées - Client:" << clientInfo << "Email:" << email;

            // VÉRIFICATION EMAIL IMMÉDIATE
            if (email.isEmpty() || email.isNull() || email == "(null)" || email == "NULL") {
                qDebug() << "❌ Email vide pour client ID:" << idClient;
                QMessageBox::warning(this, "Email manquant",
                                     "❌ Aucun email trouvé pour le client de cette séance !\n\n"
                                     "ID Client: " + QString::number(idClient) + "\n"
                                                                       "Client: " + clientInfo + "\n\n"
                                                        "Veuillez vérifier que l'email est renseigné dans la table CLIENT (colonne COLUMN8).");
                return;
            }

        } else {
            qDebug() << "❌ Séance non trouvée ID:" << idSeance;
            QMessageBox::critical(this, "Erreur", "❌ Cette séance n'existe pas !");
            return;
        }
    } else {
        qDebug() << "❌ Erreur requête:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur",
                              "❌ Erreur lors de la récupération des données !\n\n"
                              "Détails: " + query.lastError().text());
        return;
    }

    // Si on arrive ici, toutes les vérifications sont passées
    qDebug() << "✅ Toutes les vérifications passées - Email:" << email;

    // VÉRIFICATION EMAIL
    if (email.isEmpty() || email.isNull() || email == "Email non trouvé") {
        QMessageBox::warning(this, "Email manquant",
                             "❌ Aucun email trouvé pour le client de cette séance !\n\n"
                             "ID Client: " + QString::number(idClient) + "\n"
                                                               "Client: " + clientInfo + "\n\n"
                                                "Veuillez vérifier que l'email est renseigné dans la table CLIENT (colonne COLUMN8).");
        return;
    }

    // CONFIRMATION D'ENVOI
    QString confirmMsg = QString("Voulez-vous envoyer un rappel pour :\n\n"
                                 "🔹 ID Séance: %1\n"
                                 "📧 Destinataire: %2\n"
                                 "👤 Client: %3\n"
                                 "📅 Séance: %4\n").arg(idSeance).arg(email).arg(clientInfo).arg(dateInfo);

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirmation d'envoi",
        confirmMsg,
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
        return;
    }

    // Afficher message d'information pendant l'envoi
    QMessageBox::information(this, "Envoi en cours",
                             "📧 Envoi du rappel email en cours...\n\n"
                             "Veuillez patienter quelques secondes.");

    // Appeler la méthode d'envoi
    if (Seance::envoyerRappelEmail(idSeance)) {
        QMessageBox::information(this, "Succès",
                                 "✅ Rappel envoyé avec succès !\n\n"
                                 "📧 Destinataire: " + email + "\n"
                                 "👤 Client: " + clientInfo + "\n"
                                 "📅 Séance: " + dateInfo + "\n\n"
                                 "Un log a été sauvegardé dans 'emails_log.txt'");
    } else {
        QMessageBox::critical(this, "Erreur",
                              "❌ Erreur lors de l'envoi du rappel !");
    }
}


// ============================================================================
// CALENDRIER - IMPLÉMENTATION
// ============================================================================

void MainWindow::setupCalendrier()
{
    m_dateCalendrier = QDate::currentDate();

    connect(ui->pushButton_mois_precedent, &QPushButton::clicked, this, &MainWindow::onMoisPrecedent);
    connect(ui->pushButton_mois_suivant, &QPushButton::clicked, this, &MainWindow::onMoisSuivant);
    connect(ui->pushButton_rafraichir_calendrier, &QPushButton::clicked, this, &MainWindow::onAfficherCalendrier);

    updateCalendrier();
}

void MainWindow::updateCalendrier()
{
    QString moisAnnee = m_dateCalendrier.toString("MMMM yyyy");
    ui->label_mois_annee->setText(moisAnnee);

    marquerDatesAvecSeances();

    QLayoutItem* child;
    while ((child = ui->gridLayout_calendrier->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }

    QStringList jours = {"Lun", "Mar", "Mer", "Jeu", "Ven", "Sam", "Dim"};
    for (int i = 0; i < 7; ++i) {
        QLabel* label = new QLabel(jours[i]);
        label->setAlignment(Qt::AlignCenter);
        label->setStyleSheet("QLabel { background-color: #3498db; color: white; font-weight: bold; padding: 10px; border: 1px solid #2980b9; border-radius: 5px; }");
        ui->gridLayout_calendrier->addWidget(label, 0, i);
    }

    QDate premierJour(m_dateCalendrier.year(), m_dateCalendrier.month(), 1);
    int jourSemaine = premierJour.dayOfWeek() - 1;
    if (jourSemaine < 0) jourSemaine = 6;

    int ligne = 1;
    int colonne = jourSemaine;

    for (int jour = 1; jour <= m_dateCalendrier.daysInMonth(); ++jour) {
        QDate dateCourante(m_dateCalendrier.year(), m_dateCalendrier.month(), jour);

        QPushButton* btnJour = new QPushButton(QString::number(jour));
        btnJour->setMinimumSize(50, 50);
        btnJour->setProperty("date", dateCourante);

        QString style = "QPushButton { background-color: white; border: 2px solid #bdc3c7; border-radius: 8px; font-weight: bold; font-size: 14px; }"
                        "QPushButton:hover { background-color: #ecf0f1; border-color: #3498db; }";

        if (m_seancesParDate.contains(dateCourante) && m_seancesParDate[dateCourante] > 0) {
            style = "QPushButton { background-color: #e74c3c; color: white; border: 2px solid #c0392b; border-radius: 8px; font-weight: bold; font-size: 14px; }"
                    "QPushButton:hover { background-color: #c0392b; }";
            btnJour->setToolTip(QString("%1 séance(s) programmée(s)").arg(m_seancesParDate[dateCourante]));
        }

        if (dateCourante == QDate::currentDate()) {
            style = "QPushButton { background-color: #3498db; color: white; border: 2px solid #2980b9; border-radius: 8px; font-weight: bold; font-size: 14px; }"
                    "QPushButton:hover { background-color: #2980b9; }";
        }

        btnJour->setStyleSheet(style);

        connect(btnJour, &QPushButton::clicked, [this, dateCourante]() {
            onCalendrierDateClicked(dateCourante);
        });

        ui->gridLayout_calendrier->addWidget(btnJour, ligne, colonne);

        colonne++;
        if (colonne > 6) {
            colonne = 0;
            ligne++;
        }
    }
}

void MainWindow::marquerDatesAvecSeances()
{
    m_seancesParDate = Seance::getSeancesParMois(m_dateCalendrier.month(), m_dateCalendrier.year());
}

void MainWindow::onCalendrierDateClicked(const QDate &date)
{
    afficherDetailsDate(date);
}

void MainWindow::afficherDetailsDate(const QDate &date)
{
    QList<Seance> seances = Seance::getSeancesParDate(date);

    if (seances.isEmpty()) {
        QMessageBox::information(this, "Calendrier",
                                 QString("📅 %1\n\n❌ Aucune séance programmée pour cette date.")
                                     .arg(date.toString("dd/MM/yyyy")));
    } else {
        QString message = QString("📅 %1\n\n✅ Séances programmées:\n\n").arg(date.toString("dd/MM/yyyy"));

        for (const Seance& s : seances) {
            message += QString("🔹 **Séance ID: %1**\n"
                               "   ⏰ Heure: %2\n"
                               "   🕐 Durée: %3 min\n"
                               "   📋 Type: %4\n"
                               "   🚗 Appareil: %5\n"
                               "   👤 Client ID: %6\n"
                               "   👨‍🏫 Moniteur ID: %7\n\n")
                           .arg(s.getId())
                           .arg(s.getHeure().toString("HH:mm"))
                           .arg(s.getDuree())
                           .arg(s.getType())
                           .arg(s.getAppareil())
                           .arg(s.getIdClient())
                           .arg(s.getIdMoniteur());
        }

        QDialog dialog(this);
        dialog.setWindowTitle("🚗 RoadMaster - Détails des Séances");
        dialog.setFixedSize(500, 400);
        dialog.setStyleSheet("QDialog { background-color: #f8f9fa; }");

        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        QLabel *titleLabel = new QLabel("Détails des Séances");
        titleLabel->setStyleSheet("QLabel { color: #2c3e50; font-size: 18px; font-weight: bold; padding: 10px; }");
        titleLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(titleLabel);

        QTextEdit *textEdit = new QTextEdit();
        textEdit->setPlainText(message);
        textEdit->setReadOnly(true);
        textEdit->setStyleSheet("QTextEdit { background-color: white; border: 2px solid #bdc3c7; border-radius: 8px; padding: 15px; font-size: 12px; }");

        QPushButton *btnFermer = new QPushButton("Fermer");
        btnFermer->setStyleSheet("QPushButton { background-color: #3498db; color: white; border: none; padding: 10px; border-radius: 5px; font-weight: bold; }"
                                 "QPushButton:hover { background-color: #2980b9; }");
        connect(btnFermer, &QPushButton::clicked, &dialog, &QDialog::accept);

        layout->addWidget(textEdit);
        layout->addWidget(btnFermer);

        dialog.exec();
    }
}

void MainWindow::onMoisPrecedent()
{
    m_dateCalendrier = m_dateCalendrier.addMonths(-1);
    updateCalendrier();
}

void MainWindow::onMoisSuivant()
{
    m_dateCalendrier = m_dateCalendrier.addMonths(1);
    updateCalendrier();
}

void MainWindow::onAfficherCalendrier()
{
    updateCalendrier();
}

// ==================== GESTION VÉHICULES ====================
void MainWindow::refreshTableVehicule()
{
    QSqlQueryModel *model = V.afficher();
    ui->tableView_vehicules->setModel(model);
}

void MainWindow::clearFieldsVehicule()
{
    ui->lineEdit_immatricule_2->clear();
    ui->lineEdit_modele->clear();
    ui->comboBox_type_2->setCurrentIndex(0);
    ui->comboBox_transmission->setCurrentIndex(0);
    ui->comboBox_etat->setCurrentIndex(0);
    ui->dateEdit_assurance->setDate(QDate::currentDate());
}

void MainWindow::on_pushButton_validervehicule_clicked()
{
    QString imm = ui->lineEdit_immatricule_2->text().trimmed();
    QString mod = ui->lineEdit_modele->text().trimmed();
    QString typ = ui->comboBox_type_2->currentText();
    QString trans = ui->comboBox_transmission->currentText();
    QString et = ui->comboBox_etat->currentText();
    QDate date = ui->dateEdit_assurance->date();

    // 🔹 Vérification champs vides
    if (imm.isEmpty()) {
        QMessageBox::warning(this, "Attention", "L'immatriculation ne peut pas être vide !");
        return;
    }
    if (mod.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Le modèle ne peut pas être vide !");
        return;
    }

    // 🔹 Vérification stricte : format "123TU4567"
    QRegularExpression regex("^[0-9]{1,4}TU[0-9]{1,4}$", QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch match = regex.match(imm);

    if (!match.hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie",
                             "Format d'immatriculation invalide !\n\n"
                             "⚠️ Exemple valide : 123TU4567\n"
                             "L'immatriculation doit contenir :\n"
                             "- 1 à 4 chiffres\n"
                             "- suivis de 'TU'\n"
                             "- suivis de 1 à 4 chiffres");
        ui->lineEdit_immatricule_2->setFocus();
        ui->lineEdit_immatricule_2->selectAll();
        return;
    }

    // 🔹 Si tout est correct → ajout du véhicule
    Vehicule v(imm.toUpper(), mod, typ, trans, et, date);
    bool test = v.ajouter();

    if (test) {
        QMessageBox::information(this, "Succès", "✅ Véhicule ajouté avec succès !");

        enregistrerActionVehicule(imm, "📅 Ajout du véhicule");
        refreshTableVehicule();
        updateAssuranceTable();
        updateAssuranceAlertPanel();
        updateMetiersBadge();
        clearFieldsVehicule();
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Échec de l'ajout. Vérifiez que l'immatriculation n'existe pas déjà.");
    }
}
void MainWindow::on_pushButton_supprimervehicule_clicked()
{
    QString id = ui->lineEdit_immatricule_2->text().trimmed();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un véhicule à supprimer!");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer ce véhicule?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool test = V.supprimer(id);
        if (test) {
            QMessageBox::information(this, "Succès", "Véhicule supprimé avec succès ✓");

            enregistrerActionVehicule(id, "❌ Suppression du véhicule");
            refreshTableVehicule();
            updateAssuranceTable();
            updateAssuranceAlertPanel();
            updateMetiersBadge();
            clearFieldsVehicule();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression ✗");
        }
    }
}

void MainWindow::on_pushButton_modifiervehicule_clicked()
{
    QString id = ui->lineEdit_immatricule_2->text().trimmed();
    QString mod = ui->lineEdit_modele->text().trimmed();
    QString typ = ui->comboBox_type_2->currentText();
    QString trans = ui->comboBox_transmission->currentText();
    QString et = ui->comboBox_etat->currentText();
    QDate date = ui->dateEdit_assurance->date();

    // Validation
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un véhicule à modifier!");
        return;
    }
    if (mod.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Modèle ne peut pas être vide!");
        return;
    }

    Vehicule v(id, mod, typ, trans, et, date);
    bool test = v.modifier(id);

    if (test) {
        QMessageBox::information(this, "Succès", "Modification effectuée ✓");

        enregistrerActionVehicule(id, "✏️ Modification du véhicule");
        refreshTableVehicule();
        updateAssuranceTable();
        updateAssuranceAlertPanel();
        updateMetiersBadge();
        clearFieldsVehicule();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification ✗");
    }
}


void MainWindow::on_pushButton_rechercheVehicule_clicked()
{
    QString txt = ui->lineEdit_rechercheVehicule->text().trimmed();
    if (txt.isEmpty())
        return;

    // Le tableau est déjà filtré → on récupère le modèle actuel
    QAbstractItemModel *model = ui->tableView_vehicules->model();

    for (int i = 0; i < model->rowCount(); i++) {
        QString imm = model->index(i, 0).data().toString();

        if (imm.startsWith(txt, Qt::CaseInsensitive)) {
            ui->tableView_vehicules->selectRow(i);
            break;
        }
    }
    // =============================
    // Remplir automatiquement "Ajouter un véhicule"
    // =============================

    QModelIndex firstIndex = ui->tableView_vehicules->model()->index(0,0);

    if (firstIndex.isValid())
    {
        QString immatricule = firstIndex.sibling(0,0).data().toString();
        QString modele      = firstIndex.sibling(0,1).data().toString();
        QString type        = firstIndex.sibling(0,2).data().toString();
        QString transmission = firstIndex.sibling(0,3).data().toString();
        QString etat         = firstIndex.sibling(0,4).data().toString();
        QString dateStr      = firstIndex.sibling(0,5).data().toString();

        QDate dateAssurance = QDate::fromString(dateStr, "dd/MM/yyyy");
        if (!dateAssurance.isValid())
            dateAssurance = QDate::currentDate();

        // Remplissage automatique
        ui->lineEdit_immatricule_2->setText(immatricule);
        ui->lineEdit_modele->setText(modele);
        ui->comboBox_type_2->setCurrentText(type);
        ui->comboBox_transmission->setCurrentText(transmission);
        ui->comboBox_etat->setCurrentText(etat);
        ui->dateEdit_assurance->setDate(dateAssurance);
    }
    // Empêche le reload automatique
    isClearingSearch = true;
    ui->lineEdit_rechercheVehicule->clear();
    isClearingSearch = false;


}



void MainWindow::on_lineEdit_rechercheVehicule_textChanged(const QString &text)
{
    if (isClearingSearch) return; // NE RIEN FAIRE quand c’est le bouton qui efface

    if (text.isEmpty()) {
        ui->tableView_vehicules->setModel(V.afficher());
        return;
    }

    ui->tableView_vehicules->setModel(V.rechercher(text));
}

void MainWindow::on_comboBox_trierVehicule_currentIndexChanged(int index)
{
    QString critere;

    switch (index) {
    case 0:
        critere = "IMMATRICULE";
        break;
    case 1:
        critere = "MODELE";
        break;
    case 2:
        critere = "TYPE_VEHICULE";
        break;
    default:
        critere = "IMMATRICULE";
        break;
    }

    QSqlQueryModel* model = V.trier(critere);
    ui->tableView_vehicules->setModel(model);
}

void MainWindow::on_pushButton_exporterVehicule_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Exporter en PDF",
        "",
        "Fichier PDF (*.pdf)"
        );

    if (filePath.isEmpty()) return;

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setResolution(300);

    QPainter painter(&writer);
    if (!painter.isActive()) return;

    // ============================
    // PARAMÈTRES DU TABLEAU
    // ============================

    int margin = 150;
    int tableWidth = writer.width() - 2 * margin;
    int rowHeight = 140;
    int headerHeight = 160;

    painter.setPen(Qt::black);

    // ============================
    // TITRE CENTRÉ
    // ============================

    painter.setFont(QFont("Arial", 20, QFont::Bold));

    QString titre = "Liste des véhicules";
    QRect titleRect(0, 100, writer.width(), 200);
    painter.drawText(titleRect, Qt::AlignCenter, titre);

    // ============================
    // RÉCUPÉRATION DU TABLEAU
    // ============================

    QAbstractItemModel *model = ui->tableView_vehicules->model();
    int cols = model->columnCount();
    int rows = model->rowCount();

    int colWidth = tableWidth / cols;

    painter.setFont(QFont("Arial", 11));

    int y = 400;
    int x = margin;

    // ============================
    // ENTIÈTES DU TABLEAU
    // ============================

    painter.setBrush(QColor(230,230,230));
    painter.drawRect(x, y, tableWidth, headerHeight);
    painter.setBrush(Qt::NoBrush);

    for (int col = 0; col < cols; col++)
    {
        QRect cell(x + col*colWidth, y, colWidth, headerHeight);
        painter.drawRect(cell);
        painter.drawText(cell.adjusted(10, 10, -10, -10),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         model->headerData(col, Qt::Horizontal).toString());
    }

    y += headerHeight;

    // ============================
    // LIGNES DU TABLEAU
    // ============================

    for (int row = 0; row < rows; row++)
    {
        for (int col = 0; col < cols; col++)
        {
            QRect cell(x + col*colWidth, y, colWidth, rowHeight);
            painter.drawRect(cell);

            QString val = model->data(model->index(row, col)).toString();

            painter.drawText(cell.adjusted(10, 10, -10, -10),
                             Qt::AlignLeft | Qt::AlignVCenter,
                             val);
        }
        y += rowHeight;

        // *Nouvelle page si nécessaire*
        if (y + rowHeight > writer.height() - 200)
        {
            writer.newPage();
            y = 200;
        }
    }

    painter.end();

    QMessageBox::information(this, "Succès", "Le PDF a été généré avec succès !");
}


void MainWindow::on_pushButton_statistiques_2_clicked()
{
    // 🔹 Crée un objet véhicule
    Vehicule v;

    // 🔹 Appelle la fonction statistiques combinées
    QChartView *chartView = v.statistiquesV();

    // Vérifie si le graphique est bien créé
    if (!chartView) {
        QMessageBox::warning(this, "Erreur", "Impossible d'afficher les statistiques !");
        return;
    }

    // 🔹 Crée une fenêtre pour afficher les deux graphiques
    QWidget *window = new QWidget();
    window->setWindowTitle("📊 Statistiques des véhicules");
    window->resize(1000, 500);

    // 🔹 Ajoute le graphique dans la fenêtre
    QVBoxLayout *layout = new QVBoxLayout(window);
    layout->addWidget(chartView);

    // 🔹 Style de fond
    window->setStyleSheet("background-color: #f0f5f9; font-family: Arial; font-size: 14px;");

    // 🔹 Affiche la fenêtre
    window->show();
}

void MainWindow::enregistrerActionVehicule(const QString &immat, const QString &action)
{
    QString filePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
    + "/historique_vehicules.txt";

    QFile file(filePath);
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream out(&file);
    QString date = QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss");

    out << "Immatricule: " << immat << "\n";
    out << "Action: " << action << "\n";
    out << "Date: " << date << "\n";
    out << "------------------------------\n";

    file.close();
}

void MainWindow::on_pushButton_historique_clicked()
{
    // 1. Chemin du fichier d'historique (toujours le même)
    const QString logPath =
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
        + "/historique_vehicules.txt";

    QFile logFile(logPath);

    // 2. Si le fichier n'existe pas, on le crée vide
    if (!logFile.exists()) {
        if (logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&logFile);
            out << "===== HISTORIQUE DES VEHICULES =====\n";
            logFile.close();
        } else {
            QMessageBox::critical(this, "Erreur",
                                  "Impossible de créer le fichier d'historique.");
            return;
        }
    }

    // 3. Demander où sauvegarder le fichier
    QString savePath = QFileDialog::getSaveFileName(
        this,
        "Exporter l'historique des véhicules",
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)
            + "/historique_vehicules.txt",
        "Fichiers texte (*.txt)");

    if (savePath.isEmpty())
        return;

    // 4) Si le fichier existe déjà à cet endroit → on l’écrase
    if (QFile::exists(savePath)) {
        QFile::remove(savePath);
    }

    // 5) Copier le fichier source vers la destination choisie
    if (!QFile::copy(logPath, savePath)) {
        QMessageBox::critical(this, "Erreur",
                              "Impossible de copier le fichier d'historique.\n\n"
                              "Source : " + logPath + "\n"
                                              "Destination : " + savePath);
        return;
    }

    QMessageBox::information(this, "Export terminé",
                             "L'historique a été exporté dans :\n" + savePath);
}

void MainWindow::updateAssuranceTable()
{
    QSqlQuery query;
    query.prepare("SELECT IMMATRICULE, DATE_ASSURENCE FROM VEHICULE");

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL",
                              "Impossible d'afficher les assurances :\n" +
                                  query.lastError().text());
        return;
    }

    ui->tableWidget_assurance->setRowCount(0);
    ui->tableWidget_assurance->setColumnCount(3);
    ui->tableWidget_assurance->setHorizontalHeaderLabels({"Immatricule","Date assurance","Statut"});

    QDate today = QDate::currentDate();
    int row = 0;

    while (query.next()) {
        QString immat = query.value(0).toString();
        QDate dateAssur = query.value(1).toDate();
        int daysLeft = today.daysTo(dateAssur);

        ui->tableWidget_assurance->insertRow(row);
        ui->tableWidget_assurance->setItem(row, 0, new QTableWidgetItem(immat));
        ui->tableWidget_assurance->setItem(row, 1, new QTableWidgetItem(dateAssur.toString("dd-MM-yyyy")));

        QString statut;
        if (daysLeft < 0)
            statut = "🔴 Expirée";
        else if (daysLeft <= 5)
            statut = "🟡 Bientôt (" + QString::number(daysLeft) + " j)";
        else
            statut = "🟢 Valide";

        ui->tableWidget_assurance->setItem(row, 2, new QTableWidgetItem(statut));
        row++;
    }

    ui->tableWidget_assurance->resizeColumnsToContents();
}




void MainWindow::updateAssuranceAlertPanel()
{
    int expired = 0;
    int soon = 0;

    int rows = ui->tableWidget_assurance->rowCount();

    for (int i = 0; i < rows; i++) {
        QString status = ui->tableWidget_assurance->item(i, 2)->text();

        if (status.contains("Expirée"))
            expired++;
        else if (status.contains("Bientôt"))
            soon++;
    }

    ui->label_expired->setText(QString("🔴 %1 assurances expirées").arg(expired));
    ui->label_soon->setText(QString("🟡 %1 assurances bientôt expirées").arg(soon));
}


void MainWindow::updateMetiersBadge()
{
    int expired = 0;
    int soon = 0;

    // Récupérer toutes les lignes du tableau d'assurance
    int rowCount = ui->tableWidget_assurance->rowCount();
    for (int i = 0; i < rowCount; i++) {
        QString status = ui->tableWidget_assurance->item(i, 2)->text();

        if (status.contains("Expirée"))
            expired++;
        else if (status.contains("Bientôt"))
            soon++;
    }

    // Préparer le texte de l’onglet
    QString title = "Métiers";

    if (expired > 0)
        title += QString("  🔴%1").arg(expired);
    else if (soon > 0)
        title += QString("  🟡%1").arg(soon);

    // Appliquer au tabWidget principal
    ui->tabWidget_4->setTabText(2, title);
}




void MainWindow::showAssuranceNotifications()
{
    QDate today = QDate::currentDate();
    int expiredCount = 0;
    int soonCount = 0;

    QStringList expiredList;
    QStringList soonList;

    int rows = ui->tableWidget_assurance->rowCount();

    for (int row = 0; row < rows; row++) {
        QString immat = ui->tableWidget_assurance->item(row, 0)->text();
        QString dateStr = ui->tableWidget_assurance->item(row, 1)->text();

        QDate assuranceDate = QDate::fromString(dateStr, "dd-MM-yyyy");
        if (!assuranceDate.isValid()) continue;

        int diff = today.daysTo(assuranceDate);

        if (diff < 0) {
            expiredCount++;
            expiredList << QString("%1 (le %2)").arg(immat, dateStr);
        }
        else if (diff <= 5) {
            soonCount++;
            soonList << QString("%1 (le %2, dans %3 jours)")
                            .arg(immat, dateStr, QString::number(diff));
        }
    }

    if (expiredCount > 0) {
        showNotification(
            "Assurances expirées",
            QString("Il y a %1 assurance(s) expirée(s) :\n%2")
                .arg(expiredCount)
                .arg(expiredList.join("\n"))

            );
    }

    if (soonCount > 0) {
        showNotification(
            "Assurances bientôt expirées",
            QString("Il y a %1 assurance(s) qui expirent bientôt :\n%2")
                .arg(soonCount)
                .arg(soonList.join("\n"))

            );
    }
}

void MainWindow::showNotification(const QString &title, const QString &message)
{
    if (!trayIcon) return;   // sécurité

    trayIcon->showMessage(
        title,
        message,
        QSystemTrayIcon::Information,
        5000   // durée 5 secondes
        );
}



void MainWindow::on_tabWidget_4_currentChanged(int index)
{
    // Onglet 0 = Accueil, 1 = Statistiques, 2 = Métiers
    // Vérifie bien que "Métiers" est le 3ème onglet (index 2)
    if (index == 2) { // Métiers
        updateAssuranceTable();        // Met à jour le tableau
        updateAssuranceAlertPanel();   // Met à jour les compteurs
        updateMetiersBadge();          // Met à jour le badge rouge
        showAssuranceNotifications();  // 🔔 Affiche les popups
    }
}


void MainWindow::on_tableView_vehicules_clicked(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    int col = index.column();

    QString immatricule = index.sibling(index.row(), 0).data().toString();

    // ✔️ 1) Historique du modèle
    if (col == 1)
    {
        Vehicule v;
        v.afficherHistorique(immatricule)->show();
        return;
    }

    // ✔️ 2) Remplissage du formulaire
    QString modele      = index.sibling(index.row(), 1).data().toString();
    QString type        = index.sibling(index.row(), 2).data().toString();
    QString transmission= index.sibling(index.row(), 3).data().toString();
    QString etat        = index.sibling(index.row(), 4).data().toString();
    QString dateStr     = index.sibling(index.row(), 5).data().toString();

    QDate dateAssurance = QDate::fromString(dateStr, "dd/MM/yyyy");
    if (!dateAssurance.isValid())
        dateAssurance = QDate::currentDate();

    ui->lineEdit_immatricule_2->setText(immatricule);
    ui->lineEdit_modele->setText(modele);
    ui->comboBox_type_2->setCurrentText(type);
    ui->comboBox_transmission->setCurrentText(transmission);
    ui->comboBox_etat->setCurrentText(etat);
    ui->dateEdit_assurance->setDate(dateAssurance);

    // ✔️ 3) Statut assurance
    if (col == 5)
    {
        QDate today = QDate::currentDate();
        int diff = today.daysTo(dateAssurance);
        QString statut;

        if (diff < 0)
            statut = "🔴 Assurance expirée";
        else if (diff <= 5)
            statut = "🟡 Assurance bientôt expirée (" + QString::number(diff) + " jours)";
        else
            statut = "🟢 Assurance valide";

        QMessageBox::information(this, "Statut assurance", statut);
    }
}

// ==================== GESTION CIRCUITS ====================
void MainWindow::refreshTableCircuit()
{
    QSqlQueryModel *model = Circuit::afficher();
    ui->tableView_circuits->setModel(model);
    // Mettre à jour aussi le tableau de recherche
    ui->tableView_circuits_recherche->setModel(model);
}

void MainWindow::clearFieldsCircuit()
{
    ui->lineEdit_id_circuit->clear();
    ui->lineEdit_nom_circuit->clear();
    ui->textEdit_desc_circuit->clear();
    ui->lineEdit_distance->clear();
    ui->lineEdit_duree_circuit->clear();
    ui->comboBox_difficulte->setCurrentIndex(0);
    ui->lineEdit_immat_circuit->clear();
}

void MainWindow::on_pushButton_validercircuit_clicked()
{
    // Récupération des valeurs
    QString idText = ui->lineEdit_id_circuit->text().trimmed();
    QString nom = ui->lineEdit_nom_circuit->text().trimmed();
    QString desc = ui->textEdit_desc_circuit->toPlainText().trimmed();
    QString distanceText = ui->lineEdit_distance->text().trimmed();
    QString dureeText = ui->lineEdit_duree_circuit->text().trimmed();
    QString difficulte = ui->comboBox_difficulte->currentText();
    QString immat = ui->lineEdit_immat_circuit->text().trimmed();

    // Permission
    if (!canPerformAction("add_circuit")) { showAccessDenied("Ajouter circuit"); return; }

    // Validation de l'ID du circuit
    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID du circuit est obligatoire!");
        ui->lineEdit_id_circuit->setFocus();
        return;
    }

    // Vérifier que l'ID contient uniquement des chiffres
    bool idOk = false;
    int id = idText.toInt(&idOk);
    if (!idOk) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID du circuit doit être un nombre (chiffres uniquement)!");
        ui->lineEdit_id_circuit->setFocus();
        ui->lineEdit_id_circuit->selectAll();
        return;
    }

    // Vérifier qu'il n'y a pas de caractères non numériques
    for (const QChar &c : idText) {
        if (!c.isDigit()) {
            QMessageBox::warning(this, "Erreur de saisie", "L'ID du circuit doit être un nombre (chiffres uniquement)!");
            ui->lineEdit_id_circuit->setFocus();
            ui->lineEdit_id_circuit->selectAll();
            return;
        }
    }

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID du circuit doit être un nombre positif!");
        ui->lineEdit_id_circuit->setFocus();
        ui->lineEdit_id_circuit->selectAll();
        return;
    }

    // Validation du nom du circuit
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le nom du circuit est obligatoire!");
        ui->lineEdit_nom_circuit->setFocus();
        return;
    }

    if (nom.length() < 2) {
        QMessageBox::warning(this, "Erreur de saisie", "Le nom du circuit doit contenir au moins 2 caractères!");
        ui->lineEdit_nom_circuit->setFocus();
        ui->lineEdit_nom_circuit->selectAll();
        return;
    }

    // Validation de la distance
    if (distanceText.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "La distance est obligatoire!");
        ui->lineEdit_distance->setFocus();
        return;
    }

    bool distanceOk = false;
    double distance = distanceText.toDouble(&distanceOk);
    if (!distanceOk) {
        QMessageBox::warning(this, "Erreur de saisie", "La distance doit être un nombre (ex: 10.5 ou 10)!");
        ui->lineEdit_distance->setFocus();
        ui->lineEdit_distance->selectAll();
        return;
    }

    // Vérifier qu'il n'y a que des chiffres et un point décimal
    bool hasDecimalPoint = false;
    for (const QChar &c : distanceText) {
        if (c == '.' || c == ',') {
            if (hasDecimalPoint) {
                QMessageBox::warning(this, "Erreur de saisie", "La distance doit être un nombre (ex: 10.5 ou 10)!");
                ui->lineEdit_distance->setFocus();
                ui->lineEdit_distance->selectAll();
                return;
            }
            hasDecimalPoint = true;
        } else if (!c.isDigit()) {
            QMessageBox::warning(this, "Erreur de saisie", "La distance doit être un nombre (ex: 10.5 ou 10)!");
            ui->lineEdit_distance->setFocus();
            ui->lineEdit_distance->selectAll();
            return;
        }
    }

    if (distance <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "La distance doit être un nombre positif!");
        ui->lineEdit_distance->setFocus();
        ui->lineEdit_distance->selectAll();
        return;
    }

    // Validation de la durée
    if (dureeText.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "La durée est obligatoire!");
        ui->lineEdit_duree_circuit->setFocus();
        return;
    }

    bool dureeOk = false;
    int duree = dureeText.toInt(&dureeOk);
    if (!dureeOk) {
        QMessageBox::warning(this, "Erreur de saisie", "La durée doit être un nombre entier (chiffres uniquement)!");
        ui->lineEdit_duree_circuit->setFocus();
        ui->lineEdit_duree_circuit->selectAll();
        return;
    }

    // Vérifier qu'il n'y a pas de caractères non numériques
    for (const QChar &c : dureeText) {
        if (!c.isDigit()) {
            QMessageBox::warning(this, "Erreur de saisie", "La durée doit être un nombre entier (chiffres uniquement)!");
            ui->lineEdit_duree_circuit->setFocus();
            ui->lineEdit_duree_circuit->selectAll();
            return;
        }
    }

    if (duree <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "La durée doit être un nombre positif!");
        ui->lineEdit_duree_circuit->setFocus();
        ui->lineEdit_duree_circuit->selectAll();
        return;
    }

    // Validation de l'immatricule
    if (immat.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'immatricule est obligatoire!");
        ui->lineEdit_immat_circuit->setFocus();
        return;
    }

    if (immat.length() < 3) {
        QMessageBox::warning(this, "Erreur de saisie", "L'immatricule doit contenir au moins 3 caractères!");
        ui->lineEdit_immat_circuit->setFocus();
        ui->lineEdit_immat_circuit->selectAll();
        return;
    }

    // Nettoyer le texte de difficulté (enlever les emojis)
    difficulte = difficulte.replace("🟢 ", "").replace("🟡 ", "").replace("🔴 ", "").trimmed();

    Circuit circuit(id, nom, desc, distance, duree, difficulte, immat);

    if (circuit.ajouter()) {
        refreshTableCircuit();
        clearFieldsCircuit();
    }
}
void MainWindow::on_pushButton_supprimercircuit_bas_clicked()
{
    // Récupérer l'ID depuis le tableau de recherche
    QModelIndex currentIndex = ui->tableView_circuits_recherche->currentIndex();

    if (!currentIndex.isValid()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un circuit à supprimer dans le tableau!");
        return;
    }

    int row = currentIndex.row();
    int id = ui->tableView_circuits_recherche->model()->index(row, 0).data().toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer ce circuit?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Circuit::supprimer(id)) {
            refreshTableCircuit();
            clearFieldsCircuit();
            QMessageBox::information(this, "Succès", "Circuit supprimé avec succès ✓");
        }
    }
}
void MainWindow::on_pushButton_modifiercircuit_clicked()
{
    // Récupération des valeurs
    QString idText = ui->lineEdit_id_circuit->text().trimmed();
    QString nom = ui->lineEdit_nom_circuit->text().trimmed();
    QString desc = ui->textEdit_desc_circuit->toPlainText().trimmed();
    QString distanceText = ui->lineEdit_distance->text().trimmed();
    QString dureeText = ui->lineEdit_duree_circuit->text().trimmed();
    QString difficulte = ui->comboBox_difficulte->currentText();
    QString immat = ui->lineEdit_immat_circuit->text().trimmed();

    // Permission
    if (!canPerformAction("modify_circuit")) { showAccessDenied("Modifier circuit"); return; }

    // Validation de l'ID du circuit
    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez sélectionner un circuit à modifier!");
        ui->lineEdit_id_circuit->setFocus();
        return;
    }

    // Vérifier que l'ID contient uniquement des chiffres
    bool idOk = false;
    int id = idText.toInt(&idOk);
    if (!idOk) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID du circuit doit être un nombre (chiffres uniquement)!");
        ui->lineEdit_id_circuit->setFocus();
        ui->lineEdit_id_circuit->selectAll();
        return;
    }

    // Vérifier qu'il n'y a pas de caractères non numériques
    for (const QChar &c : idText) {
        if (!c.isDigit()) {
            QMessageBox::warning(this, "Erreur de saisie", "L'ID du circuit doit être un nombre (chiffres uniquement)!");
            ui->lineEdit_id_circuit->setFocus();
            ui->lineEdit_id_circuit->selectAll();
            return;
        }
    }

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID du circuit doit être un nombre positif!");
        ui->lineEdit_id_circuit->setFocus();
        ui->lineEdit_id_circuit->selectAll();
        return;
    }

    // Validation du nom du circuit
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le nom du circuit est obligatoire!");
        ui->lineEdit_nom_circuit->setFocus();
        return;
    }

    if (nom.length() < 2) {
        QMessageBox::warning(this, "Erreur de saisie", "Le nom du circuit doit contenir au moins 2 caractères!");
        ui->lineEdit_nom_circuit->setFocus();
        ui->lineEdit_nom_circuit->selectAll();
        return;
    }

    // Validation de la distance
    if (distanceText.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "La distance est obligatoire!");
        ui->lineEdit_distance->setFocus();
        return;
    }

    bool distanceOk = false;
    double distance = distanceText.toDouble(&distanceOk);
    if (!distanceOk) {
        QMessageBox::warning(this, "Erreur de saisie", "La distance doit être un nombre (ex: 10.5 ou 10)!");
        ui->lineEdit_distance->setFocus();
        ui->lineEdit_distance->selectAll();
        return;
    }

    // Vérifier qu'il n'y a que des chiffres et un point décimal
    bool hasDecimalPoint = false;
    for (const QChar &c : distanceText) {
        if (c == '.' || c == ',') {
            if (hasDecimalPoint) {
                QMessageBox::warning(this, "Erreur de saisie", "La distance doit être un nombre (ex: 10.5 ou 10)!");
                ui->lineEdit_distance->setFocus();
                ui->lineEdit_distance->selectAll();
                return;
            }
            hasDecimalPoint = true;
        } else if (!c.isDigit()) {
            QMessageBox::warning(this, "Erreur de saisie", "La distance doit être un nombre (ex: 10.5 ou 10)!");
            ui->lineEdit_distance->setFocus();
            ui->lineEdit_distance->selectAll();
            return;
        }
    }

    if (distance <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "La distance doit être un nombre positif!");
        ui->lineEdit_distance->setFocus();
        ui->lineEdit_distance->selectAll();
        return;
    }

    // Validation de la durée
    if (dureeText.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "La durée est obligatoire!");
        ui->lineEdit_duree_circuit->setFocus();
        return;
    }

    bool dureeOk = false;
    int duree = dureeText.toInt(&dureeOk);
    if (!dureeOk) {
        QMessageBox::warning(this, "Erreur de saisie", "La durée doit être un nombre entier (chiffres uniquement)!");
        ui->lineEdit_duree_circuit->setFocus();
        ui->lineEdit_duree_circuit->selectAll();
        return;
    }

    // Vérifier qu'il n'y a pas de caractères non numériques
    for (const QChar &c : dureeText) {
        if (!c.isDigit()) {
            QMessageBox::warning(this, "Erreur de saisie", "La durée doit être un nombre entier (chiffres uniquement)!");
            ui->lineEdit_duree_circuit->setFocus();
            ui->lineEdit_duree_circuit->selectAll();
            return;
        }
    }

    if (duree <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "La durée doit être un nombre positif!");
        ui->lineEdit_duree_circuit->setFocus();
        ui->lineEdit_duree_circuit->selectAll();
        return;
    }

    // Validation de l'immatricule
    if (immat.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'immatricule est obligatoire!");
        ui->lineEdit_immat_circuit->setFocus();
        return;
    }

    if (immat.length() < 3) {
        QMessageBox::warning(this, "Erreur de saisie", "L'immatricule doit contenir au moins 3 caractères!");
        ui->lineEdit_immat_circuit->setFocus();
        ui->lineEdit_immat_circuit->selectAll();
        return;
    }

    // Nettoyer le texte de difficulté (enlever les emojis)
    difficulte = difficulte.replace("🟢 ", "").replace("🟡 ", "").replace("🔴 ", "").trimmed();

    Circuit circuit(id, nom, desc, distance, duree, difficulte, immat);

    if (circuit.modifier()) {
        refreshTableCircuit();
        clearFieldsCircuit();
    }
}

void MainWindow::on_pushButton_supprimercircuit_clicked()
{
    // Permission
    if (!canPerformAction("delete_circuit")) { showAccessDenied("Supprimer circuit"); return; }

    QString idText = ui->lineEdit_id_circuit->text().trimmed();

    if (idText.isEmpty()) {
        ui->lineEdit_id_circuit->setFocus();
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un circuit à supprimer!");
        return;
    }

    // Vérifier que l'ID contient uniquement des chiffres
    bool idOk;
    int id = idText.toInt(&idOk);
    if (!idOk) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID du circuit doit être un nombre (chiffres uniquement)!");
        ui->lineEdit_id_circuit->setFocus();
        ui->lineEdit_id_circuit->selectAll();
        return;
    }

    // Vérifier qu'il n'y a pas de caractères non numériques
    for (const QChar &c : idText) {
        if (!c.isDigit()) {
            QMessageBox::warning(this, "Erreur de saisie", "L'ID du circuit doit être un nombre (chiffres uniquement)!");
            ui->lineEdit_id_circuit->setFocus();
            ui->lineEdit_id_circuit->selectAll();
            return;
        }
    }

    if (id <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID du circuit doit être un nombre positif!");
        ui->lineEdit_id_circuit->setFocus();
        ui->lineEdit_id_circuit->selectAll();
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer ce circuit?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Circuit::supprimer(id)) {
            refreshTableCircuit();
            clearFieldsCircuit();
        }
    }
}

void MainWindow::on_pushButton_recherchecircuit_clicked()
{
    QString critere = ui->lineEdit_recherche_circuit->text();
    QSqlQueryModel *model;

    if (!critere.isEmpty()) {
        model = Circuit::rechercher(critere);
        if (model->rowCount() == 0) {
            QMessageBox::information(this, "Recherche", "Aucun circuit trouvé pour: " + critere);
        }
    } else {
        model = Circuit::afficher();
    }

    // Mettre à jour le tableau dans l'onglet recherche
    ui->tableView_circuits_recherche->setModel(model);
}

void MainWindow::on_comboBox_trier_circuit_currentIndexChanged(int index)
{
    QString critere;
    switch(index) {
    case 0: critere = "ID_CIRCUIT"; break;
    case 1: critere = "NOM_CIRCUIT"; break;
    case 2: critere = "DISTANCE"; break;
    default: critere = "ID_CIRCUIT";
    }

    QSqlQueryModel *model = Circuit::trier(critere);
    // Mettre à jour le tableau dans l'onglet recherche
    ui->tableView_circuits_recherche->setModel(model);
}

void MainWindow::on_pushButton_export_circuit_clicked()
{
    if (!canPerformAction("export_circuit")) { showAccessDenied("Exporter circuits"); return; }
    // Déterminer quel tableau utiliser (principal ou recherche)
    QAbstractItemModel* model = nullptr;
    QString sourceInfo;

    if (ui->tabWidget_circuit->currentIndex() == 0) {
        // Onglet Accueil - utiliser le tableau principal
        model = ui->tableView_circuits->model();
        sourceInfo = "tous les circuits";
    } else {
        // Onglet Recherche et Tri - utiliser le tableau de recherche
        model = ui->tableView_circuits_recherche->model();
        sourceInfo = "les circuits filtrés";
    }

    if (!model || model->rowCount() == 0) {
        QMessageBox::warning(this, "Exportation",
                             "Aucune donnée à exporter!\n\n"
                             "Veuillez d'abord afficher des circuits dans le tableau.");
        return;
    }

    // Demander à l'utilisateur où sauvegarder le fichier
    QString defaultFileName = QString("circuits_export_%1.csv")
                                  .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Exporter les circuits en CSV",
        defaultPath + "/" + defaultFileName,
        "Fichiers CSV (*.csv);;Tous les fichiers (*.*)"
        );

    // Si l'utilisateur a annulé
    if (filePath.isEmpty()) {
        return;
    }

    // S'assurer que le fichier a l'extension .csv
    if (!filePath.endsWith(".csv", Qt::CaseInsensitive)) {
        filePath += ".csv";
    }

    // Ouvrir le fichier en écriture
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Erreur",
                              QString("Impossible de créer le fichier:\n%1\n\nErreur: %2")
                                  .arg(filePath)
                                  .arg(file.errorString()));
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8); // Support des caractères spéciaux (accents, etc.)

    // Écrire l'en-tête avec les noms des colonnes
    QStringList headers;
    int columnCount = model->columnCount();
    for (int col = 0; col < columnCount; ++col) {
        QString header = model->headerData(col, Qt::Horizontal).toString();
        if (header.isEmpty()) {
            header = QString("Colonne %1").arg(col + 1);
        }
        headers << escapeCsvField(header);
    }
    out << headers.join(",") << "\n";

    // Écrire les données de chaque ligne
    int rowCount = model->rowCount();
    for (int row = 0; row < rowCount; ++row) {
        QStringList rowData;
        for (int col = 0; col < columnCount; ++col) {
            QModelIndex index = model->index(row, col);
            QString value = index.data().toString();
            rowData << escapeCsvField(value);
        }
        out << rowData.join(",") << "\n";
    }

    file.close();

    // Message de confirmation
    QMessageBox::information(this, "Exportation réussie",
                             QString("✅ Exportation terminée avec succès!\n\n"
                                     "Fichier créé: %1\n"
                                     "Nombre de circuits exportés: %2\n\n"
                                     "Le fichier peut être ouvert dans Excel, LibreOffice ou tout autre tableur.")
                                 .arg(filePath)
                                 .arg(rowCount));

    qDebug() << "✅ Exportation CSV réussie:" << filePath << "-" << rowCount << "lignes";
}

// Fonction utilitaire pour échapper les champs CSV
QString MainWindow::escapeCsvField(const QString& field)
{
    if (field.contains(',') || field.contains('"') || field.contains('\n') || field.contains('\r')) {
        QString escaped = field;
        escaped.replace("\"", "\"\""); // Doubler les guillemets
        return "\"" + escaped + "\"";
    }
    return field;
}

void MainWindow::on_tableView_circuits_clicked(const QModelIndex &index)
{
    int id = index.sibling(index.row(), 0).data().toInt();
    QString nom = index.sibling(index.row(), 1).data().toString();
    QString desc = index.sibling(index.row(), 2).data().toString();
    double distance = index.sibling(index.row(), 3).data().toDouble();
    int duree = index.sibling(index.row(), 4).data().toInt();
    QString difficulte = index.sibling(index.row(), 5).data().toString();
    QString immat = index.sibling(index.row(), 6).data().toString();

    ui->lineEdit_id_circuit->setText(QString::number(id));
    ui->lineEdit_nom_circuit->setText(nom);
    ui->textEdit_desc_circuit->setPlainText(desc);
    ui->lineEdit_distance->setText(QString::number(distance));
    ui->lineEdit_duree_circuit->setText(QString::number(duree));

    // Trouver l'index correspondant dans le comboBox
    int indexDiff = -1;
    if (difficulte == "Facile") {
        indexDiff = ui->comboBox_difficulte->findText("🟢 Facile");
    } else if (difficulte == "Moyen") {
        indexDiff = ui->comboBox_difficulte->findText("🟡 Moyen");
    } else if (difficulte == "Difficile") {
        indexDiff = ui->comboBox_difficulte->findText("🔴 Difficile");
    }
    ui->comboBox_difficulte->setCurrentIndex(indexDiff != -1 ? indexDiff : 0);

    ui->lineEdit_immat_circuit->setText(immat);
}

void MainWindow::on_tableView_circuits_recherche_clicked(const QModelIndex &index)
{
    // Utiliser la même fonction que le tableau principal
    on_tableView_circuits_clicked(index);
    // Basculer vers l'onglet Accueil pour voir le formulaire
    ui->tabWidget_circuit->setCurrentIndex(0);
}

// ==================== GESTION CLIENTS ====================
void MainWindow::refreshTableClient()
{
    QSqlQueryModel *model = C_Client.afficher();

    // Convertir QSqlQueryModel en données pour QTableWidget
    if (!ui->tableWidget_6) {
        qDebug() << "⚠️ tableWidget_6 n'existe pas dans l'UI";
        return;
    }

    int colCount = model->columnCount();
    ui->tableWidget_6->setRowCount(model->rowCount());
    ui->tableWidget_6->setColumnCount(colCount);

    // Définir les en-têtes
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "CIN" << "Date Naiss." << "Adresse" << "Téléphone" << "Email";
    if (colCount > 8) headers << "Latitude";
    if (colCount > 9) headers << "Longitude";
    ui->tableWidget_6->setHorizontalHeaderLabels(headers);

    // Remplir la table
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < colCount; ++col) {
            QVariant data = model->data(model->index(row, col));
            QString text;

            // Formater la date si c'est la colonne 4 (Date Naiss.)
            if (col == 4 && data.typeId() == QMetaType::QDate) {
                QDate date = data.toDate();
                text = date.toString("dd/MM/yyyy");
            } else if (col == 4) {
                // Si c'est une string, essayer de la formater
                QString dateStr = data.toString();
                QDate date = QDate::fromString(dateStr.split("T").first(), "yyyy-MM-dd");
                if (!date.isValid()) {
                    date = QDate::fromString(dateStr, "dd/MM/yyyy");
                }
                if (date.isValid()) {
                    text = date.toString("dd/MM/yyyy");
                } else {
                    text = dateStr;
                }
            } else {
                text = data.toString();
            }

            // Débogage pour la colonne email (col 7)
            if (col == 7) {
                qDebug() << "Row" << row << "Col" << col << "Email data:" << data.toString() << "IsNull:" << data.isNull();
            }

            QTableWidgetItem *item = new QTableWidgetItem(text);
            ui->tableWidget_6->setItem(row, col, item);
        }
    }

    // Ajuster la largeur des colonnes
    ui->tableWidget_6->resizeColumnsToContents();
}

void MainWindow::clearFieldsClient()
{
    if (ui->lineEditNom) ui->lineEditNom->clear();
    if (ui->lineEditPrenom) ui->lineEditPrenom->clear();
    if (ui->lineEditCIN) ui->lineEditCIN->clear();
    if (ui->jjj) ui->jjj->setDate(QDate(2000, 1, 1));
    if (ui->lineEditMotDePasse) ui->lineEditMotDePasse->clear(); // Adresse
    if (ui->lineEditTelephone) ui->lineEditTelephone->clear();
    if (ui->lineEditPoste) ui->lineEditPoste->clear(); // Email
    if (ui->lineEditLatitude) ui->lineEditLatitude->clear();
    if (ui->lineEditLongitude) ui->lineEditLongitude->clear();
}

void MainWindow::on_btnAjouter_clicked()
{
    if (!canPerformAction("add_client")) { showAccessDenied("Ajouter client"); return; }
    if (!ui->lineEditNom || !ui->lineEditPrenom || !ui->lineEditCIN || !ui->jjj ||
        !ui->lineEditMotDePasse || !ui->lineEditTelephone || !ui->lineEditPoste ||
        !ui->lineEditLatitude || !ui->lineEditLongitude) {
        QMessageBox::warning(this, "Erreur", "Certains widgets UI sont manquants!");
        return;
    }

    QString nom = ui->lineEditNom->text().trimmed();
    QString prenom = ui->lineEditPrenom->text().trimmed();
    QString cin = ui->lineEditCIN->text().trimmed();
    QDate dateNaissance = ui->jjj->date();
    QString adresse = ui->lineEditMotDePasse->text().trimmed(); // Adresse
    QString telStr = ui->lineEditTelephone->text().trimmed();
    QString email = ui->lineEditPoste->text().trimmed(); // Email
    QString latStr = ui->lineEditLatitude->text().trimmed();
    QString lonStr = ui->lineEditLongitude->text().trimmed();

    // Validation
    if (nom.isEmpty() || prenom.isEmpty() || cin.isEmpty() || adresse.isEmpty() || telStr.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez remplir tous les champs obligatoires!");
        return;
    }

    // Validation de la date de naissance et de l'âge (18-100 ans)
    qDebug() << "Date de naissance saisie:" << dateNaissance;
    
    if (!dateNaissance.isValid()) {
        qDebug() << "Date invalide!";
        QMessageBox::warning(this, "Attention", "La date de naissance est invalide!");
        return;
    }

    QDate today = QDate::currentDate();
    int age = today.year() - dateNaissance.year();
    if (today.month() < dateNaissance.month() || 
        (today.month() == dateNaissance.month() && today.day() < dateNaissance.day())) {
        age--;
    }

    qDebug() << "Âge calculé:" << age;

    if (age < 18 || age > 100) {
        qDebug() << "Âge invalide! Âge:" << age;
        QMessageBox::warning(this, "Attention", 
                           QString("L'âge du client doit être entre 18 et 100 ans!\n"
                                   "Âge calculé: %1 ans").arg(age));
        return;
    }

    qDebug() << "Validation d'âge OK";

    // Vérifier que l'email n'est pas vide
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Attention", "L'email est obligatoire! Veuillez le remplir.");
        return;
    }

    // Vérifier le format de l'email (xyz@xyz.xyz)
    QRegularExpression emailRegex("^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Attention", "Format d'email invalide!\nFormat attendu: xyz@xyz.xyz\nExemple: jean.dupont@gmail.com");
        return;
    }

    // Conversion du téléphone (supprimer les espaces, le préfixe +216, etc.)
    QString telClean = telStr;
    telClean = telClean.remove(QRegularExpression("[^0-9]")); // Garder uniquement les chiffres

    if (telClean.length() < 8 || telClean.length() > 10) {
        QMessageBox::warning(this, "Attention", "Le numéro de téléphone doit contenir entre 8 et 10 chiffres!\nFormat accepté: +216 12345678 ou 12345678");
        return;
    }

    // Si le numéro commence par 216 (code pays), prendre les 8 derniers chiffres
    if (telClean.length() == 10 && telClean.startsWith("216")) {
        telClean = telClean.right(8);
    } else if (telClean.length() == 9 && telClean.startsWith("9")) {
        // Format tunisien: 9XXXXXXXX (9 chiffres, prendre les 8 derniers)
        telClean = telClean.right(8);
    }

    if (telClean.length() != 8) {
        QMessageBox::warning(this, "Attention", "Le numéro de téléphone doit contenir 8 chiffres!\nFormat accepté: +216 12345678 ou 12345678");
        return;
    }

    bool ok;
    int tel = telClean.toInt(&ok);
    if (!ok || tel < 10000000 || tel > 99999999) {
        QMessageBox::warning(this, "Attention", "Le numéro de téléphone doit être un nombre à 8 chiffres!");
        return;
    }

    // Vérifier le CIN (8 chiffres)
    QRegularExpression cinRegex("^[0-9]{8}$");
    if (!cinRegex.match(cin).hasMatch()) {
        QMessageBox::warning(this, "Attention", "Le CIN doit être une chaîne de 8 chiffres!");
        return;
    }

    // Vérifier la connexion à la base de données avant d'ajouter
    if (!Connection::isDatabaseOpen()) {
        QMessageBox::critical(this, "Erreur de connexion",
                              "La connexion à la base de données n'est pas active!\n\n"
                              "Veuillez redémarrer l'application ou vérifier la configuration de la base de données.");
        return;
    }

    // Tester la connexion avec une requête simple
    if (!Connection::testConnection()) {
        QMessageBox::critical(this, "Erreur de connexion",
                              "La connexion à la base de données ne répond pas!\n\n"
                              "Statut:\n" + Connection::getConnectionStatus() +
                                  "\n\nVeuillez vérifier que:\n"
                                  "- Oracle XE est démarré\n"
                                  "- Le service est accessible\n"
                                  "- Les identifiants sont corrects");
        return;
    }

    // Vérifier si le CIN existe déjà avant de créer l'objet
    if (C_Client.checkIfCinExists(cin)) {
        QMessageBox::critical(this, "Erreur", "Ce CIN existe déjà dans la base de données! Veuillez utiliser un autre CIN.");
        return;
    }

    // Conversion latitude/longitude (facultative)
    double latitude = 0.0;
    double longitude = 0.0;
    bool okLat = true;
    bool okLon = true;
    if (!latStr.isEmpty()) {
        latitude = latStr.toDouble(&okLat);
    }
    if (!lonStr.isEmpty()) {
        longitude = lonStr.toDouble(&okLon);
    }
    if (!okLat || !okLon) {
        QMessageBox::warning(this, "Attention", "Latitude ou longitude invalide (valeurs numériques attendues).");
        return;
    }

    client c(nom, prenom, cin, dateNaissance, adresse, tel, email, latitude, longitude);

    if (c.ajouter()) {
        QMessageBox::information(this, "Succès", "Client ajouté avec succès!");
        refreshTableClient();
        clearFieldsClient();
    } else {
        QString statusInfo = Connection::getConnectionStatus();
        QMessageBox::critical(this, "Erreur",
                              "Erreur lors de l'ajout!\n\n"
                              "Statut de la connexion:\n" + statusInfo +
                                  "\n\nVérifiez aussi que:\n"
                                  "- Le nom et prénom contiennent uniquement des lettres\n"
                                  "- Le CIN est unique\n"
                                  "- L'email a un format valide");
    }
}

void MainWindow::on_btnModifier_clicked()
{
    if (!canPerformAction("modify_client")) { showAccessDenied("Modifier client"); return; }
    if (!ui->lineEditNom || !ui->lineEditPrenom || !ui->lineEditCIN || !ui->jjj ||
        !ui->lineEditMotDePasse || !ui->lineEditTelephone || !ui->lineEditPoste) {
        QMessageBox::warning(this, "Erreur", "Certains widgets UI sont manquants!");
        return;
    }

    // Récupérer l'ID depuis le tableau (première colonne)
    QList<QTableWidgetItem*> selectedItems = ui->tableWidget_6->selectedItems();
    if (selectedItems.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un client à modifier!");
        return;
    }

    int row = selectedItems.first()->row();
    QString idStr = ui->tableWidget_6->item(row, 0)->text();

    bool ok;
    int id = idStr.toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Attention", "ID invalide!");
        return;
    }

    QString nom = ui->lineEditNom->text().trimmed();
    QString prenom = ui->lineEditPrenom->text().trimmed();
    QString cin = ui->lineEditCIN->text().trimmed();
    QDate dateNaissance = ui->jjj->date();
    QString adresse = ui->lineEditMotDePasse->text().trimmed(); // Adresse
    QString telStr = ui->lineEditTelephone->text().trimmed();
    QString email = ui->lineEditPoste->text().trimmed(); // Email

    // Validation
    if (nom.isEmpty() || prenom.isEmpty() || cin.isEmpty() || adresse.isEmpty() || telStr.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez remplir tous les champs obligatoires!");
        return;
    }

    // Validation de la date de naissance et de l'âge (18-100 ans)
    qDebug() << "Date de naissance saisie (modification):" << dateNaissance;
    
    if (!dateNaissance.isValid()) {
        qDebug() << "Date invalide!";
        QMessageBox::warning(this, "Attention", "La date de naissance est invalide!");
        return;
    }

    QDate today = QDate::currentDate();
    int age = today.year() - dateNaissance.year();
    if (today.month() < dateNaissance.month() || 
        (today.month() == dateNaissance.month() && today.day() < dateNaissance.day())) {
        age--;
    }

    qDebug() << "Âge calculé (modification):" << age;

    if (age < 18 || age > 100) {
        qDebug() << "Âge invalide! Âge:" << age;
        QMessageBox::warning(this, "Attention", 
                           QString("L'âge du client doit être entre 18 et 100 ans!\n"
                                   "Âge calculé: %1 ans").arg(age));
        return;
    }

    qDebug() << "Validation d'âge OK (modification)";

    // Vérifier que l'email n'est pas vide
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Attention", "L'email est obligatoire! Veuillez le remplir.");
        return;
    }

    // Vérifier le format de l'email (xyz@xyz.xyz)
    QRegularExpression emailRegex("^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!emailRegex.match(email).hasMatch()) {
        QMessageBox::warning(this, "Attention", "Format d'email invalide!\nFormat attendu: xyz@xyz.xyz\nExemple: jean.dupont@gmail.com");
        return;
    }

    // Conversion du téléphone (supprimer les espaces, le préfixe +216, etc.)
    QString telClean = telStr;
    telClean = telClean.remove(QRegularExpression("[^0-9]")); // Garder uniquement les chiffres

    if (telClean.length() < 8 || telClean.length() > 10) {
        QMessageBox::warning(this, "Attention", "Le numéro de téléphone doit contenir entre 8 et 10 chiffres!\nFormat accepté: +216 12345678 ou 12345678");
        return;
    }

    // Si le numéro commence par 216 (code pays), prendre les 8 derniers chiffres
    if (telClean.length() == 10 && telClean.startsWith("216")) {
        telClean = telClean.right(8);
    } else if (telClean.length() == 9 && telClean.startsWith("9")) {
        // Format tunisien: 9XXXXXXXX (9 chiffres, prendre les 8 derniers)
        telClean = telClean.right(8);
    }

    if (telClean.length() != 8) {
        QMessageBox::warning(this, "Attention", "Le numéro de téléphone doit contenir 8 chiffres!\nFormat accepté: +216 12345678 ou 12345678");
        return;
    }

    int tel = telClean.toInt(&ok);
    if (!ok || tel < 10000000 || tel > 99999999) {
        QMessageBox::warning(this, "Attention", "Le numéro de téléphone doit être un nombre à 8 chiffres!");
        return;
    }

    // Vérifier le CIN (8 chiffres)
    QRegularExpression cinRegex("^[0-9]{8}$");
    if (!cinRegex.match(cin).hasMatch()) {
        QMessageBox::warning(this, "Attention", "Le CIN doit être une chaîne de 8 chiffres!");
        return;
    }

    client c(nom, prenom, cin, dateNaissance, adresse, tel, email);
    c.setid_client(id);

    if (c.modifierE()) {
        QMessageBox::information(this, "Succès", "Client modifié avec succès!");
        refreshTableClient();
        clearFieldsClient();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification! Vérifiez que le CIN n'existe pas déjà pour un autre client.");
    }
}

void MainWindow::on_btnSupprimer_clicked()
{
    if (!canPerformAction("delete_client")) { showAccessDenied("Supprimer client"); return; }
    if (!ui->lineEditSupprimer) {
        QMessageBox::warning(this, "Erreur", "Widget lineEditSupprimer manquant!");
        return;
    }

    QString idStr = ui->lineEditSupprimer->text().trimmed();

    if (idStr.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez entrer l'ID du client à supprimer!");
        return;
    }

    bool ok;
    int id = idStr.toInt(&ok);
    if (!ok || id <= 0) {
        QMessageBox::warning(this, "Attention", "ID invalide!");
        return;
    }

    // Vérifier si le client existe avant de supprimer
    if (!C_Client.checkIfClientExists(id)) {
        QMessageBox::critical(this, "Erreur", "Ce client n'existe pas!\nVeuillez vérifier l'ID saisi.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer ce client?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (C_Client.supprimer(id)) {
            QMessageBox::information(this, "Succès", "Client supprimé avec succès!");
            refreshTableClient();
            ui->lineEditSupprimer->clear();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression!");
        }
    }
}

void MainWindow::on_btnChercher_clicked()
{
    if (!ui->lineEditRecherche || !ui->tableWidget_6) {
        QMessageBox::warning(this, "Erreur", "Widgets UI manquants!");
        return;
    }

    QString nom = ui->lineEditRecherche->text().trimmed();

    if (nom.isEmpty()) {
        refreshTableClient();
        return;
    }

    QSqlQueryModel *model = C_Client.rechercheParNom(nom);

    // Convertir QSqlQueryModel en données pour QTableWidget
    int colCount = model->columnCount();
    ui->tableWidget_6->setRowCount(model->rowCount());
    ui->tableWidget_6->setColumnCount(colCount);

    // Définir les en-têtes
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "CIN" << "Date Naiss." << "Adresse" << "Téléphone" << "Email";
    if (colCount > 8) headers << "Latitude";
    if (colCount > 9) headers << "Longitude";
    ui->tableWidget_6->setHorizontalHeaderLabels(headers);

    // Remplir la table
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < colCount; ++col) {
            QVariant data = model->data(model->index(row, col));
            QString text;

            // Formater la date si c'est la colonne 4 (Date Naiss.)
            if (col == 4 && data.typeId() == QMetaType::QDate) {
                QDate date = data.toDate();
                text = date.toString("dd/MM/yyyy");
            } else if (col == 4) {
                // Si c'est une string, essayer de la formater
                QString dateStr = data.toString();
                QDate date = QDate::fromString(dateStr.split("T").first(), "yyyy-MM-dd");
                if (!date.isValid()) {
                    date = QDate::fromString(dateStr, "dd/MM/yyyy");
                }
                if (date.isValid()) {
                    text = date.toString("dd/MM/yyyy");
                } else {
                    text = dateStr;
                }
            } else {
                text = data.toString();
            }

            // Débogage pour la colonne email (col 7)
            if (col == 7) {
                qDebug() << "Recherche - Row" << row << "Col" << col << "Email data:" << data.toString() << "IsNull:" << data.isNull();
            }

            QTableWidgetItem *item = new QTableWidgetItem(text);
            ui->tableWidget_6->setItem(row, col, item);
        }
    }

    // Ajuster la largeur des colonnes
    ui->tableWidget_6->resizeColumnsToContents();

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Recherche", "Aucun client trouvé pour: " + nom);
    }
}

void MainWindow::on_btnClear_clicked()
{
    // Effacer le champ de recherche
    if (ui->lineEditRecherche) {
        ui->lineEditRecherche->clear();
    }
    
    // Réinitialiser le tableau à son état initial (afficher tous les clients)
    refreshTableClient();
}

void MainWindow::on_tableWidget_6_itemClicked(QTableWidgetItem *item)
{
    if (!item || !ui->tableWidget_6) return;

    int row = item->row();

    // Vérifier que tous les widgets existent
    if (!ui->lineEditNom || !ui->lineEditPrenom || !ui->lineEditCIN ||
        !ui->jjj || !ui->lineEditMotDePasse || !ui->lineEditTelephone || !ui->lineEditPoste ||
        !ui->lineEditLatitude || !ui->lineEditLongitude) {
        return;
    }

    // Récupérer les données de la ligne sélectionnée
    QString nom = ui->tableWidget_6->item(row, 1)->text();
    QString prenom = ui->tableWidget_6->item(row, 2)->text();
    QString cin = ui->tableWidget_6->item(row, 3)->text();
    QString dateStr = ui->tableWidget_6->item(row, 4)->text();
    QString adresse = ui->tableWidget_6->item(row, 5)->text();
    QString tel = ui->tableWidget_6->item(row, 6)->text();
    QString email = ui->tableWidget_6->item(row, 7)->text();

    // Remplir les champs
    ui->lineEditNom->setText(nom);
    ui->lineEditPrenom->setText(prenom);
    ui->lineEditCIN->setText(cin);

    // Conversion de la date (formats Oracle possibles)
    QDate date;
    QStringList dateFormats = {"dd/MM/yyyy", "yyyy-MM-dd", "dd-MMM-yy", "dd/MM/yy", "yyyy-MM-ddTHH:mm:ss.zzz"};
    for (const QString &format : dateFormats) {
        date = QDate::fromString(dateStr.split("T").first(), format);
        if (date.isValid()) break;
    }
    if (!date.isValid()) {
        // Essayer de parser directement depuis QDateTime
        QDateTime dateTime = QDateTime::fromString(dateStr, Qt::ISODate);
        if (dateTime.isValid()) {
            date = dateTime.date();
        }
    }
    if (date.isValid()) {
        ui->jjj->setDate(date);
    } else {
        ui->jjj->setDate(QDate::currentDate());
    }

    ui->lineEditMotDePasse->setText(adresse);
    // Extraire seulement les chiffres du téléphone (enlever +216 ou autres préfixes)
    tel = tel.remove(QRegularExpression("[^0-9]"));
    ui->lineEditTelephone->setText(tel);
    ui->lineEditPoste->setText(email);

    // Charger latitude/longitude depuis la base
    QString idStr = ui->tableWidget_6->item(row, 0)->text();
    bool okId = false;
    int id = idStr.toInt(&okId);
    if (okId && id > 0) {
        // Vérifier dynamiquement la présence des colonnes LATITUDE / LONGITUDE
        bool hasLatitudeLongitude = false;
        {
            QSqlQuery colQuery;
            colQuery.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'CLIENT' AND COLUMN_NAME IN ('LATITUDE','LONGITUDE')");
            if (colQuery.exec()) {
                bool hasLat = false;
                bool hasLon = false;
                while (colQuery.next()) {
                    QString colName = colQuery.value(0).toString();
                    if (colName == "LATITUDE")
                        hasLat = true;
                    else if (colName == "LONGITUDE")
                        hasLon = true;
                }
                hasLatitudeLongitude = (hasLat && hasLon);
            }
        }

        if (hasLatitudeLongitude) {
            QSqlQuery query;
            query.prepare("SELECT LATITUDE, LONGITUDE FROM CLIENT WHERE ID_CLIENT = :id");
            query.bindValue(":id", id);
            if (query.exec() && query.next()) {
                QVariant latVar = query.value(0);
                QVariant lonVar = query.value(1);
                if (!latVar.isNull()) {
                    ui->lineEditLatitude->setText(QString::number(latVar.toDouble(), 'f', 6));
                } else {
                    ui->lineEditLatitude->clear();
                }
                if (!lonVar.isNull()) {
                    ui->lineEditLongitude->setText(QString::number(lonVar.toDouble(), 'f', 6));
                } else {
                    ui->lineEditLongitude->clear();
                }
            } else {
                ui->lineEditLatitude->clear();
                ui->lineEditLongitude->clear();
            }
        } else {
            qDebug() << "⚠️ Colonnes LATITUDE / LONGITUDE absentes dans CLIENT, aucun chargement GPS.";
            ui->lineEditLatitude->clear();
            ui->lineEditLongitude->clear();
        }
    } else {
        ui->lineEditLatitude->clear();
        ui->lineEditLongitude->clear();
    }
}

void MainWindow::on_lineEditRecherche_textChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        refreshTableClient();
    }
}

void MainWindow::on_comboBoxTrier_currentIndexChanged(int index)
{
    if (!ui->comboBoxTrier || !ui->tableWidget_6) {
        return;
    }

    QString orderBy;

    switch (index) {
    case 0: // ID
        orderBy = "ID_CLIENT";
        break;
    case 1: // Nom
        orderBy = "NOM_CLIENT";
        break;
    default:
        orderBy = "ID_CLIENT";
        break;
    }

    QSqlQueryModel *model = C_Client.afficher(orderBy);

    // Convertir QSqlQueryModel en données pour QTableWidget
    int colCount = model->columnCount();
    ui->tableWidget_6->setRowCount(model->rowCount());
    ui->tableWidget_6->setColumnCount(colCount);

    // Définir les en-têtes
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "CIN" << "Date Naiss." << "Adresse" << "Téléphone" << "Email";
    if (colCount > 8) headers << "Latitude";
    if (colCount > 9) headers << "Longitude";
    ui->tableWidget_6->setHorizontalHeaderLabels(headers);

    // Remplir la table
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < colCount; ++col) {
            QVariant data = model->data(model->index(row, col));
            QString text;

            // Formater la date si c'est la colonne 4 (Date Naiss.)
            if (col == 4 && data.typeId() == QMetaType::QDate) {
                QDate date = data.toDate();
                text = date.toString("dd/MM/yyyy");
            } else if (col == 4) {
                // Si c'est une string, essayer de la formater
                QString dateStr = data.toString();
                QDate date = QDate::fromString(dateStr.split("T").first(), "yyyy-MM-dd");
                if (!date.isValid()) {
                    date = QDate::fromString(dateStr, "dd/MM/yyyy");
                }
                if (date.isValid()) {
                    text = date.toString("dd/MM/yyyy");
                } else {
                    text = dateStr;
                }
            } else {
                text = data.toString();
            }

            // Débogage pour la colonne email (col 7)
            if (col == 7) {
                qDebug() << "Tri - Row" << row << "Col" << col << "Email data:" << data.toString() << "IsNull:" << data.isNull();
            }

            QTableWidgetItem *item = new QTableWidgetItem(text);
            ui->tableWidget_6->setItem(row, col, item);
        }
    }

    // Ajuster la largeur des colonnes
    ui->tableWidget_6->resizeColumnsToContents();
}

void MainWindow::on_btnExporterPDF_clicked()
{
    if (!canPerformAction("export_client")) { showAccessDenied("Exporter client"); return; }
    // Si un client est sélectionné -> exporter uniquement ce client
    // Sinon -> exporter tous les clients
    int selectedRow = ui->tableWidget_6->currentRow();
    bool exportAll = (selectedRow < 0);

    QSqlQueryModel *model = nullptr;
    QString defaultFileName;

    // Récupérer le chemin du bureau par défaut
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);

    if (exportAll) {
        // Exporter tous les clients
        model = C_Client.afficher();
        if (!model || model->rowCount() == 0) {
            QMessageBox::information(this, "Export PDF", "Aucun client à exporter.");
            return;
        }

        defaultFileName = QString("Tous_les_clients_%1.pdf")
                              .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    } else {
        // Exporter uniquement le client sélectionné
        QTableWidgetItem *idItem = ui->tableWidget_6->item(selectedRow, 0);
        if (!idItem) {
            QMessageBox::warning(this, "Export PDF", "Erreur : Impossible de récupérer les données du client.");
            return;
        }

        QString clientId = idItem->text();

        // Vérifier qu'il existe au moins un client dans la base
        QSqlQueryModel *allModel = C_Client.afficher();
        if (!allModel || allModel->rowCount() == 0) {
            QMessageBox::warning(this, "Export PDF", "Aucun client à exporter.");
            return;
        }

        // Créer un modèle avec seulement le client sélectionné
        model = new QSqlQueryModel();
        QSqlQuery query;
        query.prepare("SELECT * FROM CLIENT WHERE ID_CLIENT = :id");
        query.addBindValue(clientId);
        if (!query.exec()) {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la récupération du client : " + query.lastError().text());
            return;
        }
        model->setQuery(query);

        if (model->rowCount() == 0) {
            QMessageBox::warning(this, "Export PDF", "Client non trouvé.");
            return;
        }

        // Récupérer le nom et prénom du client pour le nom du fichier
        QString nom = ui->tableWidget_6->item(selectedRow, 1) ? ui->tableWidget_6->item(selectedRow, 1)->text() : "Client";
        QString prenom = ui->tableWidget_6->item(selectedRow, 2) ? ui->tableWidget_6->item(selectedRow, 2)->text() : "";

        // Créer un nom de fichier avec le nom du client
        defaultFileName = QString("Client_%1_%2_%3.pdf")
                              .arg(nom)
                              .arg(prenom)
                              .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    }

    QString defaultPath = desktopPath + "/" + defaultFileName;

    // Ouvrir un dialogue de sauvegarde pour permettre à l'utilisateur de choisir le chemin
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Enregistrer le PDF",
                                                    defaultPath,
                                                    "Fichiers PDF (*.pdf);;Tous les fichiers (*)");

    // Si l'utilisateur annule, retourner
    if (fileName.isEmpty()) {
        return;
    }

    // Ajouter l'extension .pdf si elle n'existe pas
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) {
        fileName += ".pdf";
    }
    
    // Format de page (A4 par défaut, peut être changé en A3 ou A5)
    QPageSize::PageSizeId pageSize = QPageSize::A4;
    
    // DPI pour la qualité (150 pour standard, 300 pour haute qualité)
    int dpi = 150;
    
    // Créer le PDF directement avec QPainter pour contrôle total
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(pageSize);
    pdfWriter.setPageOrientation(QPageLayout::Landscape);
    pdfWriter.setResolution(dpi);
    
    QPainter painter(&pdfWriter);
    
    if (!painter.isActive()) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier PDF!");
        return;
    }
    
    // Polices réduites pour plus de contenu par page
    QFont titleFont("Arial", 28, QFont::Bold);
    QFont headerFont("Arial", 14, QFont::Bold);
    QFont contentFont("Arial", 12);
    
    int pageWidth = pdfWriter.width();
    int pageHeight = pdfWriter.height();
    int margin = 30;
    int y = margin;
    
    // Titre
    painter.setFont(titleFont);
    painter.drawText(QRect(margin, y, pageWidth - 2*margin, 40), 
                     Qt::AlignCenter, "Liste des Clients");
    y += 50;
    
    // Date et Total
    painter.setFont(contentFont);
    QString dateStr = "Date: " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm");
    painter.drawText(QRect(margin, y, pageWidth - 2*margin, 20), Qt::AlignLeft, dateStr);
    y += 25;
    
    QString totalStr = "Total: " + QString::number(model->rowCount()) + " clients";
    painter.drawText(QRect(margin, y, pageWidth - 2*margin, 20), Qt::AlignLeft, totalStr);
    y += 30;
    
    // Largeurs des colonnes (réduites)
    int colWidths[] = {50, 120, 120, 100, 100, 150, 100, 150};
    int totalWidth = 0;
    for (int i = 0; i < 8; i++) {
        totalWidth += colWidths[i];
    }
    
    // En-têtes
    painter.setFont(headerFont);
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "CIN" << "Date" << "Adresse" << "Tel" << "Email";
    
    int x = margin;
    int rowHeight = 25;
    
    // Dessiner en-têtes
    painter.fillRect(x, y, totalWidth, rowHeight, QColor(70, 130, 180));
    painter.setPen(QPen(Qt::black, 3));
    
    for (int col = 0; col < 8; col++) {
        painter.drawText(QRect(x + 5, y + 5, colWidths[col] - 10, rowHeight - 10), 
                         Qt::AlignCenter | Qt::AlignVCenter, headers[col]);
        painter.drawLine(x + colWidths[col], y, x + colWidths[col], y + rowHeight);
        x += colWidths[col];
    }
    
    y += rowHeight;
    painter.setFont(contentFont);
    
    // Données
    int rowsPerPage = 30;  // Plus de lignes par page avec polices réduites
    int currentRow = 0;
    
    for (int row = 0; row < model->rowCount(); row++) {
        // Nouvelle page si nécessaire
        if (currentRow >= rowsPerPage || y + rowHeight > pageHeight - margin) {
            pdfWriter.newPage();
            y = margin;
            currentRow = 0;
            
            // Redessiner en-têtes
            painter.setFont(headerFont);
            painter.fillRect(margin, y, totalWidth, rowHeight, QColor(70, 130, 180));
            painter.setPen(QPen(Qt::black, 3));
            
            x = margin;
            for (int col = 0; col < 8; col++) {
                painter.drawText(QRect(x + 5, y + 5, colWidths[col] - 10, rowHeight - 10), 
                                 Qt::AlignCenter | Qt::AlignVCenter, headers[col]);
                painter.drawLine(x + colWidths[col], y, x + colWidths[col], y + rowHeight);
                x += colWidths[col];
            }
            
            y += rowHeight;
            painter.setFont(contentFont);
        }
        
        x = margin;
        
        // Couleur alternée
        if (row % 2 == 0) {
            painter.fillRect(margin, y, totalWidth, rowHeight, QColor(240, 240, 240));
        }
        
        // Données
        painter.setPen(QPen(Qt::black, 2));
        for (int col = 0; col < 8; col++) {
            QVariant data = model->data(model->index(row, col));
            QString text;
            
            if (col == 4 && data.typeId() == QMetaType::QDate) {
                QDate date = data.toDate();
                text = date.toString("dd/MM/yy");
            } else if (col == 4) {
                QString dateStr = data.toString();
                QDate date = QDate::fromString(dateStr.split("T").first(), "yyyy-MM-dd");
                if (!date.isValid()) {
                    date = QDate::fromString(dateStr, "dd/MM/yyyy");
                }
                text = date.isValid() ? date.toString("dd/MM/yy") : dateStr.left(8);
            } else {
                text = data.toString();
            }
            
            painter.drawText(QRect(x + 5, y + 5, colWidths[col] - 10, rowHeight - 10), 
                             Qt::AlignCenter | Qt::AlignVCenter, text);
            painter.drawLine(x + colWidths[col], y, x + colWidths[col], y + rowHeight);
            x += colWidths[col];
        }
        
        painter.drawLine(margin, y + rowHeight, margin + totalWidth, y + rowHeight);
        y += rowHeight;
        currentRow++;
    }
    
    painter.end();
    
    QMessageBox::information(this, "Succès", 
                             "Exportation PDF réussie!\n\n" 
                             "Fichier: " + fileName + "\n\n"
                             "Nombre de clients exportés: " + QString::number(model->rowCount()));
}

// Fonction pour générer un QR code.
// - Si un client est sélectionné dans tableWidget_6 => QR pour CE client uniquement.
// - Si aucun client n'est sélectionné => QR global (tous les clients) via on_btnGenererQR_clicked().
void MainWindow::on_btnGenererQRClient_clicked()
{
    if (!canPerformAction("export")) { showAccessDenied("Génération QR / Export"); return; }
    int selectedRow = ui->tableWidget_6->currentRow();

    if (selectedRow < 0) {
        on_btnGenererQR_clicked();
        return;
    }

    // Récupérer TOUTES les informations du client depuis le tableau
    // Ordre des colonnes dans tableWidget_6 (voir refreshTableClient) :
    // 0: ID, 1: Nom, 2: Prénom, 3: CIN, 4: Date Naiss., 5: Adresse, 6: Téléphone, 7: Email
    QString clientId = ui->tableWidget_6->item(selectedRow, 0) ? ui->tableWidget_6->item(selectedRow, 0)->text() : "";
    QString nom = ui->tableWidget_6->item(selectedRow, 1) ? ui->tableWidget_6->item(selectedRow, 1)->text() : "";
    QString prenom = ui->tableWidget_6->item(selectedRow, 2) ? ui->tableWidget_6->item(selectedRow, 2)->text() : "";
    QString cin = ui->tableWidget_6->item(selectedRow, 3) ? ui->tableWidget_6->item(selectedRow, 3)->text() : "";
    QString dateNaissance = ui->tableWidget_6->item(selectedRow, 4) ? ui->tableWidget_6->item(selectedRow, 4)->text() : "";
    QString adresse = ui->tableWidget_6->item(selectedRow, 5) ? ui->tableWidget_6->item(selectedRow, 5)->text() : "";
    QString telephone = ui->tableWidget_6->item(selectedRow, 6) ? ui->tableWidget_6->item(selectedRow, 6)->text() : "";
    QString email = ui->tableWidget_6->item(selectedRow, 7) ? ui->tableWidget_6->item(selectedRow, 7)->text() : "";

    // Créer le contenu du QR code avec un texte lisible multi-ligne
    // Ce texte sera affiché tel quel par l'application de scan du téléphone
    // Exemple :
    // ID: 1
    // NOM: Dupont
    // PRENOM: Jean
    // CIN: 12345678
    // DATE: 01/01/1990
    // ADRESSE: 123 Rue de Paris
    // TEL: 0612345678
    // EMAIL: jean@example.com
    QString qrContent = QString(
                "ID: %1\n"
                "NOM: %2\n"
                "PRENOM: %3\n"
                "CIN: %4\n"
                "DATE: %5\n"
                "ADRESSE: %6\n"
                "TEL: %7\n"
                "EMAIL: %8")
            .arg(clientId)
            .arg(nom)
            .arg(prenom)
            .arg(cin)
            .arg(dateNaissance)
            .arg(adresse)
            .arg(telephone)
            .arg(email);
    
    // Générer le QR code
    QImage qrImage = QRCodeGenerator::generateQRCode(qrContent, 500);
    
    // Créer une fenêtre de dialogue pour afficher le QR code
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("QR Code - Client");
    dialog->setModal(true);
    dialog->resize(600, 700);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Titre avec les informations du client
    QLabel *titleLabel = new QLabel(QString("QR Code - %1 %2").arg(nom).arg(prenom), dialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 16pt; font-weight: bold; margin: 15px; color: #4682B4;");
    layout->addWidget(titleLabel);
    
    // Label pour afficher le QR code
    QLabel *qrLabel = new QLabel(dialog);
    qrLabel->setPixmap(QPixmap::fromImage(qrImage));
    qrLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(qrLabel);
    
    // Informations détaillées du client (format texte simple)
    QLabel *infoLabel = new QLabel(dialog);
    QString infoText = QString("📋 Informations du Client\n\n"
                              "ID: %1\n"
                              "NOM: %2\n"
                              "PRENOM: %3\n"
                              "CIN: %4\n"
                              "DATE: %5\n"
                              "ADRESSE: %6\n"
                              "TEL: %7\n"
                              "EMAIL: %8\n\n"
                              "📱 Scannez ce QR code pour sélectionner ce client")
        .arg(clientId)
        .arg(nom)
        .arg(prenom)
        .arg(cin)
        .arg(dateNaissance)
        .arg(adresse)
        .arg(telephone)
        .arg(email);
    
    infoLabel->setText(infoText);
    infoLabel->setStyleSheet("color: black; font-size: 11pt; background-color: #f0f0f0; padding: 10px; border-radius: 5px;");
    infoLabel->setWordWrap(true);
    infoLabel->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    layout->addWidget(infoLabel);
    
    // Bouton Fermer
    QPushButton *closeButton = new QPushButton("Fermer", dialog);
    closeButton->setStyleSheet("QPushButton { background-color: #4682B4; color: white; padding: 8px; border-radius: 5px; }");
    connect(closeButton, &QPushButton::clicked, dialog, &QDialog::accept);
    layout->addWidget(closeButton);
    
    dialog->exec();
}

// Fonction pour traiter le QR code scanné et sélectionner le client
void MainWindow::selectClientByQRCode(const QString &qrData)
{
    // Le QR code contient juste l'ID du client
    QString clientId = qrData.trimmed();
    
    // Chercher le client dans le tableau par ID
    for (int row = 0; row < ui->tableWidget_6->rowCount(); row++) {
        QTableWidgetItem *item = ui->tableWidget_6->item(row, 0);
        if (item && item->text() == clientId) {
            // Récupérer les informations du client depuis le tableau
            QString nom = ui->tableWidget_6->item(row, 1) ? ui->tableWidget_6->item(row, 1)->text() : "";
            QString prenom = ui->tableWidget_6->item(row, 2) ? ui->tableWidget_6->item(row, 2)->text() : "";
            QString dateNaissance = ui->tableWidget_6->item(row, 3) ? ui->tableWidget_6->item(row, 3)->text() : "";
            QString adresse = ui->tableWidget_6->item(row, 4) ? ui->tableWidget_6->item(row, 4)->text() : "";
            QString telephone = ui->tableWidget_6->item(row, 5) ? ui->tableWidget_6->item(row, 5)->text() : "";
            QString email = ui->tableWidget_6->item(row, 6) ? ui->tableWidget_6->item(row, 6)->text() : "";
            
            // Sélectionner la ligne
            ui->tableWidget_6->selectRow(row);
            ui->tableWidget_6->setCurrentCell(row, 0);
            
            // Afficher les informations du client
            QString infoMessage = QString("✅ Client Sélectionné!\n\n"
                                         "ID: %1\n"
                                         "Nom: %2 %3\n"
                                         "Date de naissance: %4\n"
                                         "Adresse: %5\n"
                                         "Téléphone: %6\n"
                                         "Email: %7\n\n"
                                         "Vous pouvez maintenant exporter ses informations en PDF.")
                .arg(clientId)
                .arg(nom)
                .arg(prenom)
                .arg(dateNaissance)
                .arg(adresse)
                .arg(telephone)
                .arg(email);
            
            QMessageBox::information(this, "Client Sélectionné", infoMessage);
            return;
        }
    }
    
    QMessageBox::warning(this, "Erreur", 
                        QString("Client avec l'ID %1 non trouvé dans la base de données.").arg(clientId));
}

void MainWindow::on_pushButton_30_clicked()
{
    // Appeler la fonction de génération de QR code (ancien nom du bouton)
    on_btnGenererQRClient_clicked();
}

void MainWindow::on_btnGenererQR_clicked()
{
    // Récupérer le chemin du bureau
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    
    // Permission
    if (!canPerformAction("export")) { showAccessDenied("Génération QR / Export"); return; }

    // Créer le dossier djoPDF sur le bureau s'il n'existe pas
    QString folderPath = desktopPath + "/djoPDF";
    QDir dir;
    if (!dir.exists(folderPath)) {
        if (!dir.mkpath(folderPath)) {
            QMessageBox::critical(this, "Erreur", "Impossible de créer le dossier djoPDF sur le bureau!");
            return;
        }
    }
    
    // Créer le nom du fichier avec la date et l'heure
    QString pdfFileName = "Tous_Les_Clients_" + QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss") + ".pdf";
    QString fileName = folderPath + "/" + pdfFileName;
    
    // Créer le PDF de tous les clients (même code que btnExporterPDF)
    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize::A4);
    pdfWriter.setPageOrientation(QPageLayout::Landscape);
    pdfWriter.setPageMargins(QMarginsF(15, 15, 15, 15));
    
    QPainter painter(&pdfWriter);
    
    if (!painter.isActive()) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier PDF!");
        return;
    }
    
    // Récupérer les données des clients depuis la base de données
    QSqlQueryModel *model = C_Client.afficher();
    
    if (!model || model->rowCount() == 0) {
        QMessageBox::information(this, "Information", "Aucun client à exporter!");
        painter.end();
        return;
    }
    
    // Configuration des polices et dimensions
    QFont titleFont("Arial", 48, QFont::Bold);
    QFont headerFont("Arial", 24, QFont::Bold);
    QFont contentFont("Arial", 20);
    
    int pageWidth = pdfWriter.width();
    int pageHeight = pdfWriter.height();
    int margin = 250;
    int y = margin;
    
    // Titre du document
    painter.setFont(titleFont);
    painter.drawText(QRect(margin, y, pageWidth - 2*margin, 200), 
                     Qt::AlignCenter, "Liste des Clients");
    y += 300;
    
    // Date de génération
    painter.setFont(contentFont);
    QString dateGeneration = "Date de génération: " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss");
    painter.drawText(QRect(margin, y, pageWidth - 4*margin, 100), 
                     Qt::AlignLeft, dateGeneration);
    y += 120;
    
    // Nombre total de clients
    QString totalClients = "Nombre total de clients: " + QString::number(model->rowCount());
    painter.drawText(QRect(margin, y, pageWidth - 2*margin, 100), 
                     Qt::AlignLeft, totalClients);
    y += 150;
    
    // Définir les largeurs des colonnes
    int colWidths[] = {500, 1000, 1000, 900, 950, 1800, 1000, 1450};
    int totalWidth = 0;
    for (int i = 0; i < 8; i++) {
        totalWidth += colWidths[i];
    }
    
    int availableWidth = pageWidth - 2 * margin;
    if (totalWidth > availableWidth) {
        float scale = (float)availableWidth / totalWidth;
        for (int i = 0; i < 8; i++) {
            colWidths[i] = (int)(colWidths[i] * scale);
        }
    }
    
    // En-têtes des colonnes
    painter.setFont(headerFont);
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "CIN" << "Date Naiss." << "Adresse" << "Téléphone" << "Email";
    
    int x = margin;
    int rowHeight = 180;
    
    // Dessiner le rectangle d'en-tête
    painter.fillRect(x, y, availableWidth, rowHeight, QColor(70, 130, 180));
    painter.setPen(QPen(Qt::white, 6));
    
    for (int col = 0; col < headers.size(); col++) {
        painter.drawText(QRect(x + 25, y, colWidths[col] - 50, rowHeight), 
                         Qt::AlignCenter | Qt::AlignVCenter, headers[col]);
        x += colWidths[col];
    }
    
    y += rowHeight;
    painter.setPen(QPen(Qt::black, 3));
    painter.setFont(contentFont);
    
    int rowsPerPage = 7;
    int currentRow = 0;
    
    for (int row = 0; row < model->rowCount(); row++) {
        if (currentRow >= rowsPerPage) {
            pdfWriter.newPage();
            y = margin;
            currentRow = 0;
            
            painter.setFont(headerFont);
            painter.fillRect(margin, y, availableWidth, rowHeight, QColor(70, 130, 180));
            painter.setPen(QPen(Qt::white, 6));
            
            x = margin;
            for (int col = 0; col < headers.size(); col++) {
                painter.drawText(QRect(x + 25, y, colWidths[col] - 50, rowHeight), 
                                 Qt::AlignCenter | Qt::AlignVCenter, headers[col]);
                x += colWidths[col];
            }
            
            y += rowHeight;
            painter.setPen(QPen(Qt::black, 3));
            painter.setFont(contentFont);
        }
        
        x = margin;
        
        if (row % 2 == 0) {
            painter.fillRect(margin, y, availableWidth, rowHeight, QColor(240, 240, 240));
        }
        
        for (int col = 0; col < 8; col++) {
            QVariant data = model->data(model->index(row, col));
            QString text;
            
            if (col == 4 && data.typeId() == QMetaType::QDate) {
                QDate date = data.toDate();
                text = date.toString("dd/MM/yyyy");
            } else if (col == 4) {
                QString dateStr = data.toString();
                QDate date = QDate::fromString(dateStr.split("T").first(), "yyyy-MM-dd");
                if (!date.isValid()) {
                    date = QDate::fromString(dateStr, "dd/MM/yyyy");
                }
                if (date.isValid()) {
                    text = date.toString("dd/MM/yyyy");
                } else {
                    text = dateStr;
                }
            } else {
                text = data.toString();
            }
            
            QFontMetrics fm(contentFont);
            text = fm.elidedText(text, Qt::ElideRight, colWidths[col] - 50);
            
            painter.drawText(QRect(x + 25, y, colWidths[col] - 50, rowHeight), 
                             Qt::AlignCenter | Qt::AlignVCenter, text);
            
            painter.drawLine(x, y, x, y + rowHeight);
            x += colWidths[col];
        }
        
        painter.drawLine(x, y, x, y + rowHeight);
        painter.drawLine(margin, y + rowHeight, margin + availableWidth, y + rowHeight);
        
        y += rowHeight;
        currentRow++;
    }
    
    painter.end();
    
    // Vérifier que le PDF a bien été créé
    QFile pdfFile(fileName);
    if (!pdfFile.exists()) {
        QMessageBox::critical(this, "Erreur", "Le fichier PDF n'a pas pu être créé!\nVérifiez les permissions du dossier.");
        return;
    }
    
    // Afficher un message de confirmation
    qDebug() << "PDF créé avec succès:" << fileName;
    qDebug() << "Taille du fichier:" << pdfFile.size() << "bytes";
    
    // ========== CRÉER UN TABLEAU TEXTE POUR LE QR CODE ==========
    // Construire un tableau texte avec TOUTES les données des clients
    QString qrText = QString("=== BASE CLIENTS ===\n\n");
    qrText += QString("Total: %1 clients\n").arg(model->rowCount());
    qrText += QString("Date: %1\n").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));
    qrText += QString("========================\n\n");

    // Parcourir TOUS les clients et les inclure dans le texte du QR
    int maxClients = model->rowCount();

    for (int row = 0; row < maxClients; row++) {
        // Colonnes: 0=ID, 1=Nom, 2=Prénom, 3=CIN, 4=Date Naiss, 5=Adresse, 6=Tel, 7=Email
        QString id = model->data(model->index(row, 0)).toString();
        QString nom = model->data(model->index(row, 1)).toString();
        QString prenom = model->data(model->index(row, 2)).toString();
        QString cin = model->data(model->index(row, 3)).toString();
        
        // Date de naissance (colonne 4)
        QString dateNaissance;
        QVariant dateData = model->data(model->index(row, 4));
        if (dateData.typeId() == QMetaType::QDate) {
            dateNaissance = dateData.toDate().toString("dd/MM/yyyy");
        } else {
            QString dateStr = dateData.toString();
            QDate date = QDate::fromString(dateStr.split("T").first(), "yyyy-MM-dd");
            if (date.isValid()) {
                dateNaissance = date.toString("dd/MM/yyyy");
            } else {
                dateNaissance = dateStr;
            }
        }
        
        QString adresse = model->data(model->index(row, 5)).toString();
        QString tel = model->data(model->index(row, 6)).toString();
        QString email = model->data(model->index(row, 7)).toString();
        
        qrText += QString("CLIENT #%1\n").arg(row + 1);
        qrText += QString("-------------------\n");
        qrText += QString("ID: %1\n").arg(id);
        qrText += QString("Nom: %1 %2\n").arg(nom).arg(prenom);
        qrText += QString("CIN: %1\n").arg(cin);
        qrText += QString("Ne(e) le: %1\n").arg(dateNaissance);
        qrText += QString("Tel: %1\n").arg(tel);
        qrText += QString("Email: %1\n").arg(email);
        qrText += QString("Adresse: %1\n").arg(adresse);
        qrText += QString("\n");
    }

    qrText += QString("PDF complet disponible sur PC:\n%1").arg(pdfFileName);
    
    // Générer le QR code avec les données
    QImage qrImage = QRCodeGenerator::generateQRCode(qrText, 500);
    
    // Créer une fenêtre de dialogue pour afficher le QR code
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("QR Code - Tous les Clients");
    dialog->setModal(true);
    dialog->resize(700, 850);
    
    QVBoxLayout *layout = new QVBoxLayout(dialog);
    
    // Label pour le titre
    QLabel *titleLabel = new QLabel("QR Code - Base de Données Clients", dialog);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18pt; font-weight: bold; margin: 15px; color: #4682B4;");
    layout->addWidget(titleLabel);
    
    // Label pour afficher le QR code
    QLabel *qrLabel = new QLabel(dialog);
    qrLabel->setPixmap(QPixmap::fromImage(qrImage));
    qrLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(qrLabel);
    
    // Informations sur l'export
    QLabel *infoLabel = new QLabel(QString("<div style='text-align: center; margin: 10px;'>"
                               "<p style='font-size: 14pt; font-weight: bold; color: #0078d7;'>✅ Export PDF Réussi!</p>"
                               "<p style='font-size: 12pt;'>Nombre de clients: <b>%1</b></p>"
                               "<p style='font-size: 11pt; color: #555;'>%2</p>"
                               "<p style='font-size: 10pt; color: #777;'>📂 Bureau/djoPDF/</p>"
                               "<hr style='margin: 10px 0;'>"
                               "<p style='font-size: 12pt; color: #28a745; margin-top: 10px;'>"
                               "<b>📱 QR Code avec TOUTES les coordonnées de TOUS les clients ✓</b>"
                               "</p>"
                               "<p style='font-size: 11pt; color: #333; margin-top: 10px;'>"
                               "<b>Scannez le QR code pour voir:</b><br>"
                               "✓ <b>tous les clients</b> avec toutes les infos<br>"
                               "✓ ID, Nom, Prénom, CIN<br>"
                               "✓ Date de naissance<br>"
                               "✓ Téléphone et Email<br>"
                               "✓ Adresse complète<br>"
                               "</p>"
                               "<p style='font-size: 10pt; color: #555; margin-top: 8px; background: #d4edda; padding: 8px; border-radius: 5px;'>"
                               "💡 <b>ULTRA SIMPLE:</b> Pas besoin de WiFi !<br>"
                               "Toutes les coordonnées dans le QR code<br>"
                               "Compatible iOS et Android"
                               "</p>"
                               "<p style='font-size: 9pt; color: #888; margin-top: 5px;'>"
                               "Le PDF complet (%1 clients) reste sur votre PC"
                               "</p>"
                               "</div>")
                       .arg(model->rowCount())
                       .arg(pdfFileName));
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);
    
    // Boutons
    QHBoxLayout *buttonLayout1 = new QHBoxLayout();
    
    QPushButton *openPdfBtn = new QPushButton("📄 Ouvrir le PDF", dialog);
    openPdfBtn->setStyleSheet("font-size: 11pt; padding: 8px; background-color: #0078d7; color: white; border-radius: 5px;");
    connect(openPdfBtn, &QPushButton::clicked, [fileName]() {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
    });
    buttonLayout1->addWidget(openPdfBtn);
    
    QPushButton *shareBtn = new QPushButton("📤 Partager le PDF", dialog);
    shareBtn->setStyleSheet("font-size: 11pt; padding: 8px; background-color: #28a745; color: white; border-radius: 5px;");
    connect(shareBtn, &QPushButton::clicked, [fileName, folderPath]() {
        // Ouvrir l'explorateur et sélectionner le fichier
        QString command = "explorer /select,\"" + QDir::toNativeSeparators(fileName) + "\"";
        QProcess::startDetached(command);
        
        QMessageBox::information(nullptr, "Partager le PDF", 
                               "Le fichier est sélectionné dans l'explorateur.\n\n"
                               "Pour partager:\n"
                               "• Clic droit → Partager (Windows 10/11)\n"
                               "• Ou glissez-déposez dans WhatsApp Web\n"
                               "• Ou envoyez par email en pièce jointe");
    });
    buttonLayout1->addWidget(shareBtn);
    
    QPushButton *openFolderBtn = new QPushButton("📁 Ouvrir le dossier", dialog);
    openFolderBtn->setStyleSheet("font-size: 11pt; padding: 8px; background-color: #5cb85c; color: white; border-radius: 5px;");
    connect(openFolderBtn, &QPushButton::clicked, [folderPath]() {
        QDesktopServices::openUrl(QUrl::fromLocalFile(folderPath));
    });
    buttonLayout1->addWidget(openFolderBtn);
    
    layout->addLayout(buttonLayout1);
    
    QHBoxLayout *buttonLayout2 = new QHBoxLayout();
    
    QPushButton *saveImageBtn = new QPushButton("💾 Sauvegarder QR Code", dialog);
    saveImageBtn->setStyleSheet("font-size: 11pt; padding: 8px; background-color: #ff9800; color: white; border-radius: 5px;");
    connect(saveImageBtn, &QPushButton::clicked, [qrImage, folderPath, pdfFileName]() {
        // Créer une copie pour pouvoir utiliser replace()
        QString imageFileName = pdfFileName;
        imageFileName.replace(".pdf", "_QRCode.png");
        QString imagePath = folderPath + "/" + imageFileName;
        if (qrImage.save(imagePath)) {
            QMessageBox::information(nullptr, "Succès", "QR Code sauvegardé:\n" + imagePath);
        } else {
            QMessageBox::warning(nullptr, "Erreur", "Impossible de sauvegarder l'image!");
        }
    });
    buttonLayout2->addWidget(saveImageBtn);
    
    QPushButton *closeBtn = new QPushButton("Fermer", dialog);
    closeBtn->setStyleSheet("font-size: 11pt; padding: 8px; background-color: #4682B4; color: white; border-radius: 5px;");
    connect(closeBtn, &QPushButton::clicked, dialog, &QDialog::accept);
    buttonLayout2->addWidget(closeBtn);
    
    layout->addLayout(buttonLayout2);
    
    dialog->setLayout(layout);
    dialog->exec();
    
    delete dialog;
}

// ==================== MÉTIERS / GPS CLIENT ====================
void MainWindow::on_btnShowClientOnMap_clicked()
{
    if (!ui->lineEditClientMapSearch) {
        QMessageBox::warning(this, "GPS Client", "Champ de recherche introuvable dans l'interface.");
        return;
    }

    QString searchText = ui->lineEditClientMapSearch->text().trimmed();
    if (searchText.isEmpty()) {
        QMessageBox::warning(this, "GPS Client", "Veuillez saisir l'ID ou le nom du client.");
        return;
    }

    if (!ui->tableWidget_6) {
        QMessageBox::warning(this, "GPS Client", "Le tableau des clients n'est pas disponible.");
        return;
    }

    // Rechercher d'abord par ID exact (colonne 0)
    int foundRow = -1;
    for (int row = 0; row < ui->tableWidget_6->rowCount(); ++row) {
        QTableWidgetItem *idItem = ui->tableWidget_6->item(row, 0);
        if (idItem && idItem->text().trimmed().compare(searchText, Qt::CaseInsensitive) == 0) {
            foundRow = row;
            break;
        }
    }

    // Si non trouvé par ID, rechercher par nom / prénom (colonnes 1 et 2)
    if (foundRow < 0) {
        for (int row = 0; row < ui->tableWidget_6->rowCount(); ++row) {
            QTableWidgetItem *nomItem = ui->tableWidget_6->item(row, 1);
            QTableWidgetItem *prenomItem = ui->tableWidget_6->item(row, 2);
            QString nom = nomItem ? nomItem->text() : "";
            QString prenom = prenomItem ? prenomItem->text() : "";
            QString fullName = nom + " " + prenom;

            if ((nomItem && nom.contains(searchText, Qt::CaseInsensitive)) ||
                (!fullName.trimmed().isEmpty() && fullName.contains(searchText, Qt::CaseInsensitive))) {
                foundRow = row;
                break;
            }
        }
    }

    if (foundRow < 0) {
        QMessageBox::warning(this, "GPS Client", "Aucun client trouvé avec cet ID ou ce nom.");
        return;
    }

    // Récupérer les infos de base depuis le tableau
    QString idStr = ui->tableWidget_6->item(foundRow, 0) ? ui->tableWidget_6->item(foundRow, 0)->text() : "";
    QString nom = ui->tableWidget_6->item(foundRow, 1) ? ui->tableWidget_6->item(foundRow, 1)->text() : "";
    QString prenom = ui->tableWidget_6->item(foundRow, 2) ? ui->tableWidget_6->item(foundRow, 2)->text() : "";
    QString adresse = ui->tableWidget_6->item(foundRow, 5) ? ui->tableWidget_6->item(foundRow, 5)->text() : "";

    bool okId = false;
    int id = idStr.toInt(&okId);
    if (!okId || id <= 0) {
        QMessageBox::warning(this, "GPS Client", "ID du client invalide.");
        return;
    }

    // Lire latitude / longitude depuis la base (colonnes LATITUDE et LONGITUDE)
    bool hasLatitudeLongitude = false;
    {
        QSqlQuery colQuery;
        colQuery.prepare("SELECT COLUMN_NAME FROM USER_TAB_COLUMNS WHERE TABLE_NAME = 'CLIENT' AND COLUMN_NAME IN ('LATITUDE','LONGITUDE')");
        if (colQuery.exec()) {
            bool hasLat = false;
            bool hasLon = false;
            while (colQuery.next()) {
                QString colName = colQuery.value(0).toString();
                if (colName == "LATITUDE")
                    hasLat = true;
                else if (colName == "LONGITUDE")
                    hasLon = true;
            }
            hasLatitudeLongitude = (hasLat && hasLon);
        }
    }

    if (!hasLatitudeLongitude) {
        if (m_clientMapWidget) {
            m_clientMapWidget->setClientInfo(nom + " " + prenom, adresse, 0.0, 0.0, false);
        }
        QMessageBox::warning(this, "GPS Client",
                             "Les colonnes LATITUDE et LONGITUDE n'existent pas dans la table CLIENT.\n"
                             "Ajoutez-les dans la base pour utiliser la fonction GPS.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT LATITUDE, LONGITUDE FROM CLIENT WHERE ID_CLIENT = :id");
    query.bindValue(":id", id);

    if (!query.exec() || !query.next()) {
        QMessageBox::warning(this, "GPS Client", "Impossible de récupérer la position GPS pour ce client (colonnes LATITUDE / LONGITUDE).");
        return;
    }

    QVariant latVar = query.value(0);
    QVariant lonVar = query.value(1);

    if (latVar.isNull() || lonVar.isNull()) {
        if (m_clientMapWidget) {
            m_clientMapWidget->setClientInfo(nom + " " + prenom, adresse, 0.0, 0.0, false);
        }
        QMessageBox::information(this, "GPS Client", "Ce client n'a pas encore de coordonnées GPS (LATITUDE / LONGITUDE) enregistrées.");
        return;
    }

    double latitude = latVar.toDouble();
    double longitude = lonVar.toDouble();

    if (m_clientMapWidget) {
        m_clientMapWidget->setClientInfo(nom + " " + prenom, adresse, latitude, longitude, true);
    }

    // Sélectionner la ligne trouvée dans le tableau client (pour cohérence UI)
    ui->tableWidget_6->selectRow(foundRow);
    ui->tableWidget_6->setCurrentCell(foundRow, 0);

    QString msg = QString("Position GPS du client\n\n"
                          "ID: %1\n"
                          "Nom: %2 %3\n"
                          "Adresse: %4\n\n"
                          "Latitude: %5\n"
                          "Longitude: %6")
                     .arg(idStr)
                     .arg(nom)
                     .arg(prenom)
                     .arg(adresse)
                     .arg(QString::number(latitude, 'f', 6))
                     .arg(QString::number(longitude, 'f', 6));

    QMessageBox::information(this, "GPS Client (offline)", msg);
}

// ==================== GESTION EMPLOYÉS ====================
bool MainWindow::initDatabaseEmploye()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "❌ Erreur: Base de données non ouverte";
        return false;
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
}

bool MainWindow::saveEmploye(const Employe& emp)
{
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYE (ID_EMPLOYE, NOM_EMPLOYE, PRENOM, POSTE, MOT_DE_PASSEE, TEL_EMPLOYE, DATE_EMBAUCHE) "
                  "VALUES (:ID_EMPLOYE, :NOM_EMPLOYE, :PRENOM, :POSTE, :MOT_DE_PASSEE, :TEL_EMPLOYE, :DATE_EMBAUCHE)");
    query.bindValue(":ID_EMPLOYE", emp.ID_EMPLOYE());
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

int MainWindow::getNextIdEmploye() const
{
    QSqlQuery query("SELECT NVL(MAX(ID_EMPLOYE),0)+1 FROM EMPLOYE");
    if (query.next())
        return query.value(0).toInt();
    return 1;
}

void MainWindow::refreshTableEmploye()
{
    updateTableEmploye();
}

void MainWindow::updateTableEmploye()
{
    updateTableEmploye(QList<Employe>());
}

void MainWindow::updateTableEmploye(const QList<Employe>& employesToShow)
{
    if (!ui->tableWidget_5) return;
    const QList<Employe>& list = employesToShow.isEmpty() ? m_employes : employesToShow;
    ui->tableWidget_5->setRowCount(list.size());
    for (int i = 0; i < list.size(); ++i) {
        const Employe& emp = list[i];
        ui->tableWidget_5->setItem(i, 0, new QTableWidgetItem(QString::number(emp.ID_EMPLOYE())));
        ui->tableWidget_5->setItem(i, 1, new QTableWidgetItem(emp.NOM_EMPLOYE()));
        ui->tableWidget_5->setItem(i, 2, new QTableWidgetItem(emp.PRENOM()));
        ui->tableWidget_5->setItem(i, 3, new QTableWidgetItem(emp.POSTE()));
        ui->tableWidget_5->setItem(i, 4, new QTableWidgetItem(emp.MOT_DE_PASSEE()));
        ui->tableWidget_5->setItem(i, 5, new QTableWidgetItem(emp.TEL_EMPLOYE()));
        ui->tableWidget_5->setItem(i, 6, new QTableWidgetItem(emp.DATE_EMBAUCHE().toString("yyyy-MM-dd")));
    }
}

void MainWindow::clearFieldsEmploye()
{
    if (ui->lineEdit_24) ui->lineEdit_24->clear();
    if (ui->lineEdit_25) ui->lineEdit_25->clear();
    if (ui->lineEdit_26) ui->lineEdit_26->clear();
    if (ui->lineEdit_27) ui->lineEdit_27->clear();
    if (ui->lineEdit_28) ui->lineEdit_28->clear();
    if (ui->lineEdit_29) ui->lineEdit_29->clear();
    if (ui->dateEdit_5) ui->dateEdit_5->setDate(QDate::currentDate());
    if (ui->lineEdit_30) ui->lineEdit_30->clear();
    if (ui->lineEdit_31) ui->lineEdit_31->clear();
}

void MainWindow::on_pushButton_26_clicked()
{
    // Empêche les moniteurs d'ajouter/modifier des employés
    if (m_isModifyModeEmploye) {
        if (!canPerformAction("modify_employe")) { showAccessDenied("Modifier employé"); return; }
    } else {
        if (!canPerformAction("add_employe")) { showAccessDenied("Ajouter employé"); return; }
    }
    QString NOM_EMPLOYE = ui->lineEdit_25->text();
    QString PRENOM = ui->lineEdit_26->text();
    QString POSTE = ui->lineEdit_27->text();
    QString MOT_DE_PASSEE = ui->lineEdit_28->text();
    QString rawPhone = ui->lineEdit_29->text().trimmed();
    QString TEL_EMPLOYE = "+216" + rawPhone;
    QDate DATE_EMBAUCHE = ui->dateEdit_5->date();

    // Basic empty-field checks (note: rawPhone is phone without country code)
    if (NOM_EMPLOYE.isEmpty() || PRENOM.isEmpty() || POSTE.isEmpty() || MOT_DE_PASSEE.isEmpty() || rawPhone.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs !");
        return;
    }

    // Input validation
    // Names and poste: letters, spaces, apostrophe, hyphen (unicode letters allowed)
    QRegularExpression nameRe(QStringLiteral("^[A-Za-zÀ-ÖØ-öø-ÿ\s'-]+$"));
    if (!nameRe.match(NOM_EMPLOYE).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le nom ne doit contenir que des lettres, espaces, '-' ou '\''.");
        return;
    }
    if (!nameRe.match(PRENOM).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le prénom ne doit contenir que des lettres, espaces, '-' ou '\''.");
        return;
    }
    if (!nameRe.match(POSTE).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le poste ne doit contenir que des lettres et espaces.");
        return;
    }

    // Phone: exactly 8 digits
    QRegularExpression phoneRe(QStringLiteral("^\\d{8}$"));
    if (!phoneRe.match(rawPhone).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone doit contenir exactement 8 chiffres (sans l'indicatif). Exemple: 12345678");
        return;
    }

    // Cas 1 : modification
    if (m_isModifyModeEmploye) {
        bool ok;
        int ID_EMPLOYE = ui->lineEdit_24->text().toInt(&ok);
        if (!ok || ID_EMPLOYE <= 0) {
            QMessageBox::warning(this, "Erreur", "ID d'employé invalide !");
            return;
        }

        Employe emp(ID_EMPLOYE, NOM_EMPLOYE, PRENOM, POSTE, MOT_DE_PASSEE, TEL_EMPLOYE, DATE_EMBAUCHE);

        if (updateEmploye(ID_EMPLOYE, emp)) {
            QMessageBox::information(this, "Succès", "Employé modifié avec succès !");
            loadEmployes();
            refreshTableEmploye();
            clearFieldsEmploye();
            ui->pushButton_26->setText("Ajouter");
            m_isModifyModeEmploye = false;
        } else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de la modification de l'employé !");
        }
    }
    // Cas 2 : ajout normal
    else {
        Employe emp(getNextIdEmploye(), NOM_EMPLOYE, PRENOM, POSTE, MOT_DE_PASSEE, TEL_EMPLOYE, DATE_EMBAUCHE);

        if (saveEmploye(emp)) {
            QMessageBox::information(this, "Succès", "Employé ajouté avec succès !");
            loadEmployes();
            refreshTableEmploye();
            clearFieldsEmploye();
        } else {
            QMessageBox::warning(this, "Erreur", "Erreur lors de l'insertion de l'employé !");
        }
    }
}

void MainWindow::on_pushButton_27_clicked()
{
    if (!canPerformAction("view_employe")) { showAccessDenied("Afficher employé"); return; }
    QString critere = ui->lineEdit_30->text().trimmed();

    if (critere.isEmpty()) {
        refreshTableEmploye();
        return;
    }

    QList<Employe> filtered;
    for (const Employe& emp : m_employes) {
        if (QString::number(emp.ID_EMPLOYE()).contains(critere, Qt::CaseInsensitive) ||
            emp.NOM_EMPLOYE().contains(critere, Qt::CaseInsensitive) ||
            emp.PRENOM().contains(critere, Qt::CaseInsensitive) ||
            emp.POSTE().contains(critere, Qt::CaseInsensitive)) {
            filtered.append(emp);
        }
    }

    if (filtered.isEmpty()) {
        QMessageBox::information(this, "Recherche", "Aucun employé trouvé pour: " + critere);
    }

    updateTableEmploye(filtered);
}

void MainWindow::on_pushButton_28_clicked()
{
    if (!canPerformAction("delete_employe")) { showAccessDenied("Supprimer employé"); return; }
    QString idText = ui->lineEdit_31->text().trimmed();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer l'ID de l'employé à supprimer !");
        return;
    }

    bool ok;
    int ID_EMPLOYE = idText.toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Erreur", "L'ID doit être un nombre valide !");
        return;
    }

    // Vérifie directement dans la base de données
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT ID_EMPLOYE FROM EMPLOYE WHERE ID_EMPLOYE = :ID_EMPLOYE");
    checkQuery.bindValue(":ID_EMPLOYE", ID_EMPLOYE);

    if (!checkQuery.exec()) {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la vérification dans la base : " + checkQuery.lastError().text());
        return;
    }

    if (!checkQuery.next()) {
        QMessageBox::warning(this, "Erreur", "Aucun employé trouvé avec cet ID !");
        return;
    }

    // Confirmation avant suppression
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "Confirmation de suppression",
        QString("Voulez-vous vraiment supprimer l'employé #%1 ?").arg(ID_EMPLOYE),
        QMessageBox::Yes | QMessageBox::No
        );

    if (reply == QMessageBox::No) {
        return;
    }

    // Supprimer de la base
    if (deleteEmploye(ID_EMPLOYE)) {
        QMessageBox::information(this, "Succès", QString("Employé #%1 supprimé avec succès !").arg(ID_EMPLOYE));

        // Recharge immédiatement la liste depuis la base
        loadEmployes();
        refreshTableEmploye();

        // Vide la zone de saisie après suppression
        ui->lineEdit_31->clear();
    } else {
        QMessageBox::warning(this, "Erreur", "Erreur lors de la suppression de l'employé !");
    }
}

void MainWindow::on_tableWidget_5_itemSelectionChanged()
{
    // Si plus rien n'est sélectionné → retour au mode "ajouter"
    if (ui->tableWidget_5->selectedItems().isEmpty()) {
        m_isModifyModeEmploye = false;
        ui->pushButton_26->setText("Ajouter");
        clearFieldsEmploye();
        return;
    }

    // Sinon → on charge les infos pour modification
    int row = ui->tableWidget_5->currentRow();
    if (row < 0 || row >= m_employes.size())
        return;

    const Employe& emp = m_employes[row];
    ui->lineEdit_24->setText(QString::number(emp.ID_EMPLOYE()));
    ui->lineEdit_25->setText(emp.NOM_EMPLOYE());
    ui->lineEdit_26->setText(emp.PRENOM());
    ui->lineEdit_27->setText(emp.POSTE());
    ui->lineEdit_28->setText(emp.MOT_DE_PASSEE());
    ui->lineEdit_29->setText(emp.TEL_EMPLOYE().remove("+216"));
    ui->dateEdit_5->setDate(emp.DATE_EMBAUCHE());

    m_isModifyModeEmploye = true;
    ui->pushButton_26->setText("Modifier");
}

void MainWindow::on_comboBox_8_currentIndexChanged(int index)
{
    if (index == 0) {
        std::sort(m_employes.begin(), m_employes.end(), [](const Employe& a, const Employe& b) { return a.POSTE() < b.POSTE(); });
    } else if (index == 1) {
        std::sort(m_employes.begin(), m_employes.end(), [](const Employe& a, const Employe& b) { return a.ID_EMPLOYE() < b.ID_EMPLOYE(); });
    }
    refreshTableEmploye();
}

void MainWindow::on_exportPdfBtn_clicked()
{
    if (!canPerformAction("export_employe")) { showAccessDenied("Exporter employés"); return; }

    if (m_employes.isEmpty()) {
        QMessageBox::information(this, "Export PDF", "Aucun employé à exporter.");
        return;
    }

    // Default filename on Desktop
    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString defaultFileName = QString("Tous_les_employes_%1.pdf").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    QString defaultPath = desktopPath + "/" + defaultFileName;

    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", defaultPath, "Fichiers PDF (*.pdf);;Tous les fichiers (*)");
    if (fileName.isEmpty()) return;
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) fileName += ".pdf";

    QPageSize::PageSizeId pageSize = QPageSize::A4;
    int dpi = 150;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(pageSize);
    pdfWriter.setPageOrientation(QPageLayout::Landscape);
    pdfWriter.setResolution(dpi);

    QPainter painter(&pdfWriter);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier PDF!");
        return;
    }

    QFont titleFont("Arial", 28, QFont::Bold);
    QFont headerFont("Arial", 14, QFont::Bold);
    QFont contentFont("Arial", 12);

    int pageWidth = pdfWriter.width();
    int margin = 30;
    int y = margin;

    painter.setFont(titleFont);
    painter.drawText(QRect(margin, y, pageWidth - 5*margin, 100), Qt::AlignCenter, "Liste des Employés");
    y += 50;

    painter.setFont(contentFont);
    QString dateGeneration = "Date de génération: " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss");
    painter.drawText(QRect(margin, y, pageWidth - 5*margin, 150), Qt::AlignLeft, dateGeneration);
    y += 30;

    QString total = "Nombre total d'employés: " + QString::number(m_employes.size());
    painter.drawText(QRect(margin, y, pageWidth - 5*margin, 150), Qt::AlignLeft, total);
    y += 30;

    // Make some extra vertical space before the table so it appears lower on the page
    y += 20; // increase to move the table down

    // Column widths (adjust as needed)
    int colCount = 6; // ID, Nom, Prénom, Poste, Téléphone, Date d'embauche
    QVector<int> colWidths = {150, 700, 700, 600, 900, 700};
    int totalWidth = 0; for (int w : colWidths) totalWidth += w;
    int availableWidth = pageWidth - 2*margin;
    if (totalWidth > availableWidth) {
        double scale = double(availableWidth) / totalWidth;
        for (int &w : colWidths) w = int(w * scale);
    }

    // Draw headers
    painter.setFont(headerFont);
    int x = margin;
    QStringList headers = {"ID", "Nom", "Prénom", "Poste", "Téléphone", "Date emb."};
    for (int i = 0; i < colCount; ++i) {
        painter.drawText(QRect(x, y, colWidths[i], 24), Qt::AlignLeft | Qt::AlignVCenter, headers[i]);
        x += colWidths[i];
    }
    y += 30;
    painter.setFont(contentFont);

    // Rows
    int rowHeight = 22;
    for (int i = 0; i < m_employes.size(); ++i) {
        if (y + rowHeight > pdfWriter.height() - margin) {
            pdfWriter.newPage();
            // Reserve space on a new page for header/title area so rows start at similar vertical position
            y = margin + 80;
        }

        const Employe &emp = m_employes.at(i);
        x = margin;
        QStringList cols;
        cols << QString::number(emp.ID_EMPLOYE())
             << emp.NOM_EMPLOYE()
             << emp.PRENOM()
             << emp.POSTE()
             << emp.TEL_EMPLOYE()
             << emp.DATE_EMBAUCHE().toString("yyyy-MM-dd");

        for (int c = 0; c < colCount; ++c) {
            painter.drawText(QRect(x, y, colWidths[c], rowHeight), Qt::AlignLeft | Qt::AlignVCenter, cols.value(c));
            x += colWidths[c];
        }
        y += rowHeight + 6;
    }

    painter.end();

    QFile pdfFile(fileName);
    if (!pdfFile.exists()) {
        QMessageBox::critical(this, "Erreur", "Le fichier PDF n'a pas pu être créé! Vérifiez les permissions du dossier.");
        return;
    }

    QMessageBox::information(this, "Export PDF", QString("Exportation PDF réussie!\nFichier: %1\nEmployés: %2").arg(fileName).arg(m_employes.size()));
}

// ==================== GESTION MONITEURS ====================
void MainWindow::refreshTableMoniteur()
{
    QSqlQueryModel *model = M.afficher();
    
    if (!model) return;
    
    int rowCount = model->rowCount();
    ui->moniteurTable->setRowCount(rowCount);
    ui->moniteurTable->setColumnCount(7); // ID, CIN, Nom, Prenom, Tel, Email, Permis
    
    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < 7; ++col) {
            QModelIndex index = model->index(row, col);
            QString value = model->data(index).toString();
            ui->moniteurTable->setItem(row, col, new QTableWidgetItem(value));
        }
    }
}

void MainWindow::clearFieldsMoniteur()
{
    ui->idmod->clear();
    ui->cinmod->clear();
    ui->nommod->clear();
    ui->prenommod->clear();
    ui->telmod->clear();
    ui->emailmod->clear();
    ui->permismod->clear();
}

void MainWindow::on_addContratBtn_clicked()
{
    if (!canPerformAction("add_moniteur")) { showAccessDenied("Ajouter moniteur"); return; }
    QString cin = ui->cinmod->text().trimmed();
    QString nom = ui->nommod->text().trimmed();
    QString prenom = ui->prenommod->text().trimmed();
    QString tel = ui->telmod->text().trimmed();
    QString email = ui->emailmod->text().trimmed();
    QString permis = ui->permismod->text().trimmed();

    // Validation
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Attention", "CIN ne peut pas être vide!");
        return;
    }
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Nom ne peut pas être vide!");
        return;
    }
    if (prenom.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Prénom ne peut pas être vide!");
        return;
    }
    if (tel.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Téléphone ne peut pas être vide!");
        return;
    }
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Email ne peut pas être vide!");
        return;
    }
    if (permis.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Permis ne peut pas être vide!");
        return;
    }

    // Créer un nouveau moniteur (ID sera généré automatiquement par la BDD)
    Moniteur m("", cin, nom, prenom, tel, email, permis);
    bool test = m.ajouter();

    if (test) {
        QMessageBox::information(this, "Succès", "✓ Moniteur ajouté avec succès.");
        refreshTableMoniteur();
        clearFieldsMoniteur();
    } else {
        QMessageBox::critical(this, "Erreur", "✗ Échec de l'ajout.");
    }
}

void MainWindow::on_updateContratBtn_clicked()
{
    if (!canPerformAction("modify_moniteur")) { showAccessDenied("Modifier moniteur"); return; }
    QString id = ui->idmod->text().trimmed();
    QString cin = ui->cinmod->text().trimmed();
    QString nom = ui->nommod->text().trimmed();
    QString prenom = ui->prenommod->text().trimmed();
    QString tel = ui->telmod->text().trimmed();
    QString email = ui->emailmod->text().trimmed();
    QString permis = ui->permismod->text().trimmed();

    // Validation
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un moniteur à modifier!");
        return;
    }
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Attention", "CIN ne peut pas être vide!");
        return;
    }
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Nom ne peut pas être vide!");
        return;
    }
    if (prenom.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Prénom ne peut pas être vide!");
        return;
    }
    if (tel.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Téléphone ne peut pas être vide!");
        return;
    }
    if (email.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Email ne peut pas être vide!");
        return;
    }
    if (permis.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Permis ne peut pas être vide!");
        return;
    }

    Moniteur m(id, cin, nom, prenom, tel, email, permis);
    bool test = m.modifier(id);

    if (test) {
        QMessageBox::information(this, "Succès", "Modification effectuée ✓");
        refreshTableMoniteur();
        clearFieldsMoniteur();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification ✗");
    }
}

void MainWindow::on_deleteContratBtn_clicked()
{
    if (!canPerformAction("delete_moniteur")) { showAccessDenied("Supprimer moniteur"); return; }
    QString id = ui->idmod->text().trimmed();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un moniteur à supprimer!");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer ce moniteur?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        bool test = M.supprimer(id);
        if (test) {
            QMessageBox::information(this, "Succès", "Moniteur supprimé avec succès ✓");
            refreshTableMoniteur();
            clearFieldsMoniteur();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression ✗");
        }
    }
}

void MainWindow::on_moniteurTable_cellClicked(int row, int column)
{
    // Récupère les données de la ligne cliquée et les met dans les champs
    // Utiliser directement les items du QTableWidget (pas QSqlQueryModel)
    QTableWidgetItem *idItem      = ui->moniteurTable->item(row, 0);
    QTableWidgetItem *cinItem     = ui->moniteurTable->item(row, 1);
    QTableWidgetItem *nomItem     = ui->moniteurTable->item(row, 2);
    QTableWidgetItem *prenomItem  = ui->moniteurTable->item(row, 3);
    QTableWidgetItem *telItem     = ui->moniteurTable->item(row, 4);
    QTableWidgetItem *emailItem   = ui->moniteurTable->item(row, 5);
    QTableWidgetItem *permisItem  = ui->moniteurTable->item(row, 6);

    if (idItem) {
        ui->idmod->setText(idItem->text());
        ui->cinmod->setText(cinItem ? cinItem->text() : "");
        ui->nommod->setText(nomItem ? nomItem->text() : "");
        ui->prenommod->setText(prenomItem ? prenomItem->text() : "");
        ui->telmod->setText(telItem ? telItem->text() : "");
        ui->emailmod->setText(emailItem ? emailItem->text() : "");
        ui->permismod->setText(permisItem ? permisItem->text() : "");
    }
}

void MainWindow::on_Modifiermoniteur_clicked()
{
    // Récupère le ID depuis la table sélectionnée
    if (ui->moniteurTable->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un moniteur dans le tableau!");
        return;
    }

    int row = ui->moniteurTable->currentRow();
    
    // Utiliser directement les items du QTableWidget (pas QSqlQueryModel)
    QTableWidgetItem *idItem = ui->moniteurTable->item(row, 0);
    QTableWidgetItem *cinItem = ui->moniteurTable->item(row, 1);
    QTableWidgetItem *nomItem = ui->moniteurTable->item(row, 2);
    QTableWidgetItem *prenomItem = ui->moniteurTable->item(row, 3);
    QTableWidgetItem *telItem = ui->moniteurTable->item(row, 4);
    QTableWidgetItem *emailItem = ui->moniteurTable->item(row, 5);
    QTableWidgetItem *permisItem = ui->moniteurTable->item(row, 6);

    if (idItem) {
        ui->idmod->setText(idItem->text());
        ui->cinmod->setText(cinItem ? cinItem->text() : "");
        ui->nommod->setText(nomItem ? nomItem->text() : "");
        ui->prenommod->setText(prenomItem ? prenomItem->text() : "");
        ui->telmod->setText(telItem ? telItem->text() : "");
        ui->emailmod->setText(emailItem ? emailItem->text() : "");
        ui->permismod->setText(permisItem ? permisItem->text() : "");
        
        // Passer au tab Ajout pour la modification
        ui->contratsTabWidget->setCurrentIndex(1);
    }
}

void MainWindow::on_supprimermoniteur_clicked()
{
    // Récupère le ID depuis la table sélectionnée
    if (ui->moniteurTable->selectedItems().isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un moniteur dans le tableau!");
        return;
    }

    int row = ui->moniteurTable->currentRow();
    
    // Utiliser directement les items du QTableWidget
    QTableWidgetItem *idItem = ui->moniteurTable->item(row, 0);
    
    if (idItem) {
        QString id = idItem->text();

        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Confirmation",
                                      "Voulez-vous vraiment supprimer ce moniteur?",
                                      QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            bool test = M.supprimer(id);
            if (test) {
                QMessageBox::information(this, "Succès", "Moniteur supprimé avec succès ✓");
                refreshTableMoniteur();
                clearFieldsMoniteur();
            } else {
                QMessageBox::critical(this, "Erreur", "Échec de la suppression ✗");
            }
        }
    }
}

void MainWindow::on_searchBtn_clicked()
{
    QString text = ui->searchEdit->text().trimmed();

    if (text.isEmpty()) {
        // Si vide, afficher tout
        refreshTableMoniteur();
        return;
    }

    QSqlQueryModel *model = M.rechercher(text);
    if (!model) return;

    int rowCount = model->rowCount();
    ui->moniteurTable->setRowCount(rowCount);
    ui->moniteurTable->setColumnCount(7);

    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < 7; ++col) {
            QString value = model->data(model->index(row, col)).toString();
            ui->moniteurTable->setItem(row, col, new QTableWidgetItem(value));
        }
    }

    delete model;
}

void MainWindow::afficherStatistiqueClient()
{
    if (!canPerformAction("statistics")) { showAccessDenied("Statistiques"); return; }
    qDebug() << "=== Début afficherStatistiqueClient ===";
    
    if (!ui->tabWidget_3) {
        qDebug() << "ERREUR: tabWidget_3 est NULL!";
        return;
    }
    
    // Find the Statistiques tab
    QWidget *statTab = nullptr;
    for (int i = 0; i < ui->tabWidget_3->count(); ++i) {
        QString tabName = ui->tabWidget_3->tabText(i);
        qDebug() << "Tab" << i << ":" << tabName;
        if (tabName == "Statistiques" || tabName.contains("Stat", Qt::CaseInsensitive)) {
            statTab = ui->tabWidget_3->widget(i);
            qDebug() << "Found Statistiques tab at index" << i;
            break;
        }
    }
    
    if (!statTab) {
        qDebug() << "ERREUR: Statistiques tab not found!";
        return;
    }
    
    // Get the layout from the tab
    layoutChartClient = qobject_cast<QVBoxLayout*>(statTab->layout());
    if (!layoutChartClient) {
        qDebug() << "ERREUR: Layout not found in tab!";
        return;
    }
    
    // Clear existing widgets from layout
    QLayoutItem *item;
    while ((item = layoutChartClient->takeAt(0)) != nullptr) {
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }
    qDebug() << "Layout cleared";

    qDebug() << "Exécution de la requête SQL...";
    // Requête SQL pour récupérer les dates de naissance
    QSqlQuery query;
    query.prepare("SELECT DATE_NAISSANCE FROM CLIENT WHERE DATE_NAISSANCE IS NOT NULL");
    if (!query.exec()) {
        qDebug() << "ERREUR SQL:" << query.lastError().text();
        
        // Show error in UI
        QLabel *errorLabel = new QLabel("Erreur SQL: " + query.lastError().text(), statTab);
        errorLabel->setStyleSheet("color: red; font-size: 14px; padding: 20px;");
        errorLabel->setWordWrap(true);
        layoutChartClient->addWidget(errorLabel);
        return;
    }

    // Compter les clients par catégorie d'âge
    QMap<QString, int> ageGroups;
    ageGroups["18-25"] = 0;
    ageGroups["26-35"] = 0;
    ageGroups["36-45"] = 0;
    ageGroups["46-55"] = 0;
    ageGroups["56-65"] = 0;
    ageGroups["66+"] = 0;

    QDate today = QDate::currentDate();
    int totalClients = 0;

    while (query.next()) {
        QDate birthDate = query.value(0).toDate();
        if (birthDate.isValid()) {
            // Calculer l'âge
            int age = today.year() - birthDate.year();
            if (today.month() < birthDate.month() || 
                (today.month() == birthDate.month() && today.day() < birthDate.day())) {
                age--;
            }

            qDebug() << "Client trouvé - Date:" << birthDate << "Âge:" << age;

            // Vérifier que l'âge est réaliste (entre 0 et 120 ans)
            if (age >= 0 && age <= 120) {
                // Classer par catégorie
                if (age >= 18 && age <= 25) {
                    ageGroups["18-25"]++;
                    totalClients++;
                } else if (age >= 26 && age <= 35) {
                    ageGroups["26-35"]++;
                    totalClients++;
                } else if (age >= 36 && age <= 45) {
                    ageGroups["36-45"]++;
                    totalClients++;
                } else if (age >= 46 && age <= 55) {
                    ageGroups["46-55"]++;
                    totalClients++;
                } else if (age >= 56 && age <= 65) {
                    ageGroups["56-65"]++;
                    totalClients++;
                } else if (age > 65 && age <= 120) {
                    ageGroups["66+"]++;
                    totalClients++;
                }
            } else {
                qDebug() << "Âge invalide (hors limites):" << age;
            }
        }
    }
    
    qDebug() << "Total clients:" << totalClients;
    
    if (totalClients == 0) {
        qDebug() << "Aucune donnée statistique";
        
        // Show message in UI instead of popup
        QLabel *noDataLabel = new QLabel("Aucune donnée statistique disponible!\n\nVeuillez ajouter des clients avec des dates de naissance.", statTab);
        noDataLabel->setStyleSheet("color: orange; font-size: 16px; padding: 20px;");
        noDataLabel->setAlignment(Qt::AlignCenter);
        noDataLabel->setWordWrap(true);
        layoutChartClient->addWidget(noDataLabel);
        return;
    }

    qDebug() << "Création du graphique...";
    // Créer le graphique
    QBarSet *set = new QBarSet("Nombre de clients");
    set->setColor(QColor(52, 152, 219));
    QStringList categories;

    // Ajouter les données dans l'ordre
    categories << "18-25" << "26-35" << "36-45" << "46-55" << "56-65" << "66+";
    *set << ageGroups["18-25"] << ageGroups["26-35"] << ageGroups["36-45"] 
         << ageGroups["46-55"] << ageGroups["56-65"] << ageGroups["66+"];
    
    QBarSeries *series = new QBarSeries();
    series->append(set);
    
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Distribution des Clients par Tranche d'Âge");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setTitleText("Tranche d'âge");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setTitleText("Nombre de clients");
    axisY->setLabelFormat("%d");
    axisY->setRange(0, qMax(10, totalClients + 2));
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    series->setLabelsVisible(true);
    
    QChartView *chartView = new QChartView(chart, statTab);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(600, 400);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layoutChartClient->addWidget(chartView);
    
    // Force update
    statTab->update();
    chartView->update();
    
    qDebug() << "Graphique ajouté avec succès!";
    qDebug() << "ChartView size:" << chartView->size();
    qDebug() << "StatTab size:" << statTab->size();
}

void MainWindow::afficherStatistiqueEmploye()
{
    if (!canPerformAction("statistics")) { showAccessDenied("Statistiques"); return; }

    // Prefer the employee's internal TabWidget (tabWidget_2) and its "Statistiques" tab
    QWidget *statTab = nullptr;
    if (ui->tabWidget_2) {
        for (int i = 0; i < ui->tabWidget_2->count(); ++i) {
            QString tabName = ui->tabWidget_2->tabText(i);
            if (tabName == "Statistiques" || tabName.contains("Stat", Qt::CaseInsensitive)) {
                statTab = ui->tabWidget_2->widget(i);
                break;
            }
        }
    }

    if (!statTab) {
        // Fallback: try to find a widget on the stack (previous behaviour)
        if (ui->stack && ui->stack->count() > 1) statTab = ui->stack->widget(1);
    }

    if (!statTab) {
        qDebug() << "ERREUR: Statistiques (employe) tab introuvable!";
        return;
    }

    QVBoxLayout *layoutChartEmploye = qobject_cast<QVBoxLayout*>(statTab->layout());
    if (!layoutChartEmploye) {
        layoutChartEmploye = new QVBoxLayout(statTab);
        statTab->setLayout(layoutChartEmploye);
    }

    // Clear existing widgets
    QLayoutItem *item;
    while ((item = layoutChartEmploye->takeAt(0)) != nullptr) {
        if (item->widget()) delete item->widget();
        delete item;
    }

    QSqlQuery query;
    query.prepare("SELECT POSTE, COUNT(*) FROM EMPLOYE GROUP BY POSTE");
    if (!query.exec()) {
        QLabel *errorLabel = new QLabel("Erreur SQL: " + query.lastError().text(), statTab);
        errorLabel->setStyleSheet("color: red; font-size: 14px; padding: 20px;");
        errorLabel->setWordWrap(true);
        layoutChartEmploye->addWidget(errorLabel);
        qDebug() << "ERREUR SQL (employe):" << query.lastError().text();
        return;
    }

    int total = 0;
    QList<QPair<QString,int>> entries;
    while (query.next()) {
        QString poste = query.value(0).toString();
        int count = query.value(1).toInt();
        entries.append(qMakePair(poste, count));
        total += count;
    }

    if (total == 0) {
        QLabel *noDataLabel = new QLabel("Aucune donnée statistique disponible pour les employés.", statTab);
        noDataLabel->setStyleSheet("color: orange; font-size: 16px; padding: 20px;");
        noDataLabel->setAlignment(Qt::AlignCenter);
        layoutChartEmploye->addWidget(noDataLabel);
        return;
    }

    // Build series from collected entries
    QPieSeries *series = new QPieSeries();
    for (const auto &p : entries) {
        QString poste = p.first;
        int count = p.second;
        QString label = poste.isEmpty() ? "(non renseigné)" : poste;
        series->append(label + " (" + QString::number(count) + ")", count);
    }

    // Create summary table: Poste | Nombre | Pourcentage
    QTableWidget *summaryTable = new QTableWidget(entries.count(), 3, statTab);
    summaryTable->setHorizontalHeaderLabels(QStringList() << "Poste" << "Nombre" << "%");
    summaryTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    summaryTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    summaryTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    summaryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    summaryTable->setSelectionMode(QAbstractItemView::NoSelection);
    summaryTable->setMinimumHeight(100);

    for (int r = 0; r < entries.count(); ++r) {
        QString poste = entries[r].first;
        int count = entries[r].second;
        double percent = (total > 0) ? (100.0 * count / total) : 0.0;
        QTableWidgetItem *itemPoste = new QTableWidgetItem(poste.isEmpty() ? "(non renseigné)" : poste);
        QTableWidgetItem *itemCount = new QTableWidgetItem(QString::number(count));
        QTableWidgetItem *itemPercent = new QTableWidgetItem(QString::number(percent, 'f', 1) + "%");
        summaryTable->setItem(r, 0, itemPoste);
        summaryTable->setItem(r, 1, itemCount);
        summaryTable->setItem(r, 2, itemPercent);
    }

    layoutChartEmploye->addWidget(summaryTable);

    // Decorate slices
    for (auto slice : series->slices()) {
        slice->setLabelVisible(true);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des employés par poste (total: " + QString::number(total) + ")");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignRight);

    QChartView *chartView = new QChartView(chart, statTab);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(480, 320);
    chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    layoutChartEmploye->addWidget(chartView);
    statTab->update();
}

// ============== SLOTS POUR NOTIFICATION, VOICECHAT, STATISTIQUES MONITEUR ==============

void MainWindow::on_notification_clicked()
{
    NotificationDialog *dialog = new NotificationDialog(this);
    dialog->exec();
    delete dialog;
}

void MainWindow::on_voicechat_clicked()
{
    // Ouvrir le dialogue Voice Chat (avec transcription en temps réel)
    VoiceChatDialog dlg(this);
    dlg.exec();
}

void MainWindow::on_stat_clicked()
{
    MoniteurStatDialog *dialog = new MoniteurStatDialog(this);
    dialog->exec();
    delete dialog;
}

MainWindow::~MainWindow()
{
    // Fermer Arduino proprement
    if (arduinoSerial && arduinoSerial->isOpen()) {
        arduinoSerial->close();
    }
    delete ui;
}

void MainWindow::on_exportPdfBtnMoniteur_clicked()
{
    if (!canPerformAction("export_moniteur")) { showAccessDenied("Exporter moniteurs"); return; }

    QSqlQueryModel *model = M.afficher();
    if (!model || model->rowCount() == 0) {
        QMessageBox::information(this, "Export PDF", "Aucun moniteur à exporter.");
        return;
    }

    QString desktopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    QString defaultFileName = QString("Tous_les_moniteurs_%1.pdf").arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
    QString defaultPath = desktopPath + "/" + defaultFileName;

    QString fileName = QFileDialog::getSaveFileName(this, "Enregistrer le PDF", defaultPath, "Fichiers PDF (*.pdf);;Tous les fichiers (*)");
    if (fileName.isEmpty()) return;
    if (!fileName.endsWith(".pdf", Qt::CaseInsensitive)) fileName += ".pdf";

    QPageSize::PageSizeId pageSize = QPageSize::A4;
    int dpi = 150;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(pageSize);
    pdfWriter.setPageOrientation(QPageLayout::Landscape);
    pdfWriter.setResolution(dpi);

    QPainter painter(&pdfWriter);
    if (!painter.isActive()) {
        QMessageBox::critical(this, "Erreur", "Impossible de créer le fichier PDF!");
        return;
    }

    QFont titleFont("Arial", 28, QFont::Bold);
    QFont headerFont("Arial", 14, QFont::Bold);
    QFont contentFont("Arial", 12);

    int pageWidth = pdfWriter.width();
    int margin = 30;
    int y = margin;

    painter.setFont(titleFont);
    painter.drawText(QRect(margin, y, pageWidth - 5*margin, 100), Qt::AlignCenter, "Liste des Moniteurs");
    y += 50;

    painter.setFont(contentFont);
    QString dateGeneration = "Date de génération: " + QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss");
    painter.drawText(QRect(margin, y, pageWidth - 5*margin, 150), Qt::AlignLeft, dateGeneration);
    y += 30;

    QString total = "Nombre total de moniteurs: " + QString::number(model->rowCount());
    painter.drawText(QRect(margin, y, pageWidth - 5*margin, 150), Qt::AlignLeft, total);
    y += 30;
    y += 20;

    int colCount = 7; // ID, CIN, Nom, Prénom, Téléphone, Email, Permis
    QVector<int> colWidths = {120, 180, 300, 300, 220, 420, 120};
    int totalWidth = 0; for (int w : colWidths) totalWidth += w;
    int availableWidth = pageWidth - 2*margin;
    if (totalWidth > availableWidth) {
        double scale = double(availableWidth) / totalWidth;
        for (int &w : colWidths) w = int(w * scale);
    }

    painter.setFont(headerFont);
    int x = margin;
    QStringList headers = {"ID", "CIN", "Nom", "Prénom", "Téléphone", "Email", "Permis"};
    for (int i = 0; i < colCount; ++i) {
        painter.drawText(QRect(x, y, colWidths[i], 24), Qt::AlignLeft | Qt::AlignVCenter, headers[i]);
        x += colWidths[i];
    }
    y += 30;
    painter.setFont(contentFont);

    int rowHeight = 22;
    for (int row = 0; row < model->rowCount(); ++row) {
        if (y + rowHeight > pdfWriter.height() - margin) {
            pdfWriter.newPage();
            y = margin + 80;
        }

        x = margin;
        QStringList cols;
        for (int c = 0; c < colCount; ++c) {
            cols << model->data(model->index(row, c)).toString();
        }

        for (int c = 0; c < colCount; ++c) {
            painter.drawText(QRect(x, y, colWidths[c], rowHeight), Qt::AlignLeft | Qt::AlignVCenter, cols.value(c));
            x += colWidths[c];
        }
        y += rowHeight + 6;
    }

    painter.end();

    QFile pdfFile(fileName);
    if (!pdfFile.exists()) {
        QMessageBox::critical(this, "Erreur", "Le fichier PDF n'a pas pu être créé! Vérifiez les permissions du dossier.");
        return;
    }

    QMessageBox::information(this, "Export PDF", QString("Exportation PDF réussie!\nFichier: %1\nMoniteurs: %2").arg(fileName).arg(model->rowCount()));
    delete model;
}

void MainWindow::on_sortBtn_clicked()
{
    // Trier les moniteurs par NOM_MONITEUR asc et rafraîchir la table
    if (!Connection::isDatabaseOpen()) {
        QMessageBox::warning(this, "Attention", "Connexion DB non disponible pour le tri.");
        return;
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    QString queryStr = "SELECT ID_MONITEUR, CIN_MONITEUR, NOM_MONITEUR, PRENOM_MONITEUR, TEL_MONITEUR, EMAIL, PERMIS FROM MONITEUR ORDER BY NOM_MONITEUR";
    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        QMessageBox::critical(this, "Erreur SQL", "Erreur lors du tri: " + model->lastError().text());
        delete model;
        return;
    }

    int rowCount = model->rowCount();
    ui->moniteurTable->setRowCount(rowCount);
    ui->moniteurTable->setColumnCount(7);

    for (int row = 0; row < rowCount; ++row) {
        for (int col = 0; col < 7; ++col) {
            QString value = model->data(model->index(row, col)).toString();
            ui->moniteurTable->setItem(row, col, new QTableWidgetItem(value));
        }
    }

    delete model;
}



