#include "login.h"
#include "mainwindow.h"
#include "connection.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFrame>
#include <QApplication>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QRandomGenerator>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QByteArray>

// Replace or configure these with your Twilio credentials (prefer reading from config)
static const QString TWILIO_SID = "ACec8afb432bfb55c899baa95a7661bdb8";
static const QString TWILIO_AUTH_TOKEN = "bc79c93cb8403829a486b040a3bd340e";
static const QString TWILIO_FROM_NUMBER = "+12084358393";

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Connexion - Driving School");
    setFixedSize(800, 500);
    setModal(true);

    // Créer le layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Créer le frame principal avec style
    QFrame *mainFrame = new QFrame();
    mainFrame->setStyleSheet("background-color: rgb(85, 170, 255); border-radius: 10px;");
    mainFrame->setFixedSize(671, 441);

    QVBoxLayout *frameLayout = new QVBoxLayout(mainFrame);

    // Titre
    QLabel *titleLabel = new QLabel("S'identifier:");
    titleLabel->setStyleSheet("font-size: 14pt; font-weight: bold; color: white;");
    titleLabel->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(titleLabel);

    // Formulaire de connexion
    QFormLayout *formLayout = new QFormLayout();

    // Champ nom d'utilisateur
    QLabel *usernameLabel = new QLabel("🔠ID ou Nom:");
    usernameLabel->setStyleSheet("font-size: 12pt; color: white;");
    usernameLineEdit = new QLineEdit();
    usernameLineEdit->setStyleSheet("padding: 8px; border-radius: 5px; border: 1px solid #ccc;");
    usernameLineEdit->setPlaceholderText("Entrez votre nom d'utilisateur");
    formLayout->addRow(usernameLabel, usernameLineEdit);

    // Champ mot de passe
    QLabel *passwordLabel = new QLabel("🔑Mot de passe:");
    passwordLabel->setStyleSheet("font-size: 12pt; color: white;");
    passwordLineEdit = new QLineEdit();
    passwordLineEdit->setEchoMode(QLineEdit::Password);
    passwordLineEdit->setStyleSheet("padding: 8px; border-radius: 5px; border: 1px solid #ccc;");
    passwordLineEdit->setPlaceholderText("Entrez votre mot de passe");
    formLayout->addRow(passwordLabel, passwordLineEdit);

    frameLayout->addLayout(formLayout);

    // Boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    loginButton = new QPushButton("Log in");
    loginButton->setStyleSheet(
        "background-color: #00008B; "
        "color: white; "
        "border-radius: 8px; "
        "padding: 10px 20px; "
        "font-size: 12pt; "
        "font-weight: bold;"
    );
    loginButton->setFixedSize(171, 40);

    forgotPasswordButton = new QPushButton("Récupérer mot de passe");
    forgotPasswordButton->setStyleSheet(
        "background-color: #00008B; "
        "color: white; "
        "border-radius: 8px; "
        "padding: 10px 20px; "
        "font-size: 12pt; "
        "font-weight: bold;"
    );
    forgotPasswordButton->setFixedSize(171, 40);

    // Bypass button to skip login (useful for testing or demo)
    bypassButton = new QPushButton("bypass");
    bypassButton->setStyleSheet(
        "background-color: #A9A9A9; "
        "color: black; "
        "border-radius: 8px; "
        "padding: 10px 20px; "
        "font-size: 10pt; "
        "font-weight: bold;"
    );
    bypassButton->setFixedSize(90, 30);

    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(forgotPasswordButton);
    buttonLayout->addWidget(bypassButton);

    frameLayout->addLayout(buttonLayout);

    // Centrer le frame
    mainLayout->addStretch();
    mainLayout->addWidget(mainFrame, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    // Connecter les signaux
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(forgotPasswordButton, &QPushButton::clicked, this, &LoginDialog::onForgotPasswordClicked);
    connect(bypassButton, &QPushButton::clicked, this, [this]() {
        m_role = "bypass"; // Optionally set a role for the main window
        accept(); // Close dialog with QDialog::Accepted to continue to main window
    });

    // Permettre la connexion avec Entrée
    connect(usernameLineEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
    connect(passwordLineEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);

    // Initialize network manager for SMS sending and connect finished handler
    networkAccessManager = new QNetworkAccessManager(this);
    connect(networkAccessManager, &QNetworkAccessManager::finished, this, &LoginDialog::replyFinished);
}

void LoginDialog::replyFinished(QNetworkReply *reply)
{
    if (!reply) return;
    if (reply->error() != QNetworkReply::NoError) {
        QMessageBox::critical(this, "Erreur SMS", reply->errorString());
        reply->deleteLater();
        return;
    }

    QByteArray response = reply->readAll();
    Q_UNUSED(response);
    QMessageBox::information(this, "SMS", "SMS envoyé avec succès !");
    reply->deleteLater();
}

LoginDialog::~LoginDialog()
{
}

QString LoginDialog::role() const
{
    return m_role;
}

void LoginDialog::onLoginClicked()
{
    QString username = usernameLineEdit->text().trimmed();
    QString password = passwordLineEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }

    if (authenticateUser(username, password)) {
        QMessageBox::information(this, "Succès", "Connexion réussie !");
        accept(); // Ferme la boîte de dialogue avec succès
    } else {
        QMessageBox::critical(this, "Erreur", "Nom d'utilisateur ou mot de passe incorrect.");
    }
}

void LoginDialog::envoyerSMS(const QString &destinataire, const QString &message)
{
    if (!networkAccessManager) {
        networkAccessManager = new QNetworkAccessManager(this);
        connect(networkAccessManager, &QNetworkAccessManager::finished, this, &LoginDialog::replyFinished);
    }

    if (destinataire.trimmed().isEmpty()) {
        QMessageBox::warning(this, "SMS", "Numéro de destination invalide.");
        return;
    }

    QString url = QString("https://api.twilio.com/2010-04-01/Accounts/%1/Messages.json").arg(TWILIO_SID);

    QByteArray postData;
    // Properly percent-encode body parts
    postData.append("To=");
    postData.append(QUrl::toPercentEncoding(destinataire));
    postData.append("&From=");
    postData.append(QUrl::toPercentEncoding(TWILIO_FROM_NUMBER));
    postData.append("&Body=");
    postData.append(QUrl::toPercentEncoding(message));

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("application/x-www-form-urlencoded")));
    QByteArray auth = QByteArray(QString("%1:%2").arg(TWILIO_SID).arg(TWILIO_AUTH_TOKEN).toUtf8()).toBase64();
    request.setRawHeader(QByteArray("Authorization"), QByteArray("Basic ") + auth);

    networkAccessManager->post(request, postData);
}

bool LoginDialog::sendSMS(const QString &phoneNumber, const QString &password, 
                          const QString &nom, const QString &prenom)
{
    if (phoneNumber.trimmed().isEmpty()) return false;

    QString message = QString("Bonjour %1 %2,\nVotre nouveau mot de passe est : %3\nCordialement.")
                          .arg(nom)
                          .arg(prenom)
                          .arg(password);

    envoyerSMS(phoneNumber, message);
    return true;
}

bool LoginDialog::authenticateUser(const QString &username, const QString &password)
{
    // Ensure DB connection is open
    if (!Connection::isDatabaseOpen()) {
        QMessageBox::critical(this, "Erreur de connexion",
                              "La connexion à la base de données n'est pas active.\nVeuillez contacter l'administrateur.");
        return false;
    }

    bool ok;
    int id = username.toInt(&ok);
    if (!ok) {
        // Allow authentication by name as well (username field may contain name)
        QSqlQuery query;
        query.prepare("SELECT ID_EMPLOYE, NOM_EMPLOYE, PRENOM, POSTE FROM EMPLOYE WHERE NOM_EMPLOYE = :nom AND MOT_DE_PASSEE = :pwd");
        query.bindValue(":nom", username);
        query.bindValue(":pwd", password);
        if (!query.exec()) {
            qDebug() << "Erreur requête auth (nom):" << query.lastError().text();
            return false;
        }
        if (query.next()) {
            int foundId = query.value(0).toInt();
            QString poste = query.value(3).toString();
            m_role = poste;
            Q_UNUSED(foundId);
            return true;
        }
        return false;
    }

    QSqlQuery query;
    query.prepare("SELECT ID_EMPLOYE, NOM_EMPLOYE, PRENOM, POSTE FROM EMPLOYE WHERE ID_EMPLOYE = :id AND MOT_DE_PASSEE = :pwd");
    query.bindValue(":id", id);
    query.bindValue(":pwd", password);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur DB", "Erreur lors de la vérification des identifiants :\n" + query.lastError().text());
        return false;
    }

    if (query.next()) {
        int foundId = query.value(0).toInt();
        QString poste = query.value(3).toString();
        m_role = poste;
        qDebug() << "[Login] Authenticated:" << foundId << query.value(1).toString();
        Q_UNUSED(foundId);
        return true;
    }

    return false;
}

void LoginDialog::onForgotPasswordClicked()
{
    bool ok;
    int idEmploye = QInputDialog::getInt(this, "Récupération de mot de passe",
                                         "Veuillez entrer l'ID de l'employé :",
                                         0, 0, 100000, 1, &ok);

    if (!ok) return;

    if (!Connection::isDatabaseOpen()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base impossible !");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT NOM_EMPLOYE, PRENOM, TEL_EMPLOYE FROM EMPLOYE WHERE ID_EMPLOYE = :id");
    query.bindValue(":id", idEmploye);

    if (!query.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Erreur SQL : " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Non trouvé", "Aucun employé trouvé avec cet ID.");
        return;
    }

    QString nom = query.value(0).toString();
    QString prenom = query.value(1).toString();
    QString telephone = query.value(2).toString();

    // Générer nouveau mot de passe
    QString newPassword;
    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    for (int i = 0; i < 8; ++i) {
        newPassword += chars[QRandomGenerator::global()->bounded(chars.length())];
    }

    // Mise à jour du mot de passe dans la base
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE EMPLOYE SET MOT_DE_PASSEE = :password WHERE ID_EMPLOYE = :id");
    updateQuery.bindValue(":password", newPassword);
    updateQuery.bindValue(":id", idEmploye);

    if (!updateQuery.exec()) {
        QMessageBox::critical(this, "Erreur SQL", "Échec mise à jour : " + updateQuery.lastError().text());
        return;
    }

    // Envoi SMS au numéro enregistré (ou fallback fixe si manquant)
    QString fullPhone = telephone.trimmed();
    if (fullPhone.isEmpty()) fullPhone = "+21695503367"; // fallback

    QString smsMessage = QString("Bonjour %1 %2,\nVotre nouveau mot de passe est : %3\nMerci.").arg(nom).arg(prenom).arg(newPassword);

    envoyerSMS(fullPhone, smsMessage);

    QMessageBox::information(this, "Mot de passe réinitialisé", "Le nouveau mot de passe a été généré et envoyé par SMS.");
}
