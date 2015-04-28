#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initFlag = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_initPushButton_clicked()
{
    bool ok;
    int ret;
    int TimeError = ui->TimeErrorLineEdit->text().toInt(&ok,0);
    ESTime.ESDelay = ui->ESDelayLineEdit->text().toInt(&ok,0);
    if(ESTime.ESDelay >= TimeError){
        ESTime.ESDelay -= TimeError;
    }else{
        ESTime.ESDelay = 0;
    }
    ESTime.ESDetect = ui->ESDetectLineEdit->text().toInt(&ok,0);
    if(ESTime.ESDetect >= TimeError){
        ESTime.ESDetect -= TimeError;
    }else{
        ESTime.ESDetect = 0;
    }
    ESTime.ESWin = ui->ESWinLineEdit->text().toInt(&ok,0);
    if(ESTime.ESWin >= TimeError){
        ESTime.ESWin -= TimeError;
    }else{
        ESTime.ESWin = 0;
    }
    ESTime.HighBit_THigh = ui->THighBitHighLineEdit->text().toInt(&ok,0);
    if(ESTime.HighBit_THigh >= TimeError){
        ESTime.HighBit_THigh -= TimeError;
    }else{
        ESTime.HighBit_THigh = 0;
    }
    ESTime.HighBit_TLow = ui->THighBitLowLineEdit->text().toInt(&ok,0);
    if(ESTime.HighBit_TLow >= TimeError){
        ESTime.HighBit_TLow -= TimeError;
    }else{
        ESTime.HighBit_TLow = 0;
    }
    ESTime.LowBit_THigh = ui->TLowBitHighLineEdit->text().toInt(&ok,0);
    if(ESTime.LowBit_THigh >= TimeError){
        ESTime.LowBit_THigh -= TimeError;
    }else{
        ESTime.LowBit_THigh = 0;
    }
    ESTime.LowBit_TLow = ui->TLowBitLowLineEdit->text().toInt(&ok,0);
    if(ESTime.LowBit_TLow >= TimeError){
        ESTime.LowBit_TLow -= TimeError;
    }else{
        ESTime.LowBit_TLow = 0;
    }
    ESTime.TACKN = ui->TACKNLineEdit->text().toInt(&ok,0);
    if(ESTime.TACKN >= TimeError){
        ESTime.TACKN -= TimeError;
    }else{
        ESTime.TACKN = 0;
    }
    ESTime.TEos = ui->TEosLineEdit->text().toInt(&ok,0);
    if(ESTime.TEos >= TimeError){
        ESTime.TEos -= TimeError;
    }else{
        ESTime.TEos = 0;
    }
    ESTime.TStart = ui->TStartLineEdit->text().toInt(&ok,0);
    if(ESTime.TStart >= TimeError){
        ESTime.TStart -= TimeError;
    }else{
        ESTime.TStart = 0;
    }
    ESTime.TValACK = ui->TValACKLineEdit->text().toInt(&ok,0);
    if(ESTime.TValACK >= TimeError){
        ESTime.TValACK -= TimeError;
    }else{
        ESTime.TValACK = 0;
    }
    ESTime.TShutDown = ui->TshutDownLineEdit->text().toInt(&ok,0);
    if(ESTime.TShutDown >= TimeError){
        ESTime.TShutDown -= TimeError;
    }else{
        ESTime.TShutDown = 0;
    }
    if(VEI_ScanDevice(1)<=0){
        initFlag = false;
        QMessageBox::warning(this, "Warning", "No device connect!", tr("Yes"));
        return;
    }
    ret = VEI_OpenDevice(0,ui->deviceIndexComboBox->currentIndex(),0);
    if(ret != 0){
        initFlag = false;
        QMessageBox::warning(this, "Warning", "Open device failed!", tr("Yes"));
        return;
    }
    USHORT Channel,InitData;
    Channel = ui->channelsLineEdit->text().toInt(&ok,16);
    InitData = ui->initDataLineEdit->text().toInt(&ok,16);
    ret = VEI_InitES(0,ui->deviceIndexComboBox->currentIndex(),Channel,InitData,&ESTime);
    if(ret != 0){
        initFlag = false;
        QMessageBox::warning(this, "Warning", "Initialize device failed!", tr("Yes"));
        return;
    }else{
        initFlag = true;
        QMessageBox::information(this, "Message", "Initialize device success!");
    }
}

void MainWindow::on_writeDataPushButton_clicked()
{
    bool ok;
    int ret;
    USHORT Channel = ui->dataChannelsLineEdit->text().toInt(&ok,16);
    ESData.Data = ui->dataLineEdit->text().toInt(&ok,0);
    ESData.DevAddr = ui->devAddrLineEdit->text().toInt(&ok,16);
    ESData.RFA = ui->RFALineEdit->text().toInt(&ok,0);
    ESData.SubAddr = ui->subAddrLineEdit->text().toInt(&ok,0);
    if(ui->enterEsModeCheckBox->isChecked()){
        ESData.ESMode = 1;
    }else{
        ESData.ESMode = 0;
    }
    if(initFlag){
        ret = VEI_Transmit(0,ui->deviceIndexComboBox->currentIndex(),Channel,&ESData,1);
        if(ret != 0){
            initFlag = false;
            QMessageBox::warning(this, "Warning", "Write data failed!", tr("Yes"));
            return;
        }else{
            QMessageBox::information(this, "Message", "Write data success!");
        }
    }else{
        QMessageBox::warning(this, "Warning", "don't Initialize device!", tr("Yes"));
        return;
    }
}
