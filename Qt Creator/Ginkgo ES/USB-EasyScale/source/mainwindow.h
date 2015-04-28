#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <Windows.h>
#include "EasyScale.h"

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
    void on_initPushButton_clicked();

    void on_writeDataPushButton_clicked();

private:
    VEI_ES_TIMING ESTime;
    VEI_ES_OBJ ESData;
    bool initFlag;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
