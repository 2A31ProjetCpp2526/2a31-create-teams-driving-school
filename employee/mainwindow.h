#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include "employeeservice.h"
#include "employeewidget.h"
#include "employeetable.h"
#include "statisticswidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void setupUI();
    void setupModules();
    
    Ui::MainWindow *ui;
    
    // Services et modules
    EmployeeService *m_employeeService;
    EmployeeWidget *m_employeeWidget;
    EmployeeTable *m_employeeTable;
    StatisticsWidget *m_statisticsWidget;
    
    // Layouts
    QTabWidget *m_tabWidget;
    QWidget *m_managementTab;
    QWidget *m_statisticsTab;
    QHBoxLayout *m_managementLayout;
};
#endif // MAINWINDOW_H

