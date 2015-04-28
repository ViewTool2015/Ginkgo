//==============================================================================
//
// Title:		USB_I2C_AT24C02.c
// Purpose:		A short description of the implementation.
//
// Created on:	2014/3/7 at 15:37:23 by Win7.
// Copyright:	Win7. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files
#include "ControlI2C.h"  
#include <utility.h>
#include <stdio.h>


//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// Static global variables

//==============================================================================
// Static functions

//==============================================================================
// Global variables

//==============================================================================
// Global functions

/// HIFN  USB-I2C测试程序
/// HIPAR 无
/// HIRET 程序执行错误代码
int main (void)
{
	int ret,i;
	VII_INIT_CONFIG I2C_Config;
	uint8_t write_buffer[8]={0};
	uint8_t	read_buffer[8]={0};
	//扫描已经连接的设备
	ret = VII_ScanDevice(1);
	if(ret <= 0)
	{
		printf("No device connect!\n");
		return ret;
	}
    //打开设备
    ret = VII_OpenDevice(VII_USBI2C, 0, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Open device error!\n");
        return ret;
    }
    //初始化设备(硬件控制模式)
    I2C_Config.AddrType = VII_ADDR_7BIT;
    I2C_Config.ClockSpeed = 400000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_1BYTE;
    ret = VII_InitI2C(VII_USBI2C, 0, 0, &I2C_Config);
    if (ret != ERR_SUCCESS)
    {
        printf("Initialize device error!\n");
        return ret;
    }
	//向0x00地址开始依次写入8字节数据
	for (i = 0; i < 8; i++)
    {
        write_buffer[i] = i;
    }
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0xA0, 0x00, write_buffer, 8);
    if (ret != ERR_SUCCESS)
    {
        printf("Write data error!\n");
        return ret;
    }
    //延时一段时间，以保证数据写入完毕
    Sleep(10);
	//从0x00地址开始依次读出8字节数据
    ret = VII_ReadBytes(VII_USBI2C, 0, 0, 0xA0, 0x00, read_buffer, 8);
    if (ret != ERR_SUCCESS)
    {
        printf("Read data error!\n");
        return ret;
    }
	else
	{
		printf("Read Data:\n");
		for(i=0;i<8;i++)
		{
			printf("%02X ",read_buffer[i]);
		}
		printf("\n");
	}
	system("cmd /C pause");
	return 0;
}
