  /*
  ******************************************************************************
  * @file     : USB_SPI_Test.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/18 9:55
  * @brief    : USB_SPI_Test demo
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
#ifndef OS_UNIX  
#include "stdafx.h"
#endif
#include "ControlSPI.h"
#include <stdio.h>

int main(int argc, char* argv[])
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
    // Write 8-bits data(Master Mode, Hardware SPI, Half-Duplex)
    for (int i = 0; i < 8; i++){
        write_buffer[i] = i;
    }
    ret = VSI_WriteBytes(VSI_USBSPI, 0, 0, write_buffer, 8);
    if (ret != ERR_SUCCESS){
        printf("Write data error!\n");
        return ret;
    }
    // Read 8-bits data(Master Mode, Hardware SPI, Half-Duplex)
    ret = VSI_ReadBytes(VSI_USBSPI, 0, 0, read_buffer, 8);
    if (ret != ERR_SUCCESS){
        printf("Read data error!\n");
        return ret;
    }else{
        printf("Read data(Hex):");
        for (int i = 0; i < 8; i++){
            printf("%02X ",read_buffer[i]);
        }
		printf("\n");
    }
    // Write and read data(Master Mode, Hardware SPI, Half-Duplex),
    //  write 5-bits data and read 8-bits data immediately with chip select
    for (int i = 0; i < 5; i++){
        write_buffer[i] = i;
    }
    ret = VSI_WriteReadBytes(VSI_USBSPI, 0, 0, write_buffer, 5, read_buffer, 8);
    if (ret != ERR_SUCCESS){
        printf("WriteRead data error!\n");
        return ret;
    }else{
        printf("Read data(Hex):");
        for (int i = 0; i < 8; i++){
            printf("%02X ",read_buffer[i]);
        }
		printf("\n");
    }
    // Initialize device(Master Mode, Software SPI, Half-Duplex)
    //  function VSI_WriteBytes£¬VSI_ReadBytes£¬VSI_WriteReadBytes can be support in software SPI mode
    // Hardware SPI cannot support function VSI_WriteBits,VSI_ReadBits,VSI_WriteReadBits, but can be used in 1-wared mode 
    SPI_Config.ControlMode = 2;
    SPI_Config.MasterMode = 1;
    SPI_Config.ClockSpeed = 100000;
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
    // Write data with binary, up to 10240 bits
    ret = VSI_WriteBits(VSI_USBSPI, 0, 0, (uint8_t *)"10110100100101");// Write 14-bits data
    if (ret != ERR_SUCCESS){
        printf("Write bit error!\n");
        return ret;
    }
    // Read data with binary, up to 10240 bits
	uint8_t read_buffer_bin[10240] = {0};
    ret = VSI_ReadBits(VSI_USBSPI, 0, 0, read_buffer_bin,19);// Read 19-bits data
    if (ret != ERR_SUCCESS){
        printf("Read bit error!\n");
        return ret;
    }else{
        printf("Read bits:");
        printf("%s",read_buffer_bin);
		printf("\n");
    }
    // Read and write data with binary
    ret = VSI_WriteReadBits(VSI_USBSPI, 0, 0,(uint8_t *)"000011110101001",read_buffer_bin, 25);// Read 19-bits data
    if (ret != ERR_SUCCESS){
        printf("Write bit error!\n");
        return ret;
    }else{
        printf("Read bits:");
        printf("%s\n",read_buffer_bin);
    }
    // Initialize device(Slave Mode, Hardware SPI, Full-Duplex)
    SPI_Config.ControlMode = 0; 
    SPI_Config.MasterMode = 0;  // Slave Mode
    SPI_Config.CPHA = 0;        // Clock Polarity and Phase must be same as master
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8;    // Support 8bit mode only
	SPI_Config.SelPolarity = 0;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS){
        printf("Initialize device error!\n");
        return ret;
    }
    // Write data in slave mode(call function VSI_SlaveWriteBytes), data will not send out via MISO pin immediately until chip select by master,
    //  function VSI_SlaveWriteBytes return immediately after called, the data stored in adapter memory buffer
    for (int i = 0; i < 8; i++){
        write_buffer[i] = i;
    }
    ret = VSI_SlaveWriteBytes(VSI_USBSPI, 0, write_buffer, 8);
    if (ret != ERR_SUCCESS){
        printf("Write data error!\n");
        return ret;
    }
    // Write data in slave mode: last parameter(100) is waiting time(in us),
    //  function will return immediately if time-out(no matter whether read the data or not)
    // Function judge received number of data via parameter read_data_num
    // ATTENTION: After call function VSI_SlaveWriteBytes,
    //  slave will receive the data when master read data(slave in full-duplex)
    //  master can call function VSI_SlaveReadBytes to discard received data after read the data complete from slave
    int read_data_num = 0;
    ret = VSI_SlaveReadBytes(VSI_USBSPI, 0, read_buffer, &read_data_num,100);
    if (ret != ERR_SUCCESS){
        printf("Read data error!\n");
        return ret;
    }else{
        if (read_data_num > 0){
            printf("Read data num: %d\n",read_data_num);
            printf("Read data(Hex):");
            for (int i = 0; i < read_data_num; i++){
                printf("%02X ",read_buffer[i]);
            }
        }else{
            printf("No data!\n");
        }
    }
	return 0;
}


