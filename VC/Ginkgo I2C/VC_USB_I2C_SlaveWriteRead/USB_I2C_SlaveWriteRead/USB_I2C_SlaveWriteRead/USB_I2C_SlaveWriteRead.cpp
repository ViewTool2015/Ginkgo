  /*
  ******************************************************************************
  * @file     : USB_I2C_SlaveWriteRead.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/19 11:49
  * @brief    : USB_I2C_SlaveWriteRead demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

#include "stdafx.h"
#include "ControlI2C.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int ret,i;
	VII_INIT_CONFIG I2C_Config;
	VII_BOARD_INFO BoardInfo;
	uint16_t SlaveReadLen;
	uint8_t write_buffer[512]={0};
	uint8_t	read_buffer[512]={0};
	//Scan device
	ret = VII_ScanDevice(1);
	if(ret <= 0){
		printf("No device connect!\n");
		return ret;
	}
    //Open device
    ret = VII_OpenDevice(VII_USBI2C, 0, 0);
    if (ret != ERR_SUCCESS){
        printf("Open device error!\n");
        return ret;
    }
	//Get product information
	ret = VII_ReadBoardInfo(0,&BoardInfo);
    if (ret != ERR_SUCCESS){
        printf("Read board information error!\n");
        return ret;
    }else{
		printf("Product Name:%s\n",BoardInfo.ProductName);
		printf("Firmware Version:V%d.%d.%d\n",BoardInfo.FirmwareVersion[1],BoardInfo.FirmwareVersion[2],BoardInfo.FirmwareVersion[3]);
		printf("Hardware Version:V%d.%d.%d\n",BoardInfo.HardwareVersion[1],BoardInfo.HardwareVersion[2],BoardInfo.HardwareVersion[3]);
		printf("Serial Number:");
		for(i=0;i<12;i++){
			printf("%02X",BoardInfo.SerialNumber[i]);
		}
		printf("\n");
	}
    //Initializes the device
    I2C_Config.AddrType = VII_ADDR_7BIT;
    I2C_Config.ClockSpeed = 400000;
    I2C_Config.ControlMode = VII_HCTL_SLAVE_MODE;
    I2C_Config.MasterMode = VII_SLAVE;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_NONE;
	I2C_Config.Addr = 0xA0;	//Must be set,very important
    ret = VII_InitI2C(VII_USBI2C, 0, 0, &I2C_Config);
    if (ret != ERR_SUCCESS){
        printf("Initialize device error!\n");
        return ret;
    }
	//Slave write data
	for (i = 0; i < 16; i++){
        write_buffer[i] = i;
    }
    ret = VII_SlaveWriteBytes(VII_USBI2C, 0, 0, write_buffer, 16);
    if (ret != ERR_SUCCESS){
        printf("Slave write data error!\n");
        return ret;
    }else{
		printf("Write Data:\n");
		for(i=0;i<16;i++){
			printf("%02X ",write_buffer[i]);
			if(((i+1)%16)==0){
				printf("\n");
			}
		}
	}
	//Slave read data
	while(1){
		Sleep(50);
		ret = VII_SlaveReadBytes(VII_USBI2C, 0, 0,read_buffer,&SlaveReadLen);
		if((ret == ERR_SUCCESS)&&(SlaveReadLen > 0)){
			printf("Read Data:\n");
			for(i=0;i<SlaveReadLen;i++){
				printf("%02X ",read_buffer[i]);
				if(((i+1)%16)==0){
					printf("\n");
				}
			}
			printf("\n");
			continue;
		}
		if(ret == ERR_READ_NO_DATA){
			continue;
		}else{
			printf("Slave read data error!\n");
			return ret;
		}
	}
	return 0;
}

