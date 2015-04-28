#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "ControlADC.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void timerEvent(QTimerEvent *event);
    unsigned char getChannelSelInfo(void);
    QStringList readData(void);
    void displayData(QStringList dataList);
    int initDevice(void);
    ~MainWindow();
    QString mytr(const char* chars);
    
private slots:
    void on_initDevPushButton_clicked();

    void on_startReadDataPushButton_clicked();

private:
    Ui::MainWindow *ui;
    int autoScanDeviceTimerID,deviceNum,autoGetDataTimerID;
};

#endif // MAINWINDOW_H
