  /*
  ******************************************************************************
  * @file     : USB_SDI12_Test.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 10:55
  * @brief    : USB_SDI12_Test demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ControlSDI.h"


int main(int argc, const char* argv[])
{
	int ret,i;
	uint8_t	read_buffer[512]={0};
	uint8_t	respond_buffer[512]={0};
	// Scan connected device 
	ret = SDI_ScanDevice(1);
	if(ret <= 0){
		printf("No device connect!\n");
		return ret;
	}
    // Open device
    ret = SDI_OpenDevice(SDI_USBSDI, 0, 0);
    if (ret != ERR_SUCCESS){
        printf("Open device error!\n");
        return ret;
    }
	// Initialize SDI12 module
	ret = SDI_InitSDI(SDI_USBSDI, 0, 1);
    if (ret != ERR_SUCCESS){
        printf("Initialize SDI-12 error!\n");
        return ret;
    }
	// Send command
	printf("Please input SDI-12 command(end of '!')!\n");
	while(1){
		memset(read_buffer,0,sizeof(read_buffer));
		scanf("%s",read_buffer);
		if(strchr((char *)read_buffer,'C') != NULL){
			ret = SDI_SendCmd(SDI_USBSDI,0,1,read_buffer,respond_buffer,1);
		}else{
			ret = SDI_SendCmd(SDI_USBSDI,0,1,read_buffer,respond_buffer,0);
		}
		if(ret == ERR_SUCCESS){
			printf("%s",(char *)respond_buffer);
		}else{
			printf("Excute command error!\n");
		}
	}
	return 0;
}
