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
#include <QtWidgets/QCalendarWidget>
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
#include <QtWidgets/QTimeEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QTabWidget *tabWidget;
    QWidget *tab_2;
    QLabel *label_2;
    QComboBox *comboBox;
    QLineEdit *lineEdit;
    QLabel *label_3;
    QPushButton *pushButton_6;
    QGroupBox *groupBox;
    QLabel *label_4;
    QLineEdit *lineEdit_2;
    QLabel *label_5;
    QDateEdit *dateEdit;
    QLabel *label_8;
    QTimeEdit *timeEdit;
    QLabel *label_9;
    QLineEdit *lineEdit_3;
    QLabel *label_10;
    QComboBox *comboBox_3;
    QPushButton *pushButton_7;
    QPushButton *pushButton_8;
    QPushButton *pushButton_2;
    QLabel *label_12;
    QTableWidget *tableWidget_2;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QPushButton *pushButton_9;
    QLabel *label_13;
    QLabel *label_14;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *label_19;
    QCheckBox *checkBox_5;
    QLabel *label_20;
    QLabel *label;
    QLineEdit *lineEdit_5;
    QWidget *tab;
    QLabel *label_18;
    QLabel *label_21;
    QLabel *label_7;
    QLabel *label_11;
    QWidget *tab_3;
    QLabel *label_25;
    QCalendarWidget *calendarWidget;
    QLabel *label_6;
    QWidget *widget;
    QLabel *label_23;
    QPushButton *pushButton;
    QLabel *label_24;
    QLineEdit *lineEdit_4;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1321, 872);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        tabWidget = new QTabWidget(centralwidget);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setGeometry(QRect(0, 0, 1301, 791));
        tabWidget->setStyleSheet(QString::fromUtf8("\n"
"font: 14pt \"Segoe UI\";\n"
"color: rgb(0, 0, 0);\n"
"\n"
""));
        tabWidget->setMovable(false);
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        label_2 = new QLabel(tab_2);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(380, 260, 251, 31));
        label_2->setStyleSheet(QString::fromUtf8("font: 14pt \"Segoe UI\";"));
        comboBox = new QComboBox(tab_2);
        comboBox->addItem(QString());
        comboBox->setObjectName("comboBox");
        comboBox->setGeometry(QRect(1080, 260, 101, 41));
        comboBox->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 130, 180);\n"
"font: 14pt \"Segoe UI\";\n"
"font: 900 12pt \"Segoe UI\";\n"
"\n"
"    color: white;                /* \331\204\331\210\331\206 \330\247\331\204\331\206\330\265 */\n"
"         /* \330\247\331\204\330\255\331\210\330\247\331\201 \331\205\330\257\331\210\331\221\330\261\330\251 */\n"
"    padding: 8px 16px;  "));
        lineEdit = new QLineEdit(tab_2);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(620, 260, 141, 41));
        label_3 = new QLabel(tab_2);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(770, 260, 81, 41));
        label_3->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        pushButton_6 = new QPushButton(tab_2);
        pushButton_6->setObjectName("pushButton_6");
        pushButton_6->setGeometry(QRect(960, 680, 171, 41));
        pushButton_6->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 130, 180);\n"
"font: 14pt \"Segoe UI\";\n"
"font: 900 12pt \"Segoe UI\";\n"
"\n"
"    color: white;                /* \331\204\331\210\331\206 \330\247\331\204\331\206\330\265 */\n"
"    border-radius: 12px;         /* \330\247\331\204\330\255\331\210\330\247\331\201 \331\205\330\257\331\210\331\221\330\261\330\251 */\n"
"    padding: 8px 16px;  \n"
""));
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName("groupBox");
        groupBox->setGeometry(QRect(20, 50, 321, 671));
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
        label_4 = new QLabel(groupBox);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(30, 80, 131, 31));
        label_4->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        lineEdit_2 = new QLineEdit(groupBox);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(30, 130, 161, 26));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName("label_5");
        label_5->setGeometry(QRect(30, 170, 91, 20));
        label_5->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        dateEdit = new QDateEdit(groupBox);
        dateEdit->setObjectName("dateEdit");
        dateEdit->setGeometry(QRect(29, 210, 161, 26));
        dateEdit->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName("label_8");
        label_8->setGeometry(QRect(30, 270, 191, 31));
        label_8->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        timeEdit = new QTimeEdit(groupBox);
        timeEdit->setObjectName("timeEdit");
        timeEdit->setGeometry(QRect(30, 320, 161, 26));
        timeEdit->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        label_9 = new QLabel(groupBox);
        label_9->setObjectName("label_9");
        label_9->setGeometry(QRect(30, 370, 111, 20));
        label_9->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        lineEdit_3 = new QLineEdit(groupBox);
        lineEdit_3->setObjectName("lineEdit_3");
        lineEdit_3->setGeometry(QRect(30, 410, 161, 26));
        label_10 = new QLabel(groupBox);
        label_10->setObjectName("label_10");
        label_10->setGeometry(QRect(30, 460, 81, 31));
        label_10->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        comboBox_3 = new QComboBox(groupBox);
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->addItem(QString());
        comboBox_3->setObjectName("comboBox_3");
        comboBox_3->setGeometry(QRect(120, 470, 76, 26));
        comboBox_3->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        pushButton_7 = new QPushButton(groupBox);
        pushButton_7->setObjectName("pushButton_7");
        pushButton_7->setGeometry(QRect(20, 560, 111, 41));
        pushButton_7->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        pushButton_8 = new QPushButton(groupBox);
        pushButton_8->setObjectName("pushButton_8");
        pushButton_8->setGeometry(QRect(160, 560, 121, 41));
        pushButton_8->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        pushButton_2 = new QPushButton(tab_2);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(830, 260, 111, 41));
        pushButton_2->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 130, 180);\n"
"font: 14pt \"Segoe UI\";\n"
"font: 900 12pt \"Segoe UI\";\n"
"\n"
"    color: white;                /* \331\204\331\210\331\206 \330\247\331\204\331\206\330\265 */\n"
"    border-radius: 12px;         /* \330\247\331\204\330\255\331\210\330\247\331\201 \331\205\330\257\331\210\331\221\330\261\330\251 */\n"
"    padding: 8px 16px;  "));
        label_12 = new QLabel(tab_2);
        label_12->setObjectName("label_12");
        label_12->setGeometry(QRect(970, 260, 101, 31));
        label_12->setStyleSheet(QString::fromUtf8("font: 14pt \"Segoe UI\";"));
        tableWidget_2 = new QTableWidget(tab_2);
        if (tableWidget_2->columnCount() < 6)
            tableWidget_2->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget_2->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        tableWidget_2->setObjectName("tableWidget_2");
        tableWidget_2->setGeometry(QRect(380, 310, 871, 341));
        tableWidget_2->setStyleSheet(QString::fromUtf8("background-color: #00008B;   /* \330\243\330\262\330\261\331\202 \330\272\330\247\331\205\331\202 */\n"
"    color: white;                /* \330\247\331\204\331\206\330\265 \330\243\330\250\331\212\330\266 */\n"
"    gridline-color: white;       /* \331\204\331\210\331\206 \330\247\331\204\330\256\330\267\331\210\330\267 */\n"
"    border-radius: 8px;          /* \330\255\331\210\330\247\331\201 \331\205\330\263\330\252\330\257\331\212\330\261\330\251 */\n"
"}\n"
"QHeaderView::section {\n"
"    background-color: #34495e;   /* \330\261\331\205\330\247\330\257\331\212 \330\272\330\247\331\205\331\202 \331\204\331\204\331\200 header */\n"
"    color: white;\n"
"    padding: 4px;\n"
"    border: 1px solid #2c3e50;"));
        checkBox = new QCheckBox(tab_2);
        checkBox->setObjectName("checkBox");
        checkBox->setGeometry(QRect(1140, 390, 91, 24));
        checkBox_2 = new QCheckBox(tab_2);
        checkBox_2->setObjectName("checkBox_2");
        checkBox_2->setGeometry(QRect(1140, 440, 91, 24));
        checkBox_3 = new QCheckBox(tab_2);
        checkBox_3->setObjectName("checkBox_3");
        checkBox_3->setGeometry(QRect(1140, 490, 91, 24));
        checkBox_4 = new QCheckBox(tab_2);
        checkBox_4->setObjectName("checkBox_4");
        checkBox_4->setGeometry(QRect(1140, 540, 91, 24));
        pushButton_9 = new QPushButton(tab_2);
        pushButton_9->setObjectName("pushButton_9");
        pushButton_9->setGeometry(QRect(400, 660, 151, 41));
        pushButton_9->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 130, 180);\n"
"font: 14pt \"Segoe UI\";\n"
"font: 900 12pt \"Segoe UI\";\n"
"\n"
"    color: white;                /* \331\204\331\210\331\206 \330\247\331\204\331\206\330\265 */\n"
"    border-radius: 12px;         /* \330\247\331\204\330\255\331\210\330\247\331\201 \331\205\330\257\331\210\331\221\330\261\330\251 */\n"
"    padding: 8px 16px;  \n"
""));
        label_13 = new QLabel(tab_2);
        label_13->setObjectName("label_13");
        label_13->setGeometry(QRect(560, 670, 63, 31));
        label_13->setStyleSheet(QString::fromUtf8("font: 14pt \"Segoe UI\";"));
        label_14 = new QLabel(tab_2);
        label_14->setObjectName("label_14");
        label_14->setGeometry(QRect(1170, 380, 63, 31));
        label_14->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        label_15 = new QLabel(tab_2);
        label_15->setObjectName("label_15");
        label_15->setGeometry(QRect(1170, 430, 63, 31));
        label_15->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        label_16 = new QLabel(tab_2);
        label_16->setObjectName("label_16");
        label_16->setGeometry(QRect(1170, 480, 63, 31));
        label_16->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        label_17 = new QLabel(tab_2);
        label_17->setObjectName("label_17");
        label_17->setGeometry(QRect(1170, 530, 63, 31));
        label_17->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        label_19 = new QLabel(tab_2);
        label_19->setObjectName("label_19");
        label_19->setGeometry(QRect(360, 60, 901, 181));
        label_19->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/WhatsApp Image 2025-09-24 at 15.27.43.jpeg")));
        checkBox_5 = new QCheckBox(tab_2);
        checkBox_5->setObjectName("checkBox_5");
        checkBox_5->setGeometry(QRect(1140, 590, 91, 24));
        label_20 = new QLabel(tab_2);
        label_20->setObjectName("label_20");
        label_20->setGeometry(QRect(1170, 580, 63, 31));
        label_20->setStyleSheet(QString::fromUtf8("font: 12pt \"Segoe UI\";"));
        label = new QLabel(tab_2);
        label->setObjectName("label");
        label->setGeometry(QRect(500, 10, 481, 51));
        lineEdit_5 = new QLineEdit(tab_2);
        lineEdit_5->setObjectName("lineEdit_5");
        lineEdit_5->setGeometry(QRect(780, 690, 171, 31));
        tabWidget->addTab(tab_2, QString());
        tab = new QWidget();
        tab->setObjectName("tab");
        label_18 = new QLabel(tab);
        label_18->setObjectName("label_18");
        label_18->setGeometry(QRect(920, 70, 63, 20));
        label_21 = new QLabel(tab);
        label_21->setObjectName("label_21");
        label_21->setGeometry(QRect(110, 20, 1121, 181));
        label_21->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/WhatsApp Image 2025-09-24 at 15.27.43.jpeg")));
        label_7 = new QLabel(tab);
        label_7->setObjectName("label_7");
        label_7->setGeometry(QRect(260, 320, 801, 391));
        label_7->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/WhatsApp Image 2025-09-24 at 15.47.38.jpeg")));
        label_11 = new QLabel(tab);
        label_11->setObjectName("label_11");
        label_11->setGeometry(QRect(30, 240, 411, 61));
        label_11->setStyleSheet(QString::fromUtf8("font: 900 16pt \"Segoe UI\";"));
        tabWidget->addTab(tab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName("tab_3");
        label_25 = new QLabel(tab_3);
        label_25->setObjectName("label_25");
        label_25->setGeometry(QRect(50, 10, 1121, 181));
        label_25->setPixmap(QPixmap(QString::fromUtf8("../../Downloads/WhatsApp Image 2025-09-24 at 15.27.43.jpeg")));
        calendarWidget = new QCalendarWidget(tab_3);
        calendarWidget->setObjectName("calendarWidget");
        calendarWidget->setGeometry(QRect(40, 300, 541, 341));
        label_6 = new QLabel(tab_3);
        label_6->setObjectName("label_6");
        label_6->setGeometry(QRect(50, 210, 381, 51));
        label_6->setStyleSheet(QString::fromUtf8("font: 900 16pt \"Segoe UI\";"));
        widget = new QWidget(tab_3);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(650, 260, 591, 441));
        widget->setStyleSheet(QString::fromUtf8("background-color: rgb(135, 206, 235);"));
        label_23 = new QLabel(widget);
        label_23->setObjectName("label_23");
        label_23->setGeometry(QRect(40, 60, 421, 61));
        label_23->setStyleSheet(QString::fromUtf8("font: 900 16pt \"Segoe UI\";"));
        pushButton = new QPushButton(widget);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(130, 270, 201, 51));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: rgb(70, 130, 180);\n"
"font: 14pt \"Segoe UI\";\n"
"font: 900 12pt \"Segoe UI\";\n"
"\n"
"    color: white;                /* \331\204\331\210\331\206 \330\247\331\204\331\206\330\265 */\n"
"    border-radius: 12px;         /* \330\247\331\204\330\255\331\210\330\247\331\201 \331\205\330\257\331\210\331\221\330\261\330\251 */\n"
"    padding: 8px 16px;  \n"
""));
        label_24 = new QLabel(widget);
        label_24->setObjectName("label_24");
        label_24->setGeometry(QRect(340, 170, 141, 71));
        label_24->setStyleSheet(QString::fromUtf8("font: 20pt \"Segoe UI\";"));
        lineEdit_4 = new QLineEdit(widget);
        lineEdit_4->setObjectName("lineEdit_4");
        lineEdit_4->setGeometry(QRect(100, 190, 241, 41));
        lineEdit_4->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
        tabWidget->addTab(tab_3, QString());
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1321, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Gestion des seances", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Rechrcher une seance :", nullptr));
        comboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Date", nullptr));

#if QT_CONFIG(whatsthis)
        lineEdit->setWhatsThis(QCoreApplication::translate("MainWindow", "<html><head/><body><p>hhhh</p><p><br/></p></body></html>", nullptr));
#endif // QT_CONFIG(whatsthis)
        label_3->setText(QCoreApplication::translate("MainWindow", "\360\237\224\215", nullptr));
        pushButton_6->setText(QCoreApplication::translate("MainWindow", "Supprimer\360\237\227\221\357\270\217", nullptr));
        groupBox->setTitle(QCoreApplication::translate("MainWindow", "\342\236\225 Ajouter une seance", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "\360\237\224\221Id_seance", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "\360\237\223\205Date", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "\360\237\225\222Heure de d\303\251but", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "\342\217\263Dur\303\251e", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "\360\237\232\227Type", nullptr));
        comboBox_3->setItemText(0, QCoreApplication::translate("MainWindow", "Code", nullptr));
        comboBox_3->setItemText(1, QCoreApplication::translate("MainWindow", "Conduite", nullptr));
        comboBox_3->setItemText(2, QCoreApplication::translate("MainWindow", "Parc", nullptr));

        pushButton_7->setText(QCoreApplication::translate("MainWindow", "Valider", nullptr));
        pushButton_8->setText(QCoreApplication::translate("MainWindow", "Annuler", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "Valider", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "Trier par :", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget_2->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("MainWindow", "Id", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget_2->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("MainWindow", "Date", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget_2->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("MainWindow", " Heure d\303\251but", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget_2->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("MainWindow", "Dur\303\251e", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget_2->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("MainWindow", "Type", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget_2->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("MainWindow", "Statut", nullptr));
        checkBox->setText(QString());
        checkBox_2->setText(QString());
        checkBox_3->setText(QString());
        checkBox_4->setText(QString());
        pushButton_9->setText(QCoreApplication::translate("MainWindow", "Exporation", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "\342\254\207\357\270\217", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217", nullptr));
        label_19->setText(QString());
        checkBox_5->setText(QString());
        label_20->setText(QCoreApplication::translate("MainWindow", "\342\234\217\357\270\217", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:20pt; font-weight:700;\">Gestion des s\303\251ances</span></p></body></html>", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("MainWindow", "Acceuil", nullptr));
        label_18->setText(QString());
        label_21->setText(QString());
        label_7->setText(QString());
        label_11->setText(QCoreApplication::translate("MainWindow", "Statistiques selon les Types", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("MainWindow", "Statistiques", nullptr));
        label_25->setText(QString());
        label_6->setText(QCoreApplication::translate("MainWindow", "Affectuer une date :", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "Rappel d'une seances par Mail", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Envoyer", nullptr));
        label_24->setText(QCoreApplication::translate("MainWindow", "\360\237\223\247", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QCoreApplication::translate("MainWindow", "Autres M\303\251tiers", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
