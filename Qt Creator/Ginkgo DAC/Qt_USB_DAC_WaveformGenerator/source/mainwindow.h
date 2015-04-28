#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QMessageBox>
#ifndef OS_UNIX
#include <windows.h>
#endif
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
#include "ControlDAC.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void WavePopulate(void);
    void displayCurve(QwtPlot *plot,QwtPlotCurve *pCurve,double *pCurveData,int dataNum);
    void WavePlotInit(QwtPlot *pPlot);
private slots:
    void showWave(void);
    void on_startPushButton_clicked();

    void on_stopPushButton_clicked();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionContuct_Us_triggered();

private:
    Ui::MainWindow *ui;
    QwtPlotCurve *WaveCurve;
    double WaveData[1024];
};

#endif // MAINWINDOW_H
