#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "mfrc522.h"
#include "ControlGPIO.h"
#include "ControlSPI.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //void initCardDataTable(void);
    void initRegisterTable(void);
    bool initDevice(void);
    void timerEvent(QTimerEvent *Event);
    void showChipImage(QGraphicsView *graphicViewer, QString imageName);
    
private slots:
    void on_actionContactus_triggered();

    void on_actionAbout_triggered();

    void on_actionRC522_Datasheet_triggered();

    void on_rc522RegisterTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_rc522RegisterTreeWidget_itemChanged(QTreeWidgetItem *item, int column);

    void on_actionStart_triggered();

    void on_actionStop_triggered();

    void on_registerReadPushButton_clicked();

    void on_readSectorPushButton_clicked();

    void on_writeSectorPushButton_clicked();

    void on_sectorIndexComboBox_currentIndexChanged(int index);

    void on_blockComboBox_currentIndexChanged(int index);

    void on_valueSectorIndexComboBox_currentIndexChanged(int index);

    void on_valueBlockComboBox_currentIndexChanged(int index);

    void on_incrementPushButton_clicked();

    void on_getValuePushButton_clicked();

    void on_setValuePushButton_clicked();

    void on_decrementPushButton_clicked();

private:
    Ui::MainWindow *ui;
    QString curentStr;
    int readCardInfoTimer;
    MFRC522 *mfrc522;
    bool stopFindCard;
    QRegExpValidator *blockDataPattern,*keyPattern,*valuePattern;
};

#endif // MAINWINDOW_H
