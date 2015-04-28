#include <iostream>
#include <stdio.h>
#include "ControlSPI.h"

using namespace std;

int main()
{
    int ret,i;
    VSI_INIT_CONFIG	SPI_Init;
    uint8_t	WriteDataTemp[512]= {0};
    uint8_t	ReadDataTemp[512]= {0};
    //Check the device number of connections
    ret = VSI_ScanDevice(1);
    if(ret>0) {
        printf("The connected device number is:%d\n",ret);
    } else {
        printf("No device to connect!\n");
        getchar();
        return 0;
    }
    //open Device
    ret = VSI_OpenDevice(VSI_USBSPI,0,0);
    if(ret != ERR_SUCCESS) {
        printf("Open device error :%d\n",ret);
        getchar();
        return ret;
    } else {
        printf("Open device success!\n");
    }
    //Device initialization
    SPI_Init.ClockSpeed = 1125000;
    SPI_Init.ControlMode = 1;
    SPI_Init.CPHA = 0;
    SPI_Init.CPOL = 0;
    SPI_Init.LSBFirst = 0;
    SPI_Init.MasterMode = 1;
    SPI_Init.SelPolarity = 0;
    SPI_Init.TranBits = 8;
    ret = VSI_InitSPI(VSI_USBSPI,0,&SPI_Init);
    if(ret != ERR_SUCCESS) {
        printf("Initialization device error :%d\n",ret);
        getchar();
        return ret;
    } else {
        printf("Initialization device success!\n");
    }
    //JEDEC ID
    WriteDataTemp[0] = 0x9F;
    ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,WriteDataTemp,1,ReadDataTemp,3);
    if(ret != ERR_SUCCESS) {
        printf("Read flash ID error :%d\n",ret);
        getchar();
        return ret;
    } else {
        printf("Flash ID = 0x%06X\n",(ReadDataTemp[0]<<16)|(ReadDataTemp[1]<<8)|(ReadDataTemp[2]));
    }
    //Write Enable
    WriteDataTemp[0] = 0x06;
    ret = VSI_WriteBytes(VSI_USBSPI,0,0,WriteDataTemp,1);
    if(ret != ERR_SUCCESS) {
        printf("Flash write enable error :%d\n",ret);
        getchar();
        return ret;
    } else {
        printf("Write enable success!\n");
    }
    //Sector Erase (4KB)
    WriteDataTemp[0] = 0x20;
    WriteDataTemp[1] = 0x00;
    WriteDataTemp[2] = 0x00;
    WriteDataTemp[3] = 0x00;
    ret = VSI_WriteBytes(VSI_USBSPI,0,0,WriteDataTemp,4);
    if(ret != ERR_SUCCESS) {
        printf("Sector Erase start error :%d\n",ret);
        getchar();
        return ret;
    } else {
        printf("Sector erase start success!\n");
    }
    //Check the operation to complete
    do {
        WriteDataTemp[0] = 0x05;//Read Status Register-1
        ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,WriteDataTemp,1,ReadDataTemp,1);

    } while((ReadDataTemp[0]&0x01)&&(ret == ERR_SUCCESS));
    if(ret != ERR_SUCCESS) {
        printf("Sector Erase error :%d\n",ret);
        getchar();
        return ret;
    } else {
        printf("Sector erase success!\n");
    }
    //Write Enable
    WriteDataTemp[0] = 0x06;
    ret = VSI_WriteBytes(VSI_USBSPI,0,0,WriteDataTemp,1);
    if(ret != ERR_SUCCESS) {
        printf("Flash write enable error :%d\n",ret);
        getchar();
        return ret;
    } else {
        printf("Write enable success!\n");
    }
    //Page Program
    WriteDataTemp[0] = 0x02;//Page Program command
    WriteDataTemp[1] = 0x00;//Adress
    WriteDataTemp[2] = 0x00;
    WriteDataTemp[3] = 0x00;
    for(i=4; i<(256+4); i++) {
        WriteDataTemp[i] = i-4;
    }
    ret = VSI_WriteBytes(VSI_USBSPI,0,0,WriteDataTemp,256+4);
    if(ret != ERR_SUCCESS) {
        printf("Page program start error :%d\n",ret);
        getchar();
        return ret;
    } else {
        printf("Page program start success!\n");
    }
    //Check the operation to complete
    do {
        WriteDataTemp[0] = 0x05;//Read Status Register-1
        ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,WriteDataTemp,1,ReadDataTemp,1);

    } while((ReadDataTemp[0]&0x01)&&(ret == ERR_SUCCESS));
    if(ret != ERR_SUCCESS) {
        printf("Page program error :%d\n",ret);
        getchar();
        return ret;
    } else {
        printf("Page program success!\n");
    }
    //Read Data
    WriteDataTemp[0] = 0x03;//Read Data command
    WriteDataTemp[1] = 0x00;//Adress
    WriteDataTemp[2] = 0x00;
    WriteDataTemp[3] = 0x00;
    ret = VSI_WriteReadBytes(VSI_USBSPI,0,0,WriteDataTemp,4,ReadDataTemp,256);
    if(ret != ERR_SUCCESS) {
        printf("Read Data error :%d\n",ret);
        getchar();
        return ret;
    } else {
        printf("Read Data success\n");
    }
    for(i=0; i<256; i++) {
        if((i%16)==0) {
            printf("\n");
        }
        printf("%02X ",ReadDataTemp[i]);
    }
    printf("\n\n");
    getchar();
    return 0;
}
