#include "seance.h"
#include "connection.h"
#include <QSqlQuery>
#include <QDebug>
#include <QSqlError>
#include <QMessageBox>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QDesktopServices>
#include <QUrl>
#include <QFont>
#include <QFontMetrics>

#include <QSqlQuery>
#include <QDebug>
#include <QDebug>

#include <QSslSocket>  // AJOUTEZ CETTE LIGNE
#include <QEventLoop>  // AJOUTEZ AUSSI CETTE LIGNE

Seance::Seance() : id_seance(0), duree(0), id_client(0), id_moniteur(0) {}

Seance::Seance(int id, QDate date, QTime heure, int duree, QString type, QString appareil,
               int idClient, QString immatricule, int idMoniteur)
    : id_seance(id), date_seance(date), heure_debut(heure), duree(duree), type(type),
    appareil(appareil), id_client(idClient), immatricule(immatricule), id_moniteur(idMoniteur)
{}

bool Seance::existeDeja(int id)
{
    QSqlQuery query;
    query.prepare("SELECT ID_SEANCE FROM SEANCE WHERE ID_SEANCE = :id");
    query.bindValue(":id", id);

    if (query.exec() && query.next()) {
        return true;
    }
    return false;
}

bool Seance::ajouter()
{
    qDebug() << "=== TENTATIVE AJOUT SÉANCE ===";
    qDebug() << "ID:" << id_seance;
    qDebug() << "Date:" << date_seance;
    qDebug() << "Heure:" << heure_debut;
    qDebug() << "Durée:" << duree;
    qDebug() << "Type:" << type;
    qDebug() << "Appareil:" << appareil;
    qDebug() << "ID Client:" << id_client;
    qDebug() << "Immatricule:" << immatricule;
    qDebug() << "ID Moniteur:" << id_moniteur;

    if (existeDeja(id_seance)) {
        qDebug() << "❌ ID existe déjà:" << id_seance;
        QMessageBox::warning(nullptr, "Erreur", "Cet ID de séance existe déjà !");
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO SEANCE (ID_SEANCE, IMMATRICULE, ID_CLIENT, ID_MONITEUR, DATE_SEANCE, HEURE_DEBUT, TYPE, APPAREIL, DUREE) "
                  "VALUES (:id, :immatricule, :id_client, :id_moniteur, :date, :heure, :type, :appareil, :duree)");

    query.bindValue(":id", id_seance);
    query.bindValue(":immatricule", immatricule);
    query.bindValue(":id_client", id_client);
    query.bindValue(":id_moniteur", id_moniteur);
    query.bindValue(":date", date_seance);
    query.bindValue(":heure", heure_debut.toString("HH:mm"));
    query.bindValue(":type", type);
    query.bindValue(":appareil", appareil);
    query.bindValue(":duree", duree);

    bool success = query.exec();

    if (success) {
        qDebug() << "✅ AJOUT RÉUSSI !";
        return true;
    } else {
        qDebug() << "❌ ERREUR SQL:" << query.lastError().text();
        qDebug() << "Message driver:" << query.lastError().driverText();
        qDebug() << "Message database:" << query.lastError().databaseText();
        QMessageBox::critical(nullptr, "Erreur SQL", "Erreur lors de l'ajout : " + query.lastError().text());
        return false;
    }
}

bool Seance::modifier()
{
    qDebug() << "=== DÉBUT MODIFICATION SÉANCE ===";
    qDebug() << "ID à modifier:" << id_seance;

    if (!existeDeja(id_seance)) {
        qDebug() << "❌ Séance non trouvée avec ID:" << id_seance;
        QMessageBox::warning(nullptr, "Erreur", "Cette séance n'existe pas !");
        return false;
    }

    QSqlQuery query;
    query.prepare("UPDATE SEANCE SET IMMATRICULE=:immatricule, ID_CLIENT=:id_client, ID_MONITEUR=:id_moniteur, "
                  "DATE_SEANCE=:date, HEURE_DEBUT=:heure, TYPE=:type, APPAREIL=:appareil, DUREE=:duree "
                  "WHERE ID_SEANCE=:id");

    query.bindValue(":id", id_seance);
    query.bindValue(":immatricule", immatricule);
    query.bindValue(":id_client", id_client);
    query.bindValue(":id_moniteur", id_moniteur);
    query.bindValue(":date", date_seance);
    query.bindValue(":heure", heure_debut.toString("HH:mm"));
    query.bindValue(":type", type);
    query.bindValue(":appareil", appareil);
    query.bindValue(":duree", duree);

    bool success = query.exec();
    if (!success) {
        qDebug() << "❌ Erreur lors de la modification:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la modification : " + query.lastError().text());
    } else {
        qDebug() << "✅ Séance modifiée avec succès !";
    }

    return success;
}

bool Seance::supprimer(int id)
{
    qDebug() << "=== DÉBUT SUPPRESSION SÉANCE ===";
    qDebug() << "ID à supprimer:" << id;

    if (!existeDeja(id)) {
        qDebug() << "❌ Séance non trouvée avec ID:" << id;
        QMessageBox::warning(nullptr, "Erreur", "Cette séance n'existe pas !");
        return false;
    }

    QSqlQuery query;
    query.prepare("DELETE FROM SEANCE WHERE ID_SEANCE = :id");
    query.bindValue(":id", id);

    bool success = query.exec();
    if (!success) {
        qDebug() << "❌ Erreur lors de la suppression:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de la suppression : " + query.lastError().text());
    } else {
        qDebug() << "✅ Séance supprimée avec succès !";
    }

    return success;
}

QSqlQueryModel* Seance::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT ID_SEANCE, IMMATRICULE, ID_CLIENT, ID_MONITEUR, DATE_SEANCE, HEURE_DEBUT, TYPE, APPAREIL, DUREE FROM SEANCE");

    if (model->lastError().isValid()) {
        qDebug() << "❌ Erreur lors de l'affichage:" << model->lastError().text();
    }

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Séance"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Immatricule"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Moniteur"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Heure Début"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Appareil"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Durée"));

    return model;
}

QSqlQueryModel* Seance::rechercher(const QString& critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();

    bool isNumeric;
    int numericValue = critere.toInt(&isNumeric);

    QString queryStr;
    QSqlQuery query;

    if (isNumeric) {
        // Recherche par ID (reste exacte)
        queryStr = "SELECT ID_SEANCE, IMMATRICULE, ID_CLIENT, ID_MONITEUR, DATE_SEANCE, HEURE_DEBUT, TYPE, APPAREIL, DUREE "
                   "FROM SEANCE WHERE ID_SEANCE = :id_exact";
        query.prepare(queryStr);
        query.bindValue(":id_exact", numericValue);
    } else {
        // 🔥 RECHERCHE AVEC LIKE - COMMENCE PAR LE CRITÈRE
        queryStr = "SELECT ID_SEANCE, IMMATRICULE, ID_CLIENT, ID_MONITEUR, DATE_SEANCE, HEURE_DEBUT, TYPE, APPAREIL, DUREE "
                   "FROM SEANCE WHERE "
                   "TYPE LIKE :critere OR "
                   "APPAREIL LIKE :critere OR "
                   "IMMATRICULE LIKE :critere OR "
                   "CAST(DATE_SEANCE AS TEXT) LIKE :critere OR "
                   "HEURE_DEBUT LIKE :critere";
        query.prepare(queryStr);
        query.bindValue(":critere", critere + "%"); // 🔥 Commence par le critère
    }

    qDebug() << "🔍 Requête de recherche:" << query.lastQuery();
    qDebug() << "🔍 Critère:" << critere << "isNumeric:" << isNumeric;

    if (query.exec()) {
        model->setQuery(query);
        qDebug() << "✅ Recherche réussie - Résultats:" << model->rowCount();
    } else {
        qDebug() << "❌ Erreur recherche:" << query.lastError().text();
    }

    // Headers
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Séance"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Immatricule"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Moniteur"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Heure Début"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Appareil"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Durée"));

    return model;
}

QSqlQueryModel* Seance::trier(const QString& critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QString queryStr = "SELECT ID_SEANCE, IMMATRICULE, ID_CLIENT, ID_MONITEUR, DATE_SEANCE, HEURE_DEBUT, TYPE, APPAREIL, DUREE FROM SEANCE ";

    qDebug() << "🔍 Critère de tri reçu:" << critere;

    if (critere == "Par ID") {
        queryStr += "ORDER BY ID_SEANCE ASC";
        qDebug() << "✅ Tri par ID";
    } else if (critere == "Par Date") {
        queryStr += "ORDER BY DATE_SEANCE ASC, HEURE_DEBUT ASC";
        qDebug() << "✅ Tri par Date";
    } else if (critere == "Par Heure") {
        queryStr += "ORDER BY HEURE_DEBUT ASC";
        qDebug() << "✅ Tri par Heure";
    } else if (critere == "Par Durée") {
        queryStr += "ORDER BY DUREE ASC";
        qDebug() << "✅ Tri par Durée";
    } else if (critere == "Par Type") {
        queryStr += "ORDER BY TYPE ASC";
        qDebug() << "✅ Tri par Type";
    } else {
        // Par défaut
        queryStr += "ORDER BY DATE_SEANCE ASC, HEURE_DEBUT ASC";
        qDebug() << "✅ Tri par défaut (Date)";
    }

    qDebug() << "📝 Requête SQL:" << queryStr;

    model->setQuery(queryStr);

    if (model->lastError().isValid()) {
        qDebug() << "❌ Erreur SQL tri:" << model->lastError().text();
        qDebug() << "Message détaillé:" << model->lastError().driverText();
    } else {
        qDebug() << "✅ Tri réussi -" << model->rowCount() << "lignes";
    }

    // Headers (assurez-vous qu'ils sont corrects)
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID Séance"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Immatricule"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("ID Moniteur"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Date"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Heure Début"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Appareil"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Durée"));

    return model;
}
QMap<QString, int> Seance::getStatistiquesParType()
{
    QMap<QString, int> statistiques;

    QSqlQuery query;
    query.prepare("SELECT TYPE, COUNT(*) as nombre FROM SEANCE GROUP BY TYPE");

    if (query.exec()) {
        while (query.next()) {
            QString type = query.value("TYPE").toString();
            int count = query.value("nombre").toInt();
            statistiques[type] = count;
        }
    } else {
        qDebug() << "❌ Erreur statistiques par type:" << query.lastError().text();
    }

    return statistiques;
}
bool Seance::envoyerRappelEmail(int idSeance)
{
    qDebug() << "=== TENTATIVE ENVOI RAPPEL EMAIL ===";
    qDebug() << "ID Séance:" << idSeance;

    // VÉRIFICATION SI LA SÉANCE EXISTE
    if (!existeDeja(idSeance)) {
        qDebug() << "❌ Séance non trouvée ID:" << idSeance;
        QMessageBox::critical(nullptr, "Erreur", "❌ Cette séance n'existe pas dans la base de données !");
        return false;
    }

    // REQUÊTE POUR RÉCUPÉRER LA SÉANCE
    QSqlQuery query;
    query.prepare("SELECT DATE_SEANCE, HEURE_DEBUT, TYPE, DUREE, APPAREIL, "
                  "ID_CLIENT, IMMATRICULE, ID_MONITEUR "
                  "FROM SEANCE WHERE ID_SEANCE = :id");
    query.bindValue(":id", idSeance);

    if (!query.exec()) {
        qDebug() << "❌ Erreur requête séance:" << query.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "❌ Erreur base de données: " + query.lastError().text());
        return false;
    }

    if (!query.next()) {
        qDebug() << "❌ Aucune donnée séance pour ID:" << idSeance;
        QMessageBox::critical(nullptr, "Erreur", "❌ Aucune donnée trouvée pour cette séance !");
        return false;
    }

    // Récupération des données
    QDate dateSeance = query.value("DATE_SEANCE").toDate();
    QTime heureSeance = query.value("HEURE_DEBUT").toTime();
    QString typeSeance = query.value("TYPE").toString();
    QString appareil = query.value("APPAREIL").toString();
    int duree = query.value("DUREE").toInt();
    int idClient = query.value("ID_CLIENT").toInt();
    QString immatricule = query.value("IMMATRICULE").toString();
    int idMoniteur = query.value("ID_MONITEUR").toInt();

    qDebug() << "📅 Séance trouvée - Client ID:" << idClient;

    // RÉCUPÉRATION DU CLIENT
    QSqlQuery queryClient;
    queryClient.prepare("SELECT NOM_CLIENT, PRENOM_CLIENT, COLUMN8 as EMAIL FROM CLIENT WHERE ID_CLIENT = :id_client");
    queryClient.bindValue(":id_client", idClient);

    QString email = "";
    QString nomClient = "";
    QString prenomClient = "";

    if (queryClient.exec()) {
        if (queryClient.next()) {
            nomClient = queryClient.value("NOM_CLIENT").toString();
            prenomClient = queryClient.value("PRENOM_CLIENT").toString();
            email = queryClient.value("EMAIL").toString();

            qDebug() << "✅ Client trouvé:" << prenomClient << nomClient;
            qDebug() << "📧 Email récupéré:" << (email.isEmpty() ? "VIDE" : email);

            if (email.isEmpty() || email.isNull() || email == "(null)" || email == "NULL") {
                qDebug() << "❌ Email vide ou null pour client ID:" << idClient;
                QMessageBox::critical(nullptr, "Erreur",
                                      "❌ Le client n'a pas d'email renseigné !\n\n"
                                      "ID Client: " + QString::number(idClient) + "\n"
                                                                        "Nom: " + prenomClient + " " + nomClient + "\n\n"
                                                                             "Veuillez ajouter un email dans la table CLIENT (colonne COLUMN8).");
                return false;
            }
        } else {
            qDebug() << "❌ Client non trouvé ID:" << idClient;
            QMessageBox::critical(nullptr, "Erreur",
                                  "❌ Client non trouvé !\n\n"
                                  "ID Client: " + QString::number(idClient));
            return false;
        }
    } else {
        qDebug() << "❌ Erreur requête client:" << queryClient.lastError().text();
        QMessageBox::critical(nullptr, "Erreur", "❌ Erreur base de données client: " + queryClient.lastError().text());
        return false;
    }

    // Récupération moniteur
    QString moniteurNom = "Moniteur";
    QString moniteurPrenom = "";
    QSqlQuery queryMoniteur;
    queryMoniteur.prepare("SELECT NOM, PRENOM FROM MONITEUR WHERE ID_MONITEUR = :id_moniteur");
    queryMoniteur.bindValue(":id_moniteur", idMoniteur);

    if (queryMoniteur.exec() && queryMoniteur.next()) {
        moniteurNom = queryMoniteur.value("NOM").toString();
        moniteurPrenom = queryMoniteur.value("PRENOM").toString();
        if (moniteurNom.isEmpty()) moniteurNom = "Moniteur";
        if (moniteurPrenom.isEmpty()) moniteurPrenom = "ID: " + QString::number(idMoniteur);
    }

    // Récupération véhicule
    QString vehiculeMarque = "Véhicule";
    QString vehiculeModele = "";
    QSqlQuery queryVehicule;
    queryVehicule.prepare("SELECT MARQUE, MODELE FROM VEHICULE WHERE IMMATRICULE = :immatricule");
    queryVehicule.bindValue(":immatricule", immatricule);

    if (queryVehicule.exec() && queryVehicule.next()) {
        vehiculeMarque = queryVehicule.value("MARQUE").toString();
        vehiculeModele = queryVehicule.value("MODELE").toString();
        if (vehiculeMarque.isEmpty()) vehiculeMarque = "Marque inconnue";
        if (vehiculeModele.isEmpty()) vehiculeModele = "Modèle inconnu";
    }

    qDebug() << "✅ Toutes les données récupérées avec succès";
    qDebug() << "👤 Client:" << prenomClient << nomClient;
    qDebug() << "📧 Email final:" << email;

    // Construction du message email
    QString sujet = "🚗 RoadMaster - Rappel de Votre Séance de Conduite";

    QString messageBody = QString(
                              "Bonjour %1 %2,\n\n"
                              "Ceci est un rappel amical pour votre séance de conduite prévue :\n\n"
                              "📅 DATE : %3\n"
                              "⏰ HEURE : %4\n"
                              "🕐 DURÉE : %5 minutes\n"
                              "📋 TYPE : %6\n"
                              "🚗 APPAREIL : %7\n"
                              "👨‍🏫 MONITEUR : %8 %9\n"
                              "🚙 VÉHICULE : %10 %11\n\n"
                              "📍 LIEU : Auto-école RoadMaster\n"
                              "📞 TÉLÉPHONE : +216 27 396 388\n\n"
                              "💡 CONSEILS :\n"
                              "• Merci d'arriver 10 minutes avant l'heure prévue\n"
                              "• Pensez à apporter votre carte d'identité\n"
                              "• Portez des chaussures adaptées à la conduite\n\n"
                              "En cas d'empêchement, merci de nous prévenir au moins 24h à l'avance.\n\n"
                              "Nous sommes impatients de vous accompagner dans votre apprentissage !\n\n"
                              "Cordialement,\n"
                              "L'équipe RoadMaster 🚗\n"
                              "📞 +216 27 396 388\n"
                              "📧 contact@roadmaster.tn\n"
                              "🌐 www.roadmaster.tn"
                              ).arg(prenomClient, nomClient,
                                   dateSeance.toString("dd/MM/yyyy"),
                                   heureSeance.toString("HH:mm"),
                                   QString::number(duree),
                                   typeSeance,
                                   appareil,
                                   moniteurPrenom, moniteurNom,
                                   vehiculeMarque, vehiculeModele);

    // ==================== SOLUTION SIMPLE : OUVRIR LE CLIENT EMAIL ====================

    // Encodage URL pour le sujet et le corps
    QString sujetEncode = QUrl::toPercentEncoding(sujet);
    QString bodyEncode = QUrl::toPercentEncoding(messageBody);

    // Construction de l'URL mailto
    QString mailtoUrl = QString("mailto:%1?subject=%2&body=%3")
                            .arg(email)
                            .arg(sujetEncode)
                            .arg(bodyEncode);

    qDebug() << "📧 Ouverture du client email...";
    qDebug() << "URL:" << mailtoUrl;

    // Ouvrir le client email par défaut
    if (QDesktopServices::openUrl(QUrl(mailtoUrl))) {
        qDebug() << "✅ Client email ouvert avec succès !";

        // Sauvegarder le log
        QFile file("emails_log.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << "=== CLIENT EMAIL OUVERT ===\n";
            stream << "Date: " << QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss") << "\n";
            stream << "ID Séance: " << idSeance << "\n";
            stream << "À: " << email << "\n";
            stream << "Client: " << prenomClient << " " << nomClient << " (ID: " << idClient << ")\n";
            stream << "Séance: " << dateSeance.toString("dd/MM/yyyy") << " " << heureSeance.toString("HH:mm") << "\n";
            stream << "Sujet: " << sujet << "\n";
            stream << "Statut: CLIENT EMAIL OUVERT - UTILISATEUR DOIT CLIQUER ENVOYER\n";
            stream << "=================================\n\n";
            file.close();
        }

        QMessageBox::information(nullptr, "Succès",
                                 "✅ Client email ouvert !\n\n"
                                 "📧 Destinataire: " + email + "\n"
                                               "👤 Client: " + prenomClient + " " + nomClient + "\n"
                                                                        "📅 Séance: " + dateSeance.toString("dd/MM/yyyy") + " " + heureSeance.toString("HH:mm") + "\n\n"
                                                                                                                 "Le message est pré-rempli dans votre client email.\n"
                                                                                                                 "Il ne reste qu'à cliquer sur 'Envoyer'.");

        return true;
    } else {
        qDebug() << "❌ Impossible d'ouvrir le client email";

        // Sauvegarder le log d'erreur
        QFile file("emails_log.txt");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << "=== ERREUR OUVERTURE CLIENT EMAIL ===\n";
            stream << "Date: " << QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss") << "\n";
            stream << "ID Séance: " << idSeance << "\n";
            stream << "À: " << email << "\n";
            stream << "Client: " << prenomClient << " " << nomClient << "\n";
            stream << "Erreur: Impossible d'ouvrir le client email par défaut\n";
            stream << "=================================\n\n";
            file.close();
        }

        QMessageBox::critical(nullptr, "Erreur",
                              "❌ Impossible d'ouvrir le client email !\n\n"
                              "Veuillez :\n"
                              "1. Vérifier qu'un client email est configuré sur votre système\n"
                              "2. Ou envoyer l'email manuellement à : " + email + "\n\n"
                                            "Sujet: " + sujet + "\n\n"
                                            "Message:\n" + messageBody);

        return false;
    }
}
// ============================================================================
// MÉTHODES POUR LE CALENDRIER - UNE SEULE FOIS !
// ============================================================================

QList<Seance> Seance::getSeancesParDate(const QDate& date)
{
    QList<Seance> seances;

    QSqlQuery query;
    query.prepare("SELECT ID_SEANCE, DATE_SEANCE, HEURE_DEBUT, DUREE, TYPE, APPAREIL, "
                  "ID_CLIENT, IMMATRICULE, ID_MONITEUR "
                  "FROM SEANCE WHERE DATE_SEANCE = :date "
                  "ORDER BY HEURE_DEBUT");
    query.bindValue(":date", date);

    if (query.exec()) {
        while (query.next()) {
            Seance s;
            s.setId(query.value("ID_SEANCE").toInt());
            s.setDate(query.value("DATE_SEANCE").toDate());
            s.setHeure(query.value("HEURE_DEBUT").toTime());
            s.setDuree(query.value("DUREE").toInt());
            s.setType(query.value("TYPE").toString());
            s.setAppareil(query.value("APPAREIL").toString());
            s.setIdClient(query.value("ID_CLIENT").toInt());
            s.setImmatricule(query.value("IMMATRICULE").toString());
            s.setIdMoniteur(query.value("ID_MONITEUR").toInt());

            seances.append(s);
        }
    } else {
        qDebug() << "❌ Erreur récupération séances par date:" << query.lastError().text();
    }

    return seances;
}

QMap<QDate, int> Seance::getSeancesParMois(int mois, int annee)
{
    QMap<QDate, int> seancesParDate;

    QSqlQuery query;
    query.prepare("SELECT DATE_SEANCE, COUNT(*) as nb_seances "
                  "FROM SEANCE "
                  "WHERE EXTRACT(MONTH FROM DATE_SEANCE) = :mois "
                  "AND EXTRACT(YEAR FROM DATE_SEANCE) = :annee "
                  "GROUP BY DATE_SEANCE");
    query.bindValue(":mois", mois);
    query.bindValue(":annee", annee);

    if (query.exec()) {
        while (query.next()) {
            QDate date = query.value("DATE_SEANCE").toDate();
            int nbSeances = query.value("nb_seances").toInt();
            seancesParDate[date] = nbSeances;
        }
    } else {
        qDebug() << "❌ Erreur récupération séances par mois:" << query.lastError().text();
    }

    return seancesParDate;
}
bool Seance::exporterPDF(const QString& fileName, QSqlQueryModel* model)
{
    qDebug() << "=== TEST PDF ULTRA SIMPLE ===";

    if (!model) model = afficher();
    if (!model || model->rowCount() == 0) return false;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));

    QPainter painter(&pdfWriter);

    // Police simple
    QFont font("Arial", 14);
    painter.setFont(font);

    // Titre TRÈS SIMPLE
    painter.drawText(100, 100, "ROADMASTER SEANCES");
    painter.drawText(100, 130, QDate::currentDate().toString("dd/MM/yyyy"));

    // Quelques données
    int y = 180;
    painter.drawText(100, y, "ID  |  DATE  |  HEURE  |  TYPE");
    y += 30;

    for (int row = 0; row < qMin(10, model->rowCount()); ++row) {
        QString id = model->data(model->index(row, 0)).toString();
        QString date = model->data(model->index(row, 4)).toString();
        QString heure = model->data(model->index(row, 5)).toString();
        QString type = model->data(model->index(row, 6)).toString();

        painter.drawText(100, y, QString("%1 | %2 | %3 | %4").arg(id, date, heure, type));
        y += 20;
    }

    painter.end();
    qDebug() << "✅ PDF test cree:" << fileName;
    return true;
}
QList<Seance> Seance::getAllSeances()
{
    QList<Seance> seances;

    QSqlQuery query;
    query.prepare("SELECT ID_SEANCE, IMMATRICULE, ID_CLIENT, ID_MONITEUR, DATE_SEANCE, HEURE_DEBUT, TYPE, APPAREIL, DUREE FROM SEANCE");

    if (query.exec()) {
        while (query.next()) {
            Seance s;
            s.setId(query.value("ID_SEANCE").toInt());
            s.setImmatricule(query.value("IMMATRICULE").toString());
            s.setIdClient(query.value("ID_CLIENT").toInt());
            s.setIdMoniteur(query.value("ID_MONITEUR").toInt());
            s.setDate(query.value("DATE_SEANCE").toDate());
            s.setHeure(query.value("HEURE_DEBUT").toTime());
            s.setType(query.value("TYPE").toString());
            s.setAppareil(query.value("APPAREIL").toString());
            s.setDuree(query.value("DUREE").toInt());

            seances.append(s);
        }
    } else {
        qDebug() << "❌ Erreur récupération séances:" << query.lastError().text();
    }

    return seances;
}

QString Seance::genererHTMLSeances(const QList<Seance>& seances)
{
    QString html;

    // HTML de base avec style amélioré
    html = R"(
    <html>
    <head>
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; }
        h1 { color: #2c3e50; text-align: center; border-bottom: 2px solid #3498db; padding-bottom: 10px; }
        table { width: 100%; border-collapse: collapse; margin-top: 20px; }
        th { background-color: #3498db; color: white; padding: 12px; text-align: left; }
        td { padding: 10px; border-bottom: 1px solid #ddd; }
        tr:nth-child(even) { background-color: #f2f2f2; }
        tr:hover { background-color: #e8f4f8; }
        .info { color: #7f8c8d; text-align: center; margin: 10px 0; }
    </style>
    </head>
    <body>
    )";

    html += "<h1>🚗 RoadMaster - Liste des Séances</h1>";
    html += "<div class='info'>Export du " + QDate::currentDate().toString("dd/MM/yyyy") + " à " + QTime::currentTime().toString("HH:mm") + "</div>";

    html += "<table>";
    html += "<tr>"
            "<th>ID</th>"
            "<th>Immatricule</th>"
            "<th>Client</th>"
            "<th>Moniteur</th>"
            "<th>Date</th>"
            "<th>Heure</th>"
            "<th>Type</th>"
            "<th>Appareil</th>"
            "<th>Durée</th>"
            "</tr>";

    // Ajouter chaque séance
    for (const Seance& seance : seances) {
        html += "<tr>";
        html += "<td>" + QString::number(seance.getId()) + "</td>";
        html += "<td>" + seance.getImmatricule() + "</td>";
        html += "<td>" + QString::number(seance.getIdClient()) + "</td>";
        html += "<td>" + QString::number(seance.getIdMoniteur()) + "</td>";
        html += "<td>" + seance.getDate().toString("dd/MM/yyyy") + "</td>";
        html += "<td>" + seance.getHeure().toString("HH:mm") + "</td>";
        html += "<td>" + seance.getType() + "</td>";
        html += "<td>" + seance.getAppareil() + "</td>";
        html += "<td>" + QString::number(seance.getDuree()) + " min</td>";
        html += "</tr>";
    }

    html += "</table>";
    html += "<div class='info'>Total: " + QString::number(seances.size()) + " séances</div>";
    html += "</body></html>";

    return html;
}
