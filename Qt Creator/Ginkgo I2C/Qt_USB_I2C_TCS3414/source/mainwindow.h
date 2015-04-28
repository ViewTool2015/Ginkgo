#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <windows.h>
#include <qlayout.h>
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
#include "TCS3414.h"
#include "CIE.h"

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
    void colorPopulate(void);
    void colorPlotInit(QwtPlot *pPlot);
    void displayCurve(QwtPlot *plot, QwtPlotCurve *pCurve, double *pCurveData, int dataNum);
    bool initDevice();

    int setTimingReg(int x);
    int setInterruptSourceReg(int x);
    int setInterruptControlReg(int x);
    int setGain(int x);
    int setEnableADC();
    int clearInterrupt();

    
private:
    Ui::MainWindow *ui;
    int mesureTimerID;
    double redData[SIMPLE_NUMS];
    double greenData[SIMPLE_NUMS];
    double blueData[SIMPLE_NUMS];
    double clearData[SIMPLE_NUMS];
    QwtPlotCurve *redCurve,*greenCurve,*blueCurve,*clearCurve;

protected:
    void timerEvent(QTimerEvent *event);
private slots:
    void on_startPushButton_clicked();
};

#endif // MAINWINDOW_H
