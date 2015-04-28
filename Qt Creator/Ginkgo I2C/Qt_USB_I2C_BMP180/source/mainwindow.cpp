#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

int DeviceIndex,ChannelIndex;

signed char BMP180_I2C_bus_read(unsigned char device_addr, unsigned char reg_addr, unsigned char *reg_data, unsigned char cnt)
{
    int iError=0;
    unsigned char array[I2C_BUFFER_LEN];
    unsigned char stringpos;
    iError = VII_ReadBytes(VII_USBI2C,
                           DeviceIndex,
                           ChannelIndex,
                           device_addr<<1,
                           reg_addr,
                           array,
                           cnt);
    //iError = I2C_write_read_string(I2C0, device_addr, array, array, 1, cnt);
    for(stringpos=0;stringpos<cnt;stringpos++)
        *(reg_data + stringpos) = array[stringpos];
    return (signed char)iError;
}

signed char BMP180_I2C_bus_write(unsigned char device_addr, unsigned char reg_addr, unsigned char *reg_data, unsigned char cnt)
{
    int iError=0;
    unsigned char array[I2C_BUFFER_LEN];
    unsigned char stringpos;
    for(stringpos=0;stringpos<cnt;stringpos++)
        array[stringpos] = *(reg_data + stringpos);
    iError = VII_WriteBytes(VII_USBI2C,
                            DeviceIndex,
                            ChannelIndex,
                            device_addr<<1,
                            reg_addr,
                            array,
                            cnt);
    //iError = I2C_write_string(I2C0, device_addr, array, cnt+1);
    return (signed char)iError;
}

void BMP180_delay_msek(unsigned int msek) //delay in milliseconds
{
    Sleep(msek);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pTemperatureThermo = new ThermoMeter();
    pTemperatureThermo->setMinimum(-40);
    pTemperatureThermo->setMaximum(100);
    pTemperatureThermo->setSuffix("[C]");
    pTemperatureThermo->setValueOffset(280);
    ui->temperatureGroupBox->layout()->addWidget(pTemperatureThermo);
    pPressure = new ManoMeter();
    pPressure->setMinimum(0);
    pPressure->setMaximum(200);
    pPressure->setSuffix("[Kpa]");
    pPressure->setCritical(150);
    ui->pressureGroupBox->layout()->addWidget(pPressure);

    //initialize parameter table
    ui->parameterTableWidget->setColumnWidth(0,140);
    ui->parameterTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    for(int i=0;i<ui->parameterTableWidget->rowCount();i++){
        ui->parameterTableWidget->setRowHeight(i,25);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}






bool MainWindow::BMP180_Init(void)
{
    int ret;
    VII_INIT_CONFIG I2C_Config;
    //scan device
    ret = VII_ScanDevice(1);
    if(ret <= 0){
        QMessageBox::warning(this,"Warning","No device connect!");
        return false;
    }
    //open device
    ret = VII_OpenDevice(VII_USBI2C,
                         ui->deviceIndexComboBox->currentIndex(),
                         ui->channelIndexComboBox->currentIndex());
    if (ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device error!");
        return false;
    }
    //初始化设备(硬件控制模式)
    I2C_Config.AddrType = VII_ADDR_7BIT;
    I2C_Config.ClockSpeed = 100000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_1BYTE;
    ret = VII_InitI2C(VII_USBI2C,
                      ui->deviceIndexComboBox->currentIndex(),
                      ui->channelIndexComboBox->currentIndex(),
                      &I2C_Config);
    if (ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Initialize device error!");
        return false;
    }
    bmp180.bus_read = BMP180_I2C_bus_read;   // assign read function
    bmp180.bus_write = BMP180_I2C_bus_write; // assign write function
    bmp180.delay_msec = BMP180_delay_msek;   // assign wait function
    bmp180.oversampling_setting = 3;
    DeviceIndex = ui->deviceIndexComboBox->currentIndex();
    ChannelIndex = ui->channelIndexComboBox->currentIndex();
    bmp180_init(&bmp180);

    //display parameters
    QString strTmp;
    strTmp.sprintf("%04X",bmp180.cal_param.ac1&0xFFFF);
    ui->parameterTableWidget->item(0,1)->setText(strTmp);
    strTmp.sprintf("%04X",bmp180.cal_param.ac2&0xFFFF);
    ui->parameterTableWidget->item(1,1)->setText(strTmp);
    strTmp.sprintf("%04X",bmp180.cal_param.ac3&0xFFFF);
    ui->parameterTableWidget->item(2,1)->setText(strTmp);
    strTmp.sprintf("%04X",bmp180.cal_param.ac4&0xFFFF);
    ui->parameterTableWidget->item(3,1)->setText(strTmp);
    strTmp.sprintf("%04X",bmp180.cal_param.ac5&0xFFFF);
    ui->parameterTableWidget->item(4,1)->setText(strTmp);
    strTmp.sprintf("%04X",bmp180.cal_param.ac6&0xFFFF);
    ui->parameterTableWidget->item(5,1)->setText(strTmp);
    strTmp.sprintf("%04X",bmp180.cal_param.b1&0xFFFF);
    ui->parameterTableWidget->item(6,1)->setText(strTmp);
    strTmp.sprintf("%04X",bmp180.cal_param.b2&0xFFFF);
    ui->parameterTableWidget->item(7,1)->setText(strTmp);
    strTmp.sprintf("%04X",bmp180.cal_param.mb&0xFFFF);
    ui->parameterTableWidget->item(8,1)->setText(strTmp);
    strTmp.sprintf("%04X",bmp180.cal_param.mc&0xFFFF);
    ui->parameterTableWidget->item(9,1)->setText(strTmp);
    strTmp.sprintf("%04X",bmp180.cal_param.md&0xFFFF);
    ui->parameterTableWidget->item(10,1)->setText(strTmp);
    strTmp.sprintf("%02X",bmp180.chip_id&0xFF);
    ui->parameterTableWidget->item(11,1)->setText(strTmp);
    return true;
}

void MainWindow::on_startPushButton_clicked()
{
    if(ui->startPushButton->text()=="Start"){
        if(BMP180_Init()){
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
    //定时采样数据并显示
    if(event->timerId()==mesureTimerID){
        double t = bmp180_get_temperature(bmp180_get_ut())/10.0;
        double p = bmp180_get_pressure(bmp180_get_up())/1000.0;
        double a = (44330.0 * (1.0-pow(p*1000 / 101325.0, 1.0/5.255)) );//Altitude
        qDebug()<<"Altitude = "<<a;
        qDebug()<<"Temperature = "<< t;
        qDebug()<<"Pressure = "<< p;
        pTemperatureThermo->setValue(t);
        pPressure->setValue(((int)(p*10))/10.0);

    }
}
