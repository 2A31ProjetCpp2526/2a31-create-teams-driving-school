#include "notification.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QHeaderView>

// ============== Classe Notification (métier) ==============

Notification::Notification()
    : id_notification(0), lue(false)
{
    dateHeure = QDateTime::currentDateTime();
}

Notification::Notification(const QString &type, const QString &message, const QString &entite)
    : id_notification(0), type(type), message(message), entite(entite), lue(false)
{
    dateHeure = QDateTime::currentDateTime();
}

bool Notification::ajouter()
{
    if (!Connection::isDatabaseOpen()) {
        qDebug() << "❌ Base de données non connectée pour notification";
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO NOTIFICATION (ID_NOTIFICATION, TYPE_NOTIF, MESSAGE, ENTITE, DATE_HEURE, LUE) "
                  "VALUES (SEQ_NOTIFICATION.NEXTVAL, :type, :message, :entite, :dateHeure, :lue)");
    query.bindValue(":type", type);
    query.bindValue(":message", message);
    query.bindValue(":entite", entite);
    query.bindValue(":dateHeure", dateHeure);
    query.bindValue(":lue", lue ? 1 : 0);

    if (!query.exec()) {
        qDebug() << "❌ Erreur ajout notification:" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Notification ajoutée:" << message;
    return true;
}

bool Notification::supprimer(int id)
{
    if (!Connection::isDatabaseOpen()) {
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM NOTIFICATION WHERE ID_NOTIFICATION = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "❌ Erreur suppression notification:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool Notification::supprimerTout()
{
    if (!Connection::isDatabaseOpen()) {
        return false;
    }

    QSqlQuery query;
    if (!query.exec("DELETE FROM NOTIFICATION")) {
        qDebug() << "❌ Erreur suppression toutes notifications:" << query.lastError().text();
        return false;
    }

    qDebug() << "✅ Toutes les notifications supprimées";
    return true;
}

QSqlQueryModel* Notification::afficher()
{
    QSqlQueryModel *model = new QSqlQueryModel();

    if (!Connection::isDatabaseOpen()) {
        return model;
    }

    model->setQuery("SELECT ID_NOTIFICATION, TYPE_NOTIF, ENTITE, MESSAGE, DATE_HEURE "
                    "FROM NOTIFICATION ORDER BY DATE_HEURE DESC");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Entité"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Message"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date/Heure"));

    return model;
}

int Notification::countNonLues()
{
    if (!Connection::isDatabaseOpen()) {
        return 0;
    }

    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM NOTIFICATION WHERE LUE = 0");
    if (query.next()) {
        return query.value(0).toInt();
    }
    return 0;
}

// ============== Méthodes statiques pour notifications automatiques ==============

void Notification::notifierAjoutEmploye(const QString &nom, const QString &prenom)
{
    Notification n("AJOUT", QString("Nouvel employé ajouté: %1 %2").arg(nom).arg(prenom), "EMPLOYE");
    n.ajouter();
}

void Notification::notifierModificationEmploye(const QString &nom, const QString &prenom)
{
    Notification n("MODIFICATION", QString("Employé modifié: %1 %2").arg(nom).arg(prenom), "EMPLOYE");
    n.ajouter();
}

void Notification::notifierSuppressionEmploye(const QString &nom, const QString &prenom)
{
    Notification n("SUPPRESSION", QString("Employé supprimé: %1 %2").arg(nom).arg(prenom), "EMPLOYE");
    n.ajouter();
}

void Notification::notifierAjoutMoniteur(const QString &nom, const QString &prenom)
{
    Notification n("AJOUT", QString("Nouveau moniteur ajouté: %1 %2").arg(nom).arg(prenom), "MONITEUR");
    n.ajouter();
}

void Notification::notifierModificationMoniteur(const QString &nom, const QString &prenom)
{
    Notification n("MODIFICATION", QString("Moniteur modifié: %1 %2").arg(nom).arg(prenom), "MONITEUR");
    n.ajouter();
}

void Notification::notifierSuppressionMoniteur(const QString &nom, const QString &prenom)
{
    Notification n("SUPPRESSION", QString("Moniteur supprimé: %1 %2").arg(nom).arg(prenom), "MONITEUR");
    n.ajouter();
}

void Notification::notifierAjoutClient(const QString &nom, const QString &prenom)
{
    Notification n("AJOUT", QString("Nouveau client ajouté: %1 %2").arg(nom).arg(prenom), "CLIENT");
    n.ajouter();
}

void Notification::notifierModificationClient(const QString &nom, const QString &prenom)
{
    Notification n("MODIFICATION", QString("Client modifié: %1 %2").arg(nom).arg(prenom), "CLIENT");
    n.ajouter();
}

void Notification::notifierSuppressionClient(const QString &nom, const QString &prenom)
{
    Notification n("SUPPRESSION", QString("Client supprimé: %1 %2").arg(nom).arg(prenom), "CLIENT");
    n.ajouter();
}

void Notification::notifierAjoutVehicule(const QString &immatriculation)
{
    Notification n("AJOUT", QString("Nouveau véhicule ajouté: %1").arg(immatriculation), "VEHICULE");
    n.ajouter();
}

void Notification::notifierModificationVehicule(const QString &immatriculation)
{
    Notification n("MODIFICATION", QString("Véhicule modifié: %1").arg(immatriculation), "VEHICULE");
    n.ajouter();
}

void Notification::notifierSuppressionVehicule(const QString &immatriculation)
{
    Notification n("SUPPRESSION", QString("Véhicule supprimé: %1").arg(immatriculation), "VEHICULE");
    n.ajouter();
}

void Notification::notifierAjoutSeance(const QString &info)
{
    Notification n("AJOUT", QString("Nouvelle séance ajoutée: %1").arg(info), "SEANCE");
    n.ajouter();
}

void Notification::notifierAjoutCircuit(const QString &nom)
{
    Notification n("AJOUT", QString("Nouveau circuit ajouté: %1").arg(nom), "CIRCUIT");
    n.ajouter();
}

// ============== Dialog Notification ==============

NotificationDialog::NotificationDialog(QWidget *parent)
    : QDialog(parent), model(nullptr)
{
    setWindowTitle("📋 Notifications");
    setMinimumSize(700, 450);
    setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Label compteur
    lblCount = new QLabel("Notifications:");
    lblCount->setStyleSheet("font-size: 14pt; font-weight: bold; color: #333;");
    mainLayout->addWidget(lblCount);

    // TableView
    tableView = new QTableView();
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setAlternatingRowColors(true);
    tableView->horizontalHeader()->setStretchLastSection(true);
    tableView->setStyleSheet(
        "QTableView { background-color: white; alternate-background-color: #f0f8ff; color: black; }"
        "QTableView QTableCornerButton::section { background-color: #f0f0f0; }"
        "QHeaderView::section { background-color: #4682b4; color: white; padding: 5px; font-weight: bold; }"
    );
    mainLayout->addWidget(tableView);

    // Boutons
    QHBoxLayout *btnLayout = new QHBoxLayout();

    btnRefresh = new QPushButton("🔄 Actualiser");
    btnRefresh->setStyleSheet("background-color: #4CAF50; color: white; padding: 8px 16px; border-radius: 5px;");
    connect(btnRefresh, &QPushButton::clicked, this, &NotificationDialog::onRefreshClicked);
    btnLayout->addWidget(btnRefresh);

    btnDeleteSelected = new QPushButton("🗑️ Supprimer sélection");
    btnDeleteSelected->setStyleSheet("background-color: #ff9800; color: white; padding: 8px 16px; border-radius: 5px;");
    connect(btnDeleteSelected, &QPushButton::clicked, this, &NotificationDialog::onDeleteSelectedClicked);
    btnLayout->addWidget(btnDeleteSelected);

    btnClearAll = new QPushButton("🧹 Effacer tout");
    btnClearAll->setStyleSheet("background-color: #f44336; color: white; padding: 8px 16px; border-radius: 5px;");
    connect(btnClearAll, &QPushButton::clicked, this, &NotificationDialog::onClearAllClicked);
    btnLayout->addWidget(btnClearAll);

    btnClose = new QPushButton("❌ Fermer");
    btnClose->setStyleSheet("background-color: #607d8b; color: white; padding: 8px 16px; border-radius: 5px;");
    connect(btnClose, &QPushButton::clicked, this, &QDialog::close);
    btnLayout->addWidget(btnClose);

    mainLayout->addLayout(btnLayout);

    // Charger les notifications
    refreshNotifications();
}

NotificationDialog::~NotificationDialog()
{
    if (model) {
        delete model;
    }
}

void NotificationDialog::refreshNotifications()
{
    if (model) {
        delete model;
    }

    model = Notification::afficher();
    tableView->setModel(model);

    // Masquer colonne ID si besoin
    tableView->setColumnWidth(0, 50);
    tableView->setColumnWidth(1, 100);
    tableView->setColumnWidth(2, 100);
    tableView->setColumnWidth(3, 300);
    tableView->setColumnWidth(4, 150);

    int count = model->rowCount();
    lblCount->setText(QString("📋 Notifications (%1)").arg(count));
}

void NotificationDialog::onClearAllClicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirmation",
        "Voulez-vous vraiment supprimer TOUTES les notifications?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reply == QMessageBox::Yes) {
        if (Notification::supprimerTout()) {
            QMessageBox::information(this, "Succès", "Toutes les notifications ont été supprimées.");
            refreshNotifications();
        } else {
            QMessageBox::critical(this, "Erreur", "Impossible de supprimer les notifications.");
        }
    }
}

void NotificationDialog::onDeleteSelectedClicked()
{
    QModelIndex index = tableView->currentIndex();
    if (!index.isValid()) {
        QMessageBox::warning(this, "Attention", "Veuillez sélectionner une notification à supprimer.");
        return;
    }

    int id = model->data(model->index(index.row(), 0)).toInt();
    
    if (Notification::supprimer(id)) {
        QMessageBox::information(this, "Succès", "Notification supprimée.");
        refreshNotifications();
    } else {
        QMessageBox::critical(this, "Erreur", "Impossible de supprimer la notification.");
    }
}

void NotificationDialog::onRefreshClicked()
{
    refreshNotifications();
}
