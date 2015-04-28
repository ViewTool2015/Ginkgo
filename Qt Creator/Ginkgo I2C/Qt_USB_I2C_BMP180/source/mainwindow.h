#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QMessageBox>
#include "thermometer.h"
#include "manometer.h"
#include <math.h>
#include "ControlI2C.h"
#include "bmp180.h"

#define I2C_BUFFER_LEN 64

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool BMP180_Init(void);


private slots:
    void on_startPushButton_clicked();

private:
    Ui::MainWindow *ui;
    ThermoMeter *pTemperatureThermo;
    ManoMeter *pPressure;
    int mesureTimerID;
    bmp180_t bmp180;
protected:
    void timerEvent(QTimerEvent *event);
};

#endif // MAINWINDOW_H
