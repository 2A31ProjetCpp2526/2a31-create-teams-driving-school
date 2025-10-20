/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
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
#include <QtWidgets/QLCDNumber>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tab_3;
    QWidget *tab;
    QLabel *label;
    QGroupBox *groupBox;
    QLabel *label_2;
    QLineEdit *lineEdit_immatricule;
    QLabel *label_3;
    QLineEdit *lineEdit_modele;
    QLabel *label_4;
    QComboBox *comboBox_transmission;
    QLabel *label_5;
    QComboBox *comboBox_etat;
    QLabel *label_8;
    QDateEdit *dateEdit_assurance;
    QLabel *label_9;
    QComboBox *comboBox_type;
    QPushButton *pushButton_valider;
    QPushButton *pushButton_2;
    QLabel *label_7;
    QLabel *label_10;
    QLineEdit *lineEdit_4;
    QComboBox *comboBox_5;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_supprimer;
    QLineEdit *lineEdit_5;
    QCheckBox *checkBox_modifier;
    QCheckBox *checkBox_modifier_2;
    QCheckBox *checkBox_modifier_3;
    QCheckBox *checkBox_modifier_4;
    QCheckBox *checkBox_modifier_5;
    QTableView *tableView;
    QWidget *tab_2;
    QLabel *label_6;
    QWidget *Autresmetiers;
    QGroupBox *groupBox_2;
    QTableWidget *tableWidget_2;
    QLabel *label_23;
    QLabel *label_22;
    QLabel *label_21;
    QLabel *label_12;
    QGroupBox *groupBox_3;
    QLabel *label_14;
    QLineEdit *lineEdit_3;
    QLabel *label_15;
    QDateEdit *dateEdit_2;
    QLabel *label_16;
    QCheckBox *checkBox_6;
    QLineEdit *lineEdit_6;
    QCheckBox *checkBox_7;
    QLineEdit *lineEdit_7;
    QLineEdit *lineEdit_9;
    QLineEdit *lineEdit_8;
    QCheckBox *checkBox_8;
    QPushButton *pushButton_3;
    QCheckBox *checkBox_9;
    QPushButton *pushButton_4;
    QLabel *label_17;
    QLCDNumber *lcdNumber;
    QWidget *widget;
    QWidget *widget_2;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1028, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tab_3 = new QTabWidget(centralwidget);
        tab_3->setObjectName("tab_3");
        tab_3->setGeometry(QRect(0, 0, 1031, 561));
        tab_3->setStyleSheet(QString::fromUtf8("tab\n"
"\n"
"\n"
"\n"
"\n"
""));
        tab = new QWidget();
        tab->setObjectName("tab");
        label = new QLabel(tab);
        label->setObjectName("label");
        label->setGeometry(QRect(320, 30, 231, 31));
        QFont font;
        font.setPointSize(17);
        font.setBold(true);
        font.setUnderline(false);
        label->setFont(font);
        label->setStyleSheet(QString::fromUtf8("color: rgb(0, 96, 140);"));
        groupBox = new QGroupBox(tab);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(10, 40, 221, 461));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Segoe UI")});
        font1.setPointSize(14);
        font1.setBold(false);
        font1.setItalic(false);
        groupBox->setFont(font1);
        groupBox->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"	font: 900 9pt \"Segoe UI\";\n"
"	font: 900 9pt \"Segoe UI\";\n"
"	font: 14pt \"Segoe UI\";\n"
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
        label_2 = new QLabel(groupBox);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(0, 60, 151, 21));
        QFont font2;
        font2.setPointSize(12);
        font2.setBold(true);
        label_2->setFont(font2);
        lineEdit_immatricule = new QLineEdit(groupBox);
        lineEdit_immatricule->setObjectName("lineEdit_immatricule");
        lineEdit_immatricule->setGeometry(QRect(30, 80, 113, 16));
        label_3 = new QLabel(groupBox);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(0, 110, 151, 16));
        label_3->setFont(font2);
        lineEdit_modele = new QLineEdit(groupBox);
        lineEdit_modele->setObjectName("lineEdit_modele");
        lineEdit_modele->setGeometry(QRect(30, 130, 113, 16));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(0, 170, 131, 21));
        label_4->setFont(font2);
        comboBox_transmission = new QComboBox(groupBox);
        comboBox_transmission->addItem(QString());
        comboBox_transmission->addItem(QString());
        comboBox_transmission->setObjectName("comboBox_transmission");
        comboBox_transmission->setGeometry(QRect(30, 240, 111, 21));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(0, 220, 151, 21));
        label_5->setFont(font2);
        comboBox_etat = new QComboBox(groupBox);
        comboBox_etat->addItem(QString());
        comboBox_etat->addItem(QString());
        comboBox_etat->setObjectName("comboBox_etat");
        comboBox_etat->setGeometry(QRect(30, 300, 111, 21));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(0, 280, 141, 21));
        label_8->setFont(font2);
        dateEdit_assurance = new QDateEdit(groupBox);
        dateEdit_assurance->setObjectName("dateEdit_assurance");
        dateEdit_assurance->setGeometry(QRect(30, 360, 110, 22));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(0, 340, 161, 21));
        label_9->setFont(font2);
        comboBox_type = new QComboBox(groupBox);
        comboBox_type->addItem(QString());
        comboBox_type->addItem(QString());
        comboBox_type->addItem(QString());
        comboBox_type->setObjectName("comboBox_type");
        comboBox_type->setGeometry(QRect(30, 190, 111, 21));
        pushButton_valider = new QPushButton(groupBox);
        pushButton_valider->setObjectName("pushButton_valider");
        pushButton_valider->setGeometry(QRect(10, 400, 71, 41));
        pushButton_valider->setFont(font2);
        pushButton_valider->setTabletTracking(false);
        pushButton_valider->setStyleSheet(QString::fromUtf8("color: rgb(0, 105, 154);\n"
"background-color: rgb(202, 202, 202);"));
        pushButton_valider->setCheckable(false);
        pushButton_2 = new QPushButton(groupBox);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(130, 400, 81, 41));
        pushButton_2->setFont(font2);
        pushButton_2->setStyleSheet(QString::fromUtf8("color: rgb(0, 110, 161);\n"
"background-color: rgb(200, 200, 200);"));
        label_7 = new QLabel(tab);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(260, 120, 111, 16));
        label_7->setStyleSheet(QString::fromUtf8("font: 13pt \"Segoe UI\";"));
        label_10 = new QLabel(tab);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(700, 120, 91, 20));
        label_10->setStyleSheet(QString::fromUtf8("font: 13pt \"Segoe UI\";"));
        lineEdit_4 = new QLineEdit(tab);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(380, 120, 113, 22));
        comboBox_5 = new QComboBox(tab);
        comboBox_5->addItem(QString());
        comboBox_5->setObjectName("comboBox_5");
        comboBox_5->setGeometry(QRect(800, 120, 91, 22));
        pushButton_7 = new QPushButton(tab);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(500, 110, 101, 31));
        pushButton_7->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 130, 180);\n"
"font: 14pt \"Segoe UI\";\n"
"font: 900 12pt \"Segoe UI\";\n"
"\n"
"    color: white;                /* \331\204\331\210\331\206 \330\247\331\204\331\206\330\265 */\n"
"    border-radius: 12px;         /* \330\247\331\204\330\255\331\210\330\247\331\201 \331\205\330\257\331\210\331\221\330\261\330\251 */\n"
"    padding: 8px 16px;"));
        pushButton_8 = new QPushButton(tab);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(300, 473, 151, 41));
        pushButton_8->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 130, 180);\n"
"font: 14pt \"Segoe UI\";\n"
"font: 900 12pt \"Segoe UI\";\n"
"\n"
"    color: white;                /* \331\204\331\210\331\206 \330\247\331\204\331\206\330\265 */\n"
"    border-radius: 12px;         /* \330\247\331\204\330\255\331\210\330\247\331\201 \331\205\330\257\331\210\331\221\330\261\330\251 */\n"
"    padding: 8px 16px;"));
        pushButton_supprimer = new QPushButton(tab);
        pushButton_supprimer->setObjectName("pushButton_supprimer");
        pushButton_supprimer->setGeometry(QRect(800, 480, 151, 31));
        pushButton_supprimer->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 130, 180);\n"
"font: 14pt \"Segoe UI\";\n"
"font: 900 12pt \"Segoe UI\";\n"
"\n"
"    color: white;                /* \331\204\331\210\331\206 \330\247\331\204\331\206\330\265 */\n"
"    border-radius: 12px;         /* \330\247\331\204\330\255\331\210\330\247\331\201 \331\205\330\257\331\210\331\221\330\261\330\251 */\n"
"    padding: 8px 16px;"));
        lineEdit_5 = new QLineEdit(tab);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(680, 480, 113, 22));
        checkBox_modifier = new QCheckBox(tab);
        checkBox_modifier->setObjectName("checkBox_modifier");
        checkBox_modifier->setGeometry(QRect(960, 230, 76, 20));
        checkBox_modifier_2 = new QCheckBox(tab);
        checkBox_modifier_2->setObjectName("checkBox_modifier_2");
        checkBox_modifier_2->setGeometry(QRect(960, 270, 76, 20));
        checkBox_modifier_3 = new QCheckBox(tab);
        checkBox_modifier_3->setObjectName("checkBox_modifier_3");
        checkBox_modifier_3->setGeometry(QRect(960, 310, 76, 20));
        checkBox_modifier_4 = new QCheckBox(tab);
        checkBox_modifier_4->setObjectName("checkBox_modifier_4");
        checkBox_modifier_4->setGeometry(QRect(960, 350, 76, 20));
        checkBox_modifier_5 = new QCheckBox(tab);
        checkBox_modifier_5->setObjectName("checkBox_modifier_5");
        checkBox_modifier_5->setGeometry(QRect(960, 390, 76, 20));
        tableView = new QTableView(tab);
        tableView->setObjectName("tableView");
        tableView->setGeometry(QRect(240, 150, 781, 311));
        tab_3->addTab(tab, QString());
        tableView->raise();
        label->raise();
        groupBox->raise();
        label_7->raise();
        label_10->raise();
        lineEdit_4->raise();
        comboBox_5->raise();
        pushButton_7->raise();
        pushButton_8->raise();
        pushButton_supprimer->raise();
        lineEdit_5->raise();
        checkBox_modifier->raise();
        checkBox_modifier_2->raise();
        checkBox_modifier_3->raise();
        checkBox_modifier_4->raise();
        checkBox_modifier_5->raise();
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        label_6 = new QLabel(tab_2);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(90, 30, 681, 461));
        label_6->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/diagrame.png")));
        tab_3->addTab(tab_2, QString());
        Autresmetiers = new QWidget();
        Autresmetiers->setObjectName("Autresmetiers");
        groupBox_2 = new QGroupBox(Autresmetiers);
        groupBox_2->setObjectName("groupBox_2");
        groupBox_2->setGeometry(QRect(10, 20, 481, 461));
        groupBox_2->setFont(font1);
        groupBox_2->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"	font: 900 9pt \"Segoe UI\";\n"
"	font: 900 9pt \"Segoe UI\";\n"
"	font: 14pt \"Segoe UI\";\n"
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
        tableWidget_2 = new QTableWidget(groupBox_2);
        if (tableWidget_2->columnCount() < 3)
            tableWidget_2->setColumnCount(3);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(60, 60, 351, 281));
        tableWidget_2->setStyleSheet(QString::fromUtf8(""));
        label_23 = new QLabel(groupBox_2);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(30, 380, 71, 16));
        QFont font3;
        font3.setPointSize(10);
        font3.setBold(true);
        label_23->setFont(font3);
        label_23->setStyleSheet(QString::fromUtf8("color: rgb(6, 6, 6);"));
        label_22 = new QLabel(groupBox_2);
        label_22->setObjectName("label_22");
        label_22->setGeometry(QRect(160, 380, 141, 16));
        label_22->setFont(font3);
        label_22->setStyleSheet(QString::fromUtf8("color: rgb(4, 4, 4);"));
        label_21 = new QLabel(groupBox_2);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(340, 380, 131, 21));
        QFont font4;
        font4.setPointSize(10);
        font4.setBold(true);
        font4.setItalic(false);
        font4.setKerning(true);
        label_21->setFont(font4);
        label_21->setStyleSheet(QString::fromUtf8("color: rgb(7, 7, 7);"));
        label_12 = new QLabel(groupBox_2);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(390, 170, 101, 16));
        groupBox_3 = new QGroupBox(Autresmetiers);
        groupBox_3->setObjectName("groupBox_3");
        groupBox_3->setGeometry(QRect(520, 20, 471, 461));
        groupBox_3->setFont(font1);
        groupBox_3->setStyleSheet(QString::fromUtf8("QGroupBox {\n"
"	font: 900 9pt \"Segoe UI\";\n"
"	font: 900 9pt \"Segoe UI\";\n"
"	font: 14pt \"Segoe UI\";\n"
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
        label_14 = new QLabel(groupBox_3);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(10, 60, 121, 21));
        QFont font5;
        font5.setPointSize(12);
        font5.setBold(false);
        label_14->setFont(font5);
        label_14->setStyleSheet(QString::fromUtf8("color: rgb(6, 6, 6);"));
        lineEdit_3 = new QLineEdit(groupBox_3);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(40, 90, 113, 22));
        lineEdit_3->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_15 = new QLabel(groupBox_3);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(10, 130, 91, 21));
        QFont font6;
        font6.setPointSize(12);
        label_15->setFont(font6);
        label_15->setStyleSheet(QString::fromUtf8("color: rgb(9, 9, 9);"));
        dateEdit_2 = new QDateEdit(groupBox_3);
        dateEdit_2->setObjectName("dateEdit_2");
        dateEdit_2->setGeometry(QRect(40, 160, 110, 22));
        dateEdit_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        label_16 = new QLabel(groupBox_3);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(240, 60, 161, 16));
        label_16->setFont(font6);
        label_16->setStyleSheet(QString::fromUtf8("color: rgb(4, 4, 4);"));
        checkBox_6 = new QCheckBox(groupBox_3);
        checkBox_6->setObjectName("checkBox_6");
        checkBox_6->setGeometry(QRect(260, 100, 81, 20));
        QFont font7;
        font7.setPointSize(10);
        checkBox_6->setFont(font7);
        checkBox_6->setStyleSheet(QString::fromUtf8("color: rgb(8, 8, 8);"));
        lineEdit_6 = new QLineEdit(groupBox_3);
        lineEdit_6->setObjectName("lineEdit_6");
        lineEdit_6->setGeometry(QRect(350, 100, 101, 22));
        lineEdit_6->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        checkBox_7 = new QCheckBox(groupBox_3);
        checkBox_7->setObjectName("checkBox_7");
        checkBox_7->setGeometry(QRect(260, 130, 81, 20));
        checkBox_7->setFont(font7);
        checkBox_7->setStyleSheet(QString::fromUtf8("color: rgb(16, 16, 16);"));
        lineEdit_7 = new QLineEdit(groupBox_3);
        lineEdit_7->setObjectName("lineEdit_7");
        lineEdit_7->setGeometry(QRect(350, 130, 101, 22));
        lineEdit_7->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        lineEdit_9 = new QLineEdit(groupBox_3);
        lineEdit_9->setObjectName("lineEdit_9");
        lineEdit_9->setGeometry(QRect(350, 160, 101, 22));
        lineEdit_9->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        lineEdit_8 = new QLineEdit(groupBox_3);
        lineEdit_8->setObjectName("lineEdit_8");
        lineEdit_8->setGeometry(QRect(350, 190, 101, 22));
        lineEdit_8->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        checkBox_8 = new QCheckBox(groupBox_3);
        checkBox_8->setObjectName("checkBox_8");
        checkBox_8->setGeometry(QRect(260, 160, 76, 20));
        checkBox_8->setFont(font7);
        checkBox_8->setStyleSheet(QString::fromUtf8("color: rgb(4, 4, 4);"));
        pushButton_3 = new QPushButton(groupBox_3);
        pushButton_3->setObjectName("pushButton_3");
        pushButton_3->setGeometry(QRect(140, 240, 171, 31));
        QFont font8;
        font8.setPointSize(11);
        font8.setBold(true);
        pushButton_3->setFont(font8);
        pushButton_3->setStyleSheet(QString::fromUtf8("color: rgb(7, 7, 7);"));
        checkBox_9 = new QCheckBox(groupBox_3);
        checkBox_9->setObjectName("checkBox_9");
        checkBox_9->setGeometry(QRect(260, 190, 76, 20));
        checkBox_9->setFont(font7);
        checkBox_9->setStyleSheet(QString::fromUtf8("color: rgb(7, 7, 7);"));
        pushButton_4 = new QPushButton(groupBox_3);
        pushButton_4->setObjectName("pushButton_4");
        pushButton_4->setGeometry(QRect(294, 370, 101, 41));
        pushButton_4->setFont(font3);
        pushButton_4->setStyleSheet(QString::fromUtf8("color: rgb(8, 8, 8);"));
        label_17 = new QLabel(groupBox_3);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(70, 330, 131, 21));
        label_17->setFont(font2);
        label_17->setStyleSheet(QString::fromUtf8("color: rgb(7, 7, 7);"));
        lcdNumber = new QLCDNumber(groupBox_3);
        lcdNumber->setObjectName("lcdNumber");
        lcdNumber->setGeometry(QRect(210, 330, 101, 23));
        lcdNumber->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"color: rgb(6, 6, 6);"));
        widget = new QWidget(groupBox_3);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(10, 50, 451, 241));
        widget->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"\n"
"QGroupBox {\n"
"	font: 900 9pt \"Segoe UI\";\n"
"	font: 900 9pt \"Segoe UI\";\n"
"	font: 14pt \"Segoe UI\";\n"
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
        widget_2 = new QWidget(groupBox_3);
        widget_2->setObjectName("widget_2");
        widget_2->setGeometry(QRect(50, 310, 361, 111));
        widget_2->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        widget_2->raise();
        widget->raise();
        label_14->raise();
        lineEdit_3->raise();
        label_15->raise();
        dateEdit_2->raise();
        label_16->raise();
        checkBox_6->raise();
        lineEdit_6->raise();
        checkBox_7->raise();
        lineEdit_7->raise();
        lineEdit_9->raise();
        lineEdit_8->raise();
        checkBox_8->raise();
        pushButton_3->raise();
        checkBox_9->raise();
        pushButton_4->raise();
        label_17->raise();
        lcdNumber->raise();
        tab_3->addTab(Autresmetiers, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1028, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tab_3->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
#if QT_CONFIG(tooltip)
        tab_3->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>n</p><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        label->setText(QCoreApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><meta charset=\"utf-8\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"hr { height: 1px; border-width: 0; }\n"
"li.unchecked::marker { content: \"\\2610\"; }\n"
"li.checked::marker { content: \"\\2612\"; }\n"
"</style></head><body style=\" font-family:'Segoe UI'; font-size:17pt; font-weight:700; font-style:normal;\">\n"
"<p align=\"center\" style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Gestion des vehicules</p></body></html>", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\342\236\225Ajouter une vehicule :", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\360\237\224\242Immatricule  :", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "\360\237\232\227Mod\303\250le  :", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\360\237\232\233Type :", nullptr));
        comboBox_transmission->setItemText(0, QCoreApplication::translate("MainWindow", "\360\237\205\260\357\270\217 Automatique", nullptr));
        comboBox_transmission->setItemText(1, QCoreApplication::translate("MainWindow", "\360\237\205\274 Manuelle", nullptr));

        label_5->setText(QCoreApplication::translate("MainWindow", "\342\232\231\357\270\217 Transmission :", nullptr));
        comboBox_etat->setItemText(0, QCoreApplication::translate("MainWindow", "\360\237\237\242Disponible", nullptr));
        comboBox_etat->setItemText(1, QCoreApplication::translate("MainWindow", "\360\237\224\264Indisponible", nullptr));

        label_8->setText(QCoreApplication::translate("MainWindow", " \342\234\205Etat :", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\360\237\223\235Date d'assurance:", nullptr));
        comboBox_type->setItemText(0, QCoreApplication::translate("MainWindow", "\360\237\232\233 Camion", nullptr));
        comboBox_type->setItemText(1, QCoreApplication::translate("MainWindow", "\360\237\232\227 Voiture", nullptr));
        comboBox_type->setItemText(2, QCoreApplication::translate("MainWindow", "\360\237\217\215\357\270\217 Moto\n"
"", nullptr));

        pushButton_valider->setText(QCoreApplication::translate("MainWindow", "Valider", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215Recherche:", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\342\206\225\357\270\217Trier par :", nullptr));
        comboBox_5->setItemText(0, QCoreApplication::translate("MainWindow", "Immatricule", nullptr));

        pushButton_7->setText(QCoreApplication::translate("MainWindow", "Valider", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "Exportation\360\237\223\244", nullptr));
        pushButton_supprimer->setText(QCoreApplication::translate("MainWindow", "Supprimer \360\237\227\221\357\270\217", nullptr));
        checkBox_modifier->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217", nullptr));
        checkBox_modifier_2->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217", nullptr));
        checkBox_modifier_3->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217", nullptr));
        checkBox_modifier_4->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217", nullptr));
        checkBox_modifier_5->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217", nullptr));
        tab_3->setTabText(tab_3->indexOf(tab), QCoreApplication::translate("MainWindow", "Acceuil", nullptr));
        label_6->setText(QString());
        tab_3->setTabText(tab_3->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Statistique", nullptr));
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "\360\237\224\224Notification d'assurance", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Immatricule", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Date d'expiration", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", "Statut", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "\342\235\214Expir\303\250e", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "\342\233\224Expire aujourd'hui", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "\342\232\240\357\270\217Expire dans (J-....)", nullptr));
        label_12->setText(QString());
        groupBox_3->setTitle(QCoreApplication::translate("MainWindow", "\360\237\222\260Gestion du cout d'exploitation quotidien", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "\360\237\224\242Immatricule  :", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "\360\237\223\235Le jour :", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "\360\237\222\266Cout des depenses:", nullptr));
        checkBox_6->setText(QCoreApplication::translate("MainWindow", "Carburant", nullptr));
        checkBox_7->setText(QCoreApplication::translate("MainWindow", "Assurance", nullptr));
        checkBox_8->setText(QCoreApplication::translate("MainWindow", "Entrtien", nullptr));
        pushButton_3->setText(QCoreApplication::translate("MainWindow", "Calculer le cout total\360\237\222\262", nullptr));
        checkBox_9->setText(QCoreApplication::translate("MainWindow", "Lavage", nullptr));
        pushButton_4->setText(QCoreApplication::translate("MainWindow", "Enregistrer\360\237\223\244", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Total du jour   =", nullptr));
        tab_3->setTabText(tab_3->indexOf(Autresmetiers), QCoreApplication::translate("MainWindow", "Autres metiers", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
