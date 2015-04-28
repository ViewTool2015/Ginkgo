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
 *	USB-SPI		RC522
 *	SPI_MISO	MISO	//SPI Slave Data Output, with tri-state option
 *	SPI_MOSI	MOSI	//SPI Slave Data Input
 *	SPI_SCK		SCK		//SPI Clock
 *	SPI_SEL0	CSN		//SPI Chip Select
 *	GPIO_0		RST		//Reset Pin
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QApplication::addLibraryPath("./plugins");
    initRegisterTable();
    ui->actionStop->setDisabled(true);
    ui->baudrateComboBox->setCurrentIndex(2);
    mfrc522 = new MFRC522();
    stopFindCard = false;
    blockDataPattern = new QRegExpValidator(QRegExp("[0-9A-Fa-f]{32}"),this);
    ui->blockDataLineEdit->setValidator(blockDataPattern);
    keyPattern = new QRegExpValidator(QRegExp("[0-9A-Fa-f]{12}"),this);
    ui->keyALineEdit->setValidator(keyPattern);
    ui->keyBLineEdit->setValidator(keyPattern);
    ui->valueKeyALineEdit->setValidator(keyPattern);
    ui->valueKeyBLineEdit->setValidator(keyPattern);
    valuePattern = new QRegExpValidator(QRegExp("[0-9]{10}"),this);
    ui->valueLineEdit->setValidator(valuePattern);
    showChipImage(ui->graphicsView,QString("./images/Ginkgo USB-SPI.jpg"));
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
                    ui->rc522RegisterTreeWidget->addTopLevelItem(pTopItem);
                    addTopFlag = true;
                    addressStr = str.sprintf("0x%02X",tempList.at(0).toInt(NULL,16));
                }
            }
        }
    }
    ui->rc522RegisterTreeWidget->setColumnWidth(0,90);
    ui->rc522RegisterTreeWidget->setColumnWidth(1,120);
    ui->rc522RegisterTreeWidget->setColumnWidth(2,50);
    ui->rc522RegisterTreeWidget->setColumnWidth(3,100);
    ui->rc522RegisterTreeWidget->setColumnWidth(4,50);
}

/*void MainWindow::initCardDataTable(void)
{
    ui->cardDataTableWidget->setRowCount(64);
    ui->cardDataTableWidget->setColumnCount(4);
    QStringList rowHeadStrList;
    QString rowHeadStr;
    for(int i=0;i<64;i++){
        rowHeadStr.sprintf("Block %02d",i%4);
        rowHeadStrList << rowHeadStr;
    }
    ui->cardDataTableWidget->setVerticalHeaderLabels(rowHeadStrList);
    QStringList columnHeadStrList;
    columnHeadStrList << " "<<"Data"<<" "<<"Sector";
    ui->cardDataTableWidget->setHorizontalHeaderLabels(columnHeadStrList);
    for(int i=0;i<64;i+=4){
        ui->cardDataTableWidget->setSpan(i,3,4,1);
        for(int j=0;j<3;j++){
            ui->cardDataTableWidget->setSpan(i+j,0,1,3);
        }
    }
    for(int i=0;i<64;i++){
        ui->cardDataTableWidget->setRowHeight(i,20);
        if((i%4)==0){
            QTableWidgetItem* item = new QTableWidgetItem;
            item->setBackgroundColor(QColor(178,200,187));
            ui->cardDataTableWidget->setItem(i,0,item);
            item = new QTableWidgetItem;
            QString sectorStr;
            sectorStr.sprintf("Sector %02d",i/4);
            item->setText(sectorStr);
            item->setFlags((item->flags() & (~Qt::ItemIsEditable)));
            item->setBackgroundColor(QColor(114,83,52));
            ui->cardDataTableWidget->setItem(i,3,item);
        }else if(((i+1)%4)==0){
            for(int j=0;j<3;j++){
                QTableWidgetItem* item = new QTableWidgetItem;
                if(j==1){
                    item->setBackgroundColor(QColor(69,137,148));
                }else{
                    item->setBackgroundColor(QColor(117,121,71));
                }
                ui->cardDataTableWidget->setItem(i,j,item);
            }
        }else{
            QTableWidgetItem* item = new QTableWidgetItem;
            item->setBackgroundColor(QColor(178,200,187));
            ui->cardDataTableWidget->setItem(i,0,item);
        }
    }
}
*/
void MainWindow::on_actionContactus_triggered()
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

void MainWindow::on_actionAbout_triggered()
{
    QString AboutStr;
    AboutStr = "Ginkgo SPI Adapter RC522 Tester 1.0.0\n";
#if defined(LANG_CN)
    AboutStr.append("内江纬图虚拟仪器有限公司 2009-2013 版权所有\n");
    AboutStr.append("支持硬件：Ginkgo 2 USB-SPI Adapter");
    QMessageBox::about(this,"关于Ginkgo USB SPI RC522",AboutStr);
#else
    AboutStr.append("ViewTool Virtual Instrument 2009-2013 Copyright\n");
    AboutStr.append("Hardware Suuport:Ginkgo 2 USB-SPI Adapter");
    QMessageBox::about(this,"About Ginkgo USB SPI RC522",AboutStr);
#endif
}

void MainWindow::on_actionRC522_Datasheet_triggered()
{
    QDir dir;
    QString pathname;
    pathname = dir.currentPath()+"/mf_rc522_ds.pdf";
    QDesktopServices::openUrl(QUrl::fromLocalFile(pathname));
}


void MainWindow::on_rc522RegisterTreeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
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

void MainWindow::on_rc522RegisterTreeWidget_itemChanged(QTreeWidgetItem *item, int column)
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

bool MainWindow::initDevice(void)
{
    int ret;
    VSI_INIT_CONFIG	SPI_Init;
    mfrc522->devIndex = ui->deviceIndexComboBox->currentIndex();
    mfrc522->spiIndex = ui->channelIndexComboBox->currentIndex();
    mfrc522->rstIndex = ui->resetPinComboBox->currentIndex();
    //Check the device number of connections
    ret = VSI_ScanDevice();
    if(ret<=0){
        QMessageBox::warning(this,"Warning","Unconnected device!");
        return false;
    }
    //open Device
    ret = VSI_OpenDevice(VSI_USBSPI,mfrc522->devIndex,0);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Open device error!");
        return false;
    }
    //Initialization SPI
    SPI_Init.ClockSpeed = ui->baudrateComboBox->currentText().toInt(NULL,10);
    SPI_Init.ControlMode = 1;
    SPI_Init.CPHA = 0;
    SPI_Init.CPOL = 0;
    SPI_Init.LSBFirst = 0;
    SPI_Init.MasterMode = 1;
    SPI_Init.SelPolarity = 0;
    SPI_Init.TranBits = 8;
    ret = VSI_InitSPI(VSI_USBSPI,mfrc522->devIndex,&SPI_Init);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Initialization device error!");
        return false;
    }
    //qDebug()<<"SPI_Init.ClockSpeed = "<<SPI_Init.ClockSpeed;
    ret = VGI_SetOutput(VSI_USBSPI,mfrc522->devIndex,1<<mfrc522->rstIndex);
    if(ret != ERR_SUCCESS){
        QMessageBox::warning(this,"Warning","Initialization GPIO error!");
        return false;
    }
    return true;
}

void MainWindow::on_actionStart_triggered()
{
    static uint8_t buffer[20];
    uint8_t status;
    QString str;
    if(initDevice()){
        ui->cardInfoTextBrowser->clear();
        mfrc522->PcdReset();        //复位
        mfrc522->PcdAntennaOff();   //关闭天线
        Sleep(10);
        mfrc522->PcdAntennaOn();    //开启天线
        //mfrc522->M500PcdConfigISOType('A');
        //readCardInfoTimer = startTimer(50);
        ui->actionStart->setDisabled(true);
        ui->actionStop->setEnabled(true);
        stopFindCard = false;
        while(!stopFindCard){
            QCoreApplication::processEvents(QEventLoop::AllEvents);
            Sleep(10);
            status = mfrc522->PcdRequest(PICC_REQALL,buffer);//寻卡
            if(status != MI_OK){
                continue;
            }
            uint16_t cardType = (buffer[0]<<8)|buffer[1];
            str.sprintf("Card Type(0x%04X):",cardType);
            switch(cardType){
            case 0x4400:
                ui->cardInfoTextBrowser->moveCursor(QTextCursor::End);
                ui->cardInfoTextBrowser->append(str+"Mifare UltraLight");
                break;
            case 0x0400:
                ui->cardInfoTextBrowser->moveCursor(QTextCursor::End);
                ui->cardInfoTextBrowser->append(str+"Mifare One(S50)");
                break;
            case 0x0200:
                ui->cardInfoTextBrowser->moveCursor(QTextCursor::End);
                ui->cardInfoTextBrowser->append(str+"Mifare One(S70)");
                break;
            case 0x0800:
                ui->cardInfoTextBrowser->moveCursor(QTextCursor::End);
                ui->cardInfoTextBrowser->append(str+"Mifare Pro(X)");
                break;
            case 0x4403:
                ui->cardInfoTextBrowser->moveCursor(QTextCursor::End);
                ui->cardInfoTextBrowser->append(str+"Mifare DESFire");
                break;
            default:
                ui->cardInfoTextBrowser->moveCursor(QTextCursor::End);
                ui->cardInfoTextBrowser->append(str+"Unknown Card");
                continue;
            }

            status = mfrc522->PcdAnticoll(buffer);//防冲撞
            if(status != MI_OK){
                ui->cardInfoTextBrowser->moveCursor(QTextCursor::End);
                ui->cardInfoTextBrowser->append("Anticoll Error");
                continue;
            }else{
                str.sprintf("Serial Number:%02X%02X%02X%02X",buffer[0],buffer[1],buffer[2],buffer[3]);
                ui->cardInfoTextBrowser->moveCursor(QTextCursor::End);
                ui->cardInfoTextBrowser->append(str);
            }
            mfrc522->PcdHalt();
            continue;
        }

    }
}

void MainWindow::timerEvent(QTimerEvent *Event)
{
    if(Event->timerId() == readCardInfoTimer){

    }
}

void MainWindow::on_actionStop_triggered()
{
    //killTimer(readCardInfoTimer);
    ui->actionStop->setDisabled(true);
    ui->actionStart->setEnabled(true);
    stopFindCard = true;
}

void MainWindow::on_registerReadPushButton_clicked()
{
    int i;
    uint8_t	ReadDataTemp[256]={0};
    uint8_t Adrress;
    uint8_t RegisterData;
    int readBytesNum;
    QStringList bytesNumStrList;
    if(!initDevice()){
        return;
    }
    for(i=0;i<ui->rc522RegisterTreeWidget->topLevelItemCount();i++){
        if(ui->rc522RegisterTreeWidget->topLevelItem(i)->text(2).isEmpty()){
            readBytesNum = 1;
        }else{
            bytesNumStrList = ui->rc522RegisterTreeWidget->topLevelItem(i)->text(2).split(":");
            readBytesNum = (1+bytesNumStrList.at(0).toInt(NULL,0)-bytesNumStrList.at(1).toInt(NULL,0))/8;
        }
        Adrress = ui->rc522RegisterTreeWidget->topLevelItem(i)->text(0).toInt(NULL,16);
        RegisterData = mfrc522->ReadRawRC(Adrress);
        {
            disconnect(ui->rc522RegisterTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(on_rc522RegisterTreeWidget_itemChanged(QTreeWidgetItem *, int)));
            QString str;
            if(readBytesNum == 1){
                str.sprintf("%02X",RegisterData);
            }
            if(ui->rc522RegisterTreeWidget->topLevelItem(i)->text(3)!=str){
                if(ui->rc522RegisterTreeWidget->topLevelItem(i)->textColor(3)!=Qt::green){
                    ui->rc522RegisterTreeWidget->topLevelItem(i)->setTextColor(3,Qt::green);
                }
                ui->rc522RegisterTreeWidget->topLevelItem(i)->setText(3,str);
            }else{
                if(ui->rc522RegisterTreeWidget->topLevelItem(i)->textColor(3)!=Qt::black){
                    ui->rc522RegisterTreeWidget->topLevelItem(i)->setTextColor(3,Qt::black);
                }
            }
            for(int j=0;j<ui->rc522RegisterTreeWidget->topLevelItem(i)->childCount();j++){
                QStringList childBitsNumStrList = ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->text(2).split(":");
                if(childBitsNumStrList.length()>1){
                    int bitShiftNum =  childBitsNumStrList.at(1).toInt(NULL,0);
                    int bitNum = 1 + childBitsNumStrList.at(0).toInt(NULL,0) - childBitsNumStrList.at(1).toInt(NULL,0);
                    QString bitStr;
                    if(bitNum>8){
                        //bitStr.sprintf("%02X%02X%02X%02X%02X",ReadDataTemp[4],ReadDataTemp[3],ReadDataTemp[2],ReadDataTemp[1],ReadDataTemp[0]);
                    }else{
                        for(int k=0;k<bitNum;k++){
                            bitStr.insert(0,((ReadDataTemp[0]>>(bitShiftNum+k))&0x01)?"1":"0");
                        }
                    }
                    if(ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->text(3)!=bitStr){
                        if(ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->textColor(3)!=Qt::green){
                            ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->setTextColor(3,Qt::green);
                        }
                        ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->setText(3,bitStr);
                    }else{
                        if(ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->textColor(3)!=Qt::black){
                            ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->setTextColor(3,Qt::black);
                        }
                    }
                }else{
                    int bitShiftNum = ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->text(2).toInt(NULL,0);
                    if(ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->text(3).toInt(NULL,0)!=((ReadDataTemp[0]>>bitShiftNum)&0x01)){
                        if(ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->textColor(3)!=Qt::green){
                            ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->setTextColor(3,Qt::green);
                        }
                        QString strTmp;
                        strTmp.sprintf("%d",((ReadDataTemp[0]>>bitShiftNum)&0x01));
                        ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->setText(3,strTmp);
                    }else{
                        if(ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->textColor(3)!=Qt::black){
                            ui->rc522RegisterTreeWidget->topLevelItem(i)->child(j)->setTextColor(3,Qt::black);
                        }
                    }
                }

            }
            connect(ui->rc522RegisterTreeWidget,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(on_rc522RegisterTreeWidget_itemChanged(QTreeWidgetItem *, int)));
        }
    }
}

void MainWindow::on_readSectorPushButton_clicked()
{
    uint8_t buffer[20];
    uint8_t Card_KEYA[6] = {0};   //密码
    uint8_t Card_KEYB[6] = {0};   //密码
    uint8_t Card_Data[16];
    uint8_t status;
    uint8_t addr = ui->sectorIndexComboBox->currentIndex()*4+ui->blockComboBox->currentIndex();
    QString str;
    if(initDevice()){
        mfrc522->PcdReset();        //复位
        mfrc522->PcdAntennaOff();   //关闭天线
        Sleep(10);
        mfrc522->PcdAntennaOn();    //开启天线
        status = mfrc522->PcdRequest(PICC_REQALL,buffer);//寻卡
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","No Card!");
            return;
        }
        status = mfrc522->PcdAnticoll(buffer);//防冲撞
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Anticoll Error!");
            return;
        }
        status = mfrc522->PcdSelect(buffer);//选定卡片
        if (status != MI_OK){
            QMessageBox::warning(this,"Warning","Select Card Error!");
            return;
        }

        if(ui->keyComboBox->currentText()=="KEYA"){
            if(ui->keyALineEdit->text().length()<12){
                QMessageBox::warning(this,"Warning","Key data is illegal!");
                return;
            }else{
                for(int i=0;i<6;i++){
                    Card_KEYA[i] = ui->keyALineEdit->text().mid(i*2,2).toInt(NULL,16);
                }
            }
            status = mfrc522->PcdAuthState(PICC_AUTHENT1A,addr,Card_KEYA,buffer);
        }else{
            if(ui->keyBLineEdit->text().length()<12){
                QMessageBox::warning(this,"Warning","Key data is illegal!");
                return;
            }else{
                for(int i=0;i<6;i++){
                    Card_KEYB[i] = ui->keyBLineEdit->text().mid(i*2,2).toInt(NULL,16);
                }
            }
            status = mfrc522->PcdAuthState(PICC_AUTHENT1B,addr,Card_KEYB,buffer);
        }
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Auth State Error!");
            return;
        }
        //Read Block 0
        status = mfrc522->PcdRead(addr,Card_Data);
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Read Data Error!");
            return;
        }
        str.clear();
        for(int i=0;i<16;i++){
            QString strTmp;
            strTmp.sprintf("%02X",Card_Data[i]);
            str += strTmp;
        }
        ui->blockDataLineEdit->setText(str);
        mfrc522->PcdHalt();
    }
}

void MainWindow::on_writeSectorPushButton_clicked()
{
    uint8_t buffer[20];
    uint8_t Card_KEYA[6] = {0};   //密码
    uint8_t Card_KEYB[6] = {0};   //密码
    uint8_t Card_Data[16];
    uint8_t status;
    uint8_t addr = ui->sectorIndexComboBox->currentIndex()*4+ui->blockComboBox->currentIndex();
    if(initDevice()){
        mfrc522->PcdReset();        //复位
        mfrc522->PcdAntennaOff();   //关闭天线
        Sleep(10);
        mfrc522->PcdAntennaOn();    //开启天线
        status = mfrc522->PcdRequest(PICC_REQALL,buffer);//寻卡
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","No Card!");
            return;
        }
        status = mfrc522->PcdAnticoll(buffer);//防冲撞
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Anticoll Error!");
            return;
        }
        status = mfrc522->PcdSelect(buffer);//选定卡片
        if (status != MI_OK){
            QMessageBox::warning(this,"Warning","Select Card Error!");
            return;
        }
        if(ui->keyComboBox->currentText()=="KEYA"){
            if(ui->keyALineEdit->text().length()<12){
                QMessageBox::warning(this,"Warning","Key data is illegal!");
                return;
            }else{
                for(int i=0;i<6;i++){
                    Card_KEYA[i] = ui->keyALineEdit->text().mid(i*2,2).toInt(NULL,16);
                }
            }
            status = mfrc522->PcdAuthState(PICC_AUTHENT1A,addr,Card_KEYA,buffer);
        }else{
            if(ui->keyBLineEdit->text().length()<12){
                QMessageBox::warning(this,"Warning","Key data is illegal!");
                return;
            }else{
                for(int i=0;i<6;i++){
                    Card_KEYB[i] = ui->keyBLineEdit->text().mid(i*2,2).toInt(NULL,16);
                }
            }
            status = mfrc522->PcdAuthState(PICC_AUTHENT1B,addr,Card_KEYB,buffer);
        }
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Auth State Error!");
            return;
        }
        //Write Block 0
        if(addr > 0){
            if(ui->blockDataLineEdit->text().length()<32){
                QMessageBox::warning(this,"Warning","Block data is illegal!");
                return;
            }
            for(int i=0;i<16;i++){
                Card_Data[i] = ui->blockDataLineEdit->text().mid(i*2,2).toInt(NULL,16);
            }
            status = mfrc522->PcdWrite(addr,Card_Data);
            if(status != MI_OK){
                QMessageBox::warning(this,"Warning","Write Data Error!");
                return;
            }
        }
        mfrc522->PcdHalt();
    }
}


void MainWindow::on_sectorIndexComboBox_currentIndexChanged(int index)
{
    if((index==0)&&((ui->blockComboBox->currentIndex()==0))){
        ui->writeSectorPushButton->setDisabled(true);
    }else{
        ui->writeSectorPushButton->setEnabled(true);
    }
}

void MainWindow::on_blockComboBox_currentIndexChanged(int index)
{
    if((index==0)&&((ui->sectorIndexComboBox->currentIndex()==0))){
        ui->writeSectorPushButton->setDisabled(true);
    }else{
        ui->writeSectorPushButton->setEnabled(true);
    }
}


void MainWindow::on_valueSectorIndexComboBox_currentIndexChanged(int index)
{
    if((index==0)&&((ui->valueBlockComboBox->currentIndex()==0))){
        ui->incrementPushButton->setDisabled(true);
        ui->decrementPushButton->setDisabled(true);
        ui->setValuePushButton->setDisabled(true);
        ui->getValuePushButton->setDisabled(true);
    }else{
        ui->incrementPushButton->setEnabled(true);
        ui->decrementPushButton->setEnabled(true);
        ui->setValuePushButton->setEnabled(true);
        ui->setValuePushButton->setEnabled(true);
    }
}

void MainWindow::on_valueBlockComboBox_currentIndexChanged(int index)
{
    if((index==0)&&((ui->valueSectorIndexComboBox->currentIndex()==0))){
        ui->incrementPushButton->setDisabled(true);
        ui->decrementPushButton->setDisabled(true);
        ui->setValuePushButton->setDisabled(true);
        ui->getValuePushButton->setDisabled(true);
    }else{
        ui->incrementPushButton->setEnabled(true);
        ui->decrementPushButton->setEnabled(true);
        ui->setValuePushButton->setEnabled(true);
        ui->setValuePushButton->setEnabled(true);
    }
}

void MainWindow::showChipImage(QGraphicsView *graphicViewer, QString imageName)
{
    QImage *chipImage = new QImage(imageName);
    QGraphicsScene *scene = new QGraphicsScene;
    scene->addPixmap(QPixmap::fromImage(*chipImage));
    graphicViewer->setScene(scene);
    graphicViewer->show();
}

void MainWindow::on_incrementPushButton_clicked()
{
    uint8_t buffer[20];
    uint8_t Card_KEYA[6] = {0};   //密码
    uint8_t Card_KEYB[6] = {0};   //密码
    uint8_t Card_Data[16];
    uint8_t status;
    uint8_t addr = ui->valueSectorIndexComboBox->currentIndex()*4+ui->valueBlockComboBox->currentIndex();
    if(initDevice()){
        mfrc522->PcdReset();        //复位
        mfrc522->PcdAntennaOff();   //关闭天线
        Sleep(10);
        mfrc522->PcdAntennaOn();    //开启天线
        status = mfrc522->PcdRequest(PICC_REQALL,buffer);//寻卡
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","No Card!");
            return;
        }
        status = mfrc522->PcdAnticoll(buffer);//防冲撞
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Anticoll Error!");
            return;
        }
        status = mfrc522->PcdSelect(buffer);//选定卡片
        if (status != MI_OK){
            QMessageBox::warning(this,"Warning","Select Card Error!");
            return;
        }
        if(ui->valueKeyComboBox->currentText()=="KEYA"){
            if(ui->valueKeyALineEdit->text().length()<12){
                QMessageBox::warning(this,"Warning","Key data is illegal!");
                return;
            }else{
                for(int i=0;i<6;i++){
                    Card_KEYA[i] = ui->valueKeyALineEdit->text().mid(i*2,2).toInt(NULL,16);
                }
            }
            status = mfrc522->PcdAuthState(PICC_AUTHENT1A,addr,Card_KEYA,buffer);
        }else{
            if(ui->valueKeyBLineEdit->text().length()<12){
                QMessageBox::warning(this,"Warning","Key data is illegal!");
                return;
            }else{
                for(int i=0;i<6;i++){
                    Card_KEYB[i] = ui->valueKeyBLineEdit->text().mid(i*2,2).toInt(NULL,16);
                }
            }
            status = mfrc522->PcdAuthState(PICC_AUTHENT1B,addr,Card_KEYB,buffer);
        }
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Auth State Error!");
            return;
        }
        //Increment
        if(addr > 0){
            if(ui->valueLineEdit->text().length()<=0){
                QMessageBox::warning(this,"Warning","Value data is illegal!");
                return;
            }
            int value = ui->valueLineEdit->text().toInt(NULL,10);
            Card_Data[0] = value&0xFF;
            Card_Data[1] = (value>>8)&0xFF;
            Card_Data[2] = (value>>16)&0xFF;
            Card_Data[3] = (value>>24)&0xFF;
            status = mfrc522->PcdValue(PICC_INCREMENT,addr,Card_Data);
            if(status != MI_OK){
                qDebug()<<status;
                QMessageBox::warning(this,"Warning","Increment Value Error!");
                return;
            }
        }
        mfrc522->PcdHalt();
    }
}

void MainWindow::on_getValuePushButton_clicked()
{
    uint8_t buffer[20];
    uint8_t Card_KEYA[6] = {0};   //密码
    uint8_t Card_KEYB[6] = {0};   //密码
    uint8_t Card_Data[16];
    uint8_t status;
    uint8_t addr = ui->valueSectorIndexComboBox->currentIndex()*4+ui->valueBlockComboBox->currentIndex();
    if(initDevice()){
        mfrc522->PcdReset();        //复位
        mfrc522->PcdAntennaOff();   //关闭天线
        Sleep(10);
        mfrc522->PcdAntennaOn();    //开启天线
        status = mfrc522->PcdRequest(PICC_REQALL,buffer);//寻卡
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","No Card!");
            return;
        }
        status = mfrc522->PcdAnticoll(buffer);//防冲撞
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Anticoll Error!");
            return;
        }
        status = mfrc522->PcdSelect(buffer);//选定卡片
        if (status != MI_OK){
            QMessageBox::warning(this,"Warning","Select Card Error!");
            return;
        }
        if(ui->valueKeyComboBox->currentText()=="KEYA"){
            if(ui->valueKeyALineEdit->text().length()<12){
                QMessageBox::warning(this,"Warning","Key data is illegal!");
                return;
            }else{
                for(int i=0;i<6;i++){
                    Card_KEYA[i] = ui->valueKeyALineEdit->text().mid(i*2,2).toInt(NULL,16);
                }
            }
            status = mfrc522->PcdAuthState(PICC_AUTHENT1A,addr,Card_KEYA,buffer);
        }else{
            if(ui->valueKeyBLineEdit->text().length()<12){
                QMessageBox::warning(this,"Warning","Key data is illegal!");
                return;
            }else{
                for(int i=0;i<6;i++){
                    Card_KEYB[i] = ui->valueKeyBLineEdit->text().mid(i*2,2).toInt(NULL,16);
                }
            }
            status = mfrc522->PcdAuthState(PICC_AUTHENT1B,addr,Card_KEYB,buffer);
        }
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Auth State Error!");
            return;
        }
        //Get Value
        status = mfrc522->PcdRead(addr,Card_Data);
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Read Data Error!");
            return;
        }
        for(int i=0;i<4;i++){
            if(Card_Data[i]!=Card_Data[i+8]){
                QMessageBox::warning(this,"Warning","The data format error!");
                return;
            }
            if(Card_Data[i]!=(uint8_t)(~Card_Data[i+4])){
                QMessageBox::warning(this,"Warning","The data format error!");
                return;
            }
        }
        int value = Card_Data[0]|(Card_Data[1]<<8)|(Card_Data[2]<<16)|(Card_Data[3]<<24);
        QString str;
        str.sprintf("%d",value);
        ui->currentValueLineEdit->setText(str);
        mfrc522->PcdHalt();
    }
}

void MainWindow::on_setValuePushButton_clicked()
{
    uint8_t buffer[20];
    uint8_t Card_KEYA[6] = {0};   //密码
    uint8_t Card_KEYB[6] = {0};   //密码
    uint8_t Card_Data[16];
    uint8_t status;
    uint8_t addr = ui->valueSectorIndexComboBox->currentIndex()*4+ui->valueBlockComboBox->currentIndex();
    if(initDevice()){
        mfrc522->PcdReset();        //复位
        mfrc522->PcdAntennaOff();   //关闭天线
        Sleep(10);
        mfrc522->PcdAntennaOn();    //开启天线
        status = mfrc522->PcdRequest(PICC_REQALL,buffer);//寻卡
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","No Card!");
            return;
        }
        status = mfrc522->PcdAnticoll(buffer);//防冲撞
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Anticoll Error!");
            return;
        }
        status = mfrc522->PcdSelect(buffer);//选定卡片
        if (status != MI_OK){
            QMessageBox::warning(this,"Warning","Select Card Error!");
            return;
        }
        if(ui->valueKeyComboBox->currentText()=="KEYA"){
            if(ui->valueKeyALineEdit->text().length()<12){
                QMessageBox::warning(this,"Warning","Key data is illegal!");
                return;
            }else{
                for(int i=0;i<6;i++){
                    Card_KEYA[i] = ui->valueKeyALineEdit->text().mid(i*2,2).toInt(NULL,16);
                }
            }
            status = mfrc522->PcdAuthState(PICC_AUTHENT1A,addr,Card_KEYA,buffer);
        }else{
            if(ui->valueKeyBLineEdit->text().length()<12){
                QMessageBox::warning(this,"Warning","Key data is illegal!");
                return;
            }else{
                for(int i=0;i<6;i++){
                    Card_KEYB[i] = ui->valueKeyBLineEdit->text().mid(i*2,2).toInt(NULL,16);
                }
            }
            status = mfrc522->PcdAuthState(PICC_AUTHENT1B,addr,Card_KEYB,buffer);
        }
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Auth State Error!");
            return;
        }
        //Set Value
        if(ui->currentValueLineEdit->text().length()<=0){
            QMessageBox::warning(this,"Warning","Value data is illegal!");
            return;
        }
        int value = ui->currentValueLineEdit->text().toInt(NULL,10);
        Card_Data[0] = value&0xFF;
        Card_Data[1] = (value>>8)&0xFF;
        Card_Data[2] = (value>>16)&0xFF;
        Card_Data[3] = (value>>24)&0xFF;
        Card_Data[4] = ~Card_Data[0];
        Card_Data[5] = ~Card_Data[1];
        Card_Data[6] = ~Card_Data[2];
        Card_Data[7] = ~Card_Data[3];
        Card_Data[8] = Card_Data[0];
        Card_Data[9] = Card_Data[1];
        Card_Data[10] = Card_Data[2];
        Card_Data[11] = Card_Data[3];
        Card_Data[12] = addr;
        Card_Data[13] = ~addr;
        Card_Data[14] = addr;
        Card_Data[15] = ~addr;
        if(addr > 0){
            status = mfrc522->PcdWrite(addr,Card_Data);
            if(status != MI_OK){
                QMessageBox::warning(this,"Warning","Write Data Error!");
                return;
            }
        }
        mfrc522->PcdHalt();
    }
}

void MainWindow::on_decrementPushButton_clicked()
{
    uint8_t buffer[20];
    uint8_t Card_KEYA[6] = {0};   //密码
    uint8_t Card_KEYB[6] = {0};   //密码
    uint8_t Card_Data[16];
    uint8_t status;
    uint8_t addr = ui->valueSectorIndexComboBox->currentIndex()*4+ui->valueBlockComboBox->currentIndex();
    if(initDevice()){
        mfrc522->PcdReset();        //复位
        mfrc522->PcdAntennaOff();   //关闭天线
        Sleep(10);
        mfrc522->PcdAntennaOn();    //开启天线
        status = mfrc522->PcdRequest(PICC_REQALL,buffer);//寻卡
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","No Card!");
            return;
        }
        status = mfrc522->PcdAnticoll(buffer);//防冲撞
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Anticoll Error!");
            return;
        }
        status = mfrc522->PcdSelect(buffer);//选定卡片
        if (status != MI_OK){
            QMessageBox::warning(this,"Warning","Select Card Error!");
            return;
        }
        if(ui->valueKeyComboBox->currentText()=="KEYA"){
            if(ui->valueKeyALineEdit->text().length()<12){
                QMessageBox::warning(this,"Warning","Key data is illegal!");
                return;
            }else{
                for(int i=0;i<6;i++){
                    Card_KEYA[i] = ui->valueKeyALineEdit->text().mid(i*2,2).toInt(NULL,16);
                }
            }
            status = mfrc522->PcdAuthState(PICC_AUTHENT1A,addr,Card_KEYA,buffer);
        }else{
            if(ui->valueKeyBLineEdit->text().length()<12){
                QMessageBox::warning(this,"Warning","Key data is illegal!");
                return;
            }else{
                for(int i=0;i<6;i++){
                    Card_KEYB[i] = ui->valueKeyBLineEdit->text().mid(i*2,2).toInt(NULL,16);
                }
            }
            status = mfrc522->PcdAuthState(PICC_AUTHENT1B,addr,Card_KEYB,buffer);
        }
        if(status != MI_OK){
            QMessageBox::warning(this,"Warning","Auth State Error!");
            return;
        }
        //Increment
        if(addr > 0){
            if(ui->valueLineEdit->text().length()<=0){
                QMessageBox::warning(this,"Warning","Value data is illegal!");
                return;
            }
            int value = ui->valueLineEdit->text().toInt(NULL,10);
            Card_Data[0] = value&0xFF;
            Card_Data[1] = (value>>8)&0xFF;
            Card_Data[2] = (value>>16)&0xFF;
            Card_Data[3] = (value>>24)&0xFF;
            status = mfrc522->PcdValue(PICC_DECREMENT,addr,Card_Data);
            if(status != MI_OK){
                qDebug()<<status;
                QMessageBox::warning(this,"Warning","Decrement Value Error!");
                return;
            }
        }
        mfrc522->PcdHalt();
    }
}
