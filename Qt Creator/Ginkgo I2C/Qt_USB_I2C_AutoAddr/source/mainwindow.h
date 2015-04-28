#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "ControlI2C.h"
#include "ControlGPIO.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startPushButton_clicked();

    void on_clearPushButton_clicked();

    void openUrl(QString url);

private:
    Ui::MainWindow *ui;
    QRegExpValidator *pSlaveAddrPattern;
};

#endif // MAINWINDOW_H
