#include "client.h"
#include <QSqlQuery>
#include <QtDebug>
#include <QObject>
#include <QSqlError>
#include <QRegularExpression>

client::client()
{
    id_client = 0;
    nom_client = "";
    prenom_client = "";
    cin_client = "";
    date_naissance = QDate();
    adresse = "";
    tel_client = 0;
    code_qr = "";
    column8 = "";
}

client::client(QString nom_client, QString prenom_client, QString cin_client,
               QDate date_naissance, QString adresse, int tel_client,
               QString code_qr, QString column8)
{
    this->id_client = 0;
    this->nom_client = nom_client;
    this->prenom_client = prenom_client;
    this->cin_client = cin_client;
    this->date_naissance = date_naissance;
    this->adresse = adresse;
    this->tel_client = tel_client;
    this->code_qr = code_qr;
    this->column8 = column8;
}

int client::getid_client() const { return id_client; }
QString client::getnom_client() const { return nom_client; }
QString client::getprenom_client() const { return prenom_client; }
QString client::getcin_client() const { return cin_client; }
QDate client::getdate_naissance() const { return date_naissance; }
QString client::getadresse() const { return adresse; }
int client::gettel_client() const { return tel_client; }
QString client::getcode_qr() const { return code_qr; }
QString client::getcolumn8() const { return column8; }

void client::setid_client(int id_client) { this->id_client = id_client; }
void client::setnom_client(QString nom_client) { this->nom_client = nom_client; }
void client::setprenom_client(QString prenom_client) { this->prenom_client = prenom_client; }
void client::setcin_client(QString cin_client) { this->cin_client = cin_client; }
void client::setdate_naissance(QDate date_naissance) { this->date_naissance = date_naissance; }
void client::setadresse(QString adresse) { this->adresse = adresse; }
void client::settel_client(int tel_client) { this->tel_client = tel_client; }
void client::setcode_qr(QString code_qr) { this->code_qr = code_qr; }
void client::setcolumn8(QString column8) { this->column8 = column8; }

bool client::validerDonnees() const
{
    QRegularExpression alphaRegex("^[a-zA-Z]+$");
    if (nom_client.isEmpty() || prenom_client.isEmpty()) {
        qDebug() << "Erreur : Nom ou prénom vide.";
        return false;
    }
    if (!alphaRegex.match(nom_client).hasMatch() || !alphaRegex.match(prenom_client).hasMatch()) {
        qDebug() << "Erreur : Le nom et le prénom doivent contenir uniquement des lettres.";
        return false;
    }

    QRegularExpression cinRegex("^[0-9]{8}$");
    if (!cinRegex.match(cin_client).hasMatch()) {
        qDebug() << "Erreur : CIN doit être une chaîne de 8 chiffres.";
        return false;
    }

    if (tel_client < 10000000 || tel_client > 99999999) {
        qDebug() << "Erreur : Téléphone doit être un nombre à 8 chiffres.";
        return false;
    }

    // Validation de l'email si fourni (format: xyz@example.exemple)
    if (!column8.isEmpty()) {
        QRegularExpression emailRegex("^[a-zA-Z0-9._-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        if (!emailRegex.match(column8).hasMatch()) {
            qDebug() << "Erreur : Format d'email invalide. Format attendu : xyz@example.exemple";
            return false;
        }
    }

    return true;
}

bool client::ajouter()
{
    if (!validerDonnees()) {
        qDebug() << "Échec de la validation des données";
        return false;
    }

    qDebug() << "=== Tentative d'ajout d'un client ===";
    qDebug() << "CIN à vérifier:" << cin_client;

    // Vérifier que le CIN est unique
    bool cinExists = checkIfCinExists(cin_client);
    qDebug() << "Le CIN existe-t-il?" << cinExists;
    
    if (cinExists) {
        qDebug() << "Erreur : Ce CIN existe déjà dans la base de données.";
        return false;
    }

    QSqlQuery query;
    // Récupérer le prochain ID_CLIENT (séquence ou MAX)
    int nextId = 1;
    QSqlQuery idQuery;
    if (idQuery.exec("SELECT NVL(MAX(ID_CLIENT), 0) + 1 FROM CLIENT")) {
        if (idQuery.next()) {
            nextId = idQuery.value(0).toInt();
            qDebug() << "Prochain ID_CLIENT:" << nextId;
        }
    } else {
        qDebug() << "Erreur lors de la récupération du MAX(ID_CLIENT):" << idQuery.lastError().text();
    }
    
    query.prepare("INSERT INTO CLIENT (ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, CIN_CLIENT, DATE_NAISSANCE, ADRESSE, TEL_CLIENT, CODE_QR, COLUMN8) "
                  "VALUES (:id_client, :nom_client, :prenom_client, :cin_client, :date_naissance, :adresse, :tel_client, :code_qr, :column8)");

    query.bindValue(":id_client", nextId);
    query.bindValue(":nom_client", nom_client);
    query.bindValue(":prenom_client", prenom_client);
    query.bindValue(":cin_client", cin_client);
    query.bindValue(":date_naissance", date_naissance);
    query.bindValue(":adresse", adresse);
    query.bindValue(":tel_client", tel_client);
    query.bindValue(":code_qr", code_qr);
    query.bindValue(":column8", column8);

    qDebug() << "Exécution de l'INSERT...";
    if (!query.exec()) {
        qDebug() << "Erreur lors de l'insertion:" << query.lastError().text();
        qDebug() << "Code d'erreur natif:" << query.lastError().nativeErrorCode();
        qDebug() << "Type d'erreur:" << query.lastError().type();
        qDebug() << "Requête:" << query.lastQuery();
        return false;
    }
    
    qDebug() << "Client ajouté avec succès! ID:" << nextId;
    // Mettre à jour l'ID du client après insertion
    id_client = nextId;
    return true;
}

bool client::modifierE()
{
    if (id_client <= 0 || !validerDonnees()) return false;

    // Vérifier que le CIN est unique (en excluant le client actuel)
    if (checkIfCinExists(cin_client, id_client)) {
        qDebug() << "Erreur : Ce CIN existe déjà pour un autre client.";
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE CLIENT SET "
                  "NOM_CLIENT=:nom_client, PRENOM_CLIENT=:prenom_client, CIN_CLIENT=:cin_client, "
                  "DATE_NAISSANCE=:date_naissance, ADRESSE=:adresse, TEL_CLIENT=:tel_client, "
                  "CODE_QR=:code_qr, COLUMN8=:column8 WHERE ID_CLIENT=:id_client");

    query.bindValue(":id_client", id_client);
    query.bindValue(":nom_client", nom_client);
    query.bindValue(":prenom_client", prenom_client);
    query.bindValue(":cin_client", cin_client);
    query.bindValue(":date_naissance", date_naissance);
    query.bindValue(":adresse", adresse);
    query.bindValue(":tel_client", tel_client);
    query.bindValue(":code_qr", code_qr);
    query.bindValue(":column8", column8);

    if (!query.exec()) {
        qDebug() << "Erreur lors de la modification:" << query.lastError().text();
        return false;
    }
    
    return true;
}

QSqlQueryModel* client::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, CIN_CLIENT, DATE_NAISSANCE, ADRESSE, TEL_CLIENT, COLUMN8 FROM CLIENT ORDER BY ID_CLIENT";
    qDebug() << "Requête d'affichage:" << queryStr;
    model->setQuery(queryStr);
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'affichage:" << model->lastError().text();
    } else {
        qDebug() << "Nombre de clients affichés:" << model->rowCount();
    }
    
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Naiss."));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Email"));
    return model;
}

QSqlQueryModel* client::afficher(QString orderBy)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, CIN_CLIENT, DATE_NAISSANCE, ADRESSE, TEL_CLIENT, COLUMN8 FROM CLIENT ORDER BY " + orderBy;
    qDebug() << "Requête d'affichage avec tri:" << queryStr;
    model->setQuery(queryStr);
    
    if (model->lastError().isValid()) {
        qDebug() << "Erreur lors de l'affichage:" << model->lastError().text();
    } else {
        qDebug() << "Nombre de clients affichés:" << model->rowCount();
    }
    
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Naiss."));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Email"));
    return model;
}

bool client::supprimer(int id_client)
{
    QSqlQuery query;
    query.prepare("DELETE FROM CLIENT WHERE ID_CLIENT = :id_client");
    query.bindValue(":id_client", id_client);
    return query.exec();
}

bool client::checkIfClientExists(int id_client)
{
    QSqlQuery query;
    query.prepare("SELECT ID_CLIENT FROM CLIENT WHERE ID_CLIENT = :id_client");
    query.bindValue(":id_client", id_client);
    return query.exec() && query.next();
}

bool client::checkIfCinExists(QString cin_client, int excludeId)
{
    QSqlQuery query;
    if (excludeId > 0) {
        // Vérifier si le CIN existe en excluant l'ID spécifié (pour la modification)
        query.prepare("SELECT ID_CLIENT FROM CLIENT WHERE CIN_CLIENT = :cin_client AND ID_CLIENT != :exclude_id");
        query.bindValue(":cin_client", cin_client);
        query.bindValue(":exclude_id", excludeId);
    } else {
        // Vérifier si le CIN existe (pour l'ajout)
        query.prepare("SELECT ID_CLIENT FROM CLIENT WHERE CIN_CLIENT = :cin_client");
        query.bindValue(":cin_client", cin_client);
    }
    
    if (!query.exec()) {
        qDebug() << "Erreur lors de la vérification du CIN:" << query.lastError().text();
        qDebug() << "Requête SQL:" << query.lastQuery();
        return false;
    }
    
    if (query.next()) {
        qDebug() << "CIN trouvé avec ID:" << query.value(0).toInt();
        return true; // Le CIN existe déjà
    }
    qDebug() << "CIN non trouvé, peut être ajouté";
    return false; // Le CIN n'existe pas
}

QSqlQueryModel* client::rechercheParNom(QString nom)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT ID_CLIENT, NOM_CLIENT, PRENOM_CLIENT, CIN_CLIENT, DATE_NAISSANCE, ADRESSE, TEL_CLIENT, COLUMN8 FROM CLIENT WHERE NOM_CLIENT LIKE :nom ORDER BY ID_CLIENT");
    query.bindValue(":nom", "%" + nom + "%");
    if (query.exec()) {
        model->setQuery(query);
    } else {
        qDebug() << "Erreur recherche :" << query.lastError().text();
        return model;
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date Naiss."));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Email"));

    return model;
}

