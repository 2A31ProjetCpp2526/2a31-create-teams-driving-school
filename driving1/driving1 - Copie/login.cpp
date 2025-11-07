#include "login.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QFrame>
#include <QApplication>

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
    
    buttonLayout->addWidget(loginButton);
    buttonLayout->addWidget(forgotPasswordButton);
    
    frameLayout->addLayout(buttonLayout);
    
    // Centrer le frame
    mainLayout->addStretch();
    mainLayout->addWidget(mainFrame, 0, Qt::AlignCenter);
    mainLayout->addStretch();
    
    // Connecter les signaux
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
    connect(forgotPasswordButton, &QPushButton::clicked, this, &LoginDialog::onForgotPasswordClicked);
    
    // Permettre la connexion avec Entrée
    connect(usernameLineEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
    connect(passwordLineEdit, &QLineEdit::returnPressed, this, &LoginDialog::onLoginClicked);
}

LoginDialog::~LoginDialog()
{
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
        passwordLineEdit->clear();
        usernameLineEdit->setFocus();
    }
}

void LoginDialog::onForgotPasswordClicked()
{
    QMessageBox::information(this, "Récupération de mot de passe", 
                            "Fonctionnalité de récupération de mot de passe à implémenter.\n"
                            "Contactez l'administrateur pour réinitialiser votre mot de passe.");
}

bool LoginDialog::authenticateUser(const QString &username, const QString &password)
{
    // Pour l'instant, utilisons des identifiants simples pour la démonstration
    // Dans une vraie application, vous feriez une requête à une base de données
    
    if (username == "admin" && password == "admin123") {
        return true;
    }
    if (username == "user" && password == "user123") {
        return true;
    }
    if (username == "drivingschool" && password == "driving123") {
        return true;
    }
    
    return false;
}
