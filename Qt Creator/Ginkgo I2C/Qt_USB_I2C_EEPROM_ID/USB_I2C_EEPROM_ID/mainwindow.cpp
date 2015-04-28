#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QApplication::addLibraryPath("./plugins");
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));
    QFile IDFile("data.txt");
    char saveData[200]={0};
    if (IDFile.open(QIODevice::ReadOnly)){
        IDFile.read(saveData,199);
        QString dataStr = saveData;
        QStringList dataStrList = dataStr.split("|");
        if(dataStrList.length()>=4){
            ui->IDLineEdit->setText(dataStrList.at(0));
            ui->IDAddrLineEdit->setText(dataStrList.at(1));
            ui->userStrLineEdit->setText(dataStrList.at(2));
            ui->strAddrLineEdit->setText(dataStrList.at(3));
        }
        IDFile.close();
    }
}

MainWindow::~MainWindow()
{
    //保存数据
    char saveData[200]={0};
    int i=0,j=0;
    for(i=0;i<ui->IDLineEdit->text().length();i++){
        saveData[j++] = ui->IDLineEdit->text().at(i).cell();
    }
    saveData[j++] = '|';
    for(i=0;i<ui->IDAddrLineEdit->text().length();i++){
        saveData[j++] = ui->IDAddrLineEdit->text().at(i).cell();
    }
    saveData[j++] = '|';
    for(i=0;i<ui->userStrLineEdit->text().length();i++){
        saveData[j++] = ui->userStrLineEdit->text().at(i).cell();
    }
    saveData[j++] = '|';
    for(i=0;i<ui->strAddrLineEdit->text().length();i++){
        saveData[j++] = ui->strAddrLineEdit->text().at(i).cell();
    }
    QFile IDFile("data.txt");
    if (IDFile.open(QIODevice::WriteOnly)){
        IDFile.resize(0);
        IDFile.write((char*)saveData,j);
        IDFile.close();
    }
    delete ui;
}

void MainWindow::on_writePushButton_clicked()
{
    VII_INIT_CONFIG I2C_InitConfig;
    //检测是否有设备连接
    if(VII_ScanDevice(1)<=0){
        QMessageBox::warning(this,"警告","无设备连接！");
        return;
    }
    //打开设备
    if(ERR_SUCCESS!=VII_OpenDevice(VII_USBI2C,ui->deviceIndexComboBox->currentIndex(),0)){
        QMessageBox::warning(this,"警告","打开设备失败！");
        return;
    }
    //初始化设备
    I2C_InitConfig.AddrType = VII_ADDR_7BIT;
    I2C_InitConfig.ClockSpeed = 100000;
    I2C_InitConfig.ControlMode = VII_HCTL_MODE;
    I2C_InitConfig.MasterMode = VII_MASTER;
    I2C_InitConfig.SubAddrWidth = VII_SUB_ADDR_1BYTE;
    if(ERR_SUCCESS!=VII_InitI2C(VII_USBI2C,ui->deviceIndexComboBox->currentIndex(),ui->channelIndexComboBox->currentIndex(),&I2C_InitConfig)){
        QMessageBox::warning(this,"警告","初始化设备失败！");
        return;
    }
    //写ID
    uint8_t ID[10]={0};
    uint8_t ID_Addr = ui->IDAddrLineEdit->text().toInt(NULL,16);
    QString IDStr = ui->IDLineEdit->text();
    for(int i=0;i<5;i++){
        ID[i] = IDStr.mid(i*2,2).toInt(NULL,16);
    }
    for(int i=0;i<10;i++){
        int ret;
        ret = VII_WriteBytes(VII_USBI2C,
                             ui->deviceIndexComboBox->currentIndex(),
                             ui->channelIndexComboBox->currentIndex(),
                             0xA0,
                             ID_Addr,
                             &ID[i],
                             1);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"警告","写数据失败！");
            return;
        }
        ID_Addr++;
        Sleep(5);
    }
    //写字符串
    uint8_t Str[100]={0};
    uint8_t Str_Addr = ui->strAddrLineEdit->text().toInt(NULL,16);
    QString userStr = ui->userStrLineEdit->text();
    for(int i=0;i<userStr.length();i++){
        Str[i] = userStr.at(i).cell();
    }
    for(int i=0;i<userStr.length();i++){
        int ret;
        ret = VII_WriteBytes(VII_USBI2C,
                             ui->deviceIndexComboBox->currentIndex(),
                             ui->channelIndexComboBox->currentIndex(),
                             0xA0,
                             Str_Addr,
                             &Str[i],
                             1);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"警告","写数据失败！");
            return;
        }
        Str_Addr++;
        Sleep(5);
    }
    //ID自动递增
    if(ui->IDAutoIndexCheckBox->isChecked()){
        quint64 IDValue = ui->IDLineEdit->text().toULongLong(NULL,16);
        IDValue++;
        if(IDValue>(quint64)0xFFFFFFFFFF){
            IDValue = 0;
        }
        QString IDStrLow,IDStrHigh;
        IDStrLow.sprintf("%08X",(quint32)IDValue);
        IDStrHigh.sprintf("%02X",IDValue>>32);
        ui->IDLineEdit->setText(IDStrHigh+IDStrLow);
    }
    QMessageBox::information(this,"消息","写数据成功！");
}

