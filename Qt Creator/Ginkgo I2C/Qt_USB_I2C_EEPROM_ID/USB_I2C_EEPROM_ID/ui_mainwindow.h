/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Tue Jan 7 14:06:42 2014
**      by: Qt User Interface Compiler version 4.7.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *deviceIndexComboBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *channelIndexComboBox;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QLineEdit *IDLineEdit;
    QLabel *label_5;
    QLineEdit *IDAddrLineEdit;
    QCheckBox *IDAutoIndexCheckBox;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *userStrLineEdit;
    QLabel *label_6;
    QLineEdit *strAddrLineEdit;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer;
    QPushButton *writePushButton;
    QMenuBar *menuBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(411, 154);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        deviceIndexComboBox = new QComboBox(centralWidget);
        deviceIndexComboBox->setObjectName(QString::fromUtf8("deviceIndexComboBox"));

        horizontalLayout->addWidget(deviceIndexComboBox);


        horizontalLayout_5->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        horizontalLayout_2->addWidget(label_2);

        channelIndexComboBox = new QComboBox(centralWidget);
        channelIndexComboBox->setObjectName(QString::fromUtf8("channelIndexComboBox"));

        horizontalLayout_2->addWidget(channelIndexComboBox);


        horizontalLayout_5->addLayout(horizontalLayout_2);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_3->addWidget(label_3);

        IDLineEdit = new QLineEdit(centralWidget);
        IDLineEdit->setObjectName(QString::fromUtf8("IDLineEdit"));

        horizontalLayout_3->addWidget(IDLineEdit);

        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_3->addWidget(label_5);

        IDAddrLineEdit = new QLineEdit(centralWidget);
        IDAddrLineEdit->setObjectName(QString::fromUtf8("IDAddrLineEdit"));

        horizontalLayout_3->addWidget(IDAddrLineEdit);

        IDAutoIndexCheckBox = new QCheckBox(centralWidget);
        IDAutoIndexCheckBox->setObjectName(QString::fromUtf8("IDAutoIndexCheckBox"));

        horizontalLayout_3->addWidget(IDAutoIndexCheckBox);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_4->addWidget(label_4);

        userStrLineEdit = new QLineEdit(centralWidget);
        userStrLineEdit->setObjectName(QString::fromUtf8("userStrLineEdit"));

        horizontalLayout_4->addWidget(userStrLineEdit);

        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_4->addWidget(label_6);

        strAddrLineEdit = new QLineEdit(centralWidget);
        strAddrLineEdit->setObjectName(QString::fromUtf8("strAddrLineEdit"));

        horizontalLayout_4->addWidget(strAddrLineEdit);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        writePushButton = new QPushButton(centralWidget);
        writePushButton->setObjectName(QString::fromUtf8("writePushButton"));

        horizontalLayout_6->addWidget(writePushButton);


        verticalLayout->addLayout(horizontalLayout_6);


        gridLayout->addLayout(verticalLayout, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 411, 23));
        MainWindow->setMenuBar(menuBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "USB I2C EEPROM ID", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", " \350\256\276\345\244\207\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
        deviceIndexComboBox->clear();
        deviceIndexComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "4", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "5", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "6", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "7", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "8", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "9", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("MainWindow", "\351\200\232\351\201\223\345\217\267\357\274\232", 0, QApplication::UnicodeUTF8));
        channelIndexComboBox->clear();
        channelIndexComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("MainWindow", "ID(Hex)\357\274\232", 0, QApplication::UnicodeUTF8));
        IDLineEdit->setText(QApplication::translate("MainWindow", "0000000000", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "\345\234\260\345\235\200(Hex)\357\274\232", 0, QApplication::UnicodeUTF8));
        IDAddrLineEdit->setText(QApplication::translate("MainWindow", "00", 0, QApplication::UnicodeUTF8));
        IDAutoIndexCheckBox->setText(QApplication::translate("MainWindow", "ID\350\207\252\345\212\250\351\200\222\345\242\236", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", " \345\255\227\347\254\246\344\270\262\357\274\232", 0, QApplication::UnicodeUTF8));
        userStrLineEdit->setText(QApplication::translate("MainWindow", "abcdef", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("MainWindow", "\345\234\260\345\235\200(Hex)\357\274\232", 0, QApplication::UnicodeUTF8));
        strAddrLineEdit->setText(QApplication::translate("MainWindow", "10", 0, QApplication::UnicodeUTF8));
        writePushButton->setText(QApplication::translate("MainWindow", "\347\203\247\345\206\231", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
