#ifndef CIRCUITRATINGDIALOG_H
#define CIRCUITRATINGDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QMessageBox>
#include "circuitrating.h"

class CircuitRatingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CircuitRatingDialog(int circuitId, int clientId, QWidget *parent = nullptr);
    ~CircuitRatingDialog();

private slots:
    void submitRating();
    void cancelRating();

private:
    void setupUI();
    void createStarSelector();
    void createCommentSection();
    void createButtons();

    int circuitId;
    int clientId;

    // UI Elements
    QLabel *titleLabel;
    QLabel *circuitInfoLabel;
    QWidget *starWidget;
    QList<QPushButton*> starButtons;
    QTextEdit *commentTextEdit;
    QPushButton *submitButton;
    QPushButton *cancelButton;

    int selectedRating;
};

#endif // CIRCUITRATINGDIALOG_H