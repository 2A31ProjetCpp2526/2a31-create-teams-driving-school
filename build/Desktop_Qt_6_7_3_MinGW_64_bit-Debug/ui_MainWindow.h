/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDateEdit>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *lineEdit;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QLabel *label_4;
    QLineEdit *lineEdit_4;
    QLabel *label_5;
    QLineEdit *lineEdit_5;
    QLabel *label_6;
    QLineEdit *lineEdit_6;
    QLabel *label_7;
    QDateEdit *dateEdit;
    QPushButton *pushButton;
    QLabel *label_21;
    QTableWidget *tableWidget;
    QLabel *label_8;
    QPushButton *pushButton_2;
    QLineEdit *lineEdit_13;
    QLabel *label_17;
    QPushButton *pushButton_4;
    QLineEdit *lineEdit_14;
    QLabel *label_18;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QLabel *label_19;
    QComboBox *comboBox;
    QLabel *label_20;
    QPushButton *pushButton_5;
    QWidget *tab_2;
    QLabel *label_22;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1323, 771);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(20, 30, 1321, 711));
        tab = new QWidget();
        tab->setObjectName("tab");
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(10, 0, 281, 671));
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"	font: 900 9pt \"Segoe UI\";\n"
"	font: 900 9pt \"Segoe UI\";\n"
"	font: 16pt \"Segoe UI\";\n"
"  \n"
"	background-color: rgb(135, 206, 235);\n"
"    border: 2px solid #4682B4;\n"
"    border-radius: 8px;\n"
"    margin-top: 10px;\n"
"}\n"
"QPushButton {\n"
"    background-color: #4682B4;\n"
"    color: white;\n"
"    border-radius: 5px;\n"
"    padding: 5px;\n"
"}\n"
"QPushButton:hover {\n"
"    background-color: #5A9BD5;\n"
"}"));
        label = new QLabel(groupBox);
        label->setObjectName("label");
        label->setGeometry(QRect(20, 60, 121, 20));
        lineEdit = new QLineEdit(groupBox);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(40, 90, 113, 26));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(20, 130, 63, 20));
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(40, 160, 113, 26));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(20, 200, 63, 20));
        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(40, 240, 113, 26));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(20, 270, 63, 20));
        lineEdit_4 = new QLineEdit(groupBox);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(40, 310, 113, 26));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(20, 350, 101, 20));
        lineEdit_5 = new QLineEdit(groupBox);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(40, 390, 113, 26));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(20, 440, 141, 20));
        lineEdit_6 = new QLineEdit(groupBox);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(40, 480, 113, 26));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(20, 530, 151, 20));
        dateEdit = new QDateEdit(groupBox);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setGeometry(QRect(40, 570, 110, 26));
        pushButton = new QPushButton(groupBox);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(90, 620, 93, 29));
        label_21 = new QLabel(groupBox);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(0, 480, 41, 21));
        tableWidget = new QTableWidget(tab);
        if (tableWidget->columnCount() < 7)
            tableWidget->setColumnCount(7);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(6, __qtablewidgetitem6);
        tableWidget->setObjectName("tableWidget");
        tableWidget->setGeometry(QRect(300, 250, 981, 331));
        tableWidget->setStyleSheet(QString::fromUtf8("background-color: #00008B;   /* \330\243\330\262\330\261\331\202 \330\272\330\247\331\205\331\202 */\n"
"    color: white;                /* \330\247\331\204\331\206\330\265 \330\243\330\250\331\212\330\266 */\n"
"    gridline-color: white;       /* \331\204\331\210\331\206 \330\247\331\204\330\256\330\267\331\210\330\267 */\n"
"    border-radius: 8px;          /* \330\255\331\210\330\247\331\201 \331\205\330\263\330\252\330\257\331\212\330\261\330\251 */\n"
"}\n"
"QHeaderView::section {\n"
"    background-color: #34495e;   /* \330\261\331\205\330\247\330\257\331\212 \330\272\330\247\331\205\331\202 \331\204\331\204\331\200 header */\n"
"    color: white;\n"
"    padding: 4px;\n"
"    border: 1px solid #2c3e50;\n"
""));
        label_8 = new QLabel(tab);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(360, 20, 291, 31));
        pushButton_2 = new QPushButton(tab);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(830, 190, 151, 51));
        pushButton_2->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 130, 180);\n"
"font: 14pt \"Segoe UI\";\n"
"font: 900 12pt \"Segoe UI\";\n"
"\n"
"    color: white;                /* \331\204\331\210\331\206 \330\247\331\204\331\206\330\265 */\n"
"    border-radius: 12px;         /* \330\247\331\204\330\255\331\210\330\247\331\201 \331\205\330\257\331\210\331\221\330\261\330\251 */\n"
"    padding: 8px 16px;"));
        lineEdit_13 = new QLineEdit(tab);
        lineEdit_13->setObjectName("lineEdit_13");
        lineEdit_13->setGeometry(QRect(670, 200, 141, 31));
        label_17 = new QLabel(tab);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(500, 199, 171, 31));
        pushButton_4 = new QPushButton(tab);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(830, 610, 171, 41));
        pushButton_4->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 130, 180);\n"
"font: 14pt \"Segoe UI\";\n"
"font: 900 12pt \"Segoe UI\";\n"
"\n"
"    color: white;                /* \331\204\331\210\331\206 \330\247\331\204\331\206\330\265 */\n"
"    border-radius: 12px;         /* \330\247\331\204\330\255\331\210\330\247\331\201 \331\205\330\257\331\210\331\221\330\261\330\251 */\n"
"    padding: 8px 16px;"));
        lineEdit_14 = new QLineEdit(tab);
        lineEdit_14->setObjectName("lineEdit_14");
        lineEdit_14->setGeometry(QRect(680, 620, 131, 31));
        label_18 = new QLabel(tab);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(480, 619, 181, 31));
        checkBox_2 = new QCheckBox(tab);
        checkBox_2->setObjectName("checkBox_2");
        checkBox_2->setGeometry(QRect(1180, 310, 91, 24));
        checkBox_3 = new QCheckBox(tab);
        checkBox_3->setObjectName("checkBox_3");
        checkBox_3->setGeometry(QRect(1180, 360, 91, 24));
        checkBox_4 = new QCheckBox(tab);
        checkBox_4->setObjectName("checkBox_4");
        checkBox_4->setGeometry(QRect(1180, 410, 91, 24));
        checkBox_5 = new QCheckBox(tab);
        checkBox_5->setObjectName("checkBox_5");
        checkBox_5->setGeometry(QRect(1180, 460, 91, 24));
        checkBox_6 = new QCheckBox(tab);
        checkBox_6->setObjectName("checkBox_6");
        checkBox_6->setGeometry(QRect(1180, 510, 91, 24));
        label_19 = new QLabel(tab);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(1010, 200, 91, 31));
        comboBox = new QComboBox(tab);
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(1100, 200, 76, 31));
        label_20 = new QLabel(tab);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(470, 60, 751, 131));
        label_20->setPixmap(QPixmap(QString::fromUtf8("../Capture d\342\200\231\303\251cran 2025-09-29 142122.png")));
        pushButton_5 = new QPushButton(tab);
        pushButton_5->setObjectName("pushButton_5");
        pushButton_5->setGeometry(QRect(1020, 610, 171, 41));
        pushButton_5->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 130, 180);\n"
"font: 14pt \"Segoe UI\";\n"
"font: 900 12pt \"Segoe UI\";\n"
"\n"
"    color: white;                /* \331\204\331\210\331\206 \330\247\331\204\331\206\330\265 */\n"
"    border-radius: 12px;         /* \330\247\331\204\330\255\331\210\330\247\331\201 \331\205\330\257\331\210\331\221\330\261\330\251 */\n"
"    padding: 8px 16px;"));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        label_22 = new QLabel(tab_2);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(290, 90, 751, 131));
        label_22->setPixmap(QPixmap(QString::fromUtf8("../Capture d\342\200\231\303\251cran 2025-09-29 142122.png")));
        tabWidget->addTab(tab_2, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1323, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\342\236\225Ajouter employ\303\251:", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "\360\237\221\250\342\200\215\360\237\222\273ID Employ\303\251:", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\360\237\224\240Nom:", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\360\237\224\240Pr\303\251nom:", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\360\237\222\274Poste:", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\360\237\224\243Mot de passe:", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "\360\237\223\236Num t\303\251l\303\251phonique:", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\360\237\223\206Date d'embauche:", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "+216", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "ID", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Nom", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Prenom", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Poste", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Mot de passe", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Num t\303\251l\303\251phonique:", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->horizontalHeaderItem(6);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("MainWindow", "Date d'embauche:", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:700;\">Gestion employ\303\251s</span></p></body></html>", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Afficher \360\237\224\215", nullptr));
        lineEdit_13->setText(QString());
        label_17->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt;\">Afficher employ\303\251:</span></p></body></html>", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Supprimer \360\237\227\221", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt;\">Supprimer employ\303\251:</span></p></body></html>", nullptr));
        checkBox_2->setText(QCoreApplication::translate("MainWindow", "\360\237\226\214", nullptr));
        checkBox_3->setText(QCoreApplication::translate("MainWindow", "\360\237\226\214", nullptr));
        checkBox_4->setText(QCoreApplication::translate("MainWindow", "\360\237\226\214", nullptr));
        checkBox_5->setText(QCoreApplication::translate("MainWindow", "\360\237\226\214", nullptr));
        checkBox_6->setText(QCoreApplication::translate("MainWindow", "\360\237\226\214", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt;\">Trier par:</span></p></body></html>", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Poste ", nullptr));
        comboBox->setItemText(1, QCoreApplication::translate("MainWindow", "ID", nullptr));
        comboBox->setItemText(2, QString());

        label_20->setText(QString());
        pushButton_5->setText(QCoreApplication::translate("MainWindow", "Exporter\360\237\223\221", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Ajouter", nullptr));
        label_22->setText(QString());
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
