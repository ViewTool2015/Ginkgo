#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "AM2311.h"
#include "windows.h"

static int panelHandle;
//定义函数指针，注意函数原型，形参。
typedef int (*I2CDLLFunction)(char *in,char *out);   
//定义dll中的函数指针
I2CDLLFunction I2C_OpenDevice,I2C_Init,I2C_WriteData,I2C_ReadData,I2C_SelectDevice,I2C_USB_EnumBoard;
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
        //取dll中的函数指针地址   
		I2C_OpenDevice = (I2CDLLFunction)GetProcAddress(hinstLib, "VTIF_OpenDevice");    
		I2C_ReadData = (I2CDLLFunction)GetProcAddress(hinstLib, "VTIF_I2C_ReadData");    //取函数指针地址  
		I2C_WriteData = (I2CDLLFunction)GetProcAddress(hinstLib, "VTIF_I2C_WriteData"); 
		I2C_Init = (I2CDLLFunction)GetProcAddress(hinstLib, "VTIF_I2C_Init");    //取函数指针地址   
		I2C_SelectDevice = (I2CDLLFunction)GetProcAddress(hinstLib, "VTIF_I2C_SelectDevice");    //取函数指针地址   
		I2C_USB_EnumBoard = (I2CDLLFunction)GetProcAddress(hinstLib, "VTIF_USB_EnumBoard");    //取函数指针地址    
	}else{
		MessagePopup("提示","加载动态库失败！");
	}
	
	if ((panelHandle = LoadPanel (0, "AM2311.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
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
			ret = I2C_OpenDevice(strTemp,outData);
			if(ret){
				MessagePopup("警告","打开设备失败！"); 
				return ret;
			}
			ret = I2C_SelectDevice(strTemp,outData);
			if(ret){
				MessagePopup("警告","选择设备失败！"); 
				return ret;
			}
			sprintf(strTemp,"0x01|100000|2|0|7|0"); 
		    ret = I2C_Init(strTemp,outData);
		    if(ret!=0){
				MessagePopup("警告","初始化设备失败！"); 
		        return ret;
		    }else{
				MessagePopup("提示","初始化设备成功！"); 
			}
			break;
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
			vt_board_num=I2C_USB_EnumBoard((char*)"10",outData);
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

int CVICALLBACK MeasureCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int ret,i;
	char *strTemp[64]={NULL};
	float temperature,humidity;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			// Wake up AM2311 sensor
			ret = I2C_WriteData("0xB8|0x00|0x00|0x00",outData); 
			//Send out read temperature and huminity command
			ret = I2C_WriteData("0xB8|0x00|0x00|0x03|0x00|0x04",outData);
			if(ret){
				SetCtrlAttribute(panelHandle,PANEL_TIMER_START,ATTR_ENABLED,0);
				SetCtrlAttribute(panelHandle,PANEL_COMMANDBUTTON_START,ATTR_LABEL_TEXT,"__StartMeasure");  
				MessagePopup("警告","请求读取温湿度数据失败！");  
				return ret;
			}
			// Read out temperature and huminity
			ret = I2C_ReadData("0xB8|0x00|0x00|0x08",outData);
			if(ret){
				SetCtrlAttribute(panelHandle,PANEL_TIMER_START,ATTR_ENABLED,0);
				SetCtrlAttribute(panelHandle,PANEL_COMMANDBUTTON_START,ATTR_LABEL_TEXT,"__StartMeasure");  
				MessagePopup("警告","读取温湿度数据失败！"); 
				return ret;
			}
			//将返回的数据字符串中的温度和湿度数据提取出来
			i=0;
	        strTemp[i]= (char *)strtok(outData,"|");
	        while(strTemp[i++]!=NULL) {
	            strTemp[i]=(char *)strtok(NULL,"|");
	        }
			humidity = ((strtol(strTemp[2],NULL,0)<<8)|(strtol(strTemp[3],NULL,0)))/10.0;
			temperature = ((strtol(strTemp[4],NULL,0)<<8)|(strtol(strTemp[5],NULL,0)))/10.0;  
			//显示测量值
			SetCtrlVal(panelHandle,PANEL_NUMERICMETER_TMP,temperature);
			SetCtrlVal(panelHandle,PANEL_NUMERICMETER_HUM,humidity); 
			break;
	}
	return 0;
}
