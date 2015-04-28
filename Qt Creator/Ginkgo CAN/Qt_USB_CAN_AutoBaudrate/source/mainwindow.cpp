#include "mainwindow.h"
#include "ui_mainwindow.h"

typedef  struct {
  unsigned char   SJW;
  unsigned char   BS1;
  unsigned char   BS2;
  unsigned short  PreScale;
} tCAN_BaudRate;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList resultTableItems;
    resultTableItems << "Baud Rate" << "SJW" <<"BS1" << "BS2" << "BRP" << "Sampling Point" << "Result";
    ui->resultTableWidget->setColumnCount(resultTableItems.count());
    ui->resultTableWidget->setHorizontalHeaderLabels(resultTableItems);
    ui->resultTableWidget->setColumnWidth(0,115);
    ui->resultTableWidget->setColumnWidth(1,90);
    ui->resultTableWidget->setColumnWidth(2,90);
    ui->resultTableWidget->setColumnWidth(3,90);
    ui->resultTableWidget->setColumnWidth(4,40);
    ui->resultTableWidget->setColumnWidth(5,100);
    ui->resultTableWidget->setColumnWidth(6,128);
    ui->resultTableWidget->verticalHeader()->setHidden(true);
    ui->resultTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->resultTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
    stopAutoBaudRate = false;
    connect(ui->aboutSoftWareAction, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->contactUsAction, SIGNAL(triggered()), this, SLOT(contactus()));
    connect(ui->quitAction, SIGNAL(triggered()), this, SLOT(quit()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_recognitionModeComboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "standard"){
        ui->startBaudRateLineEdit->setDisabled(true);
        ui->endBaudRateLineEdit->setDisabled(true);
    }else{
        ui->startBaudRateLineEdit->setEnabled(true);
        ui->endBaudRateLineEdit->setEnabled(true);
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
        case 10000 :return 25;
        case 5000 :return 26;
        case 3000 :return 27;
        case 2000 :return 28;
        default:return 0;
    }
}
void MainWindow::on_startAutoBaudratePushButton_clicked()
{
    unsigned int BaudRateTable[29]={
    1000000,900000,800000,666000,600000,500000,400000,300000,250000,225000,200000,
    160000,150000,144000,125000,120000,100000,90000,80000,75000,60000,50000,40000,
    30000,20000,10000,5000,3000,2000};

    tCAN_BaudRate  CAN_BaudRateInitTab[29]= {      // CLK=36MHz
       {1,5,3,4},     // 1M
       {1,4,3,5},     // 900K
       {1,5,3,5},     // 800K
       {1,5,3,6},     // 666K
       {1,6,3,6},     // 600K
       {1,5,2,9},     // 500K
       {1,7,2,9},     // 400K
       {1,16,3,6},    // 300K
       {1,4,3,18},    // 250K
       {1,6,3,16},	// 225K
       {1,8,3,15},    // 200K
       {1,4,4,25},	// 160K
       {1,5,4,24},    // 150K
       {1,5,4,25},	// 144K
       {1,13,4,16},   // 125K
       {1,10,4,20},	// 120K
       {1,9,5,24},    // 100K
       {1,10,5,25},   // 90K
       {1,12,5,25},   // 80K
       {1,9,6,30},	// 75K
       {1,7,7,40},    // 60K
       {1,8,7,45},    // 50K
       {1,9,8,50},    // 40K
       {1,6,3,120},   // 30K
       {1,3,2,300},   // 20K
       {1,3,2,600},   // 10K
       {2,6,4,600},   // 5K
       {2,6,4,1000},  // 3K
       {2,10,6,1000}, // 2K
    };
    ui->deviceIndexComboBox->setDisabled(true);
    ui->channelIndexComboBox->setDisabled(true);
    ui->recognitionModeComboBox->setDisabled(true);
    ui->startBaudRateLineEdit->setDisabled(true);
    ui->endBaudRateLineEdit->setDisabled(true);
    ui->recognizeTimeLineEdit->setDisabled(true);
    ui->startAutoBaudratePushButton->setDisabled(true);
    ui->stopAutoBaudratePushButton->setEnabled(true);

    int DeviceIndex = ui->deviceIndexComboBox->currentText().toInt(NULL,0);
    int ChannelIndex = ui->channelIndexComboBox->currentText().toInt(NULL,0);
    int StartBaudRate = ui->startBaudRateLineEdit->text().toInt(NULL,0);
    int EndBaudRate = ui->endBaudRateLineEdit->text().toInt(NULL,0);
    int RecognizeTime = ui->recognizeTimeLineEdit->text().toInt(NULL,0);
    int ret,ErrCount=0,i,j,k=0;
    int BS1,BS2,SJW,BRP;
    bool matched=false;
    tCAN_BaudRate  CAN_BaudRateDataTab[80000];
    //int StartBRP=50,EndBRP=100;
    VCI_INIT_CONFIG_EX InitConfig;
    VCI_CAN_STATUS CANStatus;
    VCI_FILTER_CONFIG FilterConfig;
    //ui->resultTableWidget->clear();
    ui->resultTableWidget->setRowCount(0);
    stopAutoBaudRate = false;
    //打开设备
    ret = VCI_OpenDevice(VCI_USBCAN2,DeviceIndex,0);
    if(ret != STATUS_OK){
        QMessageBox::warning(this,"Warning","Open device faild!");
        on_stopAutoBaudratePushButton_clicked();
        return;
    }
    //开始波特率识别
    if(ui->recognitionModeComboBox->currentText()=="extend"){
        //参数合法性检查
        //获取所有波特率
        for(SJW=1;SJW<=1;SJW++){
            for(BS1=1;BS1<=16;BS1++){
                for(BS2=1;BS2<=8;BS2++){
                    for(BRP=1;BRP<=1024;BRP++){
                        float tmp = 36000.0/BRP/(SJW+BS1+BS2);
                        if((tmp>=StartBaudRate)&&(tmp<=EndBaudRate)){
                            float site = (float)(1+BS1)/(1+BS1+BS2);
                            if(tmp>800){
                                if((site>0.70)&&((1+SJW+BS1+BS2)<=25)&&((1+SJW+BS1+BS2)>=8)&&(BS1>=BS2)){
                                    CAN_BaudRateDataTab[k].BS1 = BS1;
                                    CAN_BaudRateDataTab[k].BS2 = BS2;
                                    CAN_BaudRateDataTab[k].SJW = SJW;
                                    CAN_BaudRateDataTab[k].PreScale = BRP;
                                    k++;
                                }
                            }else if(tmp>500){
                                if((site>0.70)&&((1+SJW+BS1+BS2)<=25)&&((1+SJW+BS1+BS2)>=8)&&(BS1>=BS2)){
                                    CAN_BaudRateDataTab[k].BS1 = BS1;
                                    CAN_BaudRateDataTab[k].BS2 = BS2;
                                    CAN_BaudRateDataTab[k].SJW = SJW;
                                    CAN_BaudRateDataTab[k].PreScale = BRP;
                                    k++;
                                }
                            }else{
                                if((site>0.70)&&((1+SJW+BS1+BS2)<=25)&&((1+SJW+BS1+BS2)>=8)&&(BS1>=BS2)){
                                    CAN_BaudRateDataTab[k].BS1 = BS1;
                                    CAN_BaudRateDataTab[k].BS2 = BS2;
                                    CAN_BaudRateDataTab[k].SJW = SJW;
                                    CAN_BaudRateDataTab[k].PreScale = BRP;
                                    k++;
                                }
                            }
                        }
                    }
                }
            }
        }
        for(i=0;i<(k-1);i++){
            for (j=0;j<k-i-1;j++){
                float tmp1 = 36000.0/CAN_BaudRateDataTab[j].PreScale/(CAN_BaudRateDataTab[j].SJW+CAN_BaudRateDataTab[j].BS1+CAN_BaudRateDataTab[j].BS2);
                float tmp2 = 36000.0/CAN_BaudRateDataTab[j+1].PreScale/(CAN_BaudRateDataTab[j+1].SJW+CAN_BaudRateDataTab[j+1].BS1+CAN_BaudRateDataTab[j+1].BS2);
                tCAN_BaudRate BaudTmp;
                if (tmp1<tmp2)     //交换
                {
                    BaudTmp=CAN_BaudRateDataTab[j];
                    CAN_BaudRateDataTab[j]=CAN_BaudRateDataTab[j+1];
                    CAN_BaudRateDataTab[j+1]=BaudTmp;
                }
            }
        }
        float oldBaudTmp=0;
        for(i=0;i<k;i++){
            if(stopAutoBaudRate){
                break;
            }
            InitConfig.CAN_ABOM = 0;
            BRP = InitConfig.CAN_BRP = CAN_BaudRateDataTab[i].PreScale;
            BS1 = InitConfig.CAN_BS1 = CAN_BaudRateDataTab[i].BS1;
            BS2 = InitConfig.CAN_BS2 = CAN_BaudRateDataTab[i].BS2;
            SJW = InitConfig.CAN_SJW = CAN_BaudRateDataTab[i].SJW;
            InitConfig.CAN_Mode = 2;
            InitConfig.CAN_NART = 0;
            InitConfig.CAN_RFLM = 0;
            InitConfig.CAN_TXFP = 0;
            InitConfig.CAN_RELAY = 0;
            ret = VCI_InitCANEx(VCI_USBCAN2,DeviceIndex,ChannelIndex,&InitConfig);
            if(ret != STATUS_OK){
                VCI_ERR_INFO ErrorInfo;
                VCI_ReadErrInfo(VCI_USBCAN2,DeviceIndex,ChannelIndex,&ErrorInfo);
                qDebug()<<"ErrorInfo.ERROR = "<<ErrorInfo.ErrCode;
                QMessageBox::warning(this,"Warning","Initialization device faild!");
                on_stopAutoBaudratePushButton_clicked();
                return;
            }
            FilterConfig.Enable = 1;
            FilterConfig.ExtFrame = 0;
            FilterConfig.FilterIndex = 0;
            FilterConfig.FilterMode = 0;
            FilterConfig.ID_IDE = 0;
            FilterConfig.ID_RTR = 0;
            FilterConfig.ID_Std_Ext = 0;
            FilterConfig.MASK_IDE = 0;
            FilterConfig.MASK_RTR = 0;
            FilterConfig.MASK_Std_Ext = 0;
            ret = VCI_SetFilter(VCI_USBCAN2,DeviceIndex,ChannelIndex,&FilterConfig);
            if(ret != STATUS_OK){
                QMessageBox::warning(this,"Warning","Set filter faild!");
                on_stopAutoBaudratePushButton_clicked();
                return;
            }
            ErrCount = 0;
            float NewBaudTmp = 36000.0/InitConfig.CAN_BRP/(InitConfig.CAN_BS1+InitConfig.CAN_BS2+InitConfig.CAN_SJW);
            if(NewBaudTmp==oldBaudTmp){
                continue;
            }
            ret = VCI_StartCAN(VCI_USBCAN2,DeviceIndex,ChannelIndex);
            if(ret != STATUS_OK){
                QMessageBox::warning(this,"Warning","Start CAN faild!");
                on_stopAutoBaudratePushButton_clicked();
                return;
            }
            if(oldBaudTmp != NewBaudTmp){
                QString BaudRateInfo;
                float site = (float)(1+BS1)/(1+BS1+BS2);
                BaudRateInfo.sprintf("%.3f Kbps|CAN_SJW_%dtq|CAN_BS1_%dtq|CAN_BS2_%dtq|%d|%.3f%%|%s",NewBaudTmp,SJW,BS1,BS2,BRP,site*100,"Matching...");
                displayAutoBaudRate(BaudRateInfo);
            }
            QTime dieTime = QTime::currentTime().addMSecs(RecognizeTime);
            while( QTime::currentTime() < dieTime ){
                QCoreApplication::processEvents(QEventLoop::AllEvents);
                if(stopAutoBaudRate){
                    break;
                }
                ret = VCI_ReadCANStatus(VCI_USBCAN2,DeviceIndex,ChannelIndex,&CANStatus);
                if(ret != STATUS_OK){
                    QMessageBox::warning(this,"Warning","Read CAN status faild!");
                    on_stopAutoBaudratePushButton_clicked();
                    return;
                }
                ErrCount += CANStatus.regRECounter;//接收错误计数器
                if(ErrCount){
                    if(oldBaudTmp != NewBaudTmp){
                        QString BaudRateInfo;
                        float site = (float)(1+BS1)/(1+BS1+BS2);
                        BaudRateInfo.sprintf("%.3f Kbps|CAN_SJW_%dtq|CAN_BS1_%dtq|CAN_BS2_%dtq|%d|%.3f%%|%s",NewBaudTmp,SJW,BS1,BS2,BRP,site*100,"Don't Match");
                        displayAutoBaudRate(BaudRateInfo);
                    }
                    break;
                }
            }
            if(ErrCount==0){
                QString BaudRateInfo;
                float site = (float)(1+BS1)/(1+BS1+BS2);
                if(VCI_GetReceiveNum(VCI_USBCAN2,DeviceIndex,ChannelIndex) == 0){
                    BaudRateInfo.sprintf("%.3f Kbps|CAN_SJW_%dtq|CAN_BS1_%dtq|CAN_BS2_%dtq|%d|%.3f%%|%s",NewBaudTmp,SJW,BS1,BS2,BRP,site*100,"No data");
                    displayAutoBaudRate(BaudRateInfo);
                    break;
                }
                if(oldBaudTmp != NewBaudTmp){
                    matched = true;
                    BaudRateInfo.sprintf("%.3f Kbps|CAN_SJW_%dtq|CAN_BS1_%dtq|CAN_BS2_%dtq|%d|%.3f%%|%s",NewBaudTmp,SJW,BS1,BS2,BRP,site*100,"Matched");
                    displayAutoBaudRate(BaudRateInfo);
                }
            }else{
                if(matched){
                    //break;
                }
            }
            oldBaudTmp = NewBaudTmp;
        }
        VCI_ResetCAN(VCI_USBCAN2,DeviceIndex,ChannelIndex);
        on_stopAutoBaudratePushButton_clicked();
    }else{
        for(i=0;i<29;i++){
            if(stopAutoBaudRate){
                break;
            }
            InitConfig.CAN_ABOM = 0;
            BRP = InitConfig.CAN_BRP = CAN_BaudRateInitTab[CAN_GetBaudRateNum(BaudRateTable[i])].PreScale;
            BS1 = InitConfig.CAN_BS1 = CAN_BaudRateInitTab[CAN_GetBaudRateNum(BaudRateTable[i])].BS1;
            BS2 = InitConfig.CAN_BS2 = CAN_BaudRateInitTab[CAN_GetBaudRateNum(BaudRateTable[i])].BS2;
            SJW = InitConfig.CAN_SJW = CAN_BaudRateInitTab[CAN_GetBaudRateNum(BaudRateTable[i])].SJW;
            InitConfig.CAN_Mode = 2;
            InitConfig.CAN_NART = 0;
            InitConfig.CAN_RFLM = 0;
            InitConfig.CAN_TXFP = 0;
            InitConfig.CAN_RELAY = 0;
            ret = VCI_InitCANEx(VCI_USBCAN2,DeviceIndex,ChannelIndex,&InitConfig);
            if(ret != STATUS_OK){
                QMessageBox::warning(this,"Warning","Initialization device faild!");
                on_stopAutoBaudratePushButton_clicked();
                return;
            }
            FilterConfig.Enable = 1;
            FilterConfig.ExtFrame = 0;
            FilterConfig.FilterIndex = 0;
            FilterConfig.FilterMode = 0;
            FilterConfig.ID_IDE = 0;
            FilterConfig.ID_RTR = 0;
            FilterConfig.ID_Std_Ext = 0;
            FilterConfig.MASK_IDE = 0;
            FilterConfig.MASK_RTR = 0;
            FilterConfig.MASK_Std_Ext = 0;
            ret = VCI_SetFilter(VCI_USBCAN2,DeviceIndex,ChannelIndex,&FilterConfig);
            if(ret != STATUS_OK){
                QMessageBox::warning(this,"Warning","Set filter faild!");
                on_stopAutoBaudratePushButton_clicked();
                return;
            }
            ret = VCI_StartCAN(VCI_USBCAN2,DeviceIndex,ChannelIndex);
            if(ret != STATUS_OK){
                QMessageBox::warning(this,"Warning","Start CAN faild!");
                on_stopAutoBaudratePushButton_clicked();
                return;
            }
            ErrCount = 0;
            QString BaudRateInfo;
            float site = (float)(1+BS1)/(1+BS1+BS2);
            float NewBaudTmp = BaudRateTable[i];
            BaudRateInfo.sprintf("%.3f Kbps|CAN_SJW_%dtq|CAN_BS1_%dtq|CAN_BS2_%dtq|%d|%.3f%%|%s",NewBaudTmp,SJW,BS1,BS2,BRP,site*100,"Matching...");
            displayAutoBaudRate(BaudRateInfo);
            QTime dieTime = QTime::currentTime().addMSecs(RecognizeTime);
            while( QTime::currentTime() < dieTime ){
                QCoreApplication::processEvents(QEventLoop::AllEvents);
            //for(j=0;j<RecognizeNum;j++){
                if(stopAutoBaudRate){
                    break;
                }
                ret = VCI_ReadCANStatus(VCI_USBCAN2,DeviceIndex,ChannelIndex,&CANStatus);
                if(ret != STATUS_OK){
                    QMessageBox::warning(this,"Warning","Read CAN status faild!");
                    on_stopAutoBaudratePushButton_clicked();
                    return;
                }
                ErrCount += CANStatus.regRECounter;//接收错误计数器
                if(ErrCount){
                    QString BaudRateInfo;
                    float site = (float)(1+BS1)/(1+BS1+BS2);
                    float NewBaudTmp = BaudRateTable[i];
                    BaudRateInfo.sprintf("%.3f Kbps|CAN_SJW_%dtq|CAN_BS1_%dtq|CAN_BS2_%dtq|%d|%.3f%%|%s",NewBaudTmp,SJW,BS1,BS2,BRP,site*100,"Don't Match");
                    displayAutoBaudRate(BaudRateInfo);
                    break;
                }
            }
            if(ErrCount==0){
                QString BaudRateInfo;
                float site = (float)(1+BS1)/(1+BS1+BS2);
                float NewBaudTmp = BaudRateTable[i];
                if(VCI_GetReceiveNum(VCI_USBCAN2,DeviceIndex,ChannelIndex) == 0){
                    BaudRateInfo.sprintf("%.3f Kbps|CAN_SJW_%dtq|CAN_BS1_%dtq|CAN_BS2_%dtq|%d|%.3f%%|%s",NewBaudTmp,SJW,BS1,BS2,BRP,site*100,"No data");
                }else{
                    BaudRateInfo.sprintf("%.3f Kbps|CAN_SJW_%dtq|CAN_BS1_%dtq|CAN_BS2_%dtq|%d|%.3f%%|%s",NewBaudTmp,SJW,BS1,BS2,BRP,site*100,"Matched");
                }
                displayAutoBaudRate(BaudRateInfo);
                break;
            }
        }
        VCI_ResetCAN(VCI_USBCAN2,DeviceIndex,ChannelIndex);
        on_stopAutoBaudratePushButton_clicked();
    }

}

void MainWindow::displayAutoBaudRate(QString displayString)
{
    QStringList colorList = QColor::colorNames();//获得QT所有知道名称的颜色名称列表
    QStringList dspStrList = displayString.split("|");
    int rowNum = ui->resultTableWidget->rowCount();
    //ui->resultTableWidget->setRowCount(rowNum+1);
    //ui->resultTableWidget->setRowHeight(rowNum,20);
    if(dspStrList.at(6) == "Matching..."){
        ui->resultTableWidget->setRowCount(rowNum+1);
        ui->resultTableWidget->setRowHeight(rowNum,20);
        for(int i=0;i<7;i++){
            QTableWidgetItem *item = new QTableWidgetItem(dspStrList.at(i));
            item->setForeground(QColor(colorList[82]));//146：黄色，82：绿色
            item->setBackground(QColor(colorList[7]));
            ui->resultTableWidget->setItem(rowNum,i,item);
        }
    }else{
        if(dspStrList.at(6) == "Don't Match"){
            for(int i=0;i<7;i++){
                QTableWidgetItem *item = new QTableWidgetItem(dspStrList.at(i));
                item->setForeground(QColor(colorList[146]));//146：黄色，82：绿色
                item->setBackground(QColor(colorList[7]));
                ui->resultTableWidget->setItem(rowNum-1,i,item);
            }
        }else if(dspStrList.at(6) == "Matched"){
            for(int i=0;i<7;i++){
                QTableWidgetItem *item = new QTableWidgetItem(dspStrList.at(i));
                item->setForeground(QColor(colorList[82]));//146：黄色，82：绿色
                item->setBackground(QColor(colorList[7]));
                ui->resultTableWidget->setItem(rowNum-1,i,item);
            }
        }else{
            for(int i=0;i<7;i++){
                QTableWidgetItem *item = new QTableWidgetItem(dspStrList.at(i));
                item->setForeground(QColor(colorList[119]));//146：黄色，82：红色
                item->setBackground(QColor(colorList[7]));
                ui->resultTableWidget->setItem(rowNum-1,i,item);
            }
        }
    }
    ui->resultTableWidget->scrollToBottom();
    QCoreApplication::processEvents(QEventLoop::AllEvents);
}

void MainWindow::mySleep(unsigned int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MainWindow::on_stopAutoBaudratePushButton_clicked()
{
    ui->deviceIndexComboBox->setEnabled(true);
    ui->channelIndexComboBox->setEnabled(true);
    ui->recognitionModeComboBox->setEnabled(true);
    if(ui->recognitionModeComboBox->currentText() == "standard"){
        ui->startBaudRateLineEdit->setDisabled(true);
        ui->endBaudRateLineEdit->setDisabled(true);
    }else{
        ui->startBaudRateLineEdit->setEnabled(true);
        ui->endBaudRateLineEdit->setEnabled(true);
    }
    ui->recognizeTimeLineEdit->setEnabled(true);
    ui->startAutoBaudratePushButton->setEnabled(true);
    ui->stopAutoBaudratePushButton->setDisabled(true);
    int DeviceIndex = ui->deviceIndexComboBox->currentText().toInt(NULL,0);
    VCI_CloseDevice(VCI_USBCAN2,DeviceIndex);
    stopAutoBaudRate = true;
}

void MainWindow::about()
{
    QString AboutStr;
    AboutStr = "Ginkgo USB-CAN AutoBaudRate 1.0.0\n";
#if defined(LANG_CN)
    AboutStr.append("内江纬图虚拟仪器有限公司 2009-2013 版权所有\n");
    AboutStr.append("支持硬件：Ginkgo 2 USB-CAN Adapter");
    QMessageBox::about(this,"关于Ginkgo USB-CAN AutoBaudRate",AboutStr);
#else
    AboutStr.append("ViewTool Virtual Instrument 2009-2013 Copyright\n");
    AboutStr.append("Hardware Suuport: Ginkgo 2 USB-CAN Adapter");
    QMessageBox::about(this,"About Ginkgo USB-CAN AutoBaudRate",AboutStr);
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

