/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon Jan 20 15:20:18 2014
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
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QStatusBar>
#include <QtGui/QTableView>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionAbout;
    QAction *actionContact_Us;
    QWidget *centralWidget;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_7;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QTableView *nodeListTableView;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QTextEdit *logTextEdit;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_6;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *deviceIndexComboBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *channelIndexComboBox;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_3;
    QComboBox *baudRateComboBox;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_5;
    QLineEdit *startAddrLineEdit;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *fileLineEdit;
    QPushButton *openFilePushButton;
    QPushButton *updatePushButton;
    QPushButton *executePushButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(662, 428);
        MainWindow->setMinimumSize(QSize(662, 0));
        MainWindow->setMaximumSize(QSize(662, 16777215));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionContact_Us = new QAction(MainWindow);
        actionContact_Us->setObjectName(QString::fromUtf8("actionContact_Us"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_3 = new QGridLayout(centralWidget);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setMaximumSize(QSize(317, 16777215));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        nodeListTableView = new QTableView(groupBox);
        nodeListTableView->setObjectName(QString::fromUtf8("nodeListTableView"));
        nodeListTableView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout->addWidget(nodeListTableView, 0, 0, 1, 1);


        horizontalLayout_7->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        logTextEdit = new QTextEdit(groupBox_2);
        logTextEdit->setObjectName(QString::fromUtf8("logTextEdit"));

        gridLayout_2->addWidget(logTextEdit, 0, 0, 1, 1);


        horizontalLayout_7->addWidget(groupBox_2);


        verticalLayout_2->addLayout(horizontalLayout_7);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label);

        deviceIndexComboBox = new QComboBox(centralWidget);
        deviceIndexComboBox->setObjectName(QString::fromUtf8("deviceIndexComboBox"));
        deviceIndexComboBox->setMinimumSize(QSize(37, 0));

        horizontalLayout->addWidget(deviceIndexComboBox);


        horizontalLayout_6->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_2);

        channelIndexComboBox = new QComboBox(centralWidget);
        channelIndexComboBox->setObjectName(QString::fromUtf8("channelIndexComboBox"));
        channelIndexComboBox->setMinimumSize(QSize(37, 0));

        horizontalLayout_2->addWidget(channelIndexComboBox);


        horizontalLayout_6->addLayout(horizontalLayout_2);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_3->addWidget(label_3);

        baudRateComboBox = new QComboBox(centralWidget);
        baudRateComboBox->setObjectName(QString::fromUtf8("baudRateComboBox"));
        baudRateComboBox->setMinimumSize(QSize(98, 0));
        baudRateComboBox->setMaximumSize(QSize(98, 16777215));

        horizontalLayout_3->addWidget(baudRateComboBox);


        horizontalLayout_6->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        horizontalLayout_5->addWidget(label_5);

        startAddrLineEdit = new QLineEdit(centralWidget);
        startAddrLineEdit->setObjectName(QString::fromUtf8("startAddrLineEdit"));
        startAddrLineEdit->setEnabled(true);
        startAddrLineEdit->setMinimumSize(QSize(98, 0));

        horizontalLayout_5->addWidget(startAddrLineEdit);


        horizontalLayout_6->addLayout(horizontalLayout_5);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_4);

        fileLineEdit = new QLineEdit(centralWidget);
        fileLineEdit->setObjectName(QString::fromUtf8("fileLineEdit"));
        fileLineEdit->setMaximumSize(QSize(236, 16777215));

        horizontalLayout_4->addWidget(fileLineEdit);

        openFilePushButton = new QPushButton(centralWidget);
        openFilePushButton->setObjectName(QString::fromUtf8("openFilePushButton"));
        openFilePushButton->setMinimumSize(QSize(98, 0));

        horizontalLayout_4->addWidget(openFilePushButton);

        updatePushButton = new QPushButton(centralWidget);
        updatePushButton->setObjectName(QString::fromUtf8("updatePushButton"));
        updatePushButton->setMinimumSize(QSize(98, 0));

        horizontalLayout_4->addWidget(updatePushButton);

        executePushButton = new QPushButton(centralWidget);
        executePushButton->setObjectName(QString::fromUtf8("executePushButton"));
        executePushButton->setMinimumSize(QSize(98, 0));

        horizontalLayout_4->addWidget(executePushButton);


        verticalLayout->addLayout(horizontalLayout_4);


        verticalLayout_2->addLayout(verticalLayout);


        gridLayout_3->addLayout(verticalLayout_2, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 662, 23));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionExit);
        menuHelp->addAction(actionAbout);
        menuHelp->addAction(actionContact_Us);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CAN Bootloader", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        actionContact_Us->setText(QApplication::translate("MainWindow", "Contact Us", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "Node List", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "Operation Log", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "Device Index:", 0, QApplication::UnicodeUTF8));
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
        label_2->setText(QApplication::translate("MainWindow", "Channel Index:", 0, QApplication::UnicodeUTF8));
        channelIndexComboBox->clear();
        channelIndexComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("MainWindow", "Baud Rate:", 0, QApplication::UnicodeUTF8));
        baudRateComboBox->clear();
        baudRateComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "1000Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "800Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "666Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "600Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "500Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "400Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "300Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "250Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "200Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "150Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "100Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "90Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "80Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "60Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "50Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "40Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "30Kbps", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "20Kbps", 0, QApplication::UnicodeUTF8)
        );
        label_5->setText(QApplication::translate("MainWindow", "Start Addr(Hex):", 0, QApplication::UnicodeUTF8));
        startAddrLineEdit->setText(QApplication::translate("MainWindow", "08004000", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Firmware File:", 0, QApplication::UnicodeUTF8));
        openFilePushButton->setText(QApplication::translate("MainWindow", "Open File", 0, QApplication::UnicodeUTF8));
        updatePushButton->setText(QApplication::translate("MainWindow", "Update", 0, QApplication::UnicodeUTF8));
        executePushButton->setText(QApplication::translate("MainWindow", "Execute", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
