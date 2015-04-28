#include <cvirte.h>		
#include <userint.h>
#include "VoiceControl.h"
#include "windows.h"
#include <ansi_c.h>
#include <mmsystem.h>

static int panelHandle;
typedef int (*USARTDLLFunction)(char *in,char *out);   //定义函数指针，注意函数原型，形参。
//定义dll中的函数指针
USARTDLLFunction USART_OpenDevice,USART_Init,USART_ReadData,USART_WriteData,USART_USB_EnumBoard,USART_SelectDevice;
USARTDLLFunction GPIO_SelectDevice,GPIO_Init,GPIO_WriteData,GPIO_ReadData;
//定义函数操作数据缓冲区
char inData[10240],outData[102400];
int deviceNum=0;//设备数量

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	//加载dll文件并获取函数指针
    HMODULE hinstLib = LoadLibrary("Ginkgo_Driver.dll");    //装载动态链接库Ginkgo_Driver.dll
    if (hinstLib != NULL)                                  	//成功装载动态链接库Ginkgo_Driver.dll
    {
         USART_OpenDevice = (USARTDLLFunction)GetProcAddress(hinstLib, "VTIF_OpenDevice");    //取函数指针地址 
		 USART_ReadData = (USARTDLLFunction)GetProcAddress(hinstLib, "VTIF_USART_ReadData");    //取函数指针地址   
		 USART_WriteData = (USARTDLLFunction)GetProcAddress(hinstLib, "VTIF_USART_WriteData");    //取函数指针地址    
		 USART_Init = (USARTDLLFunction)GetProcAddress(hinstLib, "VTIF_USART_Init");    //取函数指针地址   
		 USART_SelectDevice = (USARTDLLFunction)GetProcAddress(hinstLib, "VTIF_USART_SelectDevice");    //取函数指针地址   
		 USART_USB_EnumBoard = (USARTDLLFunction)GetProcAddress(hinstLib, "VTIF_USB_EnumBoard");    //取函数指针地址   
		 GPIO_SelectDevice = (USARTDLLFunction)GetProcAddress(hinstLib, "VTIF_GPIO_SelectDevice");
		 GPIO_Init = (USARTDLLFunction)GetProcAddress(hinstLib, "VTIF_GPIO_Init");
		 GPIO_WriteData = (USARTDLLFunction)GetProcAddress(hinstLib, "VTIF_GPIO_WriteData"); 
		 GPIO_ReadData = (USARTDLLFunction)GetProcAddress(hinstLib, "VTIF_GPIO_ReadData"); 
	}else{
		MessagePopup("提示","加载动态库失败！");
	}
	if ((panelHandle = LoadPanel (0, "VoiceControl.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK RingDeviceCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			break;
	}
	return 0;
}

int CVICALLBACK InitDeviceCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char strTemp[100]={0};
	int deviceIndex=0;
	int ret;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlIndex(panelHandle,PANEL_RING_DEVICE,&deviceIndex);
			sprintf(strTemp,"%d",deviceIndex);
			ret = USART_OpenDevice(strTemp,outData);
			if(ret){
				MessagePopup("警告","打开设备失败！"); 
				return ret;
			}
			ret = USART_SelectDevice(strTemp,outData);
			if(ret){
				MessagePopup("警告","选择设备失败！"); 
				return ret;
			}
			GPIO_SelectDevice(strTemp,outData);   
			sprintf(strTemp,"1|9600|0|0|0|232"); 
		    ret = USART_Init(strTemp,outData);
		    if(ret!=0){
				MessagePopup("警告","初始化设备失败！"); 
		        return ret;
		    }else{
				MessagePopup("提示","初始化设备成功！"); 
			}
			GPIO_Init("0x0001|0x0000",outData);
			break;
	}
	return 0;
}

int CVICALLBACK StartMeasureCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	static int flag=0;
	switch (event)
	{
		case EVENT_COMMIT:
			if(flag==0){
				SetCtrlAttribute(panelHandle,PANEL_TIMER_START,ATTR_ENABLED,1);
				SetCtrlAttribute(panelHandle,PANEL_COMMANDBUTTON_START,ATTR_LABEL_TEXT,"__StopMeasure");  
				flag = 1;
			}else{
				SetCtrlAttribute(panelHandle,PANEL_TIMER_START,ATTR_ENABLED,0);
				SetCtrlAttribute(panelHandle,PANEL_COMMANDBUTTON_START,ATTR_LABEL_TEXT,"__StartMeasure"); 
				flag = 0;
			}
			break;
	}
	return 0;
}

int CVICALLBACK QuitCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK ScanDeviceCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
    char deviceInfo[1024]={0};
    int vt_board_num,i;
    char *strTemp[64]={NULL};
    char getDataTmp[512]={0};
	char deviceName[100]={0};
	switch (event)
	{
		case EVENT_TIMER_TICK:
			vt_board_num=USART_USB_EnumBoard((char*)"10",outData);
	        strcpy(getDataTmp,outData);
	        i=0;
	        strTemp[i]= (char *)strtok(getDataTmp,"|");
	        while(strTemp[i++]!=NULL) {
	            strTemp[i]=(char *)strtok(NULL,"|");
	        }
	        if(vt_board_num!=deviceNum){//增加一个设备
				ClearListCtrl (panelHandle,PANEL_RING_DEVICE);//清除之前罗列的设备
	            for(i=0;i<vt_board_num;i++){
	                sprintf(deviceName,"DEVICE %d",(int)strtol(strTemp[i]+2,NULL,0));
					InsertListItem(panelHandle,PANEL_RING_DEVICE,i,deviceName,i);//增加一个设备列表
	            }
	            if(vt_board_num>0){
	            }else{
	                InsertListItem(panelHandle,PANEL_RING_DEVICE,0,"NO DEVICE",0);//没有设备连接
	            }
	        }
	        deviceNum=vt_board_num;
			break;
	}
	return 0;
}
void displayData(char *indata)
{
	int data=0;
	SetCtrlVal(panelHandle,PANEL_COMMAND,indata);
	data = (int)strtol(indata,NULL,0); 
	switch(data)
	{
		case 0x00:SetCtrlVal(panelHandle,PANEL_LED_1,1);break;//开灯1
		case 0x01:SetCtrlVal(panelHandle,PANEL_LED_2,1);break;//开灯2  
		case 0x02:SetCtrlVal(panelHandle,PANEL_LED_3,1);break;//开灯3  
		case 0x03:SetCtrlVal(panelHandle,PANEL_LED_4,1);break;//开灯4  
		case 0x04:SetCtrlVal(panelHandle,PANEL_LED_5,1);break;//开灯5  
		case 0x05:SetCtrlVal(panelHandle,PANEL_LED_6,1);break;//开灯6  
		case 0x06:SetCtrlVal(panelHandle,PANEL_LED_7,1);break;//开灯7  
		case 0x07:SetCtrlVal(panelHandle,PANEL_LED_8,1);break;//开灯8  
		case 0x08:SetCtrlVal(panelHandle,PANEL_LED_9,1);break;//开灯9  
		case 0x09:SetCtrlVal(panelHandle,PANEL_LED_10,1);break;//开灯10  
		
		case 0x0A:SetCtrlVal(panelHandle,PANEL_LED_1,0);break;//关灯1
		case 0x0B:SetCtrlVal(panelHandle,PANEL_LED_2,0);break;//关灯2   
		case 0x0C:SetCtrlVal(panelHandle,PANEL_LED_3,0);break;//关灯3   
		case 0x0D:SetCtrlVal(panelHandle,PANEL_LED_4,0);break;//关灯4   
		case 0x0E:SetCtrlVal(panelHandle,PANEL_LED_5,0);break;//关灯5     
		case 0x0F:SetCtrlVal(panelHandle,PANEL_LED_6,0);break;//关灯6     
		case 0x10:SetCtrlVal(panelHandle,PANEL_LED_7,0);break;//关灯7     
		case 0x11:SetCtrlVal(panelHandle,PANEL_LED_8,0);break;//关灯8     
		case 0x12:SetCtrlVal(panelHandle,PANEL_LED_9,0);break;//关灯9     
		case 0x13:SetCtrlVal(panelHandle,PANEL_LED_10,0);break;//关灯10    
		
		case 0x17://全部开启
			SetCtrlVal(panelHandle,PANEL_LED_1,1);
			SetCtrlVal(panelHandle,PANEL_LED_2,1);  
			SetCtrlVal(panelHandle,PANEL_LED_3,1);  
			SetCtrlVal(panelHandle,PANEL_LED_4,1);  
			SetCtrlVal(panelHandle,PANEL_LED_5,1);  
			SetCtrlVal(panelHandle,PANEL_LED_6,1);  
			SetCtrlVal(panelHandle,PANEL_LED_7,1);  
			SetCtrlVal(panelHandle,PANEL_LED_8,1);  
			SetCtrlVal(panelHandle,PANEL_LED_9,1);  
			SetCtrlVal(panelHandle,PANEL_LED_10,1); 
			break;
		case 0x18://全部关闭
			SetCtrlVal(panelHandle,PANEL_LED_1,0);
			SetCtrlVal(panelHandle,PANEL_LED_2,0);  
			SetCtrlVal(panelHandle,PANEL_LED_3,0);  
			SetCtrlVal(panelHandle,PANEL_LED_4,0);  
			SetCtrlVal(panelHandle,PANEL_LED_5,0);  
			SetCtrlVal(panelHandle,PANEL_LED_6,0);  
			SetCtrlVal(panelHandle,PANEL_LED_7,0);  
			SetCtrlVal(panelHandle,PANEL_LED_8,0);  
			SetCtrlVal(panelHandle,PANEL_LED_9,0);  
			SetCtrlVal(panelHandle,PANEL_LED_10,0); 
			break;
		case 0x16:DisplayImageFile(panelHandle,PANEL_PICTURE,"Desert.jpg");break; //显示图片
		case 0x15:sndPlaySound ("Mode_Story.wav", 3);break;//讲个故事 
		case 0x14:sndPlaySound ("Mode_Song.wav", 3);break;//唱歌个吧    
		case 0x19:sndPlaySound ("Story1.wav", 3);break;//第一个	
		case 0x1A:sndPlaySound ("Story2.wav", 3);break;//第二个	
		case 0x1B:sndPlaySound ("Story3.wav", 3);break;//第三个	
		default:break;
	}
}

int CVICALLBACK MeasureCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char strTemp[100]={0}; 
	int ret,GPIO_Data;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			GPIO_ReadData("0x0001",outData);
			GPIO_Data = (int)strtol(outData,NULL,0); 
			if(GPIO_Data){
				SetCtrlVal(panelHandle,PANEL_LED_BUSY,1);
				return 0;
			}else{
				SetCtrlVal(panelHandle,PANEL_LED_BUSY,0); 
			}
			ret = USART_ReadData("",outData);
			if(ret){
				return ret;
			}else{
				displayData(outData);
			}
			break;
	}
	return 0;
}
