#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "ControlI2C.h"

using namespace std;

int main()
{
    int ret;
    VII_INIT_CONFIG I2C_Config;
    uint8_t write_buffer[8]= {0};
    uint8_t	read_buffer[8]= {0};
    //Scan device(must call)
    ret = VII_ScanDevice(1);
    if(ret <= 0) {
        printf("No device connect!\n");
        getchar();
        return ret;
    }
    //Open device(must call)
    ret = VII_OpenDevice(VII_USBI2C, 0, 0);
    if (ret != ERR_SUCCESS) {
        printf("Open device error!\n");
        getchar();
        return ret;
    }
    //Initialize the device (hardware control mode)
    I2C_Config.AddrType = VII_ADDR_7BIT;
    I2C_Config.ClockSpeed = 100000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_NONE;
    ret = VII_InitI2C(VII_USBI2C, 0, 0, &I2C_Config);
    if (ret != ERR_SUCCESS) {
        printf("Initialize device error!\n");
        getchar();
        return ret;
    }
    //Send measurement commands
    write_buffer[0] = 0x01;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x46, 0x00, write_buffer, 1);
    if (ret != ERR_SUCCESS) {
        printf("Write data error!\n");
        getchar();
        return ret;
    }
    //Start measuring under 1 lx resolution
    write_buffer[0] = 0x10;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x46, 0x00, write_buffer, 1);
    if (ret != ERR_SUCCESS) {
        printf("Write data error!\n");
        getchar();
        return ret;
    }
    //Loop measurement
    while(1) {
        //Delay, waiting for the measurement is complete
#ifndef OS_UNIX
        Sleep(1000);
#else
        usleep(1000000);
#endif // OS_UNIX
        //Read the data
        ret = VII_ReadBytes(VII_USBI2C, 0, 0, 0x46, 0x00, read_buffer, 2);
        if (ret != ERR_SUCCESS) {
            printf("Read data error!\n");
            getchar();
            return ret;
        } else {
            double illuminance = ((read_buffer[0] << 8) | read_buffer[1]) / 1.2;
#ifndef OS_UNIX
            system("cls");
#else
            system("clear");
#endif // OS_UNIX
            printf("Light Intensity: %.2f lx\n" ,illuminance);
        }
    }
    getchar();
    return 0;
}
