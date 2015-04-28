  /*
  ******************************************************************************
  * @file     : USB_SPI_M95040.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/17 18:10
  * @brief    : USB_SPI_M95040 demo
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
  Hardware Connection   for reference only
  M95040            Ginkgo USB-SPI Adapter
  1.S(CS)      <-->  SPI_SEL0(Pin11)
  2.Q(DO)	   <-->  SPI_MISO(Pin15)
  3.W(WP)      <-->  VCC(Pin2)
  4.Vss(GND)   <-->  GND(Pin19/Pin20)
  5.D(DI)	   <-->  VCC(Pin2)SPI_MOSI(Pin17)
  6.C(SCK)	   <-->  VCC(Pin2)SPI_SCK(Pin13)
  7.HOLD	   <-->  VCC(Pin2)
  8.Vcc	       <-->  VCC(Pin2)
  */

  
#include <stdio.h>
#include "ControlSPI.h"


int main(int argc, const char* argv[])
{
	int ret,i;
	uint8_t write_buffer[1024]={0};
	uint8_t read_buffer[1024]={0};
	VSI_INIT_CONFIG SPI_Config;
    // Scan connected device 
    ret = VSI_ScanDevice(1);
    if (ret <= 0)
    {
        printf("No device connect!\n");
        return ret;
    }
    // Open device
    ret = VSI_OpenDevice(VSI_USBSPI, 0, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Open device error!\n");
        return ret;
    }
    // Initialize device(Master Mode, Hardware SPI, Half-Duplex)
    SPI_Config.ControlMode = 1;
    SPI_Config.MasterMode = 1;
    SPI_Config.ClockSpeed = 4500000;
    SPI_Config.CPHA = 0;
    SPI_Config.CPOL = 0;
    SPI_Config.LSBFirst = 0;
    SPI_Config.TranBits = 8;
	SPI_Config.SelPolarity = 0;
    ret = VSI_InitSPI(VSI_USBSPI, 0, &SPI_Config);
    if (ret != ERR_SUCCESS)
    {
        printf("Initialize device error! %d\n",ret);
        return ret;
    }
	// Write Enable
	write_buffer[0] = 0x06;
	ret = VSI_WriteBytes(VSI_USBSPI,0,0,write_buffer,1);
	if(ret != ERR_SUCCESS){
		printf("Flash write enable error :%d\n",ret);
		return ret;
	}else{
		printf("Write enable success!\n");
	}
	// Write data to address 0
	write_buffer[0] = 0x02;
	write_buffer[1] = 0x00;// low 8 bits address, write_buffer[0].bit3 is highest order bit address
	for(i=0;i<16;i++)
	{
		write_buffer[i+2] = i;
	}
	ret = VSI_WriteBytes(VSI_USBSPI,0,0,write_buffer,18);
	if(ret != ERR_SUCCESS){
		printf("Write data error!\n");
		return ret;
	}else{
		printf("Write 16 byte success!\n");
	}
	// Delay to ensure data write completely
	usleep(20*1000);
	// Read the written data
	write_buffer[0] = 0x03;
	write_buffer[1] = 0x00;//  low 8 bits address, write_buffer[0].bit3 is highest order bit address
	ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,write_buffer,2,read_buffer,16);
	if(ret != ERR_SUCCESS){
		printf("Read Data error :%d\n",ret);
		return ret;
	}else{
		printf("Read Data success\n");
	}
	for(i=0;i<16;i++){
		printf("%02X ",read_buffer[i]);
	}
	printf("\n");
	return 0;
}

