#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    pSlaveAddrPattern = new QRegExpValidator(QRegExp("[0-9A-Fa-f]{2}"),this);
    ui->startAddrLineEdit->setValidator(pSlaveAddrPattern);
    ui->endAddrLineEdit->setValidator(pSlaveAddrPattern);

    QLabel* label = new QLabel("<a href = http://www.viewtool.com>www.viewtool.com</a>",this);
    ui->statusBar->addWidget(label,0);
    connect(label,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
    QLabel* label2 = new QLabel("<a href = http://viewtool.taobao.com>viewtool.taobao.com</a>",this);
    ui->statusBar->addWidget(label2,1);
    connect(label2,SIGNAL(linkActivated(QString)),this,SLOT(openUrl(QString)));
}

void MainWindow::openUrl(QString url)
{
    QDesktopServices::openUrl(QUrl(url));
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_startPushButton_clicked()
{
    int ret;
    VII_INIT_CONFIG I2C_Config;
    uint8_t write_buffer[8]={0};
    //扫描已经连接的设备
    ret = VII_ScanDevice(1);
    if(ret <= 0){
        QMessageBox::warning(this,"Warning","No device connect!");
        return;
    }
    //打开设备
    ret = VII_OpenDevice(VII_USBI2C,
                         ui->deviceIndexComboBox->currentIndex(),
                         ui->channelIndexComboBox->currentIndex());
    if (ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device error!");
        return ;
    }
    //初始化设置I2C管脚
    ret = VGI_SetOpenDrain(VGI_USBGPIO,
                           ui->deviceIndexComboBox->currentIndex(),
                           ui->channelIndexComboBox->currentIndex()==0?VGI_GPIO_PIN1|VGI_GPIO_PIN2:VGI_GPIO_PIN5|VGI_GPIO_PIN6);
    if (ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Initialize GPIO error!");
        return ;
    }
    //设置I2C管脚为高电平
    ret = VGI_SetPins(VGI_USBGPIO,
                      ui->deviceIndexComboBox->currentIndex(),
                      ui->channelIndexComboBox->currentIndex()==0?VGI_GPIO_PIN1|VGI_GPIO_PIN2:VGI_GPIO_PIN5|VGI_GPIO_PIN6);
    if (ret != ERR_SUCCESS){
       QMessageBox::warning(this,"Warning","Set GPIO error!");
       return ;
    }
    //获取管脚状态
    uint16_t GPIO_Data;
    if(ui->channelIndexComboBox->currentIndex()==0){
        ret = VGI_ReadDatas(VGI_USBGPIO,ui->deviceIndexComboBox->currentIndex(),VGI_GPIO_PIN1|VGI_GPIO_PIN2,&GPIO_Data);
        if (ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Get GPIO  data error!");
            return ;
        }else{
            if((GPIO_Data&(VGI_GPIO_PIN1|VGI_GPIO_PIN2))==0){
                QMessageBox::warning(this,"Warning","SDA0 and SCL0 is low level\nIs Pull-up resistor connected?");
                return ;
            }else if((GPIO_Data&(VGI_GPIO_PIN1))==0){
                QMessageBox::warning(this,"Warning","SCL0 is low level\nIs Pull-up resistor connected?");
                return ;
            }else if((GPIO_Data&(VGI_GPIO_PIN2))==0){
                QMessageBox::warning(this,"Warning","SDA0 is low level\nIs Pull-up resistor connected?");
                return ;
            }
        }
    }else{
        ret = VGI_ReadDatas(VGI_USBGPIO,ui->deviceIndexComboBox->currentIndex(),VGI_GPIO_PIN5|VGI_GPIO_PIN6,&GPIO_Data);
        if (ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Get GPIO  data error!");
            return ;
        }else{
            if((GPIO_Data&(VGI_GPIO_PIN5|VGI_GPIO_PIN6))==0){
                QMessageBox::warning(this,"Warning","SDA1 and SCL1 is low level\nIs Pull-up resistor connected?");
                return ;
            }else if((GPIO_Data&(VGI_GPIO_PIN5))==0){
                QMessageBox::warning(this,"Warning","SCL1 is low level\nIs Pull-up resistor connected?");
                return ;
            }else if((GPIO_Data&(VGI_GPIO_PIN6))==0){
                QMessageBox::warning(this,"Warning","SDA1 is low level\nIs Pull-up resistor connected?");
                return ;
            }
        }
    }
    //初始化设备(硬件控制模式)
    I2C_Config.AddrType = VII_ADDR_7BIT;
    I2C_Config.ClockSpeed = 100000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_NONE;
    ret = VII_InitI2C(VII_USBI2C,
                      ui->deviceIndexComboBox->currentIndex(),
                      ui->channelIndexComboBox->currentIndex(),
                      &I2C_Config);
    if (ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Initialize device error!");
        return ;
    }
    uint16_t StartAddr = ui->startAddrLineEdit->text().toInt(NULL,16)&0xFE;
    uint16_t EndAddr = ui->endAddrLineEdit->text().toInt(NULL,16)&0xFE;
    uint16_t Addr = StartAddr;
    ui->msgTextBrowser->clear();
    while(1){
        if(Addr <= EndAddr){
            ret = VII_WriteBytes(VII_USBI2C,
                                 ui->deviceIndexComboBox->currentIndex(),
                                 ui->channelIndexComboBox->currentIndex(),
                                 Addr, 0x00, write_buffer, 0);
            if (ret != ERR_SUCCESS){
                QString str;
                str.sprintf("ADDR-%02X : ",Addr);
                ui->msgTextBrowser->moveCursor(QTextCursor::End);
                ui->msgTextBrowser->setTextColor(QColor(0,0,0));
                ui->msgTextBrowser->append(str);
                ui->msgTextBrowser->setTextColor(QColor(255,0,0));
                ui->msgTextBrowser->insertPlainText("NACK");
                Addr+=2;
                continue;
            }else{
                QString str;
                str.sprintf("ADDR-%02X : ",Addr);
                ui->msgTextBrowser->moveCursor(QTextCursor::End);
                ui->msgTextBrowser->setTextColor(QColor(0,0,0));
                ui->msgTextBrowser->append(str);
                ui->msgTextBrowser->setTextColor(QColor(0,255,0));
                ui->msgTextBrowser->insertPlainText("ACK");
                Addr+=2;
            }
            QApplication::processEvents();
        }else{
            break;
        }
    }
}

void MainWindow::on_clearPushButton_clicked()
{
    ui->msgTextBrowser->clear();
}
