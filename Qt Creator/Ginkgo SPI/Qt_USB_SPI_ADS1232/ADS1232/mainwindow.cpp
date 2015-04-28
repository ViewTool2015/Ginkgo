#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_readVoltagePushButton_clicked()
{
    int ret;
    ret = VSI_OpenDevice(VSI_USBSPI,ui->deviceIndexcCmboBox->currentIndex(),0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device error!");
        return;
    }
    VSI_INIT_CONFIG SPI_Cofig;
    SPI_Cofig.ClockSpeed = 281250;
    SPI_Cofig.ControlMode = 1;
    SPI_Cofig.MasterMode = 1;
    SPI_Cofig.CPHA = 1;
    SPI_Cofig.CPOL = 0;
    SPI_Cofig.LSBFirst = 0;
    SPI_Cofig.SelPolarity = 0;
    SPI_Cofig.TranBits = 8;
    ret = VSI_InitSPI(VSI_USBSPI,ui->deviceIndexcCmboBox->currentIndex(),&SPI_Cofig);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Initialize device error!");
        return;
    }
    uint8_t Data[3]={0};
    ret = VSI_ReadBytes(VSI_USBSPI,ui->deviceIndexcCmboBox->currentIndex(),0,Data,3);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Read data error!");
        return;
    }
    float ref = ui->referenceVoltageLineEdit->text().toFloat(NULL);
    QString str;
    if(Data[2]&0x80){
        str.sprintf("%f",((~((Data[2]<<16)|(Data[1]<<8)|(Data[0])))+1)*ref/(0xFFFFFF));
    }else{
        str.sprintf("%f",((Data[2]<<16)|(Data[1]<<8)|(Data[0]))*ref/(0xFFFFFF));
    }
    ui->getVoltageLineEdit->setText(str);
}
