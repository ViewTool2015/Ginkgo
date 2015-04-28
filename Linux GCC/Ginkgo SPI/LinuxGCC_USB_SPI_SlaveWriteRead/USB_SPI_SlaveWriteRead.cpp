  /*
  ******************************************************************************
  * @file     : USB_SPI_SlaveWriteRead.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/18 9:10
  * @brief    : USB_SPI_SlaveWriteRead demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

  /*
  Hardware Connection

  No hardware connected

  */

#include <stdio.h>
#include "ControlSPI.h"

#define	SLAVE_WRITE	0
#define SLAVE_READ	1
#define DEVICE_INDEX	0

int main(int argc, const char* argv[])
{
    int ret;
	VSI_INIT_CONFIG SPI_Config;
	VSI_BOARD_INFO BoardInfo;
    // Scan device
    ret = VSI_ScanDevice(1);
    if (ret <= 0){
        printf("No device connect!\n");
        return ret;
    }
    // Open device
    ret = VSI_OpenDevice(VSI_USBSPI, DEVICE_INDEX, 0);
    if (ret != ERR_SUCCESS){
        printf("Open device error!\n");
        return ret;
    }
	// Get board information
	ret = VSI_ReadBoardInfo(DEVICE_INDEX,&BoardInfo);
    if (ret != ERR_SUCCESS){
        printf("Read board information error!\n");
        return ret;
    }else{
		printf("Product Name:%s\n",BoardInfo.ProductName);
		printf("Firmware Version:V%d.%d.%d\n",BoardInfo.FirmwareVersion[1],BoardInfo.FirmwareVersion[2],BoardInfo.FirmwareVersion[3]);
		printf("Hardware Version:V%d.%d.%d\n",BoardInfo.HardwareVersion[1],BoardInfo.HardwareVersion[2],BoardInfo.HardwareVersion[3]);
		printf("Serial Number:");
		for(int i=0;i<12;i++){
			printf("%02X",BoardInfo.SerialNumber[i]);
		}
		printf("\n");
	}
    // Initializes the device
    SPI_Config.ControlMode = 1;
    SPI_Config.MasterMode = 0;
    SPI_Config.ClockSpeed = 36000000;
    SPI_Config.CPHA = 0;
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8;
	SPI_Config.SelPolarity = 0;
    ret = VSI_InitSPI(VSI_USBSPI, DEVICE_INDEX, &SPI_Config);
    if (ret != ERR_SUCCESS){
        printf("Initialize device error!\n");
        return ret;
    }
	uint8_t write_buffer[10240];
	uint8_t read_buffer[10240];
#if SLAVE_WRITE
	for(int i=0;i<64;i++){
		write_buffer[i] = i;
	}
	ret = VSI_SlaveWriteBytes(VSI_USBSPI,DEVICE_INDEX,write_buffer,64);
	if(ret != ERR_SUCCESS){
		printf("Slave write data error!\n");
        return ret;
	}
#endif
#if SLAVE_READ
	// Get data
	while(1)
	{
		int read_data_num = 0;
		ret = VSI_SlaveReadBytes(VSI_USBSPI, DEVICE_INDEX, read_buffer, &read_data_num,10);
		if (ret != ERR_SUCCESS){
			printf("Read data error!\n");
			return ret;
		}else{
			if (read_data_num > 0){
				printf("Read data(Hex):\n");
				for (int i = 0; i < read_data_num; i++){
					printf("%02X ",read_buffer[i]);
				}
				printf("\n");
			}
		}
		usleep(100*1000);
	}
#endif
	return 0;
}

