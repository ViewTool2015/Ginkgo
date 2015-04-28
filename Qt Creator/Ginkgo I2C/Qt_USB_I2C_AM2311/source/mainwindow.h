#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "thermometer.h"
#include <qwt_plot.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_series_data.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_text.h>
#include <qwt_math.h>
#include <math.h>
#include "ControlI2C.h"

#define SIMPLE_NUMS 100

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void plotInit(QwtPlot *pPlot);
    void populate(void);
    bool AM2311_Init(void);
    void displayCurve(QwtPlotCurve *pCurve,double *pCurveData,int dataNum);
private slots:
    void on_startPushButton_clicked();

private:
    Ui::MainWindow *ui;
    ThermoMeter *pTemperatureThermo,*pHumidityThermo;
    QwtPlotCurve *pHumidityCurve,*pTemperatureCurve;
    int mesureTimerID;
    double HumidityData[SIMPLE_NUMS];
    double TemperatureData[SIMPLE_NUMS];
protected:
    void timerEvent(QTimerEvent *event);
};

#endif // MAINWINDOW_H
