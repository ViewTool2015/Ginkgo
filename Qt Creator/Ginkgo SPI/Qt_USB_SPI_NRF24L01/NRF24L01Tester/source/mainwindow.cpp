/**
  ******************************************************************************
  * @file    mainwindow.c
  * $Author: wdluo $
  * $Revision: 193 $
  * $Date:: 2013-06-21 14:39:39 +0800 #$
  * @brief   Ginkgo USB-SPI适配器读写控制NRF24L01芯片程序。
  ******************************************************************************
  * @attention
  *
  *<h3><center>&copy; Copyright 2009-2012, ViewTool</center>
  *<center><a href="http:\\www.viewtool.com">http://www.viewtool.com</a></center>
  *<center>All Rights Reserved</center></h3>
  *
  *产品购买地址:http://viewtool.taobao.com
  *资料下载地址:http://www.viewtool.com
  ******************************************************************************
  */
/**
 *硬件连接
 *	USB-SPI		NRF2401
 *	SPI_MISO	MISO	//SPI Slave Data Output, with tri-state option
 *	SPI_MOSI	MOSI	//SPI Slave Data Input
 *	SPI_SCK		SCK		//SPI Clock
 *	SPI_SEL0	CSN		//SPI Chip Select
 *	GPIO_0		IRQ		//Maskable interrupt pin(active low)
 *	GPIO_1		CE		//Chip Enable Activates RX(High) or TX(Low) mode
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ControlSPI.h"
#include "NRF24L01.h"
#include "ControlGPIO.h"
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QApplication::addLibraryPath("./plugins");
    QObject::connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    QObject::connect(ui->actionContact_us, SIGNAL(triggered()), this, SLOT(contactus()));
    QObject::connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(quit()));
    ui->actionStop->setDisabled(true);
    ui->spiBaudRateComboBox->setCurrentIndex(2);
    ui->RXPayloadWidthPomboBox->setCurrentIndex(7);
    ui->TXPayloadWidthPomboBox->setCurrentIndex(7);
    devConnectStateLabel = new QLabel("Not Connected");
    devConnectStateLabel->setStyleSheet("background-color:red");
    ui->statusBar->addWidget(devConnectStateLabel);
    RxCounterLabel = new QLabel("    RX : 000000    ");
    TxCounterLabel = new QLabel("    TX : 000000    ");
    ui->statusBar->addWidget(RxCounterLabel);
    ui->statusBar->addWidget(TxCounterLabel);
    TxCounter = 0;
    RxCounter = 0;
    scanDevTimer = startTimer(1000);
    initRegisterTable();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initRegisterTable(void)
{
    QTreeWidgetItem *pTopItem;
    QTreeWidgetItem *pChildItem;
    QString RegisterInitfilename = "RegisterInit.ini";
    QString tempStr,addressStr;
    QStringList tempList;
    bool addTopFlag = false;
    QFile RegisterInitefile(RegisterInitfilename);
    if (RegisterInitefile.open(QFile::ReadOnly | QFile::Text))
    {
        while(!RegisterInitefile.atEnd()){
            tempStr = RegisterInitefile.readLine();
            if((tempStr.length()>1)&&(tempStr.at(0)==QChar('0'))&&(tempStr.at(1)==QChar('x'))){
                tempStr.resize(tempStr.length()-1);
                tempStr.replace("\\n","\n");
                tempList = tempStr.split('|');
                QString str;
                //将寄存器信息添加到列表
                if(addressStr != str.sprintf("0x%02X",tempList.at(0).toInt(NULL,16))){
                    addTopFlag = false;
                }
                if(addTopFlag){
                    tempList.replace(0,"");
                    pChildItem = new QTreeWidgetItem(tempList);
                    pChildItem->setIcon(0,QIcon("./images/processor.png"));
                    pTopItem->addChild(pChildItem);
                }else{
                    str.sprintf("%02X",tempList.at(0).toInt(NULL,16));
                    tempList.replace(0,str);
                    pTopItem = new QTreeWidgetItem(tempList);
                    pTopItem->setIcon(0,QIcon("./images/chip.png"));
                    ui->registerTreeWidget->addTopLevelItem(pTopItem);
                    addTopFlag = true;
                    addressStr = str.sprintf("0x%02X",tempList.at(0).toInt(NULL,16));
                }
            }
        }
    }
    ui->registerTreeWidget->setColumnWidth(0,90);
    ui->registerTreeWidget->setColumnWidth(1,120);
    ui->registerTreeWidget->setColumnWidth(2,50);
    ui->registerTreeWidget->setColumnWidth(3,100);
    ui->registerTreeWidget->setColumnWidth(4,50);
    //Reset CE
    if(!initDevice()){
        return;
    }
    int32_t ret;
    uint32_t devIndex=ui->deviceIndexComboBox->currentIndex();
    ret = VGI_WriteDatas(VSI_USBSPI,devIndex,CE_PIN,0);//Reset CE
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Write CE Pin error!");
        return;
    }
    on_workingModeComboBox_currentIndexChanged("RX Mode");
}
bool MainWindow::initDevice(void)
{
    int ret;
    VSI_INIT_CONFIG	SPI_Init;
    uint32_t devIndex=ui->deviceIndexComboBox->currentIndex();
    //Check the device number of connections
    ret = VSI_ScanDevice();
    if(ret<=0){
        QMessageBox::warning(this,"Warning","Unconnected device!");
        return false;
    }
    //open Device
    ret = VSI_OpenDevice(VSI_USBSPI,devIndex,0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device error!");
        return false;
    }
    //Initialization SPI
    SPI_Init.ClockSpeed = ui->spiBaudRateComboBox->currentText().toInt(NULL,10);
    SPI_Init.ControlMode = 1;
    SPI_Init.CPHA = 0;
    SPI_Init.CPOL = 0;
    SPI_Init.LSBFirst = 0;
    SPI_Init.MasterMode = 1;
    SPI_Init.SelPolarity = 0;
    SPI_Init.TranBits = 8;
    ret = VSI_InitSPI(VSI_USBSPI,devIndex,&SPI_Init);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Initialization device error!");
        return false;
    }
    ret = VGI_InitGPIO(VSI_USBSPI,devIndex,VGI_GPIO_PIN0|VGI_GPIO_PIN1,0x04);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Initialization GPIO error!");
        return false;
    }
    return true;
}

void MainWindow::on_refreshRegisterPushButton_clicked()
{
    int ret,i;
    uint8_t	WriteDataTemp[256]={0};
    uint8_t	ReadDataTemp[256]={0};
    uint32_t devIndex=ui->deviceIndexComboBox->currentIndex();
    uint32_t spiIndex=ui->channelIndexComboBox->currentIndex();
    int readBytesNum;
    QStringList bytesNumStrList;
    if(!initDevice()){
        return;
    }
    for(i=0;i<ui->registerTreeWidget->topLevelItemCount();i++){
        if(ui->registerTreeWidget->topLevelItem(i)->text(2).isEmpty()){
            readBytesNum = 1;
        }else{
            bytesNumStrList = ui->registerTreeWidget->topLevelItem(i)->text(2).split(":");
            readBytesNum = (1+bytesNumStrList.at(0).toInt(NULL,0)-bytesNumStrList.at(1).toInt(NULL,0))/8;
        }

        WriteDataTemp[0] = i;
        ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,readBytesNum);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Get NRF24L01 register data error!");
        }else{
            disconnect(ui->registerTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(on_registerTreeWidget_itemChanged(QTreeWidgetItem *, int)));
            QString str;
            if(readBytesNum == 1){
                str.sprintf("%02X",ReadDataTemp[0]);
            }else{
                str.sprintf("%02X%02X%02X%02X%02X",ReadDataTemp[4],ReadDataTemp[3],ReadDataTemp[2],ReadDataTemp[1],ReadDataTemp[0]);
            }
            if(ui->registerTreeWidget->topLevelItem(i)->text(3)!=str){
                if(ui->registerTreeWidget->topLevelItem(i)->textColor(3)!=Qt::green){
                    ui->registerTreeWidget->topLevelItem(i)->setTextColor(3,Qt::green);
                }
                ui->registerTreeWidget->topLevelItem(i)->setText(3,str);
            }else{
                if(ui->registerTreeWidget->topLevelItem(i)->textColor(3)!=Qt::black){
                    ui->registerTreeWidget->topLevelItem(i)->setTextColor(3,Qt::black);
                }
            }
            for(int j=0;j<ui->registerTreeWidget->topLevelItem(i)->childCount();j++){
                QStringList childBitsNumStrList = ui->registerTreeWidget->topLevelItem(i)->child(j)->text(2).split(":");
                if(childBitsNumStrList.length()>1){
                    int bitShiftNum =  childBitsNumStrList.at(1).toInt(NULL,0);
                    int bitNum = 1 + childBitsNumStrList.at(0).toInt(NULL,0) - childBitsNumStrList.at(1).toInt(NULL,0);
                    QString bitStr;
                    if(bitNum>8){
                        bitStr.sprintf("%02X%02X%02X%02X%02X",ReadDataTemp[4],ReadDataTemp[3],ReadDataTemp[2],ReadDataTemp[1],ReadDataTemp[0]);
                    }else{
                        for(int k=0;k<bitNum;k++){
                            bitStr.insert(0,((ReadDataTemp[0]>>(bitShiftNum+k))&0x01)?"1":"0");
                        }
                    }
                    if(ui->registerTreeWidget->topLevelItem(i)->child(j)->text(3)!=bitStr){
                        if(ui->registerTreeWidget->topLevelItem(i)->child(j)->textColor(3)!=Qt::green){
                            ui->registerTreeWidget->topLevelItem(i)->child(j)->setTextColor(3,Qt::green);
                        }
                        ui->registerTreeWidget->topLevelItem(i)->child(j)->setText(3,bitStr);
                    }else{
                        if(ui->registerTreeWidget->topLevelItem(i)->child(j)->textColor(3)!=Qt::black){
                            ui->registerTreeWidget->topLevelItem(i)->child(j)->setTextColor(3,Qt::black);
                        }
                    }
                }else{
                    int bitShiftNum = ui->registerTreeWidget->topLevelItem(i)->child(j)->text(2).toInt(NULL,0);
                    if(ui->registerTreeWidget->topLevelItem(i)->child(j)->text(3).toInt(NULL,0)!=((ReadDataTemp[0]>>bitShiftNum)&0x01)){
                        if(ui->registerTreeWidget->topLevelItem(i)->child(j)->textColor(3)!=Qt::green){
                            ui->registerTreeWidget->topLevelItem(i)->child(j)->setTextColor(3,Qt::green);
                        }
                        QString strTmp;
                        strTmp.sprintf("%d",((ReadDataTemp[0]>>bitShiftNum)&0x01));
                        ui->registerTreeWidget->topLevelItem(i)->child(j)->setText(3,strTmp);
                    }else{
                        if(ui->registerTreeWidget->topLevelItem(i)->child(j)->textColor(3)!=Qt::black){
                            ui->registerTreeWidget->topLevelItem(i)->child(j)->setTextColor(3,Qt::black);
                        }
                    }
                }

            }
            connect(ui->registerTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(on_registerTreeWidget_itemChanged(QTreeWidgetItem *, int)));
        }
    }
}


void MainWindow::on_registerTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(column==3){
        if((item->text(4).contains('W'))&&(item->text(1)!="Reserved")){
            item->setFlags(Qt::ItemIsEditable|Qt::ItemIsEnabled);
            curentStr = item->text(3);
        }
    }else{
        item->setFlags(item->flags()&~(Qt::ItemIsEditable));
    }
}

void MainWindow::on_registerTreeWidget_itemChanged(QTreeWidgetItem *item, int column)
{
    int registerData=0;
    QString str("00000000");

    QTreeWidgetItem *parentItem = item->parent();
    if(parentItem == NULL){
        if(column==3){
            if(curentStr != item->text(3)){
                //这里需要做参数检查
                item->setTextColor(3,Qt::red);
            }
        }
        return;
    }

    if(column==3){
        QStringList bitLengthStrList = item->text(2).split(":");
        int bitLength;
        if(bitLengthStrList.length()>1){
            bitLength = 1 + bitLengthStrList.at(0).toInt(NULL,10)-bitLengthStrList.at(1).toInt(NULL,10);
        }else{
            bitLength = 1;
        }
        //判断是否包含非法字符
        for(int i=0;i<item->text(3).length();i++){
              if((item->text(3).at(i) != QChar('0'))&&(item->text(3).at(i) != QChar('1'))){
                if(bitLengthStrList.length()>1){
                    str = "00000000";
                    str.resize(bitLength);
                    item->setText(3,str);
                    //qDebug()<<"set "<<str;
                }else{
                    item->setText(3,"0");
                }
            }
        }
        //判断数据长度是否符合规定
        if(item->text(3).length()>bitLength){
            str = item->text(3);
            str.resize(bitLength);
            item->setText(3,str);
        }
        if(item->text(3).length()<bitLength){
            str = item->text(3).rightJustified(bitLength,'0');
            item->setText(3,str);
        }
        item->setTextColor(3,Qt::red);
        for(int i=0;i<parentItem->childCount();i++){
            if(parentItem->child(i)->text(2).length()>2){
                QStringList bitLengthStrList = parentItem->child(i)->text(2).split(":");
                registerData |= parentItem->child(i)->text(3).toInt(NULL,2)<<(bitLengthStrList.at(1).toInt(NULL,0));
            }else{
                registerData |= parentItem->child(i)->text(3).toInt(NULL,0)<<(parentItem->child(i)->text(2).toInt(NULL,10));
            }
        }
        QString str;
        str.sprintf("%02X",registerData);
        parentItem->setText(3,str);
        parentItem->setTextColor(3,Qt::red);
        qDebug()<<str;
    }
}

void MainWindow::on_updateRegisterPushButton_clicked()
{
    int ret,i,writeBytesNum;
    uint8_t	WriteDataTemp[10]={0};
    uint32_t devIndex=ui->deviceIndexComboBox->currentIndex();
    uint32_t spiIndex=ui->channelIndexComboBox->currentIndex();
    QStringList bytesNumStrList;
    if(!initDevice()){
        return;
    }
    disconnect(ui->registerTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(on_registerTreeWidget_itemChanged(QTreeWidgetItem *, int)));
    for(i=0;i<ui->registerTreeWidget->topLevelItemCount();i++){
        if(ui->registerTreeWidget->topLevelItem(i)->textColor(3)==Qt::red){
            if(ui->registerTreeWidget->topLevelItem(i)->text(2).isEmpty()){
                writeBytesNum = 1;
            }else{
                bytesNumStrList = ui->registerTreeWidget->topLevelItem(i)->text(2).split(":");
                writeBytesNum = (1+bytesNumStrList.at(0).toInt(NULL,0)-bytesNumStrList.at(1).toInt(NULL,0))/8;
            }
            //qDebug()<<"writeBytesNum = "<<writeBytesNum;
            WriteDataTemp[0] = WRITE_REG_NRF24L01 + i;
            if(writeBytesNum == 1){
                WriteDataTemp[1] = ui->registerTreeWidget->topLevelItem(i)->text(3).toInt(NULL,16);
            }else{
                QString str = ui->registerTreeWidget->topLevelItem(i)->text(3);
                if(str.length()%2){
                    str.insert(0,"0");
                }
                for(int k=0;(k<str.length())&&(k<10);k+=2){
                    WriteDataTemp[k/2+1] = str.mid(str.length()-k-2,2).toInt(NULL,16);
                }
            }
            ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,writeBytesNum+1);
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Update register data error!");
                return;
            }else{
                ui->registerTreeWidget->topLevelItem(i)->setTextColor(3,Qt::black);
                for(int j=0;j<ui->registerTreeWidget->topLevelItem(i)->childCount();j++){
                    if(ui->registerTreeWidget->topLevelItem(i)->child(j)->textColor(3)!=Qt::black){
                        ui->registerTreeWidget->topLevelItem(i)->child(j)->setTextColor(3,Qt::black);
                    }
                }
            }
        }
    }
    connect(ui->registerTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(on_registerTreeWidget_itemChanged(QTreeWidgetItem *, int)));
}

void MainWindow::about()
{
    QString AboutStr;
    AboutStr = "Ginkgo SPI Adapter nRF24L01Tester 1.0.0\n";
#if defined(LANG_CN)
    AboutStr.append("内江纬图虚拟仪器有限公司 2009-2013 版权所有\n");
    AboutStr.append("支持硬件：Ginkgo 2 USB-SPI Adapter");
    QMessageBox::about(this,"关于Ginkgo USB SPI Classic",AboutStr);
#else
    AboutStr.append("ViewTool Virtual Instrument 2009-2013 Copyright\n");
    AboutStr.append("Hardware Suuport:Ginkgo 2 USB-SPI Adapter");
    QMessageBox::about(this,"About Ginkgo USB SPI Classic",AboutStr);
#endif
}
void MainWindow::contactus()
{
    QString AboutStr;
#if defined(LANG_CN)
    AboutStr.append("官方网站<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>：<a href=\"http://www.viewtool.com\">www.viewtool.com</a><br>");
    AboutStr.append("官方论坛<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>：<a href=\"http://www.viewtool.com/bbs\">www.viewtool.com/bbs</a><br>");
    AboutStr.append("官方淘宝店<span style=\"font-size:9px;\">&nbsp;&nbsp;&nbsp;</span>：<a href=\"http://viewtool.taobao.com/\">viewtool.taobao.com</a><br>");
    AboutStr.append("技术支持QQ：1713047813<br>");
    AboutStr.append("产品咨询QQ：191431236");
    QMessageBox::about(this,"联系我们",AboutStr);
#else
    AboutStr.append("Official website<span style=\"font-size:12px;\">&nbsp;</span>:&nbsp;<a href=\"http://www.viewtool.com\">www.viewtool.com</a><br>");
    AboutStr.append("Official BBS<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>:&nbsp;<a href=\"http://www.viewtool.com/bbs\">www.viewtool.com/bbs</a><br>");
    AboutStr.append("Official Taobao<span style=\"font-size: 9px;\">&nbsp;</span>:&nbsp;<a href=\"http://viewtool.taobao.com/\">viewtool.taobao.com</a><br>");
    AboutStr.append("Technical Suuport QQ&nbsp;:&nbsp;1713047813<br>");
    AboutStr.append("Product Consultant QQ&nbsp;:&nbsp;191431236");
    QMessageBox::about(this,"Contact us",AboutStr);
#endif
}

void MainWindow::quit()
{
    this->close();
}

void MainWindow::on_actionStop_triggered()
{
    ui->actionStart->setEnabled(true);
    ui->actionStop->setDisabled(true);
    killTimer(getDataTimer);
}

void MainWindow::on_actionStart_triggered()
{
    ui->actionStart->setDisabled(true);
    ui->actionStop->setEnabled(true);
    getDataTimer = startTimer(10);
}

void MainWindow::timerEvent(QTimerEvent *Event)
{
    int ret,i;
    QString str;
    uint8_t	WriteDataTemp[256]={0};
    uint8_t	ReadDataTemp[256]={0};
    uint32_t devIndex=ui->deviceIndexComboBox->currentIndex();
    uint32_t spiIndex=ui->channelIndexComboBox->currentIndex();
    if(Event->timerId() == getDataTimer){
        if(!initDevice()){
            on_actionStop_triggered();
            return;
        }
        WriteDataTemp[0] = RF_CH;
        ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,1);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Get RF_CH register data error!");
            return;
        }else{
            str.sprintf("%d Mhz",2400+ReadDataTemp[0]);
            ui->RF_CH_Label->setText(str);
        }
        WriteDataTemp[0] = RF_SETUP;
        ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,1);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Get RF_SETUP register data error!");
            return;
        }else{
            int pwr = (ReadDataTemp[0]>>1)&0x03;
            switch(pwr){
            case 0:ui->RF_PWR_Label->setText("-18 dBm");break;
            case 1:ui->RF_PWR_Label->setText("-12 dBm");break;
            case 2:ui->RF_PWR_Label->setText("-6 dBm");break;
            case 3:ui->RF_PWR_Label->setText("0 dBm");break;
            }
            if((ReadDataTemp[0]>>3)&0x01){
                ui->RF_DR_Label->setText("2 Mbps");
            }else{
                ui->RF_DR_Label->setText("1 Mbps");
            }
        }
        WriteDataTemp[0] = TX_ADDR;
        ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,5);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Get TX_ADDR register data error!");
            return;
        }else{
            str.sprintf("%02X%02X%02X%02X%02X",ReadDataTemp[0],ReadDataTemp[1],ReadDataTemp[2],ReadDataTemp[3],ReadDataTemp[4]);
            ui->TX_ADDR_Label->setText(str);
        }
        WriteDataTemp[0] = RX_ADDR_P0;
        ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,5);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Get RX_ADDR_P0 register data error!");
            return;
        }else{
            str.sprintf("%02X%02X%02X%02X%02X",ReadDataTemp[0],ReadDataTemp[1],ReadDataTemp[2],ReadDataTemp[3],ReadDataTemp[4]);
            ui->RX_ADDR_P0_Label->setText(str);
        }
        WriteDataTemp[0] = RX_ADDR_P1;
        ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,5);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Get RX_ADDR_P1 register data error!");
            return;
        }else{
            str.sprintf("%02X%02X%02X%02X%02X",ReadDataTemp[0],ReadDataTemp[1],ReadDataTemp[2],ReadDataTemp[3],ReadDataTemp[4]);
            ui->RX_ADDR_P1_Label->setText(str);
        }
        for(i=0;i<4;i++){
            WriteDataTemp[0] = RX_ADDR_P2+i;
            ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,&ReadDataTemp[i],1);
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Get RX_ADDR_P25 register data error!");
                return;
            }
        }
        str.sprintf("%02X%02X%02X%02X",ReadDataTemp[0],ReadDataTemp[1],ReadDataTemp[2],ReadDataTemp[3]);
        ui->RX_ADDR_P25_Label->setText(str);
        uint16_t PinData;
        ret = VGI_ReadDatas(VSI_USBSPI,devIndex,IRQ_PIN,&PinData);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Get IRQ status error!");
            return;
        }else{
            if(PinData&0x01){
                ui->IRQ_label->setText("1");
            }else{
                ui->IRQ_label->setText("0");
            }
        }

        //读取状态寄存器看是否接收到了数据
        //检查是否还有多的数据
        WriteDataTemp[0] = FIFO_STATUS;
        ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,1);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Get FIFO_STATUS register data error!");
            return;
        }
        if(!(ReadDataTemp[0]&0x01)){
            WriteDataTemp[0] = RD_RX_PLOAD;
            ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,ui->RXPayloadWidthPomboBox->currentText().toInt(NULL,0));
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Read FIFO register data error!");
                return;
            }else{
                if(ui->receiveHexDisplayCheckBox->isChecked()){
                for(int i=0;i<ui->RXPayloadWidthPomboBox->currentText().toInt(NULL,0);i++){
                    str.sprintf("%02X ",ReadDataTemp[i]);
                    ui->receiveDataTextBrowser->moveCursor(QTextCursor::End);
                    ui->receiveDataTextBrowser->insertPlainText(str);
                }
                }else{
                    str = (char*)ReadDataTemp;
                    ui->receiveDataTextBrowser->insertPlainText(str);
                }
                RxCounter += ui->RXPayloadWidthPomboBox->currentText().toInt(NULL,0);
                str.sprintf("    RX : %06d",RxCounter);
                RxCounterLabel->setText(str);
                //清除标志
                WriteDataTemp[0] = WRITE_REG_NRF24L01 + STATUS;
                WriteDataTemp[1] = 0x40;
                ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
                if(ret != ERR_SUCCESS){
                    QMessageBox::warning(this,"Warning","Write STATUS register data error!");
                    return;
                }
            }
        }
    }
    if(Event->timerId() == autoSendTimer){
        on_sendDataPushButton_clicked();
    }
    if(Event->timerId() == scanDevTimer){
        if(VSI_ScanDevice()){
            devConnectStateLabel->setText("Connected    ");
            devConnectStateLabel->setStyleSheet("background-color:green");
        }else{
            devConnectStateLabel->setText("Not Connected");
            devConnectStateLabel->setStyleSheet("background-color:red");
        }
    }
}

void MainWindow::on_readRegisterPushButton_clicked()
{
    int ret;
    uint8_t	WriteDataTemp[10]={0};
    uint8_t	ReadDataTemp[10]={0};
    uint32_t devIndex=ui->deviceIndexComboBox->currentIndex();
    uint32_t spiIndex=ui->channelIndexComboBox->currentIndex();
    if(!initDevice()){
        return;
    }
    WriteDataTemp[0] = ui->registerAddrcomboBox->currentIndex();
    if((ui->registerAddrcomboBox->currentText()=="RX_ADDR_P0")||
       (ui->registerAddrcomboBox->currentText()=="RX_ADDR_P1")||
       (ui->registerAddrcomboBox->currentText()=="TX_ADDR")){
        ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,5);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Read register data error!");
            return;
        }else{
            QString str;
            str.sprintf("%02X%02X%02X%02X%02X",ReadDataTemp[4],ReadDataTemp[3],ReadDataTemp[2],ReadDataTemp[1],ReadDataTemp[0]);
            ui->registerDataLineEdit->setText(str);
        }
    }else{
        ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,1);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Read register data error!");
            return;
        }else{
            QString str;
            str.sprintf("%02X",ReadDataTemp[0]);
            ui->registerDataLineEdit->setText(str);
        }
    }
}

void MainWindow::on_writeRegisterPushButton_clicked()
{
    int ret;
    uint8_t	WriteDataTemp[10]={0};
    uint32_t devIndex=ui->deviceIndexComboBox->currentIndex();
    uint32_t spiIndex=ui->channelIndexComboBox->currentIndex();
    if(!initDevice()){
        return;
    }
    ret = VGI_WriteDatas(VSI_USBSPI,devIndex,CE_PIN,0);//Reset CE
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Write CE Pin error!");
        return;
    }
    WriteDataTemp[0] = ui->registerAddrcomboBox->currentIndex()+WRITE_REG_NRF24L01;
    if((ui->registerAddrcomboBox->currentText()=="RX_ADDR_P0")||
       (ui->registerAddrcomboBox->currentText()=="RX_ADDR_P1")||
       (ui->registerAddrcomboBox->currentText()=="TX_ADDR")){
        QString str = ui->registerDataLineEdit->text();
        if(str.length()%2){
            str.insert(0,"0");
        }
        for(int i=0;(i<str.length())&&(i<10);i+=2){
            WriteDataTemp[i/2+1] = str.mid(str.length()-i-2,2).toInt(NULL,16);
        }
        ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,6);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write register data error!");
            ret = VGI_WriteDatas(VSI_USBSPI,devIndex,CE_PIN,CE_PIN);//Set CE
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Write CE Pin error!");
                return;
            }
            return;
        }
    }else{
        WriteDataTemp[1] = ui->registerDataLineEdit->text().toInt(NULL,16);
        ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write register data error!");
            ret = VGI_WriteDatas(VSI_USBSPI,devIndex,CE_PIN,CE_PIN);//Set CE
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Write CE Pin error!");
                return;
            }
            return;
        }
    }
    ret = VGI_WriteDatas(VSI_USBSPI,devIndex,CE_PIN,CE_PIN);//Set CE
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Write CE Pin error!");
        return;
    }
}

void MainWindow::on_FLUSH_TX_PushButton_clicked()
{
    int ret;
    uint8_t	WriteDataTemp[2]={0};
    uint32_t devIndex=ui->deviceIndexComboBox->currentIndex();
    uint32_t spiIndex=ui->channelIndexComboBox->currentIndex();
    if(!initDevice()){
        return;
    }
    WriteDataTemp[0] = FLUSH_TX;
    ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Write FLUSH_TX data error!");
        return;
    }
}

void MainWindow::on_FLUSH_RX_PushButton_clicked()
{
    int ret;
    uint8_t	WriteDataTemp[2]={0};
    uint32_t devIndex=ui->deviceIndexComboBox->currentIndex();
    uint32_t spiIndex=ui->channelIndexComboBox->currentIndex();
    if(!initDevice()){
        return;
    }
    WriteDataTemp[0] = FLUSH_RX;
    ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Write FLUSH_TX data error!");
        return;
    }
}


void MainWindow::on_workingModeComboBox_currentIndexChanged(const QString &arg1)
{
    int ret;
    uint8_t     WriteDataTemp[2]={0};
    uint8_t     ReadDataTemp[2]={0};
    uint32_t    devIndex=ui->deviceIndexComboBox->currentIndex();
    uint32_t    spiIndex=ui->channelIndexComboBox->currentIndex();
    qDebug()<<arg1;
    if(arg1 == "RX Mode"){
        ui->autoSendDataCheckBox->setDisabled(true);
        ui->sendDataPushButton->setDisabled(true);
        ui->TXPayloadWidthPomboBox->setDisabled(true);
        ui->RXPayloadWidthPomboBox->setEnabled(true);
        if(!initDevice()){
            return;
        }
        ret = VGI_WriteDatas(VSI_USBSPI,devIndex,CE_PIN,0);//Reset CE
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write CE Pin error!");
            return;
        }
        //Write EN_AA
        WriteDataTemp[0] = WRITE_REG_NRF24L01 + EN_AA;
        WriteDataTemp[1] = 0x00;
        ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write EN_AA rigister error!");
            return;
        }
        //Write EN_RXADDR
        WriteDataTemp[0] = WRITE_REG_NRF24L01 + EN_RXADDR;
        WriteDataTemp[1] = (0x01<<ui->dataPipeComboBox->currentIndex());//PWR_UP,PRIM_RX
        ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write EN_RXADDR rigister error!");
            return;
        }
        //Write RX_PW_P0
        WriteDataTemp[0] = WRITE_REG_NRF24L01 + RX_PW_P0 + ui->dataPipeComboBox->currentText().toInt(NULL,0);
        WriteDataTemp[1] = ui->RXPayloadWidthPomboBox->currentText().toInt(NULL,0);
        ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write RX_PW_P rigister error!");
            return;
        }
        //Read CONFIG
        WriteDataTemp[0] = CONFIG;
        ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,1);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Read register data error!");
            return;
        }else{
            //Write CONFIG
            WriteDataTemp[0] = WRITE_REG_NRF24L01 + CONFIG;
            WriteDataTemp[1] = ReadDataTemp[0] | 0x7F;//PWR_UP,PRIM_RX
            ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Write CONFIG rigister error!");
                return;
            }
        }
        ret = VGI_WriteDatas(VSI_USBSPI,devIndex,CE_PIN,CE_PIN);//Set CE
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write CE Pin error!");
            return;
        }
    }
    if(arg1 == "TX Mode"){
        ui->autoSendDataCheckBox->setEnabled(true);
        ui->sendDataPushButton->setEnabled(true);
        ui->TXPayloadWidthPomboBox->setEnabled(true);
        ui->RXPayloadWidthPomboBox->setDisabled(true);
        if(!initDevice()){
            return;
        }
        ret = VGI_WriteDatas(VSI_USBSPI,devIndex,CE_PIN,0);//Reset CE
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write CE Pin error!");
            return;
        }
        //Write EN_AA
        WriteDataTemp[0] = WRITE_REG_NRF24L01 + EN_AA;
        WriteDataTemp[1] = 0x00;
        ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write EN_AA rigister error!");
            return;
        }
        //Write EN_RXADDR
        WriteDataTemp[0] = WRITE_REG_NRF24L01 + EN_RXADDR;
        WriteDataTemp[1] = 0x00;
        ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write EN_RXADDR rigister error!");
            return;
        }
        //Write SETUP_RETR
        WriteDataTemp[0] = WRITE_REG_NRF24L01 + SETUP_RETR;
        WriteDataTemp[1] = 0x00;
        ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write SETUP_RETR rigister error!");
            return;
        }
        //Write CONFIG
        WriteDataTemp[0] = WRITE_REG_NRF24L01 + CONFIG;
        WriteDataTemp[1] = 0x7E;
        ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write CONFIG rigister error!");
            return;
        }
        ret = VGI_WriteDatas(VSI_USBSPI,devIndex,CE_PIN,CE_PIN);//Set CE
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write CE Pin error!");
            return;
        }
    }
}

void MainWindow::on_sendDataPushButton_clicked()
{
    uint8_t     sendDataBuff[32]={0};
    uint8_t     WriteDataTemp[10]={0};
    uint8_t     ReadDataTemp[10]={0};
    int         ret,i,packNum=0,j;
    uint32_t    devIndex=ui->deviceIndexComboBox->currentIndex();
    uint32_t    spiIndex=ui->channelIndexComboBox->currentIndex();
    uint8_t     sendPayload = ui->TXPayloadWidthPomboBox->currentText().toInt(NULL,0);
    QString     sendDataStr = ui->sendDataTextEdit->toPlainText();
    QStringList sendDataStrList;
    QString     str;
    uint32_t    TimeOut = 0;
    if(ui->sendHexDisplayCheckBox->isChecked()){
        sendDataStrList = sendDataStr.split(" ",QString::SkipEmptyParts);
        packNum = (sendDataStrList.length()/sendPayload);
        TxCounter += sendDataStrList.length();
        str.sprintf("    TX : %06d",TxCounter);
        TxCounterLabel->setText(str);
        for(j=0;j<packNum;j++){
            for(i=0;(i<sendPayload);i++){
                sendDataBuff[i+1] = sendDataStrList.at(j*sendPayload+i).toInt(NULL,16);
            }
            sendDataBuff[0] = WR_TX_PLOAD;
            ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,sendDataBuff,1+i);
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Write WR_TX_PLOAD rigister error!");
                killTimer(autoSendTimer);
                return;
            }
            //读取STATUS寄存器，等待数据发送完毕
            TimeOut = 0;
            do{
                WriteDataTemp[0] = STATUS;
                ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,1);
                if(ret != ERR_SUCCESS){
                    QMessageBox::warning(this,"Warning","Read STATUS register data error!");
                    return;
                }
                TimeOut++;
                Sleep(10);
                if(TimeOut > 50){
                    QMessageBox::warning(this,"Warning","Read STATUS register time out!");
                    return;
                }
            }while(!(ReadDataTemp[0]&0x30));
            //清除标志
            WriteDataTemp[0] = WRITE_REG_NRF24L01 + STATUS;
            WriteDataTemp[1] = 0x30;
            ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Write STATUS rigister error!");
                return;
            }

        }
        if(sendDataStrList.length()%sendPayload){
            for(i=0;(i<(sendDataStrList.length()%sendPayload));i++){
                sendDataBuff[i+1] = sendDataStrList.at(j*sendPayload+i).toInt(NULL,16);
            }
            sendDataBuff[0] = WR_TX_PLOAD;
            ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,sendDataBuff,1+i);
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Write WR_TX_PLOAD rigister error!");
                killTimer(autoSendTimer);
                return;
            }
            //读取STATUS寄存器，等待数据发送完毕
            TimeOut = 0;
            do{
                WriteDataTemp[0] = STATUS;
                ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,1);
                if(ret != ERR_SUCCESS){
                    QMessageBox::warning(this,"Warning","Read STATUS register data error!");
                    return;
                }
                TimeOut++;
                Sleep(10);
                if(TimeOut > 50){
                    QMessageBox::warning(this,"Warning","Read STATUS register time out!");
                    return;
                }
            }while(!(ReadDataTemp[0]&0x20));
            //清除标志
            WriteDataTemp[0] = WRITE_REG_NRF24L01 + STATUS;
            WriteDataTemp[1] = 0x20;
            ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Write STATUS rigister error!");
                return;
            }
        }
    }else{
        packNum = (sendDataStr.length()/sendPayload);
        TxCounter += sendDataStr.length();
        str.sprintf("    TX : %06d",TxCounter);
        TxCounterLabel->setText(str);
        for(j=0;j<packNum;j++){
            for(i=0;(i<sendPayload);i++){
                sendDataBuff[i+1] = sendDataStr.at(j*sendPayload+i).cell();
            }
            sendDataBuff[0] = WR_TX_PLOAD;
            ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,sendDataBuff,1+i);
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Write WR_TX_PLOAD rigister error!");
                killTimer(autoSendTimer);
                return;
            }
            //读取STATUS寄存器，等待数据发送完毕
            TimeOut = 0;
            do{
                WriteDataTemp[0] = STATUS;
                ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,1);
                if(ret != ERR_SUCCESS){
                    QMessageBox::warning(this,"Warning","Read STATUS register data error!");
                    return;
                }
                TimeOut++;
                Sleep(10);
                if(TimeOut > 50){
                    QMessageBox::warning(this,"Warning","Read STATUS register time out!");
                    return;
                }
            }while(!(ReadDataTemp[0]&0x20));
            //清除标志
            WriteDataTemp[0] = WRITE_REG_NRF24L01 + STATUS;
            WriteDataTemp[1] = 0x20;
            ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Write STATUS rigister error!");
                return;
            }
        }
        if(sendDataStr.length()%sendPayload){
            for(i=0;(i<(sendDataStr.length()%sendPayload));i++){
                sendDataBuff[i+1] = sendDataStr.at(j*sendPayload+i).cell();
            }
            sendDataBuff[0] = WR_TX_PLOAD;
            ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,sendDataBuff,1+i);
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Write WR_TX_PLOAD rigister error!");
                killTimer(autoSendTimer);
                return;
            }
            //读取STATUS寄存器，等待数据发送完毕
            TimeOut = 0;
            do{
                WriteDataTemp[0] = STATUS;
                ret = VSI_WriteReadBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,1,ReadDataTemp,1);
                if(ret != ERR_SUCCESS){
                    QMessageBox::warning(this,"Warning","Read STATUS register data error!");
                    return;
                }
                TimeOut++;
                Sleep(10);
                if(TimeOut > 50){
                    QMessageBox::warning(this,"Warning","Read STATUS register time out!");
                    return;
                }
            }while(!(ReadDataTemp[0]&0x20));
            //清除标志
            WriteDataTemp[0] = WRITE_REG_NRF24L01 + STATUS;
            WriteDataTemp[1] = 0x20;
            ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
            if(ret != ERR_SUCCESS){
                QMessageBox::warning(this,"Warning","Write STATUS rigister error!");
                return;
            }
        }
    }
}

void MainWindow::on_registerAddrcomboBox_currentIndexChanged(const QString &arg1)
{
    if((arg1=="RX_ADDR_P0")||(arg1=="TX_ADDR")){
        ui->registerDataLineEdit->setText("E7E7E7E7E7");
    }else if(arg1=="RX_ADDR_P1"){
        ui->registerDataLineEdit->setText("C2C2C2C2C2");
    }else{
        ui->registerDataLineEdit->setText("00");
    }
}

void MainWindow::on_clearReceiveDataPushButton_clicked()
{
    ui->receiveDataTextBrowser->clear();
    RxCounter = 0;
    QString str;
    str.sprintf("    RX : %06d",RxCounter);
    RxCounterLabel->setText(str);
    TxCounter = 0;
    str.sprintf("    TX : %06d",TxCounter);
    TxCounterLabel->setText(str);
}

void MainWindow::on_clearSendDataPushButton_clicked()
{
    ui->sendDataTextEdit->clear();
}

void MainWindow::on_receiveHexDisplayCheckBox_clicked(bool checked)
{
    QString str,strTmp;
    if(checked){
        str = ui->receiveDataTextBrowser->toPlainText();
        ui->receiveDataTextBrowser->clear();
        for(int i=0;i<str.length();i++){
            strTmp.sprintf("%02X ",str.at(i).cell());
            ui->receiveDataTextBrowser->insertPlainText(strTmp);
        }
        displayMsg("Set hexadecimal display");
    }else{
        str = ui->receiveDataTextBrowser->toPlainText();
        QStringList strList = str.split(" ",QString::SkipEmptyParts);
        ui->receiveDataTextBrowser->clear();
        for(int i=0;i<strList.length();i++){
            char strData[2]={0};
            strData[0] = strList.at(i).toInt(NULL,16);
            ui->receiveDataTextBrowser->insertPlainText((char *)strData);
        }
        displayMsg("Set decimalism display");
    }
}

void MainWindow::on_sendHexDisplayCheckBox_clicked(bool checked)
{
    QString str,strTmp;
    if(checked){
        str = ui->sendDataTextEdit->toPlainText();
        ui->sendDataTextEdit->clear();
        for(int i=0;i<str.length();i++){
            strTmp.sprintf("%02X ",str.at(i).cell());
            ui->sendDataTextEdit->insertPlainText(strTmp);
        }
        displayMsg("Set hexadecimal display");
    }else{
        str = ui->sendDataTextEdit->toPlainText();
        QStringList strList = str.split(" ",QString::SkipEmptyParts);
        ui->sendDataTextEdit->clear();
        for(int i=0;i<strList.length();i++){
            char strData[2]={0};
            strData[0] = strList.at(i).toInt(NULL,16);
            ui->sendDataTextEdit->insertPlainText((char *)strData);
        }
        displayMsg("Set decimalism display");
    }
}

void MainWindow::on_RXPayloadWidthPomboBox_currentIndexChanged(const QString &arg1)
{
    int ret;
    uint8_t     WriteDataTemp[2]={0};
    uint32_t    devIndex=ui->deviceIndexComboBox->currentIndex();
    uint32_t    spiIndex=ui->channelIndexComboBox->currentIndex();
    if(!initDevice()){
        return;
    }
    ret = VGI_WriteDatas(VSI_USBSPI,devIndex,CE_PIN,0);//Reset CE
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Write CE Pin error!");
        return;
    }
    //Write RX_PW_P0
    WriteDataTemp[0] = WRITE_REG_NRF24L01 + RX_PW_P0 + ui->dataPipeComboBox->currentText().toInt(NULL,0);
    WriteDataTemp[1] = arg1.toInt(NULL,0);
    ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,2);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Write RX_PW_P rigister error!");
        ret = VGI_WriteDatas(VSI_USBSPI,devIndex,CE_PIN,CE_PIN);//Set CE
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Write CE Pin error!");
            return;
        }
        return;
    }
    ret = VGI_WriteDatas(VSI_USBSPI,devIndex,CE_PIN,CE_PIN);//Set CE
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Write CE Pin error!");
        return;
    }
}

void MainWindow::on_autoSendDataCheckBox_clicked(bool checked)
{
    if(checked){
        autoSendTimer = startTimer(ui->autoSendTimeLineEdit->text().toInt(NULL,0));
    }else{
        killTimer(autoSendTimer);
    }
}

void MainWindow::displayMsg(QString msg)
{
    ui->msgTextBrowser->moveCursor(QTextCursor::End);
    ui->msgTextBrowser->setTextColor(QColor(0,255,0));
    ui->msgTextBrowser->append(QTime::currentTime().toString("hh:mm:ss:zzz")+"[INFO]:");
    ui->msgTextBrowser->setTextColor(QColor(0,0,0));
    ui->msgTextBrowser->insertPlainText(msg);
    ui->msgTextBrowser->moveCursor(QTextCursor::End);
}

void MainWindow::on_writeAllRegisterPushButton_clicked()
{
    int ret,i,writeBytesNum;
    uint8_t	WriteDataTemp[10]={0};
    uint32_t devIndex=ui->deviceIndexComboBox->currentIndex();
    uint32_t spiIndex=ui->channelIndexComboBox->currentIndex();
    QStringList bytesNumStrList;
    if(!initDevice()){
        return;
    }
    disconnect(ui->registerTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(on_registerTreeWidget_itemChanged(QTreeWidgetItem *, int)));
    for(i=0;i<ui->registerTreeWidget->topLevelItemCount();i++){
        if(ui->registerTreeWidget->topLevelItem(i)->text(2).isEmpty()){
            writeBytesNum = 1;
        }else{
            bytesNumStrList = ui->registerTreeWidget->topLevelItem(i)->text(2).split(":");
            writeBytesNum = (1+bytesNumStrList.at(0).toInt(NULL,0)-bytesNumStrList.at(1).toInt(NULL,0))/8;
        }
        //qDebug()<<"writeBytesNum = "<<writeBytesNum;
        WriteDataTemp[0] = WRITE_REG_NRF24L01 + i;
        if(writeBytesNum == 1){
            WriteDataTemp[1] = ui->registerTreeWidget->topLevelItem(i)->text(3).toInt(NULL,16);
        }else{
            QString str = ui->registerTreeWidget->topLevelItem(i)->text(3);
            if(str.length()%2){
                str.insert(0,"0");
            }
            for(int k=0;(k<str.length())&&(k<10);k+=2){
                WriteDataTemp[k/2+1] = str.mid(str.length()-k-2,2).toInt(NULL,16);
            }
        }
        ret = VSI_WriteBytes(VSI_USBSPI,devIndex,spiIndex,WriteDataTemp,writeBytesNum+1);
        if(ret != ERR_SUCCESS){
            QMessageBox::warning(this,"Warning","Update register data error!");
            return;
        }else{
            ui->registerTreeWidget->topLevelItem(i)->setTextColor(3,Qt::black);
            for(int j=0;j<ui->registerTreeWidget->topLevelItem(i)->childCount();j++){
                if(ui->registerTreeWidget->topLevelItem(i)->child(j)->textColor(3)!=Qt::black){
                    ui->registerTreeWidget->topLevelItem(i)->child(j)->setTextColor(3,Qt::black);
                }
            }
        }
    }
    connect(ui->registerTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(on_registerTreeWidget_itemChanged(QTreeWidgetItem *, int)));

}

void MainWindow::on_actionNRF24L01_Datasheet_triggered()
{
    QDir dir;
    QString pathname;
    pathname = dir.currentPath()+"/nRF24L01.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(pathname));
}
