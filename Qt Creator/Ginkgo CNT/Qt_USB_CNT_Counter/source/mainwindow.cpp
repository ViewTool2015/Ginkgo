#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ControlCNT.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    GetCoutTimer = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startPushButton_clicked()
{
    int ret;
    CNT_INIT_CONFIG CounterInit;
    uint8_t Channel = 0;
    if(ui->startPushButton->text()=="Start"){
        ret = CNT_ScanDevice(1);
        if(ret <= 0){
            return;
        }
        ret = CNT_OpenDevice(CNT_USBCNT,ui->deviceIndexComboBox->currentIndex(),0);
        if(ret != ERR_SUCCESS){
            return;
        }
        CounterInit.CounterBitWide = 16+ui->bitWideComboBox->currentIndex()*16;
        CounterInit.CounterMode = ui->counterModeComboBox->currentIndex();
        CounterInit.CounterPolarity = ui->counterPolarityComboBox->currentIndex();
        if(ui->channe0EnCheckBox->isChecked()){
            Channel |= 0x01;
        }
        if(ui->channe1EnCheckBox->isChecked()){
            Channel |= 0x02;
        }
        if(ui->channe2EnCheckBox->isChecked()){
            Channel |= 0x04;
        }
        if(ui->channe3EnCheckBox->isChecked()){
            Channel |= 0x08;
        }
        ret = CNT_InitCounter(CNT_USBCNT,
                              ui->deviceIndexComboBox->currentIndex(),
                              Channel,
                              &CounterInit);
        if(ret != ERR_SUCCESS){
            return;
        }
        ret = CNT_StartCounter(CNT_USBCNT,
                               ui->deviceIndexComboBox->currentIndex(),
                               Channel);
        if(ret != ERR_SUCCESS){
            return;
        }
        ui->startPushButton->setText("Stop");
        GetCoutTimer = startTimer(10);
    }else{
        if(ui->channe0EnCheckBox->isChecked()){
            Channel |= 0x01;
        }
        if(ui->channe1EnCheckBox->isChecked()){
            Channel |= 0x02;
        }
        if(ui->channe2EnCheckBox->isChecked()){
            Channel |= 0x04;
        }
        if(ui->channe3EnCheckBox->isChecked()){
            Channel |= 0x08;
        }
        ret = CNT_StopCounter(CNT_USBCNT,
                               ui->deviceIndexComboBox->currentIndex(),
                               Channel);
        if(ret != ERR_SUCCESS){
            return;
        }
        ui->startPushButton->setText("Start");
        killTimer(GetCoutTimer);
    }
}

void MainWindow::timerEvent(QTimerEvent *Event)
{
    if(Event->timerId() == GetCoutTimer){
        uint8_t Channel = 0;
        uint32_t CounterValue[4]={0};
        int ret;
        if(ui->channe0EnCheckBox->isChecked()){
            Channel |= 0x01;
        }
        if(ui->channe1EnCheckBox->isChecked()){
            Channel |= 0x02;
        }
        if(ui->channe2EnCheckBox->isChecked()){
            Channel |= 0x04;
        }
        if(ui->channe3EnCheckBox->isChecked()){
            Channel |= 0x08;
        }
        ret = CNT_GetCounter(CNT_USBCNT,
                             ui->deviceIndexComboBox->currentIndex(),
                             Channel,
                             CounterValue);
        if(ret != ERR_SUCCESS){
            killTimer(GetCoutTimer);
            ui->startPushButton->setText("Start");
            return;
        }else{
            QLCDNumber *LcdNumber[]={ui->Channel0LcdNumber,ui->Channel1LcdNumber,ui->Channel2LcdNumber,ui->Channel3LcdNumber};
            uint32_t *pValue = CounterValue;
            for(int i=0;i<4;i++){
                if(Channel&(1<<i)){
                    if((*pValue) > 999999){
                        LcdNumber[i]->setDigitCount(10);
                    }else{
                        LcdNumber[i]->setDigitCount(6);
                    }
                    LcdNumber[i]->display((int)(*pValue++));
                }
            }
        }
    }
}

void MainWindow::on_resetPushButton_clicked()
{
    uint8_t Channel = 0;
    uint32_t CounterValue[4]={0};
    int ret;
    if(ui->channe0EnCheckBox->isChecked()){
        Channel |= 0x01;
    }
    if(ui->channe1EnCheckBox->isChecked()){
        Channel |= 0x02;
    }
    if(ui->channe2EnCheckBox->isChecked()){
        Channel |= 0x04;
    }
    if(ui->channe3EnCheckBox->isChecked()){
        Channel |= 0x08;
    }
    ret = CNT_SetCounter(CNT_USBCNT,
                         ui->deviceIndexComboBox->currentIndex(),
                         Channel,
                         CounterValue);
    if(ret != ERR_SUCCESS){
        return;
    }
}

void MainWindow::on_channe0EnCheckBox_clicked(bool checked)
{
    if(checked){
        ui->Channel0LcdNumber->setEnabled(true);
    }else{
        ui->Channel0LcdNumber->setDisabled(true);
    }
}

void MainWindow::on_channe1EnCheckBox_clicked(bool checked)
{
    if(checked){
        ui->Channel1LcdNumber->setEnabled(true);
    }else{
        ui->Channel1LcdNumber->setDisabled(true);
    }
}

void MainWindow::on_channe2EnCheckBox_clicked(bool checked)
{
    if(checked){
        ui->Channel2LcdNumber->setEnabled(true);
    }else{
        ui->Channel2LcdNumber->setDisabled(true);
    }
}

void MainWindow::on_channe3EnCheckBox_clicked(bool checked)
{
    if(checked){
        ui->Channel3LcdNumber->setEnabled(true);
    }else{
        ui->Channel3LcdNumber->setDisabled(true);
    }
}
