#include "mainwindow.h"
#include "seance.h"
#include "vehicule.h"
#include "circuit.h"
#include "ui_mainwindow.h"
#include "connection.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_isModifyModeEmploye(false)
{
    ui->setupUi(this);

    // Initialisation de la base de données
    Connection conn;
    if (conn.createConnect()) {
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

        QMessageBox::information(this, "Succès", "Base de données initialisée avec succès!");
    } else {
        QMessageBox::critical(this, "Erreur", "Connexion à la base de données échouée!");
    }

    // Connexions pour la navigation entre pages
    connect(ui->seance, &QPushButton::clicked, this, &MainWindow::seanceclick);
    connect(ui->employe, &QPushButton::clicked, this, &MainWindow::employeclick);
    connect(ui->client, &QPushButton::clicked, this, &MainWindow::clientclick);
    connect(ui->vehicule, &QPushButton::clicked, this, &MainWindow::vehiculeclick);
    connect(ui->contrat, &QPushButton::clicked, this, &MainWindow::contratclick);
    connect(ui->cercuit, &QPushButton::clicked, this, &MainWindow::cercuitclick);

    // Connexion pour le tableau de recherche des circuits
    connect(ui->tableView_circuits_recherche, &QTableView::clicked, this, &MainWindow::on_tableView_circuits_recherche_clicked);

    // Configuration des pages
    for (int i = 1; i <= 4; ++i) {
        QWidget *page = new QWidget();
        QLabel *label = new QLabel("Page " + QString::number(i) + " - Contenu à ajouter", page);
        label->setGeometry(100, 100, 300, 50);
        ui->stack->addWidget(page);
    }

    ui->stack->setCurrentIndex(0);
}

// ==================== NAVIGATION ====================
void MainWindow::seanceclick() {
    ui->stack->setCurrentIndex(0);
    refreshTableSeance();
}

void MainWindow::vehiculeclick(){
    ui->stack->setCurrentIndex(3);
    refreshTableVehicule();
}

void MainWindow::employeclick() {
    ui->stack->setCurrentIndex(1);
    refreshTableEmploye();
}

void MainWindow::clientclick() {
    ui->stack->setCurrentIndex(2);
    refreshTableClient();
    // Initialiser le comboBox de tri à ID par défaut
    if (ui->comboBoxTrier) {
        ui->comboBoxTrier->setCurrentIndex(0);
    }
}

void MainWindow::contratclick() {
    ui->stack->setCurrentIndex(4);
}

void MainWindow::cercuitclick() {
    // Trouver l'index de la page circuit dans le stack
    for (int i = 0; i < ui->stack->count(); i++) {
        QWidget *widget = ui->stack->widget(i);
        if (widget && widget->objectName() == "page_circuit") {
            ui->stack->setCurrentIndex(i);
            refreshTableCircuit();
            return;
        }
    }
    // Si la page n'est pas trouvée, utiliser l'index 5 par défaut
    ui->stack->setCurrentIndex(5);
    refreshTableCircuit();
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
        QMessageBox::information(this, "Succès", "Séance ajoutée avec succès!");
        refreshTableSeance();
        clearFieldsSeance();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout!");
    }
}

void MainWindow::on_pushButton_modifier_clicked()
{
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
    int id = ui->lineEdit_id->text().toInt();

    if (id <= 0) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner une séance à supprimer!");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer cette séance?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (Seance::supprimer(id)) {
            QMessageBox::information(this, "Succès", "Séance supprimée avec succès!");
            refreshTableSeance();
            clearFieldsSeance();
        } else {
            QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression!");
        }
    }
}

void MainWindow::on_pushButton_recherche_clicked()
{
    QString critere = ui->lineEdit_recherche->text();
    QSqlQueryModel *model;

    if (!critere.isEmpty()) {
        model = Seance::rechercher(critere);
        if (model->rowCount() == 0) {
            QMessageBox::information(this, "Recherche", "Aucune séance trouvée pour: " + critere);
        }
    } else {
        model = Seance::afficher();
    }

    ui->tableView_seances->setModel(model);
}

void MainWindow::on_tableView_seances_clicked(const QModelIndex &index)
{
    int id = index.sibling(index.row(), 0).data().toInt();
    QString immatricule = index.sibling(index.row(), 1).data().toString();
    int id_client = index.sibling(index.row(), 2).data().toInt();
    int id_moniteur = index.sibling(index.row(), 3).data().toInt();
    QDate date = index.sibling(index.row(), 4).data().toDate();
    QString heureStr = index.sibling(index.row(), 5).data().toString();
    QString type = index.sibling(index.row(), 6).data().toString();
    QString appareil = index.sibling(index.row(), 7).data().toString();
    int duree = index.sibling(index.row(), 8).data().toInt();

    // Conversion de l'heure string vers QTime
    QTime heure = QTime::fromString(heureStr, "HH:mm");
    if (!heure.isValid()) {
        heure = QTime::fromString(heureStr, "hh:mm");
    }

    ui->lineEdit_id->setText(QString::number(id));
    ui->dateEdit_date->setDate(date);
    ui->timeEdit_heure->setTime(heure);
    ui->lineEdit_duree->setText(QString::number(duree));

    int indexType = ui->comboBox_type->findText(type);
    if (indexType != -1) {
        ui->comboBox_type->setCurrentIndex(indexType);
    } else {
        ui->comboBox_type->setCurrentIndex(0);
    }

    ui->lineEdit_appareil->setText(appareil);
    ui->lineEdit_id_client->setText(QString::number(id_client));
    ui->lineEdit_immatricule->setText(immatricule);
    ui->lineEdit_id_moniteur->setText(QString::number(id_moniteur));
}

// ==================== GESTION VÉHICULES ====================
void MainWindow::refreshTableVehicule()
{
    QSqlQueryModel *model = V.afficher();
    ui->tableView_vehicules->setModel(model);
}

void MainWindow::clearFieldsVehicule()
{
    ui->lineEdit_immatricule->clear();
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

    // Validation
    if (imm.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Immatriculation ne peut pas être vide!");
        return;
    }
    if (mod.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Modèle ne peut pas être vide!");
        return;
    }

    Vehicule v(imm, mod, typ, trans, et, date);
    bool test = v.ajouter();

    if (test) {
        QMessageBox::information(this, "Succès", "✓ Véhicule ajouté avec succès.");
        refreshTableVehicule();
        clearFieldsVehicule();
    } else {
        QMessageBox::critical(this, "Erreur", "✗ Échec de l'ajout. Vérifiez que l'immatriculation n'existe pas déjà.");
    }
}

void MainWindow::on_pushButton_supprimervehicule_clicked()
{
    QString id = ui->lineEdit_immatricule->text().trimmed();

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
            refreshTableVehicule();
            clearFieldsVehicule();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression ✗");
        }
    }
}

void MainWindow::on_pushButton_modifiervehicule_clicked()
{
    QString id = ui->lineEdit_immatricule->text().trimmed();
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
        refreshTableVehicule();
        clearFieldsVehicule();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification ✗");
    }
}

void MainWindow::on_tableView_vehicules_clicked(const QModelIndex &index)
{
    QString immatricule = index.sibling(index.row(), 0).data().toString();
    QString modele      = index.sibling(index.row(), 1).data().toString();
    QString type        = index.sibling(index.row(), 2).data().toString();
    QString transmission= index.sibling(index.row(), 3).data().toString();
    QString etat        = index.sibling(index.row(), 4).data().toString();

    // La date est maintenant au format DD/MM/YYYY depuis TO_CHAR
    QString dateStr = index.sibling(index.row(), 5).data().toString();
    QDate dateAssurance = QDate::fromString(dateStr, "dd/MM/yyyy");

    if (!dateAssurance.isValid()) {
        dateAssurance = QDate::currentDate();
    }

    ui->lineEdit_immatricule->setText(immatricule);
    ui->lineEdit_modele->setText(modele);

    int indexType = ui->comboBox_type_2->findText(type);
    ui->comboBox_type_2->setCurrentIndex(indexType != -1 ? indexType : 0);

    int indexTrans = ui->comboBox_transmission->findText(transmission);
    ui->comboBox_transmission->setCurrentIndex(indexTrans != -1 ? indexTrans : 0);

    int indexEtat = ui->comboBox_etat->findText(etat);
    ui->comboBox_etat->setCurrentIndex(indexEtat != -1 ? indexEtat : 0);

    ui->dateEdit_assurance->setDate(dateAssurance);
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

    // Validation de l'ID du circuit
    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID du circuit est obligatoire!");
        ui->lineEdit_id_circuit->setFocus();
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

    bool distanceOk;
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

    bool dureeOk;
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

    // Validation de l'ID du circuit
    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez sélectionner un circuit à modifier!");
        ui->lineEdit_id_circuit->setFocus();
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

    bool distanceOk;
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

    bool dureeOk;
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
    on_pushButton_supprimercircuit_clicked();
    QString idText = ui->lineEdit_id_circuit->text().trimmed();

    if (idText.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Veuillez sélectionner un circuit à supprimer!");
        ui->lineEdit_id_circuit->setFocus();
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

    ui->tableWidget_6->setRowCount(model->rowCount());
    ui->tableWidget_6->setColumnCount(8);

    // Définir les en-têtes
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "CIN" << "Date Naiss." << "Adresse" << "Téléphone" << "Email";
    ui->tableWidget_6->setHorizontalHeaderLabels(headers);

    // Remplir la table
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < 8; ++col) {
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

            QTableWidgetItem *item = new QTableWidgetItem(text);
            ui->tableWidget_6->setItem(row, col, item);
        }
    }

    // Ajuster la largeur des colonnes
    ui->tableWidget_6->resizeColumnsToContents();
}

void MainWindow::clearFieldsClient()
{
    if (ui->lineEditID) ui->lineEditID->clear();
    if (ui->lineEditNom) ui->lineEditNom->clear();
    if (ui->lineEditPrenom) ui->lineEditPrenom->clear();
    if (ui->lineEditCIN) ui->lineEditCIN->clear();
    if (ui->jjj) ui->jjj->setDate(QDate(2000, 1, 1));
    if (ui->lineEditMotDePasse) ui->lineEditMotDePasse->clear(); // Adresse
    if (ui->lineEditTelephone) ui->lineEditTelephone->clear();
    if (ui->lineEditPoste) ui->lineEditPoste->clear(); // Email
}

void MainWindow::on_btnAjouter_clicked()
{
    if (!ui->lineEditNom || !ui->lineEditPrenom || !ui->lineEditCIN || !ui->jjj ||
        !ui->lineEditMotDePasse || !ui->lineEditTelephone || !ui->lineEditPoste) {
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

    // Validation
    if (nom.isEmpty() || prenom.isEmpty() || cin.isEmpty() || adresse.isEmpty() || telStr.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez remplir tous les champs obligatoires!");
        return;
    }

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

    client c(nom, prenom, cin, dateNaissance, adresse, tel, "", email);

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
    if (!ui->lineEditID || !ui->lineEditNom || !ui->lineEditPrenom || !ui->lineEditCIN || !ui->jjj ||
        !ui->lineEditMotDePasse || !ui->lineEditTelephone || !ui->lineEditPoste) {
        QMessageBox::warning(this, "Erreur", "Certains widgets UI sont manquants!");
        return;
    }

    QString idStr = ui->lineEditID->text().trimmed();

    if (idStr.isEmpty()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner un client à modifier!");
        return;
    }

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

    client c(nom, prenom, cin, dateNaissance, adresse, tel, "", email);
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
    ui->tableWidget_6->setRowCount(model->rowCount());
    ui->tableWidget_6->setColumnCount(8);

    // Définir les en-têtes
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "CIN" << "Date Naiss." << "Adresse" << "Téléphone" << "Email";
    ui->tableWidget_6->setHorizontalHeaderLabels(headers);

    // Remplir la table
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < 8; ++col) {
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

void MainWindow::on_tableWidget_6_itemClicked(QTableWidgetItem *item)
{
    if (!item || !ui->tableWidget_6) return;

    int row = item->row();

    // Vérifier que tous les widgets existent
    if (!ui->lineEditID || !ui->lineEditNom || !ui->lineEditPrenom || !ui->lineEditCIN ||
        !ui->jjj || !ui->lineEditMotDePasse || !ui->lineEditTelephone || !ui->lineEditPoste) {
        return;
    }

    // Récupérer les données de la ligne sélectionnée
    QString id = ui->tableWidget_6->item(row, 0)->text();
    QString nom = ui->tableWidget_6->item(row, 1)->text();
    QString prenom = ui->tableWidget_6->item(row, 2)->text();
    QString cin = ui->tableWidget_6->item(row, 3)->text();
    QString dateStr = ui->tableWidget_6->item(row, 4)->text();
    QString adresse = ui->tableWidget_6->item(row, 5)->text();
    QString tel = ui->tableWidget_6->item(row, 6)->text();
    QString email = ui->tableWidget_6->item(row, 7)->text();

    // Remplir les champs
    ui->lineEditID->setText(id);
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
    ui->tableWidget_6->setRowCount(model->rowCount());
    ui->tableWidget_6->setColumnCount(8);

    // Définir les en-têtes
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "CIN" << "Date Naiss." << "Adresse" << "Téléphone" << "Email";
    ui->tableWidget_6->setHorizontalHeaderLabels(headers);

    // Remplir la table
    for (int row = 0; row < model->rowCount(); ++row) {
        for (int col = 0; col < 8; ++col) {
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

            QTableWidgetItem *item = new QTableWidgetItem(text);
            ui->tableWidget_6->setItem(row, col, item);
        }
    }

    // Ajuster la largeur des colonnes
    ui->tableWidget_6->resizeColumnsToContents();
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
    QString NOM_EMPLOYE = ui->lineEdit_25->text();
    QString PRENOM = ui->lineEdit_26->text();
    QString POSTE = ui->lineEdit_27->text();
    QString MOT_DE_PASSEE = ui->lineEdit_28->text();
    QString TEL_EMPLOYE = "+216" + ui->lineEdit_29->text();
    QDate DATE_EMBAUCHE = ui->dateEdit_5->date();

    if (NOM_EMPLOYE.isEmpty() || PRENOM.isEmpty() || POSTE.isEmpty() || MOT_DE_PASSEE.isEmpty() || TEL_EMPLOYE.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs !");
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

    if (reply == QMessageBox::No)
        return;

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

MainWindow::~MainWindow()
{
    delete ui;
}
