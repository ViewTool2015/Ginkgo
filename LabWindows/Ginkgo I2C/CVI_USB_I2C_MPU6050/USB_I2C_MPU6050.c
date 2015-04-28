//==============================================================================
//
// Title:		USB_I2C_MPU6050.c
// Purpose:		A short description of the implementation.
//
// Created on:	2014/3/7 at 16:07:06 by Win7.
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
	uint8_t write_buffer[16]={0};
	uint8_t	read_buffer[16]={0};
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
    //Initialize MPU6050 sensor
    write_buffer[0] = 0;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0xD0, 0x6B, write_buffer, 1);
    if (ret != ERR_SUCCESS)
    {
		printf("Write data error!\n");
		system("cmd /C pause");
        return ret;
    }
    Sleep(10);
    write_buffer[0] = 0;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0xD0, 0x6C, write_buffer, 1);
    if (ret != ERR_SUCCESS)
    {
		printf("Write data error!\n");
		system("cmd /C pause");
        return ret;
    }
    Sleep(10);
    write_buffer[0] = 0x08;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0xD0, 0x1B, write_buffer, 1);
    if (ret != ERR_SUCCESS)
    {
		printf("Write data error!\n");
		system("cmd /C pause");
        return ret;
    }
    Sleep(10);
    write_buffer[0] = 0x01;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0xD0, 0x1A, write_buffer, 1);
    if (ret != ERR_SUCCESS)
    {
		printf("Write data error!\n");
		system("cmd /C pause");
        return ret;
    }
    Sleep(10);
    write_buffer[0] = 0x08;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0xD0, 0x1C, write_buffer, 1);
    if (ret != ERR_SUCCESS)
    {
		printf("Write data error!\n");
		system("cmd /C pause");
        return ret;
    }
    Sleep(10);
    while (1)
    {
        //Get data
        ret = VII_ReadBytes(VII_USBI2C, 0, 0, 0xD0, 0x3B, read_buffer, 14);
        if (ret != ERR_SUCCESS)
        {
			printf("Read data error!\n");
			system("cmd /C pause");
            return ret;
        }
        //Process the data
        int16_t AccelX, AccelY, AccelZ;
        AccelX = (int16_t)((read_buffer[0] << 8) | read_buffer[1]);
        AccelY = (int16_t)((read_buffer[2] << 8) | read_buffer[3]);
        AccelZ = (int16_t)((read_buffer[4] << 8) | read_buffer[5]);
        int16_t GyroX, GyroY, GyroZ;
        GyroX = (int16_t)((read_buffer[8] << 8) | read_buffer[9]);
        GyroY = (int16_t)((read_buffer[10] << 8) | read_buffer[11]);
        GyroZ = (int16_t)((read_buffer[12] << 8) | read_buffer[13]);
        //Display the data
        system("cmd /C cls");
        printf("AccelX = %d\n" , AccelX);
        printf("AccelY = %d\n" , AccelY);
        printf("AccelZ = %d\n" , AccelZ);
        printf("GyroX = %d\n" , GyroX);
        printf("GyroY = %d\n" , GyroY);
        printf("GyroZ = %d\n" , GyroZ);
        Sleep(100);
    }
	return 0;
}
