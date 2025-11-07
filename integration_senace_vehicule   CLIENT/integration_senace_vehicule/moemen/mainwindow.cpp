#include "mainwindow.h"
#include "seance.h"
#include "vehicule.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QDate>
#include <QTime>
#include <QDateTime>
#include <QRegularExpression>
#include <QTableWidgetItem>
#include <QSqlQueryModel>
#include <QComboBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
        // Insertion de données de test pour véhicules
        query.exec("INSERT INTO VEHICULE (IMMATRICULE, MODELE_VEHICULE, TYPE_VEHICULE, TRANSMISSION, KILOMETRAGE, ETAT, DATE_ASSURANCE) "
                   "SELECT '123TU1234', 'Peugeot 208', 'Voiture', 'Manuelle', 15000, 'Disponible', TO_DATE('01/01/2025', 'DD/MM/YYYY') FROM DUAL "
                   "WHERE NOT EXISTS (SELECT 1 FROM VEHICULE WHERE IMMATRICULE = '123TU1234')");

        query.exec("INSERT INTO VEHICULE (IMMATRICULE, MODELE_VEHICULE, TYPE_VEHICULE, TRANSMISSION, KILOMETRAGE, ETAT, DATE_ASSURANCE) "
                   "SELECT '200TU5678', 'Renault Clio', 'Voiture', 'Automatique', 8000, 'En maintenance', TO_DATE('01/06/2024', 'DD/MM/YYYY') FROM DUAL "
                   "WHERE NOT EXISTS (SELECT 1 FROM VEHICULE WHERE IMMATRICULE = '200TU5678')");


        refreshTableSeance();
        refreshTableVehicule();
        refreshTableClient();

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
    
    // Connexion pour le tri des clients
    connect(ui->comboBoxTrier, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &MainWindow::on_comboBoxTrier_currentIndexChanged);

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
}

void MainWindow::clientclick() {
    ui->stack->setCurrentIndex(2);
    refreshTableClient();
    // Initialiser le comboBox de tri à ID par défaut
    ui->comboBoxTrier->setCurrentIndex(0);
}

void MainWindow::contratclick() {
    ui->stack->setCurrentIndex(4);
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

// ==================== GESTION CLIENTS ====================
void MainWindow::refreshTableClient()
{
    QSqlQueryModel *model = C.afficher();
    
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
            if (col == 4 && data.type() == QVariant::Date) {
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
    ui->lineEditID->clear();
    ui->lineEditNom->clear();
    ui->lineEditPrenom->clear();
    ui->lineEditCIN->clear();
    ui->jjj->setDate(QDate(2000, 1, 1));
    ui->lineEditMotDePasse->clear(); // Adresse
    ui->lineEditTelephone->clear();
    ui->lineEditPoste->clear(); // Email
}

void MainWindow::on_btnAjouter_clicked()
{
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
    
    // Conversion du téléphone
    bool ok;
    int tel = telStr.toInt(&ok);
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
    
    // Vérifier si le CIN existe déjà avant de créer l'objet
    if (C.checkIfCinExists(cin)) {
        QMessageBox::critical(this, "Erreur", "Ce CIN existe déjà dans la base de données! Veuillez utiliser un autre CIN.");
        return;
    }
    
    client c(nom, prenom, cin, dateNaissance, adresse, tel, "", email);
    
    if (c.ajouter()) {
        QMessageBox::information(this, "Succès", "Client ajouté avec succès!");
        refreshTableClient();
        clearFieldsClient();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout!\n\nVérifiez que:\n- Le nom et prénom contiennent uniquement des lettres\n- Le CIN est unique\n- L'email a un format valide");
    }
}

void MainWindow::on_btnModifier_clicked()
{
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
    
    // Conversion du téléphone
    int tel = telStr.toInt(&ok);
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
    if (!C.checkIfClientExists(id)) {
        QMessageBox::critical(this, "Erreur", "Ce client n'existe pas!\nVeuillez vérifier l'ID saisi.");
        return;
    }
    
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Voulez-vous vraiment supprimer ce client?",
                                  QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (C.supprimer(id)) {
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
    QString nom = ui->lineEditRecherche->text().trimmed();
    
    if (nom.isEmpty()) {
        refreshTableClient();
        return;
    }
    
    QSqlQueryModel *model = C.rechercheParNom(nom);
    
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
            if (col == 4 && data.type() == QVariant::Date) {
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
    if (!item) return;
    
    int row = item->row();
    
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
    
    QSqlQueryModel *model = C.afficher(orderBy);
    
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
            if (col == 4 && data.type() == QVariant::Date) {
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

MainWindow::~MainWindow()
{
    delete ui;
}
