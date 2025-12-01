#ifndef EMPLOYEETABLE_H
#define EMPLOYEETABLE_H

#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include "employee.h"
#include "employeeservice.h"

class EmployeeTable : public QWidget
{
    Q_OBJECT

public:
    explicit EmployeeTable(EmployeeService *service, QWidget *parent = nullptr);

private slots:
    void onSearchEmployee();
    void onDeleteEmployee();
    void onExportData();
    void onSortChanged();
    void refreshTable();
    void onDataChanged();

private:
    void setupUI();
    void connectSignals();
    void populateTable(const QList<Employee> &employees);
    void setupTableHeaders();

    EmployeeService *m_service;
    
    // UI Components
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_searchLayout;
    QHBoxLayout *m_deleteLayout;
    QHBoxLayout *m_exportLayout;
    QHBoxLayout *m_sortLayout;
    
    QLabel *m_titleLabel;
    QLabel *m_searchLabel;
    QLabel *m_deleteLabel;
    QLabel *m_sortLabel;
    
    QLineEdit *m_searchEdit;
    QLineEdit *m_deleteEdit;
    
    QPushButton *m_searchButton;
    QPushButton *m_deleteButton;
    QPushButton *m_exportButton;
    
    QComboBox *m_sortComboBox;
    
    QTableWidget *m_tableWidget;
};

#endif // EMPLOYEETABLE_H
