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
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QFrame *frame;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_2;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label_4;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        frame = new QFrame(centralwidget);
        frame->setObjectName("frame");
        frame->setGeometry(QRect(70, 70, 671, 441));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(85, 170, 255);\n"
""));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);
        label_3 = new QLabel(frame);
        label_3->setObjectName("label_3");
        label_3->setGeometry(QRect(260, 20, 141, 41));
        label_2 = new QLabel(frame);
        label_2->setObjectName("label_2");
        label_2->setGeometry(QRect(70, 190, 161, 31));
        label = new QLabel(frame);
        label->setObjectName("label");
        label->setGeometry(QRect(80, 120, 161, 31));
        lineEdit = new QLineEdit(frame);
        lineEdit->setObjectName("lineEdit");
        lineEdit->setGeometry(QRect(250, 120, 191, 31));
        lineEdit_2 = new QLineEdit(frame);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setGeometry(QRect(250, 190, 191, 31));
        pushButton = new QPushButton(frame);
        pushButton->setObjectName("pushButton");
        pushButton->setGeometry(QRect(100, 320, 171, 31));
        pushButton->setStyleSheet(QString::fromUtf8("background-color: #00008B;   /* \330\243\330\262\330\261\331\202 \330\272\330\247\331\205\331\202 */\n"
"    color: white;                /* \330\247\331\204\331\206\330\265 \330\243\330\250\331\212\330\266 */\n"
"    gridline-color: white;       /* \331\204\331\210\331\206 \330\247\331\204\330\256\330\267\331\210\330\267 */\n"
"    border-radius: 8px;          /* \330\255\331\210\330\247\331\201 \331\205\330\263\330\252\330\257\331\212\330\261\330\251 */\n"
"}\n"
"QHeaderView::section {\n"
"    background-color: #34495e;   /* \330\261\331\205\330\247\330\257\331\212 \330\272\330\247\331\205\331\202 \331\204\331\204\331\200 header */\n"
"    color: white;\n"
"    padding: 4px;\n"
"    border: 1px solid #2c3e50;"));
        pushButton_2 = new QPushButton(frame);
        pushButton_2->setObjectName("pushButton_2");
        pushButton_2->setGeometry(QRect(340, 320, 171, 31));
        pushButton_2->setStyleSheet(QString::fromUtf8("background-color: #00008B;   /* \330\243\330\262\330\261\331\202 \330\272\330\247\331\205\331\202 */\n"
"    color: white;                /* \330\247\331\204\331\206\330\265 \330\243\330\250\331\212\330\266 */\n"
"    gridline-color: white;       /* \331\204\331\210\331\206 \330\247\331\204\330\256\330\267\331\210\330\267 */\n"
"    border-radius: 8px;          /* \330\255\331\210\330\247\331\201 \331\205\330\263\330\252\330\257\331\212\330\261\330\251 */\n"
"}\n"
"QHeaderView::section {\n"
"    background-color: #34495e;   /* \330\261\331\205\330\247\330\257\331\212 \330\272\330\247\331\205\331\202 \331\204\331\204\331\200 header */\n"
"    color: white;\n"
"    padding: 4px;\n"
"    border: 1px solid #2c3e50;"));
        label_4 = new QLabel(frame);
        label_4->setObjectName("label_4");
        label_4->setGeometry(QRect(490, -10, 201, 81));
        label_4->setPixmap(QPixmap(QString::fromUtf8("../Desktop/444444.png")));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 26));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:14pt; font-weight:700;\">S'identifier:</span></p></body></html>", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt;\">\360\237\224\221Mot de passe:</span></p></body></html>", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:12pt;\">\360\237\224\240ID ou Nom:</span></p></body></html>", nullptr));
        pushButton->setText(QCoreApplication::translate("MainWindow", "Log in", nullptr));
        pushButton_2->setText(QCoreApplication::translate("MainWindow", "R\303\251cuperer mot de passe", nullptr));
        label_4->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
