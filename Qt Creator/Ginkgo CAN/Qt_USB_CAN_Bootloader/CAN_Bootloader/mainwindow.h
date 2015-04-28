#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "ControlCAN.h"

typedef  struct {
    unsigned char   local_addr[128];
    unsigned char   mac_addr[128][8];
} tMAC_Addr;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void nodeTableInit(void);
    bool canInit(void);
    int CAN_GetBaudRateNum(unsigned int BaudRate);
    bool eraseSectors(unsigned char can_node_addr,unsigned int startAddr,unsigned int EndAddr);
    bool setStartAddr(unsigned char can_node_addr,unsigned int startAddr,unsigned int dataSize);
    void logMessage(QString str);
    void onlineCheck(void);
private slots:
    void on_updatePushButton_clicked();

    void on_openFilePushButton_clicked();

    void on_executePushButton_clicked();

    void on_actionExit_triggered();

    void on_actionAbout_triggered();

    void on_actionContact_Us_triggered();

private:
    Ui::MainWindow *ui;
    void timerEvent(QTimerEvent * event);
    QStandardItemModel *model;
    int scanNodeTimerID;
    tMAC_Addr can_addr;
};

#endif // MAINWINDOW_H
