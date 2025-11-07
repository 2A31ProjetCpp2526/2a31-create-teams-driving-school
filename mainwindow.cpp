#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QHeaderView>
#include <QDate>

employe::employe(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::employe)
    , m_isModifyMode(false)
{
    ui->setupUi(this);

    // Configuration du tableau
    ui->tableWidget->setColumnCount(7);
    QStringList headers = {"ID", "Nom", "Prénom", "Poste", "Mot de passe", "Num Tel", "Date Embauche"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Connexions des signaux
    connect(ui->pushButton, &QPushButton::clicked, this, &employe::on_pushButton_clicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &employe::on_pushButton_2_clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &employe::on_pushButton_4_clicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &employe::on_pushButton_5_clicked);
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &employe::on_comboBox_currentIndexChanged);
    connect(ui->tableWidget->selectionModel(), &QItemSelectionModel::selectionChanged, this, &employe::on_tableWidget_itemSelectionChanged);

    // Données d'exemple (optionnel, pour tester)
    m_employes.append(Employe(1, "Dupont", "Jean", "Développeur", "pass123", "+21612345678", QDate(2023, 1, 15)));
    m_employes.append(Employe(2, "Martin", "Marie", "Manager", "mdp456", "+21698765432", QDate(2022, 5, 20)));
    updateTable();
}

employe::~employe()
{
    delete ui;
}

// Slot: Ajouter ou Modifier
void employe::on_pushButton_clicked()
{
    // Validation basique
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

    if (m_isModifyMode) {
        // Mode modification : remplacer l'employé existant
        bool found = false;
        for (int i = 0; i < m_employes.size(); ++i) {
            if (m_employes[i].id() == id) {
                m_employes[i] = emp;
                found = true;
                break;
            }
        }
        if (!found) {
            showError("Employé non trouvé pour modification !");
            return;
        }
        ui->pushButton->setText("Ajouter");
        m_isModifyMode = false;
    } else {
        // Mode ajout
        m_employes.append(emp);
    }

    updateTable();
    clearForm();
    QMessageBox::information(this, "Succès", m_isModifyMode ? "Employé modifié !" : "Employé ajouté !");
}

// Slot: Afficher/Rechercher
void employe::on_pushButton_2_clicked()
{
    QString searchText = ui->lineEdit_13->text().toLower();
    QList<Employe> filtered;

    if (searchText.isEmpty()) {
        filtered = m_employes;  // Afficher tout
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
void employe::on_pushButton_4_clicked()
{
    bool ok;
    int idToDelete = ui->lineEdit_14->text().toInt(&ok);
    if (!ok || idToDelete <= 0) {
        showError("ID invalide pour suppression !");
        return;
    }

    for (int i = 0; i < m_employes.size(); ++i) {
        if (m_employes[i].id() == idToDelete) {
            m_employes.removeAt(i);
            updateTable();
            ui->lineEdit_14->clear();
            QMessageBox::information(this, "Succès", "Employé supprimé !");
            return;
        }
    }
    showError("Employé non trouvé pour suppression !");
}

// Slot: Exporter (vers CSV)
void employe::on_pushButton_5_clicked()
{
    QFile file("employes.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        showError("Erreur lors de l'export !");
        return;
    }

    QTextStream out(&file);
    out << "ID,Nom,Prénom,Poste,Mot de passe,Num Tel,Date Embauche\n";
    for (const Employe& emp : m_employes) {
        out << emp.id() << "," << emp.nom() << "," << emp.prenom() << "," << emp.poste() << ","
            << emp.motDePasse() << "," << emp.numTel() << "," << emp.dateEmbauche().toString("yyyy-MM-dd") << "\n";
    }
    file.close();
    QMessageBox::information(this, "Succès", "Exporté vers employes.csv !");
}

// Slot: Trier
void employe::on_comboBox_currentIndexChanged(int index)
{
    if (index == 0) {  // Poste
        std::sort(m_employes.begin(), m_employes.end(), [](const Employe& a, const Employe& b) {
            return a.poste() < b.poste();
        });
    } else if (index == 1) {  // ID
        std::sort(m_employes.begin(), m_employes.end(), [](const Employe& a, const Employe& b) {
            return a.id() < b.id();
        });
    }
    updateTable();
}

// Slot: Charger sélection pour modification
void employe::on_tableWidget_itemSelectionChanged()
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
    ui->lineEdit_6->setText(emp.numTel().remove("+216"));  // Enlever préfixe pour affichage
    ui->dateEdit->setDate(emp.dateEmbauche());

    m_isModifyMode = true;
    ui->pushButton->setText("Modifier");
}

// Génère prochain ID
int employe::getNextId() const
{
    if (m_employes.isEmpty()) return 1;
    return m_employes.last().id() + 1;
}

// Met à jour le tableau
void employe::updateTable(const QList<Employe>& employesToShow)
{
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
void employe::clearForm()
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
void employe::showError(const QString& message)
{
    QMessageBox::warning(this, "Erreur", message);
}
