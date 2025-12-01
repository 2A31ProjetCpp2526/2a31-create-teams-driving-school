#include "circuitratingdialog.h"
#include "circuit.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QMessageBox>
#include <QGroupBox>
#include <QApplication>
#include <QDebug>

CircuitRatingDialog::CircuitRatingDialog(int circuitId, int clientId, QWidget *parent)
    : QDialog(parent), circuitId(circuitId), clientId(clientId), currentRating(0)
{
    setWindowTitle("Rate Circuit");
    setModal(true);
    setFixedSize(500, 400);
    setStyleSheet(
        "QDialog { "
        "background-color: #2c3e50; "
        "color: white; "
        "border-radius: 10px; "
        "}"
        "QLabel { color: white; }"
        "QTextEdit { "
        "background-color: #34495e; "
        "color: white; "
        "border: 1px solid #4682b4; "
        "border-radius: 5px; "
        "padding: 5px; "
        "}"
        "QPushButton { "
        "background-color: #3498db; "
        "color: white; "
        "border: none; "
        "border-radius: 5px; "
        "padding: 8px 16px; "
        "font-weight: bold; "
        "}"
        "QPushButton:hover { background-color: #2980b9; }"
        "QPushButton:pressed { background-color: #21618c; }"
        "QPushButton#cancelButton { background-color: #e74c3c; }"
        "QPushButton#cancelButton:hover { background-color: #c0392b; }"
        "QPushButton#starButton { "
        "background-color: transparent; "
        "border: none; "
        "font-size: 24px; "
        "padding: 5px; "
        "}"
        "QPushButton#starButton:hover { background-color: rgba(255,255,255,0.1); }"
    );

    setupUI();
}

CircuitRatingDialog::~CircuitRatingDialog()
{
    // Cleanup is handled by Qt parent-child system
}

void CircuitRatingDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    titleLabel = new QLabel("🎯 Rate Your Circuit Experience", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; color: #3498db; margin-bottom: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);

    // Circuit Info
    Circuit *circuit = new Circuit(); // Temporary object to get circuit name
    // In a real implementation, you'd query the database for the circuit name
    circuitInfoLabel = new QLabel("Circuit: Getting circuit information...", this);
    circuitInfoLabel->setStyleSheet("font-size: 14px; color: #bdc3c7; margin-bottom: 15px;");
    circuitInfoLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(circuitInfoLabel);
    delete circuit;

    // Rating Section
    setupStars();

    // Comment Section
    QGroupBox *commentGroup = new QGroupBox("Additional Comments (Optional)", this);
    commentGroup->setStyleSheet(
        "QGroupBox { "
        "font-weight: bold; "
        "border: 2px solid #4682b4; "
        "border-radius: 5px; "
        "margin-top: 10px; "
        "} "
        "QGroupBox::title { "
        "subcontrol-origin: margin; "
        "left: 10px; "
        "padding: 0 5px 0 5px; "
        "color: #3498db; "
        "font-size: 14px; "
        "} "
    );

    QVBoxLayout *commentLayout = new QVBoxLayout(commentGroup);
    commentEdit = new QTextEdit(this);
    commentEdit->setPlaceholderText("Share your experience, suggestions, or any issues you encountered... (max 500 characters)");
    commentEdit->setMaximumHeight(80);
    commentEdit->setStyleSheet(
        "QTextEdit { "
        "background-color: #34495e; "
        "color: white; "
        "border: 1px solid #4682b4; "
        "border-radius: 5px; "
        "padding: 8px; "
        "font-size: 12px; "
        "} "
        "QTextEdit:focus { border-color: #3498db; } "
    );
    commentLayout->addWidget(commentEdit);

    QLabel *charLimitLabel = new QLabel("Maximum 500 characters", this);
    charLimitLabel->setStyleSheet("font-size: 10px; color: #7f8c8d; margin-top: 2px;");
    commentLayout->addWidget(charLimitLabel);

    mainLayout->addWidget(commentGroup);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    QPushButton *cancelButton = new QPushButton("Cancel", this);
    cancelButton->setObjectName("cancelButton");
    cancelButton->setFixedHeight(35);

    submitButton = new QPushButton("Submit Rating", this);
    submitButton->setFixedHeight(35);
    submitButton->setEnabled(false); // Initially disabled until rating is selected

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(submitButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    // Connect signals
    connect(submitButton, &QPushButton::clicked, this, &CircuitRatingDialog::submitRating);
    connect(cancelButton, &QPushButton::clicked, this, &CircuitRatingDialog::reject);

    // Character limit for comments
    connect(commentEdit, &QTextEdit::textChanged, this, [this]() {
        QString text = commentEdit->toPlainText();
        if (text.length() > 500) {
            text = text.left(500);
            commentEdit->setPlainText(text);
        }
    });

    mainLayout->addStretch();
}

void CircuitRatingDialog::createStarSelector()
{
    starWidget = new QWidget(this);
    QVBoxLayout *starLayout = new QVBoxLayout(starWidget);

    QLabel *ratingLabel = new QLabel("How would you rate this circuit?", this);
    ratingLabel->setStyleSheet("font-size: 16px; font-weight: bold; margin-bottom: 10px;");
    starLayout->addWidget(ratingLabel, 0, Qt::AlignCenter);

    QHBoxLayout *starsLayout = new QHBoxLayout();
    starsLayout->setSpacing(5);
    starsLayout->setAlignment(Qt::AlignCenter);

    starButtons.clear();
    for (int i = 1; i <= 5; i++) {
        QPushButton *starButton = new QPushButton("☆", this);
        starButton->setObjectName("starButton");
        starButton->setFixedSize(40, 40);
        starButton->setProperty("starValue", i);
        starButton->setStyleSheet(
            "QPushButton { "
            "background-color: transparent; "
            "border: none; "
            "font-size: 24px; "
            "color: #95a5a6; "
            "padding: 5px; "
            "}"
            "QPushButton:hover { "
            "color: #f39c12; "
            "background-color: rgba(255,255,255,0.1); "
            "transform: scale(1.1); "
            "}"
        );

        connect(starButton, &QPushButton::clicked, this, [this, i]() {
            setSelectedRating(i);
        });

        starButtons.append(starButton);
        starsLayout->addWidget(starButton);
    }

    starLayout->addLayout(starsLayout);

    QLabel *ratingTextLabel = new QLabel("Click on the stars to rate", this);
    ratingTextLabel->setStyleSheet("font-size: 12px; color: #7f8c8d; margin-top: 5px;");
    ratingTextLabel->setAlignment(Qt::AlignCenter);
    starLayout->addWidget(ratingTextLabel, 0, Qt::AlignCenter);
}

void CircuitRatingDialog::applyStyles()
{
    // Styles are applied in constructor
}
    QGroupBox *commentGroup = new QGroupBox("Additional Comments (Optional)", this);
    commentGroup->setStyleSheet(
        "QGroupBox { "
        "font-weight: bold; "
        "border: 2px solid #4682b4; "
        "border-radius: 5px; "
        "margin-top: 10px; "
        "}"
        "QGroupBox::title { "
        "subcontrol-origin: margin; "
        "left: 10px; "
        "padding: 0 5px 0 5px; "
        "color: #3498db; "
        "font-size: 14px; "
        "}"
    );

    QVBoxLayout *commentLayout = new QVBoxLayout(commentGroup);

    commentTextEdit = new QTextEdit(this);
    commentTextEdit->setPlaceholderText("Share your experience, suggestions, or any issues you encountered...");
    commentTextEdit->setMaximumHeight(80);
    commentTextEdit->setStyleSheet(
        "QTextEdit { "
        "background-color: #34495e; "
        "color: white; "
        "border: 1px solid #4682b4; "
        "border-radius: 5px; "
        "padding: 8px; "
        "font-size: 12px; "
        "}"
        "QTextEdit:focus { border-color: #3498db; }"
    );

    commentLayout->addWidget(commentTextEdit);

    QLabel *charLimitLabel = new QLabel("Maximum 500 characters", this);
    charLimitLabel->setStyleSheet("font-size: 10px; color: #7f8c8d; margin-top: 2px;");
    commentLayout->addWidget(charLimitLabel);

    layout()->addWidget(commentGroup);
}

void CircuitRatingDialog::createButtons()
{
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);

    cancelButton = new QPushButton("Cancel", this);
    cancelButton->setObjectName("cancelButton");
    cancelButton->setFixedHeight(35);

    submitButton = new QPushButton("Submit Rating", this);
    submitButton->setFixedHeight(35);
    submitButton->setEnabled(false); // Initially disabled until rating is selected

    buttonLayout->addStretch();
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(submitButton);
    buttonLayout->addStretch();

    layout()->addLayout(buttonLayout);

    // Connect buttons
    connect(submitButton, &QPushButton::clicked, this, &CircuitRatingDialog::submitRating);
    connect(cancelButton, &QPushButton::clicked, this, &CircuitRatingDialog::cancelRating);

    // Connect text edit for character limit
    connect(commentTextEdit, &QTextEdit::textChanged, this, [this]() {
        QString text = commentTextEdit->toPlainText();
        if (text.length() > 500) {
            text = text.left(500);
            commentTextEdit->setPlainText(text);
        }
    });
}

void CircuitRatingDialog::setSelectedRating(int rating)
{
    selectedRating = rating;

    // Update star appearances
    for (int i = 0; i < starButtons.size(); i++) {
        QPushButton *button = starButtons[i];
        bool isSelected = (i < rating);
        button->setText(isSelected ? "⭐" : "☆");
        button->setStyleSheet(
            QString("QPushButton { "
                    "background-color: transparent; "
                    "border: none; "
                    "font-size: 24px; "
                    "color: %1; "
                    "padding: 5px; "
                    "}")
                    "QPushButton:hover { "
                    "color: #f39c12; "
                    "background-color: rgba(255,255,255,0.1); "
                    "transform: scale(1.1); "
                    "}")
                    .arg(isSelected ? "#f1c40f" : "#95a5a6")
        );
    }

    // Enable submit button
    submitButton->setEnabled(true);
    submitButton->setText(QString("Submit %1 Star Rating").arg(rating));
}

void CircuitRatingDialog::submitRating()
{
    if (selectedRating == 0) {
        QMessageBox::warning(this, "Rating Required", "Please select a rating before submitting.");
        return;
    }

    // Check if client already rated this circuit
    if (CircuitRating::existeDeja(circuitId, clientId)) {
        QMessageBox::information(this, "Already Rated",
            "You have already rated this circuit. Each circuit can only be rated once.");
        reject();
        return;
    }

    // Create and save the rating
    CircuitRating rating(0, circuitId, clientId, selectedRating, commentTextEdit->toPlainText().trimmed());

    if (rating.ajouter()) {
        QMessageBox::information(this, "Thank You!", "Your rating has been submitted successfully!");
        accept();
    } else {
        QMessageBox::critical(this, "Error", "Failed to submit rating. Please try again.");
    }
}

void CircuitRatingDialog::cancelRating()
{
    if (selectedRating > 0 || !commentTextEdit->toPlainText().isEmpty()) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this, "Discard Rating?",
            "Are you sure you want to cancel? Your rating will not be saved.",
            QMessageBox::Yes | QMessageBox::No
        );

        if (reply == QMessageBox::No) {
            return;
        }
    }

    reject();
}