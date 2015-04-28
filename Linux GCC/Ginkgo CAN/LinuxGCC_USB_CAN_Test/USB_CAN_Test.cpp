  /*
  ******************************************************************************
  * @file     : CANTest.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 9:27
  * @brief    : CANTest demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif
#include "ControlCAN.h"
#include <stdio.h>

#define	CAN_MODE_LOOP_BACK		0
#define	CAN_SEND_DATA			1
#define CAN_GET_BOARD_INFO		1
#define CAN_READ_DATA			1
#define CAN_CALLBACK_READ_DATA	0
#define	CAN_INIT_EX				1
#define CAN_GET_STATUS			1


#if CAN_CALLBACK_READ_DATA
void WINAPI GetDataCallback(uint32_t DevIndex,uint32_t CANIndex,uint32_t Len)
{
	int ReadDataNum;
	int DataNum = VCI_GetReceiveNum(VCI_USBCAN2, 0, 0);
	VCI_CAN_OBJ	*pCAN_ReceiveData = (VCI_CAN_OBJ *)malloc(DataNum*sizeof(VCI_CAN_OBJ));
    if((DataNum > 0)&&(pCAN_ReceiveData != NULL)){
        ReadDataNum = VCI_Receive(VCI_USBCAN2, 0, 0, pCAN_ReceiveData, DataNum);
        for (int i = 0; i < ReadDataNum; i++)
        {
			printf("\n");
            printf("--CAN_ReceiveData.RemoteFlag = %d\n",pCAN_ReceiveData[i].RemoteFlag);
            printf("--CAN_ReceiveData.ExternFlag = %d\n",pCAN_ReceiveData[i].ExternFlag);
            printf("--CAN_ReceiveData.ID = 0x%X\n",pCAN_ReceiveData[i].ID);
            printf("--CAN_ReceiveData.DataLen = %d\n",pCAN_ReceiveData[i].DataLen);
            printf("--CAN_ReceiveData.Data:");
            for(int j=0;j<pCAN_ReceiveData[i].DataLen;j++){
                printf("%02X ",pCAN_ReceiveData[i].Data[j]);
            }
            printf("\n");
            printf("--CAN_ReceiveData.TimeStamp = %d\n\n",pCAN_ReceiveData[i].TimeStamp);
        }
    }
	free(pCAN_ReceiveData);
}
#endif

int main(void)
{
    int DevNum,Status;  
    //Scan device
    DevNum = VCI_ScanDevice(1);
    if(DevNum > 0){
        printf("Have %d device connected!\n",DevNum);
    }else{
        printf("No device connected!\n");
        return 0;
    }
    //Get board info
#if CAN_GET_BOARD_INFO
	VCI_BOARD_INFO_EX CAN_BoardInfo;
    Status = VCI_ReadBoardInfoEx(0,&CAN_BoardInfo);//It will open device
    if(Status==STATUS_ERR){
        printf("Get board info failed!\n");
        return 0;
    }else{
        printf("--CAN_BoardInfo.ProductName = %s\n",CAN_BoardInfo.ProductName);
        printf("--CAN_BoardInfo.FirmwareVersion = V%d.%d.%d\n",CAN_BoardInfo.FirmwareVersion[1],CAN_BoardInfo.FirmwareVersion[2],CAN_BoardInfo.FirmwareVersion[3]);
        printf("--CAN_BoardInfo.HardwareVersion = V%d.%d.%d\n",CAN_BoardInfo.HardwareVersion[1],CAN_BoardInfo.HardwareVersion[2],CAN_BoardInfo.HardwareVersion[3]);
        printf("--CAN_BoardInfo.SerialNumber = %08X%08X%08X\n",(uint32_t)(*(uint32_t*)(&CAN_BoardInfo.SerialNumber[0])),(uint32_t)(*(uint32_t*)(&CAN_BoardInfo.SerialNumber[4])),(uint32_t)(*(uint32_t*)(&CAN_BoardInfo.SerialNumber[8])));
    }
#else
    //Open device
    Status = VCI_OpenDevice(VCI_USBCAN2,0,0);
    if(Status==STATUS_ERR){
        printf("Open device failed!\n");
        return 0;
    }else{
        printf("Open device success!\n");
    }
#endif
#if CAN_INIT_EX
	VCI_INIT_CONFIG_EX	CAN_InitEx;
    //Config device
    CAN_InitEx.CAN_ABOM = 0;
#if CAN_MODE_LOOP_BACK
    CAN_InitEx.CAN_Mode = 1;
#else
    CAN_InitEx.CAN_Mode = 0;
#endif
    //1Mbps
    CAN_InitEx.CAN_BRP = 54;//6;
    CAN_InitEx.CAN_BS1 = 6;//3;
    CAN_InitEx.CAN_BS2 = 1;//2;
    CAN_InitEx.CAN_SJW = 1;

    CAN_InitEx.CAN_NART = 0;
    CAN_InitEx.CAN_RFLM = 0;
    CAN_InitEx.CAN_TXFP = 1;
	CAN_InitEx.CAN_RELAY = 0;
    Status = VCI_InitCANEx(VCI_USBCAN2,0,0,&CAN_InitEx);
    if(Status==STATUS_ERR){
        printf("Init device failed!\n");
        return 0;
    }else{
        printf("Init device success!\n");
    }
    //Set filter
	VCI_FILTER_CONFIG CAN_FilterConfig;
    CAN_FilterConfig.FilterIndex = 0;
    CAN_FilterConfig.Enable = 1;		//Enable
    CAN_FilterConfig.ExtFrame = 0;
    CAN_FilterConfig.FilterMode = 0;
    CAN_FilterConfig.ID_IDE = 0;
    CAN_FilterConfig.ID_RTR = 0;
    CAN_FilterConfig.ID_Std_Ext = 0;
    CAN_FilterConfig.MASK_IDE = 0;
    CAN_FilterConfig.MASK_RTR = 0;
    CAN_FilterConfig.MASK_Std_Ext = 0;
    Status = VCI_SetFilter(VCI_USBCAN2,0,0,&CAN_FilterConfig);
    if(Status==STATUS_ERR){
        printf("Set filter failed!\n");
        return 0;
    }else{
        printf("Set filter success!\n");
    }
#else
	VCI_INIT_CONFIG	CAN_Init;
    //Config device
	CAN_Init.AccCode = 0x00000000;
	CAN_Init.AccMask = 0xFFFFFFFF;
	CAN_Init.Filter = 1;
	CAN_Init.Mode = 0;
	CAN_Init.Timing0 = 0x00;
	CAN_Init.Timing1 = 0x14;
    Status = VCI_InitCAN(VCI_USBCAN2,0,0,&CAN_Init);
    if(Status==STATUS_ERR){
        printf("Init device failed!\n");
        return 0;
    }else{
        printf("Init device success!\n");
    }
#endif
	//Register callback function
#if CAN_CALLBACK_READ_DATA
	VCI_RegisterReceiveCallback(0,GetDataCallback);
#endif
    //Start CAN
    Status = VCI_StartCAN(VCI_USBCAN2,0,0);
    if(Status==STATUS_ERR){
        printf("Start CAN failed!\n");
        return 0;
    }else{
        printf("Start CAN success!\n");
    }
    //Send data
#if CAN_SEND_DATA
	VCI_CAN_OBJ	CAN_SendData[2];
	for(int j=0;j<2;j++){
		CAN_SendData[j].DataLen = 8;
		for(int i=0;i<CAN_SendData[j].DataLen;i++){
			CAN_SendData[j].Data[i] = 0x55;
		}
		CAN_SendData[j].ExternFlag = 0;
		CAN_SendData[j].RemoteFlag = 0;
		CAN_SendData[j].ID = 0x155+j;
#if CAN_MODE_LOOP_BACK
		CAN_SendData[j].SendType = 2;
#else
		CAN_SendData[j].SendType = 0;
#endif//CAN_MODE_LOOP_BACK
	}
    Status = VCI_Transmit(VCI_USBCAN2,0,0,CAN_SendData,2);
    if(Status==STATUS_ERR){
        printf("Send CAN data failed!\n");
        VCI_ResetCAN(VCI_USBCAN2,0,0);
    }else{
        printf("Send CAN data success!\n");
    }
#endif//CAN_SEND_DATA

#ifndef OS_UNIX
    Sleep(10);
#else
	sleep(0.01);
#endif
#if CAN_GET_STATUS
            VCI_CAN_STATUS CAN_Status;
            Status = VCI_ReadCANStatus(VCI_USBCAN2, 0, 0, &CAN_Status);
            if (Status == STATUS_ERR)
            {
                printf("Get CAN status failed!\n");
                return 1;
            }
            else
            {
                printf("Buffer Size : %d\n",CAN_Status.BufferSize);
                printf("ESR : 0x%08X\n" ,CAN_Status.regESR);
                printf("------Error warning flag : %d\n" ,((CAN_Status.regESR>>0)&0x01));
                printf("------Error passive flag : %d\n" , ((CAN_Status.regESR >> 1) & 0x01));
                printf("------Bus-off flag : %d\n" , ((CAN_Status.regESR >> 2) & 0x01));
                printf("------Last error code(%d) : ",(CAN_Status.regESR>>4)&0x07);
                switch ((CAN_Status.regESR>>4)&0x07)
                {
                    case 0:
                        printf("No Error\n");
                        break;
                    case 1:
                        printf("Stuff Error\n");
                        break;
                    case 2:
                        printf("Form Error\n");
                        break;
                    case 3:
                        printf("Acknowledgment Error\n");
                        break;
                    case 4:
                        printf("Bit recessive Error\n");
                        break;
                    case 5:
                        printf("Bit dominant Error\n");
                        break;
                    case 6:
                        printf("CRC Error\n");
                        break;
                    case 7:
                        printf("Set by software\n");
                        break;
                    default:
                        break;
                }
                printf("------Transmit error counter : %d\n" , ((CAN_Status.regESR >> 16) & 0xFF));
                printf("------Receive error counter : %d\n" , ((CAN_Status.regESR >> 24) & 0xFF));
                printf("TSR : 0x%08X\n" , CAN_Status.regTSR);

            }
#endif

#if CAN_READ_DATA
	while(1){
		int ReadDataNum;
		int DataNum = VCI_GetReceiveNum(VCI_USBCAN2, 0, 0);
		VCI_CAN_OBJ	*pCAN_ReceiveData = (VCI_CAN_OBJ *)malloc(DataNum*sizeof(VCI_CAN_OBJ));
		if((DataNum > 0)&&(pCAN_ReceiveData != NULL)){
			ReadDataNum = VCI_Receive(VCI_USBCAN2, 0, 0, pCAN_ReceiveData, DataNum);
			for (int i = 0; i < ReadDataNum; i++)
			{
				printf("\n");
				printf("--CAN_ReceiveData.RemoteFlag = %d\n",pCAN_ReceiveData[i].RemoteFlag);
				printf("--CAN_ReceiveData.ExternFlag = %d\n",pCAN_ReceiveData[i].ExternFlag);
				printf("--CAN_ReceiveData.ID = 0x%X\n",pCAN_ReceiveData[i].ID);
				printf("--CAN_ReceiveData.DataLen = %d\n",pCAN_ReceiveData[i].DataLen);
				printf("--CAN_ReceiveData.Data:");
				for(int j=0;j<pCAN_ReceiveData[i].DataLen;j++){
					printf("%02X ",pCAN_ReceiveData[i].Data[j]);
				}
				printf("\n");
				printf("--CAN_ReceiveData.TimeStamp = %d\n\n",pCAN_ReceiveData[i].TimeStamp);
			}
		}
		free(pCAN_ReceiveData);
	}
#endif
    while(getchar()!='\n');
#if CAN_CALLBACK_READ_DATA
	VCI_LogoutReceiveCallback(0);
	printf("VCI_LogoutReceiveCallback\n");
#endif
#ifndef OS_UNIX
	Sleep(10);
#else
	usleep(10*1000);
#endif
	//Stop receive can data
	Status = VCI_ResetCAN(VCI_USBCAN2,0,0);
	printf("VCI_ResetCAN %d\n",Status);
	VCI_CloseDevice(VCI_USBCAN2,0);
	printf("VCI_CloseDevice\n");
    return 0;
}
