#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pTemperatureThermo = new ThermoMeter();
    pTemperatureThermo->setMinimum(-40);
    pTemperatureThermo->setMaximum(100);
    pTemperatureThermo->setSuffix(" [C]");
    pTemperatureThermo->setValueOffset(280);
    ui->temperatureGroupBox->layout()->addWidget(pTemperatureThermo);

    ui->EEPROMTableWidget->setColumnWidth(1,150);
    ui->EEPROMTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int i=0;i<ui->EEPROMTableWidget->rowCount();i++){
        ui->EEPROMTableWidget->setRowHeight(i,20);
    }
    ui->RAMTableWidget->setColumnWidth(1,150);
    ui->RAMTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int i=0;i<ui->RAMTableWidget->rowCount();i++){
        ui->RAMTableWidget->setRowHeight(i,20);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::MLX90614_Init(void)
{
    int ret;
    //scan device
    ret = SMBus_ScanDevice(1);
    if(ret <= 0){
        QMessageBox::warning(this,"Warning","No device connect!");
        return false;
    }
    //open device
    ret = SMBus_OpenDevice(VII_USBI2C,
                           ui->deviceIndexComboBox->currentIndex(),
                           ui->channelIndexComboBox->currentIndex());
    if (ret != SMBUS_OK){
        QMessageBox::warning(this,"Warning","Open device error!");
        return false;
    }
    //Switching from PWM mode to SMBus
    VGI_SetOutput(VGI_USBGPIO,
                  ui->deviceIndexComboBox->currentIndex(),
                  ui->channelIndexComboBox->currentIndex()==0?SCL0:SCL1);
    VGI_SetPins(VGI_USBGPIO,
                ui->deviceIndexComboBox->currentIndex(),
                ui->channelIndexComboBox->currentIndex()==0?SCL0:SCL1);
    VGI_ResetPins(VGI_USBGPIO,
                  ui->deviceIndexComboBox->currentIndex(),
                  ui->channelIndexComboBox->currentIndex()==0?SCL0:SCL1);
    Sleep(100);
    VGI_SetPins(VGI_USBGPIO,
                ui->deviceIndexComboBox->currentIndex(),
                ui->channelIndexComboBox->currentIndex()==0?SCL0:SCL1);
    //初始化设备(硬件控制模式)
    ret = SMBus_HardInit(VII_USBI2C,
                         ui->deviceIndexComboBox->currentIndex(),
                         ui->channelIndexComboBox->currentIndex(),
                         100000,
                         0x00);
    if (ret != SMBUS_OK){
        QMessageBox::warning(this,"Warning","Initialize device error!");
        return false;
    }
    for(int i=0;i<ui->EEPROMTableWidget->rowCount();i++){
        uint16_t dataTmp;
        uint8_t cmd = ui->EEPROMTableWidget->item(i,0)->text().toInt(NULL,16);
        ret = SMBus_ReadWordProtocol(VII_USBI2C,
                                     ui->deviceIndexComboBox->currentIndex(),
                                     ui->channelIndexComboBox->currentIndex(),
                                     0x5A<<1,
                                     cmd|0x20,
                                     &dataTmp,
                                     1);
        if (ret != SMBUS_OK){
            QMessageBox::warning(this,"Warning","Read data error!");
            return false;
        }
        QString strTmp;
        strTmp.sprintf("%04X",dataTmp&0xFFFF);
        ui->EEPROMTableWidget->item(i,2)->setText(strTmp);
    }

    return true;
}

void MainWindow::on_startPushButton_clicked()
{
    if(ui->startPushButton->text()=="Start"){
        if(MLX90614_Init()){
            mesureTimerID = startTimer(500);
            ui->startPushButton->setText("Stop");
        }
    }else{
        killTimer(mesureTimerID);
        ui->startPushButton->setText("Start");
    }
}

/**
  * @brief  定时器时间函数
  * @param  event 定时器指针
  * @retval None
  */
void MainWindow::timerEvent(QTimerEvent *event)
{
    int ret;
    //定时采样数据并显示
    if(event->timerId()==mesureTimerID){
        for(int i=0;i<ui->RAMTableWidget->rowCount()-1;i++){
            uint16_t dataTmp;
            uint8_t cmd = ui->RAMTableWidget->item(i,0)->text().toInt(NULL,16);
            ret = SMBus_ReadWordProtocol(VII_USBI2C,
                                         ui->deviceIndexComboBox->currentIndex(),
                                         ui->channelIndexComboBox->currentIndex(),
                                         0x5A<<1,
                                         cmd,
                                         &dataTmp,
                                         1);
            if (ret != SMBUS_OK){
                QMessageBox::warning(this,"Warning","Read data error!");
                on_startPushButton_clicked();
                return;
            }
            QString strTmp;
            strTmp.sprintf("%04X",dataTmp&0xFFFF);
            ui->RAMTableWidget->item(i,2)->setText(strTmp);
            //
            //This converts high and low bytes together and processes temperature, MSB is a error bit and is ignored for temps
            double tempFactor = 0.02; // 0.02 degrees per LSB (measurement resolution of the MLX90614)
            double tempData = 0x0000; // zero out the data
            // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
            tempData = (dataTmp * tempFactor)-0.01;

            float celcius = tempData - 273.15;
            strTmp.sprintf("%.2f",celcius);
            ui->RAMTableWidget->item(i,3)->setText(strTmp);
            pTemperatureThermo->setValue(celcius);
        }
    }
}
