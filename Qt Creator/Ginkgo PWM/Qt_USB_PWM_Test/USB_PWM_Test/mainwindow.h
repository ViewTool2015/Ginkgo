#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>

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
    void on_startCh1PushButton_clicked();

    void on_freqSpinBox_valueChanged(int arg1);

    void on_freqHorizontalSlider_valueChanged(int value);

    void on_dutyCycleCh1SpinBox_valueChanged(int arg1);

    void on_dutyCycleCh1HorizontalSlider_valueChanged(int value);

    void on_dutyCycleCh2SpinBox_valueChanged(int arg1);

    void on_dutyCycleCh2HorizontalSlider_valueChanged(int value);

    void on_startCh2PushButton_clicked();

    void on_stopCh1PushButton_clicked();

    void on_stopCh2PushButton_clicked();

    void on_actionContact_Us_triggered();

    void on_actionAbout_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
