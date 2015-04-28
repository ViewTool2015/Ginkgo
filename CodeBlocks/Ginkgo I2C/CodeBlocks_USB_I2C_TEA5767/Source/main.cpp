#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ControlI2C.h"

using namespace std;

int main()
{
    int ret,i;
    VII_INIT_CONFIG I2C_Config;
    uint8_t write_buffer[8]= {0};
    char cmdString[256]= {0};
    int pll;
    int frequency;//The radio frequency
    int dotNum;//In the input string decimal number
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
    I2C_Config.ClockSpeed = 200000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_NONE;
    ret = VII_InitI2C(VII_USBI2C, 0, 0, &I2C_Config);
    if (ret != ERR_SUCCESS) {
        printf("Initialize device error!\n");
        getchar();
        return ret;
    }
    while(1) {
        printf("Please input the radio frequency (unit for MHz):");
        scanf("%s",cmdString);
        if(strcmp(cmdString,"quit")==0) {
            printf("Exit!\n");
            break;
        } else {
            dotNum = 0;
            for(i=0; i<strlen(cmdString); i++) {
                if((cmdString[i]!='.')&&(cmdString[i]<'0')&&(cmdString[i]>'9')) {
                    printf("Error input!");
                    getchar();
                    return -1;
                }
                if(cmdString[i]=='.') {
                    dotNum++;
                }
            }
            if(dotNum>1) {
                printf("Error input!");
                getchar();
                return -1;
            } else if(dotNum>0) {
                frequency = atof(cmdString)*1000;
            } else {
                frequency = atoi(cmdString)*1000;
            }
            pll = (unsigned int)((float)((frequency+225))/(float)8.192);
            write_buffer[0] = pll>>8;
            write_buffer[1] = pll;
            write_buffer[2] = 0x70;
            write_buffer[3] = 0x16;
            write_buffer[4] = 0x80;
            ret = VII_WriteBytes(VII_USBI2C, 0, 0, 0xC0, 0x00, write_buffer, 5);
            if (ret != ERR_SUCCESS) {
                printf("Set the frequency failure: %d\n",ret);
                getchar();
                return ret;
            } else {
                printf("Set the frequency success!\n");
            }
        }
    }
    getchar();
    return 0;
}
