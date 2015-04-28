#include <iostream>
#include <stdio.h>
#include "ControlI2C.h"

using namespace std;

int main()
{
    int ret;
    VII_INIT_CONFIG I2C_Config;
    uint8_t write_buffer[8]= {0};
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
    I2C_Config.ClockSpeed = 400000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_1BYTE;
    ret = VII_InitI2C(VII_USBI2C, 0, 0, &I2C_Config);
    if (ret != ERR_SUCCESS) {
        printf("Initialize device error!\n");
        getchar();
        return ret;
    }
    //Set all ports to output mode
    write_buffer[0] = 0;
    write_buffer[1] = 0;
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x40, 0x06, write_buffer, 2);
    if (ret != ERR_SUCCESS) {
        printf("Write data error!\n");
        getchar();
        return ret;
    } else {
        printf("Start...\n");
    }
    while(1) {
        //Control all the port output low level
        write_buffer[0] = 0;
        write_buffer[1] = 0;
        ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x40, 0x02, write_buffer, 2);
        if (ret != ERR_SUCCESS) {
            printf("Write data error!\n");
            getchar();
            return ret;
        }
#ifndef OS_UNIX
        Sleep(500);
#else
        usleep(500000);
#endif // OS_UNIX
        //Control of all port output high level
        write_buffer[0] = 0xFF;
        write_buffer[1] = 0xFF;
        ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0x40, 0x02, write_buffer, 2);
        if (ret != ERR_SUCCESS) {
            printf("Write data error!\n");
            getchar();
            return ret;
        }
#ifndef OS_UNIX
        Sleep(500);
#else
        usleep(500000);
#endif // OS_UNIX
    }
    return 0;
}
