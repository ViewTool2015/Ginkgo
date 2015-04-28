//==============================================================================
//
// Title:		USB_I2C_BH1750FVI.c
// Purpose:		A short description of the implementation.
//
// Created on:	2014/3/7 at 15:58:10 by Win7.
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
    I2C_Config.ClockSpeed = 100000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_NONE;
    ret = VII_InitI2C(VII_USBI2C, 0, 0, &I2C_Config);
    if (ret != ERR_SUCCESS)
    {
		printf("Initialize device error!\n");
		system("cmd /C pause");
        return ret;
    }
    //Send measurement commands
    write_buffer[0] = 0x01;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x46, 0x00, write_buffer, 1);
    if (ret != ERR_SUCCESS)
    {
		printf("Write data error!\n");
		system("cmd /C pause");
        return ret;
    }
    //Start measuring under 1 lx resolution
    write_buffer[0] = 0x10;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x46, 0x00, write_buffer, 1);
    if (ret != ERR_SUCCESS)
    {
		printf("Write data error!\n");
		system("cmd /C pause");
        return ret;
    }
    //Loop measurement
	while(1)
	{
        //Delay, waiting for the measurement is complete
        Sleep(1000);
        //Read the data
        ret = VII_ReadBytes(VII_USBI2C, 0, 0, 0x46, 0x00, read_buffer, 2);
        if (ret != ERR_SUCCESS)
        {
			printf("Read data error!\n");
			system("cmd /C pause");
            return ret;
        }
        else
        {
            double illuminance = ((read_buffer[0] << 8) | read_buffer[1]) / 1.2;
            system("cmd /C cls");
            printf("Light Intensity£º%.2f lx\n" ,illuminance);
        }
	}
	system("cmd /C pause");
	return 0;
}
