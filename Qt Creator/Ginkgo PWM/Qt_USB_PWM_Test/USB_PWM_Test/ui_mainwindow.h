/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sat Feb 8 13:55:38 2014
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
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QSpinBox>
#include <QtGui/QStatusBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionContact_Us;
    QAction *actionAbout;
    QAction *actionExit;
    QWidget *centralWidget;
    QGridLayout *gridLayout_4;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_3;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *deviceIndexComboBox;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *pwmIndexComboBox;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_3;
    QSlider *freqHorizontalSlider;
    QSpinBox *freqSpinBox;
    QGroupBox *groupBox;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label_4;
    QSlider *dutyCycleCh1HorizontalSlider;
    QSpinBox *dutyCycleCh1SpinBox;
    QHBoxLayout *horizontalLayout_8;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_5;
    QComboBox *polarityCh1ComboBox;
    QHBoxLayout *horizontalLayout_7;
    QLabel *label_6;
    QComboBox *pwmModeCh1ComboBox;
    QPushButton *startCh1PushButton;
    QPushButton *stopCh1PushButton;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_7;
    QSlider *dutyCycleCh2HorizontalSlider;
    QSpinBox *dutyCycleCh2SpinBox;
    QHBoxLayout *horizontalLayout_10;
    QHBoxLayout *horizontalLayout_11;
    QLabel *label_8;
    QComboBox *polarityCh2ComboBox;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_9;
    QComboBox *pwmModeCh2ComboBox;
    QPushButton *startCh2PushButton;
    QPushButton *stopCh2PushButton;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(498, 331);
        actionContact_Us = new QAction(MainWindow);
        actionContact_Us->setObjectName(QString::fromUtf8("actionContact_Us"));
        actionAbout = new QAction(MainWindow);
        actionAbout->setObjectName(QString::fromUtf8("actionAbout"));
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        gridLayout_4 = new QGridLayout(centralWidget);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        gridLayout_3 = new QGridLayout(groupBox_3);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setMinimumSize(QSize(80, 0));
        label->setMaximumSize(QSize(80, 16777215));
        label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout->addWidget(label);

        deviceIndexComboBox = new QComboBox(groupBox_3);
        deviceIndexComboBox->setObjectName(QString::fromUtf8("deviceIndexComboBox"));

        horizontalLayout->addWidget(deviceIndexComboBox);


        horizontalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_2->addWidget(label_2);

        pwmIndexComboBox = new QComboBox(groupBox_3);
        pwmIndexComboBox->setObjectName(QString::fromUtf8("pwmIndexComboBox"));

        horizontalLayout_2->addWidget(pwmIndexComboBox);


        horizontalLayout_3->addLayout(horizontalLayout_2);


        gridLayout_3->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setMinimumSize(QSize(80, 0));
        label_3->setMaximumSize(QSize(80, 16777215));
        label_3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_4->addWidget(label_3);

        freqHorizontalSlider = new QSlider(groupBox_3);
        freqHorizontalSlider->setObjectName(QString::fromUtf8("freqHorizontalSlider"));
        freqHorizontalSlider->setMinimum(1);
        freqHorizontalSlider->setMaximum(20000000);
        freqHorizontalSlider->setSingleStep(1);
        freqHorizontalSlider->setPageStep(10);
        freqHorizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_4->addWidget(freqHorizontalSlider);

        freqSpinBox = new QSpinBox(groupBox_3);
        freqSpinBox->setObjectName(QString::fromUtf8("freqSpinBox"));
        freqSpinBox->setMinimumSize(QSize(60, 0));
        freqSpinBox->setMinimum(1);
        freqSpinBox->setMaximum(20000000);
        freqSpinBox->setSingleStep(1);

        horizontalLayout_4->addWidget(freqSpinBox);


        gridLayout_3->addLayout(horizontalLayout_4, 1, 0, 1, 1);


        verticalLayout_4->addWidget(groupBox_3);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout = new QGridLayout(groupBox);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QString::fromUtf8("horizontalLayout_5"));
        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setMinimumSize(QSize(80, 0));
        label_4->setMaximumSize(QSize(80, 16777215));
        label_4->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_5->addWidget(label_4);

        dutyCycleCh1HorizontalSlider = new QSlider(groupBox);
        dutyCycleCh1HorizontalSlider->setObjectName(QString::fromUtf8("dutyCycleCh1HorizontalSlider"));
        dutyCycleCh1HorizontalSlider->setMinimum(0);
        dutyCycleCh1HorizontalSlider->setMaximum(100);
        dutyCycleCh1HorizontalSlider->setSingleStep(1);
        dutyCycleCh1HorizontalSlider->setPageStep(10);
        dutyCycleCh1HorizontalSlider->setValue(50);
        dutyCycleCh1HorizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_5->addWidget(dutyCycleCh1HorizontalSlider);

        dutyCycleCh1SpinBox = new QSpinBox(groupBox);
        dutyCycleCh1SpinBox->setObjectName(QString::fromUtf8("dutyCycleCh1SpinBox"));
        dutyCycleCh1SpinBox->setMinimumSize(QSize(60, 0));
        dutyCycleCh1SpinBox->setMinimum(0);
        dutyCycleCh1SpinBox->setMaximum(100);
        dutyCycleCh1SpinBox->setSingleStep(1);
        dutyCycleCh1SpinBox->setValue(50);

        horizontalLayout_5->addWidget(dutyCycleCh1SpinBox);


        gridLayout->addLayout(horizontalLayout_5, 0, 1, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QString::fromUtf8("horizontalLayout_8"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QString::fromUtf8("horizontalLayout_6"));
        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setMinimumSize(QSize(80, 0));
        label_5->setMaximumSize(QSize(80, 16777215));
        label_5->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_6->addWidget(label_5);

        polarityCh1ComboBox = new QComboBox(groupBox);
        polarityCh1ComboBox->setObjectName(QString::fromUtf8("polarityCh1ComboBox"));

        horizontalLayout_6->addWidget(polarityCh1ComboBox);


        horizontalLayout_8->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QString::fromUtf8("horizontalLayout_7"));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout_7->addWidget(label_6);

        pwmModeCh1ComboBox = new QComboBox(groupBox);
        pwmModeCh1ComboBox->setObjectName(QString::fromUtf8("pwmModeCh1ComboBox"));

        horizontalLayout_7->addWidget(pwmModeCh1ComboBox);


        horizontalLayout_8->addLayout(horizontalLayout_7);

        startCh1PushButton = new QPushButton(groupBox);
        startCh1PushButton->setObjectName(QString::fromUtf8("startCh1PushButton"));

        horizontalLayout_8->addWidget(startCh1PushButton);

        stopCh1PushButton = new QPushButton(groupBox);
        stopCh1PushButton->setObjectName(QString::fromUtf8("stopCh1PushButton"));

        horizontalLayout_8->addWidget(stopCh1PushButton);


        gridLayout->addLayout(horizontalLayout_8, 1, 1, 1, 1);


        verticalLayout_4->addWidget(groupBox);

        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        gridLayout_2 = new QGridLayout(groupBox_2);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QString::fromUtf8("horizontalLayout_9"));
        label_7 = new QLabel(groupBox_2);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setMinimumSize(QSize(80, 0));
        label_7->setMaximumSize(QSize(80, 16777215));
        label_7->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_9->addWidget(label_7);

        dutyCycleCh2HorizontalSlider = new QSlider(groupBox_2);
        dutyCycleCh2HorizontalSlider->setObjectName(QString::fromUtf8("dutyCycleCh2HorizontalSlider"));
        dutyCycleCh2HorizontalSlider->setMaximum(100);
        dutyCycleCh2HorizontalSlider->setValue(50);
        dutyCycleCh2HorizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout_9->addWidget(dutyCycleCh2HorizontalSlider);

        dutyCycleCh2SpinBox = new QSpinBox(groupBox_2);
        dutyCycleCh2SpinBox->setObjectName(QString::fromUtf8("dutyCycleCh2SpinBox"));
        dutyCycleCh2SpinBox->setMinimumSize(QSize(60, 0));
        dutyCycleCh2SpinBox->setMaximum(100);
        dutyCycleCh2SpinBox->setValue(50);

        horizontalLayout_9->addWidget(dutyCycleCh2SpinBox);


        gridLayout_2->addLayout(horizontalLayout_9, 0, 0, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QString::fromUtf8("horizontalLayout_10"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QString::fromUtf8("horizontalLayout_11"));
        label_8 = new QLabel(groupBox_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setMinimumSize(QSize(80, 0));
        label_8->setMaximumSize(QSize(80, 16777215));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        horizontalLayout_11->addWidget(label_8);

        polarityCh2ComboBox = new QComboBox(groupBox_2);
        polarityCh2ComboBox->setObjectName(QString::fromUtf8("polarityCh2ComboBox"));

        horizontalLayout_11->addWidget(polarityCh2ComboBox);


        horizontalLayout_10->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QString::fromUtf8("horizontalLayout_12"));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        horizontalLayout_12->addWidget(label_9);

        pwmModeCh2ComboBox = new QComboBox(groupBox_2);
        pwmModeCh2ComboBox->setObjectName(QString::fromUtf8("pwmModeCh2ComboBox"));

        horizontalLayout_12->addWidget(pwmModeCh2ComboBox);


        horizontalLayout_10->addLayout(horizontalLayout_12);

        startCh2PushButton = new QPushButton(groupBox_2);
        startCh2PushButton->setObjectName(QString::fromUtf8("startCh2PushButton"));

        horizontalLayout_10->addWidget(startCh2PushButton);

        stopCh2PushButton = new QPushButton(groupBox_2);
        stopCh2PushButton->setObjectName(QString::fromUtf8("stopCh2PushButton"));

        horizontalLayout_10->addWidget(stopCh2PushButton);


        gridLayout_2->addLayout(horizontalLayout_10, 1, 0, 1, 1);


        verticalLayout_4->addWidget(groupBox_2);


        gridLayout_4->addLayout(verticalLayout_4, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 498, 23));
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
        menuHelp->addAction(actionContact_Us);
        menuHelp->addAction(actionAbout);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "USB PWM", 0, QApplication::UnicodeUTF8));
        actionContact_Us->setText(QApplication::translate("MainWindow", "Contact Us", 0, QApplication::UnicodeUTF8));
        actionAbout->setText(QApplication::translate("MainWindow", "About", 0, QApplication::UnicodeUTF8));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Setting", 0, QApplication::UnicodeUTF8));
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
        label_2->setText(QApplication::translate("MainWindow", "PWM Index:", 0, QApplication::UnicodeUTF8));
        pwmIndexComboBox->clear();
        pwmIndexComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "PWM1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "PWM2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "PWM3", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "PWM4", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("MainWindow", "Frequency:", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "CH1", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Duty Cycle:", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "  Polarity:", 0, QApplication::UnicodeUTF8));
        polarityCh1ComboBox->clear();
        polarityCh1ComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Low", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "High", 0, QApplication::UnicodeUTF8)
        );
        label_6->setText(QApplication::translate("MainWindow", "PWM Mode:", 0, QApplication::UnicodeUTF8));
        pwmModeCh1ComboBox->clear();
        pwmModeCh1ComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Mode1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Mode2", 0, QApplication::UnicodeUTF8)
        );
        startCh1PushButton->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
        stopCh1PushButton->setText(QApplication::translate("MainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "CH2", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "Duty Cycle:", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "  Polarity:", 0, QApplication::UnicodeUTF8));
        polarityCh2ComboBox->clear();
        polarityCh2ComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Low", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "High", 0, QApplication::UnicodeUTF8)
        );
        label_9->setText(QApplication::translate("MainWindow", "PWM Mode:", 0, QApplication::UnicodeUTF8));
        pwmModeCh2ComboBox->clear();
        pwmModeCh2ComboBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Mode1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "Mode2", 0, QApplication::UnicodeUTF8)
        );
        startCh2PushButton->setText(QApplication::translate("MainWindow", "Start", 0, QApplication::UnicodeUTF8));
        stopCh2PushButton->setText(QApplication::translate("MainWindow", "Stop", 0, QApplication::UnicodeUTF8));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0, QApplication::UnicodeUTF8));
        menuHelp->setTitle(QApplication::translate("MainWindow", "Help", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
