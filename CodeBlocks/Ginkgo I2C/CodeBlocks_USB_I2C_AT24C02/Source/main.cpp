#include <iostream>
#include <stdio.h>
#include "ControlI2C.h"

using namespace std;

int main(void)
{
    int ret,i;
    VII_INIT_CONFIG I2C_Config;
    VII_BOARD_INFO BoardInfo;
    uint8_t write_buffer[8]= {0};
    uint8_t	read_buffer[8]= {0};
    //Scan device
    ret = VII_ScanDevice(1);
    if(ret <= 0) {
        printf("No device connect!\n");
        return ret;
    }
    //Open device
    ret = VII_OpenDevice(VII_USBI2C, 0, 0);
    if (ret != ERR_SUCCESS) {
        printf("Open device error!\n");
        return ret;
    }
    //Get board information
    ret = VII_ReadBoardInfo(0,&BoardInfo);
    if (ret != ERR_SUCCESS) {
        printf("Read board information error!\n");
        return ret;
    } else {
        printf("Product Name:%s\n",BoardInfo.ProductName);
        printf("Firmware Version:V%d.%d.%d\n",BoardInfo.FirmwareVersion[1],BoardInfo.FirmwareVersion[2],BoardInfo.FirmwareVersion[3]);
        printf("Hardware Version:V%d.%d.%d\n",BoardInfo.HardwareVersion[1],BoardInfo.HardwareVersion[2],BoardInfo.HardwareVersion[3]);
        printf("Serial Number:");
        for(i=0; i<12; i++) {
            printf("%02X",BoardInfo.SerialNumber[i]);
        }
        printf("\n");
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
        return ret;
    }
    //Write 8 bytes of data to address 0x00
    for (i = 0; i < 8; i++) {
        write_buffer[i] = i;
    }
    ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0xA0, 0x00, write_buffer, 8);
    if (ret != ERR_SUCCESS) {
        printf("Write data error!\n");
        return ret;
    }
    //Delay for a period of time, to ensure that data to complete
#ifndef OS_UNIX
    Sleep(10);
#else
    usleep(10000);
#endif // OS_UNIX
    //Read 8 bytes of data from address 0x00
    ret = VII_ReadBytes(VII_USBI2C, 0, 0, 0xA0, 0x00, read_buffer, 8);
    if (ret != ERR_SUCCESS) {
        printf("Read data error!\n");
        return ret;
    } else {
        printf("Read Data:\n");
        for(i=0; i<8; i++) {
            printf("%02X ",read_buffer[i]);
        }
        printf("\n");
    }
    getchar();
    return 0;
}
