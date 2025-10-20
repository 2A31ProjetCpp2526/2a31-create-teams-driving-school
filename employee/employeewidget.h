#ifndef EMPLOYEEWIDGET_H
#define EMPLOYEEWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QMessageBox>
#include "employee.h"
#include "employeeservice.h"

class EmployeeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EmployeeWidget(EmployeeService *service, QWidget *parent = nullptr);

private slots:
    void onAddEmployee();
    void clearForm();

private:
    void setupUI();
    void connectSignals();
    Employee createEmployeeFromForm() const;
    bool validateForm() const;

    EmployeeService *m_service;
    
    // UI Components
    QGroupBox *m_groupBox;
    QLineEdit *m_idEdit;
    QLineEdit *m_nomEdit;
    QLineEdit *m_prenomEdit;
    QLineEdit *m_posteEdit;
    QLineEdit *m_motDePasseEdit;
    QLineEdit *m_telephoneEdit;
    QDateEdit *m_dateEmbaucheEdit;
    QPushButton *m_addButton;
    QPushButton *m_clearButton;
};

#endif // EMPLOYEEWIDGET_H
