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
    void initRegisterTable(void);
    void timerEvent(QTimerEvent * Event);
    bool initDevice(void);
    void displayMsg(QString msg);
    
private slots:
    void on_refreshRegisterPushButton_clicked();

    void on_registerTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_registerTreeWidget_itemChanged(QTreeWidgetItem *item, int column);

    void on_writeRegisterPushButton_clicked();

    void about();
    void contactus();
    void quit();

    void on_actionStop_triggered();

    void on_actionStart_triggered();

    void on_readRegisterPushButton_clicked();

    void on_updateRegisterPushButton_clicked();

    void on_FLUSH_TX_PushButton_clicked();

    void on_FLUSH_RX_PushButton_clicked();

    void on_workingModeComboBox_currentIndexChanged(const QString &arg1);

    void on_sendDataPushButton_clicked();

    void on_registerAddrcomboBox_currentIndexChanged(const QString &arg1);

    void on_clearReceiveDataPushButton_clicked();

    void on_clearSendDataPushButton_clicked();

    void on_receiveHexDisplayCheckBox_clicked(bool checked);

    void on_sendHexDisplayCheckBox_clicked(bool checked);

    void on_RXPayloadWidthPomboBox_currentIndexChanged(const QString &arg1);

    void on_autoSendDataCheckBox_clicked(bool checked);

    void on_writeAllRegisterPushButton_clicked();

    void on_actionNRF24L01_Datasheet_triggered();

private:
    Ui::MainWindow *ui;
    int getDataTimer;
    int autoSendTimer;
    int scanDevTimer;
    QLabel *devConnectStateLabel;
    QLabel *RxCounterLabel;
    QLabel *TxCounterLabel;
    int RxCounter,TxCounter;
    QString curentStr;

};

#endif // MAINWINDOW_H
