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
    void PM25Populate(void);
    void PM25PlotInit(QwtPlot *pPlot);
    void displayCurve(QwtPlot *plot, QwtPlotCurve *pCurve, double *pCurveData, int dataNum);
    void initDevice();
    
private:
    Ui::MainWindow *ui;
    int autoScanDeviceTimerID,mesureTimerID;
    int deviceNum;
    double PM25Data[SIMPLE_NUMS];
    double PM25ADData[SIMPLE_NUMS];
    int AccelDataIndex;
    QwtPlotCurve *PM25Curve,*PM25ADCurve;
    QByteArray PM25ADDataBuffer;

protected:
    void timerEvent(QTimerEvent *event);
private slots:
    void on_startPushButton_clicked();
    void about();
    void contactus();
    void quit();
};

#endif // MAINWINDOW_H
