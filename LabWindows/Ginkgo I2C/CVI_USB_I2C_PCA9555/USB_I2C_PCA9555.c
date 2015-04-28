//==============================================================================
//
// Title:		USB_I2C_PCA9555.c
// Purpose:		A short description of the implementation.
//
// Created on:	2014/3/7 at 16:12:12 by Win7.
// Copyright:	Win7. All Rights Reserved.
//
//==============================================================================

//==============================================================================
// Include files

#include "ControlI2C.h"
#include <ansi_c.h>
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

/// HIFN  What does your function do?
/// HIPAR x/What inputs does your function expect?
/// HIRET What does your function return?
int main (void)
{
	int ret,i;
	VII_INIT_CONFIG I2C_Config;
	uint8_t write_buffer[8]={0};
	uint8_t	read_buffer[8]={0};
	//Scan device(must call)
	ret = VII_ScanDevice(1);
	if(ret <= 0)
	{
		printf("No device connect!\n");
		system("cmd /C pause");
		return ret;
	}
    //Open device(must call)
    ret = VII_OpenDevice(VII_USBI2C, 0, 0);
    if (ret != ERR_SUCCESS)
    {
		printf("Open device error!\n");
		system("cmd /C pause");
        return ret;
    }
    //Initialize the device (hardware control mode)
    I2C_Config.AddrType = VII_ADDR_7BIT;
    I2C_Config.ClockSpeed = 400000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_1BYTE;
    ret = VII_InitI2C(VII_USBI2C, 0, 0, &I2C_Config);
    if (ret != ERR_SUCCESS)
    {
		printf("Initialize device error!\n");
		system("cmd /C pause");
        return ret;
    }
    //Set all ports to output mode
    write_buffer[0] = 0;
    write_buffer[1] = 0;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x40, 0x06, write_buffer, 2);
    if (ret != ERR_SUCCESS)
    {
		printf("Write data error!\n");
		system("cmd /C pause");
        return ret;
	}else{
        printf("Start...\n");
    }
	while(1)
	{
        //Control all the port output low level
        write_buffer[0] = 0;
        write_buffer[1] = 0;
        ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x40, 0x02, write_buffer, 2);
        if (ret != ERR_SUCCESS)
        {
			printf("Write data error!\n");
			system("cmd /C pause");
            return ret;
        }
        Sleep(500);
        //Control of all port output high level
        write_buffer[0] = 0xFF;
        write_buffer[1] = 0xFF;
        ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x40, 0x02, write_buffer, 2);
        if (ret != ERR_SUCCESS)
        {
			printf("Write data error!\n");
			system("cmd /C pause");
            return ret;
        }
        Sleep(500);
	}
	return 0;
}
