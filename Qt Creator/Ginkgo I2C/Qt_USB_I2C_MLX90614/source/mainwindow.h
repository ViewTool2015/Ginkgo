#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "thermometer.h"
#include <math.h>
#include "ControlGPIO.h"
#include "ControlSMBus.h"

#define SIMPLE_NUMS 100
#define SCL0    VGI_GPIO_PIN1
#define SCL1    VGI_GPIO_PIN5
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool MLX90614_Init(void);
private slots:
    void on_startPushButton_clicked();

private:
    Ui::MainWindow *ui;
    ThermoMeter *pTemperatureThermo;
    int mesureTimerID;
protected:
    void timerEvent(QTimerEvent *event);
};

#endif // MAINWINDOW_H
