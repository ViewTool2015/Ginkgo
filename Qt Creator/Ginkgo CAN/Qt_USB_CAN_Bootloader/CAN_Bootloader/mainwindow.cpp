#include "mainwindow.h"
#include "ui_mainwindow.h"

typedef  struct {
  unsigned char   SJW;
  unsigned char   BS1;
  unsigned char   BS2;
  unsigned short  PreScale;
} tCAN_BaudRate;

#define GET_ADDR			0x0	//向主机请求本机地址，XX为本机地址，一共7bit
#define ERASE				0x1	//擦除应用程序
#define WRITE				0x2	//将程序写入Flash
#define READ				0x3	//将程序从Flash中读出
#define GO					0x4	//执行应用程序
#define SET_WRITE_ADDR		0x5	//设置写数据起始地址
#define WRITE_CONTINUOUS	0x6	//连续写数据，写数据地址自动递增
#define SET_READ_ADDR		0x7	//设置读数据起始地址
#define READ_CONTINUOUS		0x8	//连续读数据，读数据起始地址递增
#define GET_VERSION			0x9	//获取Bootloader固件版本
#define	ONLINE_CHECK		0xA	//检测CAN节点是否在线
#define ADDR_OK				0xC	//请求地址可用
#define ADDR_ERROR			0xD	//请求地址不可用
#define CMD_OK				0xE	//命令执行成功
#define CMD_ERROR			0xF	//命令执行失败

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    int ret;
    ui->setupUi(this);
    for(int i=0;i<128;i++){
        can_addr.local_addr[i]=0;
    }
    nodeTableInit();
    ret = VCI_ScanDevice(1);
    if(ret<=0){
        QMessageBox::warning(this,"Warning","No device Connected!");
        return;
    }
    ret = VCI_OpenDevice(VCI_USBCAN2,ui->deviceIndexComboBox->currentIndex(),0);
    if(ret!=STATUS_OK){
        QMessageBox::warning(this,"Warning","Open device Error!");
        return;
    }
    if(canInit()){
        ret = VCI_StartCAN(VCI_USBCAN2,
                           ui->deviceIndexComboBox->currentIndex(),
                           ui->channelIndexComboBox->currentIndex());
        if(ret!=STATUS_OK){
            QMessageBox::warning(this,"Warning","Start CAN Error!");
            return;
        }
    }
    scanNodeTimerID = startTimer(100);
}

MainWindow::~MainWindow()
{
    VCI_ResetCAN(VCI_USBCAN2,
                 ui->deviceIndexComboBox->currentIndex(),
                 ui->channelIndexComboBox->currentIndex());
    VCI_CloseDevice(VCI_USBCAN2,ui->deviceIndexComboBox->currentIndex());
    delete ui;
}

void MainWindow::nodeTableInit(void)
{
    QStringList strList;
    strList << "Addr" << "MAC(Hex)" << "Version";
    model = new QStandardItemModel;
    model->setColumnCount(strList.count());
    model->setRowCount(127);
    model->setHorizontalHeaderLabels(strList);
    ui->nodeListTableView->setModel(model);
    ui->nodeListTableView->setColumnWidth(0, 50);
    ui->nodeListTableView->setColumnWidth(1, 160);
    ui->nodeListTableView->setColumnWidth(2, 60);
    ui->nodeListTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->nodeListTableView->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    ui->nodeListTableView->verticalHeader()->setDefaultSectionSize(20);
    ui->nodeListTableView->verticalHeader()->hide();
    for(int r=0;r<model->rowCount();r++){
        for(int c = 0; c < model->columnCount(); c++)
        {
            QStandardItem *item = new QStandardItem("");
            model->setItem(r,c,item);
        }
    }
}
int MainWindow::CAN_GetBaudRateNum(unsigned int BaudRate)
{
    switch(BaudRate){
        case 1000000 :return 0;
        case 900000 :return 1;
        case 800000 :return 2;
        case 666000 :return 3;
        case 600000 :return 4;
        case 500000 :return 5;
        case 400000 :return 6;
        case 300000 :return 7;
        case 250000 :return 8;
        case 225000:return 9;
        case 200000 :return 10;
        case 160000:return 11;
        case 150000 :return 12;
        case 144000:return 13;
        case 125000 :return 14;
        case 120000:return 15;
        case 100000 :return 16;
        case 90000 :return 17;
        case 80000 :return 18;
        case 75000:return 19;
        case 60000 :return 20;
        case 50000 :return 21;
        case 40000 :return 22;
        case 30000 :return 23;
        case 20000 :return 24;
        default:return 0;
    }
}
bool MainWindow::canInit(void)
{
    int ret;
    tCAN_BaudRate  CAN_BaudRateInitTab[]= {      // CLK=36MHz
       1,5,3,4,     // 1M
       1,4,3,5,     // 900K
       1,5,3,5,     // 800K
       1,5,3,6,     // 666K
       1,6,3,6,     // 600K
       1,5,2,9,     // 500K
       1,7,2,9,     // 400K
       1,16,3,6,    // 300K
       1,4,3,18,    // 250K
       1,6,3,16,	// 225K
       1,8,3,15,    // 200K
       1,4,4,25,	// 160K
       1,5,4,24,    // 150K
       1,5,4,25,	// 144K
       1,13,4,16,   // 125K
       1,10,4,20,	// 120K
       1,9,5,24,    // 100K
       1,10,5,25,   // 90K
       1,12,5,25,   // 80K
       1,9,6,30,	// 75K
       1,7,7,40,    // 60K
       1,8,7,45,    // 50K
       1,9,8,50,    // 40K
       1,6,3,120,   // 30K
       1,3,2,300,   // 20K
    };
    VCI_INIT_CONFIG_EX CAN_InitConfig;
    QString str = ui->baudRateComboBox->currentText();
    str.resize(str.length()-4);
    int baud = str.toInt(NULL,10)*1000;
    CAN_InitConfig.CAN_BRP = CAN_BaudRateInitTab[CAN_GetBaudRateNum(baud)].PreScale;
    CAN_InitConfig.CAN_SJW = CAN_BaudRateInitTab[CAN_GetBaudRateNum(baud)].SJW;
    CAN_InitConfig.CAN_BS1 = CAN_BaudRateInitTab[CAN_GetBaudRateNum(baud)].BS1;
    CAN_InitConfig.CAN_BS2 = CAN_BaudRateInitTab[CAN_GetBaudRateNum(baud)].BS2;
    CAN_InitConfig.CAN_ABOM = 1;
    CAN_InitConfig.CAN_Mode = 0;
    CAN_InitConfig.CAN_NART = 0;
    CAN_InitConfig.CAN_RELAY = 0;
    CAN_InitConfig.CAN_RFLM = 0;
    CAN_InitConfig.CAN_TXFP = 1;
    ret = VCI_InitCANEx(VCI_USBCAN2,
                        ui->deviceIndexComboBox->currentIndex(),
                        ui->channelIndexComboBox->currentIndex(),
                        &CAN_InitConfig);
    if(ret!=STATUS_OK){
        QMessageBox::warning(this,"Warning","Initialize device Error!");
        return false;
    }
    VCI_FILTER_CONFIG CAN_Filter;
    CAN_Filter.Enable = 1;
    CAN_Filter.ExtFrame = 0;
    CAN_Filter.FilterIndex = 0;
    CAN_Filter.FilterMode = 0;
    CAN_Filter.ID_IDE = 0;
    CAN_Filter.ID_RTR = 0;
    CAN_Filter.ID_Std_Ext = 0;
    CAN_Filter.MASK_IDE = 0;
    CAN_Filter.MASK_RTR = 0;
    CAN_Filter.MASK_Std_Ext = 0;
    ret = VCI_SetFilter(VCI_USBCAN2,
                        ui->deviceIndexComboBox->currentIndex(),
                        ui->channelIndexComboBox->currentIndex(),
                        &CAN_Filter);
    if(ret!=STATUS_OK){
        QMessageBox::warning(this,"Warning","Set filter Error!");
        return false;
    }
    return true;
}

void MainWindow::logMessage(QString str)
{
    ui->logTextEdit->moveCursor(QTextCursor::End);
    ui->logTextEdit->append(str);
}
bool MainWindow::setStartAddr(unsigned char can_node_addr,unsigned int startAddr,unsigned int dataSize)
{
    VCI_CAN_OBJ CAN_SendData;
    VCI_CAN_OBJ CAN_ReadData;
    int ret,retry;
    CAN_SendData.Data[0] = startAddr>>24;
    CAN_SendData.Data[1] = startAddr>>16;
    CAN_SendData.Data[2] = startAddr>>8;
    CAN_SendData.Data[3] = startAddr>>0;
    CAN_SendData.Data[4] = dataSize>>24;
    CAN_SendData.Data[5] = dataSize>>16;
    CAN_SendData.Data[6] = dataSize>>8;
    CAN_SendData.Data[7] = dataSize>>0;
    CAN_SendData.DataLen = 8;
    CAN_SendData.ID = (can_node_addr<<4)|SET_WRITE_ADDR;
    CAN_SendData.ExternFlag = 0;
    CAN_SendData.RemoteFlag = 0;
    CAN_SendData.SendType = 0;
    ret = VCI_Transmit(VCI_USBCAN2,
                       ui->deviceIndexComboBox->currentIndex(),
                       ui->channelIndexComboBox->currentIndex(),
                       &CAN_SendData,
                       1);
    if(ret!=1){
        logMessage("Set start address error!");
        QMessageBox::warning(this,"Warning","Set start address error!");
        return false;
    }
    retry = 0;
    while(1){
        Sleep(10);
        ret = VCI_Receive(VCI_USBCAN2,
                          ui->deviceIndexComboBox->currentIndex(),
                          ui->channelIndexComboBox->currentIndex(),
                          &CAN_ReadData,
                          1);
        if((ret==1)&&(can_node_addr==(CAN_ReadData.ID>>4))&&((CAN_ReadData.ID&0xF)==CMD_OK)){
            break;
        }else{
            Sleep(10);
            retry++;
            if(retry>5){
                logMessage("Set start address error!");
                QMessageBox::warning(this,"Warning","Set start address error!");
                return false;
            }
            QCoreApplication::processEvents(QEventLoop::AllEvents);
        }
    }
    //logMessage("Set start success!");
    return true;
}

bool MainWindow::eraseSectors(unsigned char can_node_addr,unsigned int startAddr,unsigned int EndAddr)
{
    VCI_CAN_OBJ CAN_SendData;
    VCI_CAN_OBJ CAN_ReadData;
    int ret,retry;
    if(startAddr>EndAddr){
        QMessageBox::warning(this,"Warning","Address error!");
        return false;
    }else{
        CAN_SendData.Data[0] = startAddr>>24;
        CAN_SendData.Data[1] = startAddr>>16;
        CAN_SendData.Data[2] = startAddr>>8;
        CAN_SendData.Data[3] = startAddr>>0;
        CAN_SendData.Data[4] = EndAddr>>24;
        CAN_SendData.Data[5] = EndAddr>>16;
        CAN_SendData.Data[6] = EndAddr>>8;
        CAN_SendData.Data[7] = EndAddr>>0;
        CAN_SendData.DataLen = 8;
        CAN_SendData.ID = (can_node_addr<<4)|ERASE;
        CAN_SendData.ExternFlag = 0;
        CAN_SendData.RemoteFlag = 0;
        CAN_SendData.SendType = 0;
        ret = VCI_Transmit(VCI_USBCAN2,
                           ui->deviceIndexComboBox->currentIndex(),
                           ui->channelIndexComboBox->currentIndex(),
                           &CAN_SendData,
                           1);
        if(ret!=1){
            logMessage("Erase sectors error!");
            QMessageBox::warning(this,"Warning","Send Data Error!");
            return false;
        }
        logMessage("Start erase sectors,please wait...");
        retry = 0;
        while(1){
            Sleep(10);
            ret = VCI_Receive(VCI_USBCAN2,
                              ui->deviceIndexComboBox->currentIndex(),
                              ui->channelIndexComboBox->currentIndex(),
                              &CAN_ReadData,
                              1);
            if((ret==1)&&(can_node_addr==(CAN_ReadData.ID>>4))&&((CAN_ReadData.ID&0xF)==CMD_OK)){
                break;
            }else{
                if(((CAN_ReadData.ID&0xF)==CMD_ERROR)){
                    logMessage("Erase sectors error!");
                    QMessageBox::warning(this,"Warning","Erase sector error!");
                    return false;
                }
                Sleep(10);
                retry++;
                if(retry>200){
                    logMessage("Erase sectors error!");
                    QMessageBox::warning(this,"Warning","Erase sector error!");
                    return false;
                }
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            }
        }
        logMessage("Erase sector success!");
        return true;
    }
}

void MainWindow::on_updatePushButton_clicked()
{
    int ret,retry,i,j;
    int read_data_num;
    QString str;
    uint32_t startAddr = ui->startAddrLineEdit->text().toInt(NULL,16);
    VCI_CAN_OBJ CAN_SendData[1024];
    VCI_CAN_OBJ CAN_ReadData;
    QFile firmwareFile(ui->fileLineEdit->text());
    if(model->item(ui->nodeListTableView->currentIndex().row(),0)->text().isEmpty()){
        QMessageBox::warning(this,"Warning","Please select online can node!");
        return;
    }
    uint8_t can_node_addr = ui->nodeListTableView->currentIndex().row()+1;
    if (firmwareFile.open(QFile::ReadOnly)){
        killTimer(scanNodeTimerID);
        LARGE_INTEGER litmp;
        LONGLONG StartTime,EndTime;
        double dfFreq;
        int sendedDataNum;
        QueryPerformanceFrequency(&litmp);//取得高精度运行计数器的频率f,单位是每秒多少次（n/s），
        dfFreq = (double)litmp.QuadPart;
        QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
        StartTime = litmp.QuadPart; //开始计时
        //擦出程序扇区
        if(!eraseSectors(can_node_addr,startAddr,startAddr+firmwareFile.size())){
            scanNodeTimerID = startTimer(100);
            return;
        }
        logMessage("Start write firmware data...");
        str.sprintf("CAN node address:%d",can_node_addr);
        logMessage(str);
        str.sprintf("Firmware start address(Hex):%08X",startAddr);
        logMessage(str);
        str.sprintf("Firmware size:%d Byte",firmwareFile.size());
        logMessage(str);
        str.sprintf("Current Progress:%3d%%",0);
        logMessage(str);
        for(i=0;i<firmwareFile.size();i+=1024){
            for(j=0;j<(1024/8);j++){
                read_data_num = firmwareFile.read((char*)(&CAN_SendData[j].Data[0]),8);
                CAN_SendData[j].DataLen = read_data_num;
                CAN_SendData[j].ExternFlag = 0;
                CAN_SendData[j].RemoteFlag = 0;
                CAN_SendData[j].SendType = 0;
                CAN_SendData[j].ID = (can_node_addr<<4)|WRITE_CONTINUOUS;
                if(read_data_num<8){
                    break;
                }
            }
            //设置程序起始地址
            if(!setStartAddr(can_node_addr,startAddr,(j>0)?(j-1)*8+read_data_num:0)){
                scanNodeTimerID = startTimer(50);
                return;
            }
            startAddr += (j>0)?(j-1)*8+read_data_num:0;
            //发送数据
            for(int k=0;k<j;k++){
                ret = VCI_Transmit(VCI_USBCAN2,
                                   ui->deviceIndexComboBox->currentIndex(),
                                   ui->channelIndexComboBox->currentIndex(),
                                   &CAN_SendData[k],
                                   1);
                if(ret!=1){
                    logMessage("Write firmware data error!");
                    QMessageBox::warning(this,"Warning","Send Data Error!");
                    scanNodeTimerID = startTimer(100);
                    return;
                }
            }
            //设置更新进度
            QTextCursor cursor = ui->logTextEdit->textCursor();
            cursor.deletePreviousChar();
            cursor.deletePreviousChar();
            cursor.deletePreviousChar();
            cursor.deletePreviousChar();
            str.sprintf("%3d%%",i*100/firmwareFile.size());
            ui->logTextEdit->insertPlainText(str);
            QCoreApplication::processEvents(QEventLoop::AllEvents);
            retry = 0;
            while(1){
                ret = VCI_Receive(VCI_USBCAN2,
                                  ui->deviceIndexComboBox->currentIndex(),
                                  ui->channelIndexComboBox->currentIndex(),
                                  &CAN_ReadData,
                                  1);
                if((ret==1)&&(can_node_addr==(CAN_ReadData.ID>>4))&&((CAN_ReadData.ID&0xF)==CMD_OK)){
                    //qDebug()<<"SUCCESS";
                    break;
                }else{
                    if((CAN_ReadData.ID&0xF)==CMD_ERROR){
                        logMessage("Write firmware data error!");
                        QMessageBox::warning(this,"Warning","Get ACK error!");
                        scanNodeTimerID = startTimer(100);
                        return;
                    }
                    Sleep(10);
                    retry++;
                    if(retry>100){
                        logMessage("Write firmware data error!");
                        QMessageBox::warning(this,"Warning","Get ACK error!");
                        scanNodeTimerID = startTimer(100);
                        return;
                    }
                }
            }
            QCoreApplication::processEvents(QEventLoop::AllEvents);
        }
        //设置更新进度
        QTextCursor cursor = ui->logTextEdit->textCursor();
        cursor.deletePreviousChar();
        cursor.deletePreviousChar();
        cursor.deletePreviousChar();
        cursor.deletePreviousChar();
        str.sprintf("%3d%%",100);
        ui->logTextEdit->insertPlainText(str);
        logMessage("Write firmware data success!");
        QueryPerformanceCounter(&litmp);//取得高精度运行计数器的数值
        EndTime = litmp.QuadPart; //终止计时
        QString timeStr;
        timeStr.sprintf("Elapsed time:%.6f seconds",(double)(EndTime-StartTime)/dfFreq);
        logMessage(timeStr);
        QMessageBox::information(this,"Information","Update firmware success!");
        scanNodeTimerID = startTimer(100);
        firmwareFile.close();
    }else{
        QMessageBox::warning(this,"Warning","Open file error!");
    }
}

void MainWindow::timerEvent(QTimerEvent * event)
{
    if(event->timerId() == scanNodeTimerID){
        onlineCheck();
    }

}

void MainWindow::onlineCheck(void)
{
    VCI_CAN_OBJ CAN_ReadData[200];
    VCI_CAN_OBJ CAN_SendData;
    int ret,i,k;
    QString mac_str;
    QString str;
    ret = VCI_Receive(VCI_USBCAN2,
                      ui->deviceIndexComboBox->currentIndex(),
                      ui->channelIndexComboBox->currentIndex(),
                      &CAN_ReadData[0],
                      200);
    for(k=0;k<ret;k++){
        if((CAN_ReadData[k].ID&0xF)==GET_ADDR){
            mac_str.sprintf("%02X%02X%02X%02X%02X%02X%02X%02X",
                            CAN_ReadData[k].Data[0],CAN_ReadData[k].Data[1],
                            CAN_ReadData[k].Data[2],CAN_ReadData[k].Data[3],
                            CAN_ReadData[k].Data[4],CAN_ReadData[k].Data[5],
                            CAN_ReadData[k].Data[6],CAN_ReadData[k].Data[7]);
            for(i=1;i<128;i++){
                if(can_addr.local_addr[i]==0){
                    can_addr.local_addr[i] = i;
                    for(int j=0;j<8;j++){
                        can_addr.mac_addr[i][j] = CAN_ReadData[k].Data[j];
                        CAN_SendData.Data[j] = CAN_ReadData[k].Data[j];
                    }
                    CAN_SendData.DataLen = 8;
                    CAN_SendData.ExternFlag = 0;
                    CAN_SendData.RemoteFlag = 0;
                    CAN_SendData.SendType = 0;
                    CAN_SendData.ID = (i<<4)|ADDR_OK;//设置从机地址
                    break;
                }else if(mac_str==model->item(i-1,1)->text()){
                    for(int j=0;j<8;j++){
                        CAN_SendData.Data[j] = CAN_ReadData[k].Data[j];
                    }
                    CAN_SendData.DataLen = 8;
                    CAN_SendData.ExternFlag = 0;
                    CAN_SendData.RemoteFlag = 0;
                    CAN_SendData.SendType = 0;
                    CAN_SendData.ID = (i<<4)|ADDR_OK;//设置从机地址
                    break;
                }else{
                    continue;
                }
            }
            if(i==128){
                CAN_SendData.DataLen = 0;
                CAN_SendData.ExternFlag = 0;
                CAN_SendData.RemoteFlag = 0;
                CAN_SendData.SendType = 0;
                CAN_SendData.ID = ADDR_ERROR;
            }
            ret = VCI_Transmit(VCI_USBCAN2,
                               ui->deviceIndexComboBox->currentIndex(),
                               ui->channelIndexComboBox->currentIndex(),
                               &CAN_SendData,
                               1);
            if(ret!=1){
                QMessageBox::warning(this,"Warning","Send Data Error!");
                killTimer(scanNodeTimerID);
            }else{
                //将正确获取到设备地址的节点在界面列表里面显示出来
                if((CAN_SendData.ID&0xF)!=ADDR_ERROR){
                    str.sprintf("%d",i);
                    model->item(i-1,0)->setText(str);
                    model->item(i-1,1)->setText(mac_str);
                }
            }
        }
    }
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    //检测在线状态
    for(i=0;i<model->rowCount();i++){
        uint32_t local_addr,retry=0;
        if(!model->item(i,0)->text().isEmpty()){
            //清除之前接收的数据
            VCI_ClearBuffer(VCI_USBCAN2,
                            ui->deviceIndexComboBox->currentIndex(),
                            ui->channelIndexComboBox->currentIndex());
            local_addr = model->item(i,0)->text().toInt(NULL,0);
            //发送在线检测命令
            CAN_SendData.DataLen = 0;
            CAN_SendData.ExternFlag = 0;
            CAN_SendData.RemoteFlag = 0;
            CAN_SendData.SendType = 0;
            CAN_SendData.ID = (local_addr<<4)|ONLINE_CHECK;
            ret = VCI_Transmit(VCI_USBCAN2,
                               ui->deviceIndexComboBox->currentIndex(),
                               ui->channelIndexComboBox->currentIndex(),
                               &CAN_SendData,
                               1);
            if(ret!=1){
                QMessageBox::warning(this,"Warning","Send Data Error!");
            }
            retry = 0;
            while(1){
                ret = VCI_Receive(VCI_USBCAN2,
                                  ui->deviceIndexComboBox->currentIndex(),
                                  ui->channelIndexComboBox->currentIndex(),
                                  &CAN_ReadData[0],
                                  1);
                if((ret==1)&&(local_addr==(CAN_ReadData[0].ID>>4))&&((CAN_ReadData[0].ID&0xF)==CMD_OK)){
                    mac_str.sprintf("%02X%02X%02X%02X%02X%02X%02X%02X",
                                    CAN_ReadData[0].Data[0],CAN_ReadData[0].Data[1],
                                    CAN_ReadData[0].Data[2],CAN_ReadData[0].Data[3],
                                    CAN_ReadData[0].Data[4],CAN_ReadData[0].Data[5],
                                    CAN_ReadData[0].Data[6],CAN_ReadData[0].Data[7]);
                    model->item(i,1)->setText(mac_str);
                    break;
                }else{
                    if(retry<10){
                        Sleep(10);
                        retry++;
                    }else{
                        for(int c=0;c<model->columnCount();c++){
                            model->item(i,c)->setText("");
                        }
                        can_addr.local_addr[i+1]=0;
                        break;
                    }
                }
            }
        }
    }
    QCoreApplication::processEvents(QEventLoop::AllEvents);
    //获取版本信息
    for(i=0;i<model->rowCount();i++){
        uint32_t local_addr,retry=0;
        if(!model->item(i,0)->text().isEmpty()){
            //清除之前接收的数据
            VCI_ClearBuffer(VCI_USBCAN2,
                            ui->deviceIndexComboBox->currentIndex(),
                            ui->channelIndexComboBox->currentIndex());
            local_addr = model->item(i,0)->text().toInt(NULL,0);
            //发送在线检测命令
            CAN_SendData.DataLen = 0;
            CAN_SendData.ExternFlag = 0;
            CAN_SendData.RemoteFlag = 0;
            CAN_SendData.SendType = 0;
            CAN_SendData.ID = (local_addr<<4)|GET_VERSION;
            ret = VCI_Transmit(VCI_USBCAN2,
                               ui->deviceIndexComboBox->currentIndex(),
                               ui->channelIndexComboBox->currentIndex(),
                               &CAN_SendData,
                               1);
            if(ret!=1){
                QMessageBox::warning(this,"Warning","Send Data Error!");
            }
            retry = 0;
            while(1){
                ret = VCI_Receive(VCI_USBCAN2,
                                  ui->deviceIndexComboBox->currentIndex(),
                                  ui->channelIndexComboBox->currentIndex(),
                                  &CAN_ReadData[0],
                                  1);
                if((ret==1)&&(local_addr==(CAN_ReadData[0].ID>>4))&&((CAN_ReadData[0].ID&0xF)==GET_VERSION)){
                    str.sprintf("V%d.%d",CAN_ReadData[0].Data[0],CAN_ReadData[0].Data[1]);
                    model->item(local_addr-1,2)->setText(str);
                    break;
                }else{
                    if(retry<10){
                        Sleep(10);
                        retry++;
                    }else{
                        break;
                    }
                }
            }
        }
    }
}

void MainWindow::on_openFilePushButton_clicked()
{
    QString fileName;
    fileName=QFileDialog::getOpenFileName(this,
                                          tr("Open files"),
                                          "",
                                          "Binary Files (*.bin);;Hex Files (*.hex);;All Files (*.*)");
    if(fileName.isNull()){
        return;
    }
    ui->fileLineEdit->setText(fileName);
}

void MainWindow::on_executePushButton_clicked()
{
    VCI_CAN_OBJ CAN_SendData;
    int ret;
    QString str;
    if(model->item(ui->nodeListTableView->currentIndex().row(),0)->text().isEmpty()){
        QMessageBox::warning(this,"Warning","Please select online can node!");
        return;
    }
    uint8_t can_node_addr = ui->nodeListTableView->currentIndex().row()+1;
    uint32_t startAddr = ui->startAddrLineEdit->text().toInt(NULL,16);
    CAN_SendData.Data[0] = startAddr>>24;
    CAN_SendData.Data[1] = startAddr>>16;
    CAN_SendData.Data[2] = startAddr>>8;
    CAN_SendData.Data[3] = startAddr>>0;
    CAN_SendData.DataLen = 4;
    CAN_SendData.ExternFlag = 0;
    CAN_SendData.RemoteFlag = 0;
    CAN_SendData.SendType = 0;
    CAN_SendData.ID = (can_node_addr<<4)|GO;
    ret = VCI_Transmit(VCI_USBCAN2,
                       ui->deviceIndexComboBox->currentIndex(),
                       ui->channelIndexComboBox->currentIndex(),
                       &CAN_SendData,
                       1);
    if(ret!=1){
        logMessage("Execute firmware error!");
        str.sprintf("Firmware start Address(Hex):%08X",startAddr);
        logMessage(str);
        QMessageBox::warning(this,"Warning","Execute firmware error!");
    }else{
        logMessage("Execute firmware success!");
        str.sprintf("Firmware start Address(Hex):%08X",startAddr);
        logMessage(str);
        QMessageBox::information(this,"Information","Execute firmware success!");
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

void MainWindow::on_actionAbout_triggered()
{
    QString AboutStr;
    AboutStr = "Ginkgo USB-CAN Bootloader 1.0.0\n";
    AboutStr.append("ViewTool Virtual Instrument 2009-2013 Copyright\n");
    AboutStr.append("Hardware Suuport: Ginkgo 2 USB-CAN Adapter");
    QMessageBox::about(this,"About Ginkgo USB-CAN Bootloader",AboutStr);
}

void MainWindow::on_actionContact_Us_triggered()
{
    QString AboutStr;
    AboutStr.append("Official website<span style=\"font-size:12px;\">&nbsp;</span>:&nbsp;<a href=\"http://www.viewtool.com\">www.viewtool.com</a><br>");
    AboutStr.append("Official BBS<span style=\"font-size:12px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span>:&nbsp;<a href=\"http://www.viewtool.com/bbs\">www.viewtool.com/bbs</a><br>");
    AboutStr.append("Official Taobao<span style=\"font-size: 9px;\">&nbsp;</span>:&nbsp;<a href=\"http://viewtool.taobao.com/\">viewtool.taobao.com</a><br>");
    AboutStr.append("Technical Suuport QQ&nbsp;:&nbsp;1713047813<br>");
    AboutStr.append("Product Consultant QQ&nbsp;:&nbsp;191431236");
    QMessageBox::about(this,"Contact us",AboutStr);
}
