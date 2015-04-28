  /*
  ******************************************************************************
  * @file       USB_SPI_BlockWriteRead.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/17 17:27
  * @brief    : USB_SPI_BlockWriteRead demo
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

#include "stdafx.h"
#include "ControlSPI.h"


int _tmain(int argc, _TCHAR* argv[])
{
    int ret;
	VSI_INIT_CONFIG SPI_Config;
	VSI_BOARD_INFO BoardInfo;
    // Scan connected device 
    ret = VSI_ScanDevice(1);
    if (ret <= 0){
        printf("No device connect!\n");
        return ret;
    }
    // Open device
    ret = VSI_OpenDevice(VSI_USBSPI, 0, 0);
    if (ret != ERR_SUCCESS){
        printf("Open device error!\n");
        return ret;
    }
	// Get product information
	ret = VSI_ReadBoardInfo(0,&BoardInfo);
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
    // Initialize device(Master Mode, Hardware SPI, Half-Duplex)
    SPI_Config.ControlMode = 1;
    SPI_Config.MasterMode = 1;
    SPI_Config.ClockSpeed = 36000000;
    SPI_Config.CPHA = 0;
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8;
	SPI_Config.SelPolarity = 0;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS){
        printf("Initialize device error!\n");
        return ret;
    }
	uint8_t write_buffer[10240];
	uint8_t read_buffer[10240];
	int block_num = 5;
	int block_size = 2;
	int IntervalTime = 100;

    // Block mode write data, data will be send out block_num time(s), send out block_size bytes of data every time
    // Set CS to low before send data and set CS to high after send data complete when every time send data
    // Each time send data, interval is IntervalTime us, that is CS set to high and hold IntervalTime us
    for (int i = 0; i < block_num; i++){
		for(int j=0;j<block_size;j++){
			write_buffer[i*block_size+j] = i*j;
		}
    }
    ret = VSI_BlockWriteBytes(VSI_USBSPI, 0, 0, write_buffer, block_size,block_num,IntervalTime);
    if (ret != ERR_SUCCESS){
        printf("Block write data error!\n");
        return ret;
    }

    // Block mode read data: same as write data
	ret = VSI_BlockReadBytes(VSI_USBSPI, 0, 0, read_buffer, block_size,block_num,IntervalTime);
	if (ret != ERR_SUCCESS){
		printf("Block read data error!\n");
		return ret;
	}else{
		printf("Read data(Hex):\n");
		for (int i = 0; i < block_num; i++){
			for(int j=0;j<block_size;j++){
				printf("%02X ",read_buffer[i*block_size+j]);
			}
			printf("\n");
		}
	}

    // Block mode: write and read data
	int write_block_size = 1;
	int read_block_size = 2;
	for (int i = 0; i < block_num; i++){
		for(int j=0;j<write_block_size;j++){
			write_buffer[i*write_block_size+j] = i*j;
		}
	}
	ret = VSI_BlockWriteReadBytes(VSI_USBSPI, 0, 0, write_buffer, write_block_size,read_buffer,read_block_size,block_num,IntervalTime);
	if (ret != ERR_SUCCESS){
		printf("Block write read data error!\n");
		return ret;
	}else{
		printf("Read data(Hex):\n");
		for (int i = 0; i < block_num; i++){
			for(int j=0;j<read_block_size;j++){
				printf("%02X ",read_buffer[i*read_block_size+j]);
			}
			printf("\n");
		}
	}
	getchar();
	return 0;
}

