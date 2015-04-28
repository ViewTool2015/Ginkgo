  /*
  ******************************************************************************
  * @file     : USB_CNT_Frequency.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 10:03
  * @brief    : USB_CNT_Frequency demo
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
#include "ControlCNT.h"


int main(int argc, const char* argv[])
{
	int ret;
	//Scan device
	ret = CNT_ScanDevice(1);
	if(ret <= 0){
		printf("No device connected!\n");
		return ret;
	}
	//Open device
	ret = CNT_OpenDevice(CNT_USBCNT,0,0);
	if(ret != ERR_SUCCESS){
		printf("Open device error!\n");
		return ret;
	}
	//Config device
	CNT_INIT_CONFIG CNTConfig;
	CNTConfig.CounterBitWide = 32;
	CNTConfig.CounterMode = 0;
	CNTConfig.CounterPolarity = 0;
	ret = CNT_InitCounter(CNT_USBCNT,0,CNT_CH0,&CNTConfig);
	if(ret != ERR_SUCCESS){
		printf("Config device error!\n");
		return ret;
	}
	//Clear counter
	uint32_t CounterValue[4]={0,0,0,0};
	ret = CNT_SetCounter(CNT_USBCNT,0,CNT_CH0,CounterValue);
	if(ret != ERR_SUCCESS){
		printf("Set counter value error!\n");
		return ret;
	}
	//Start counter
	ret = CNT_StartCounter(CNT_USBCNT,0,CNT_CH0);
	if(ret != ERR_SUCCESS){
		printf("Start counter error!\n");
		return ret;
	}
	while(1)
	{
		//Get counter value
		ret = CNT_GetCounter(CNT_USBCNT,0,CNT_CH0,CounterValue);
		if(ret != ERR_SUCCESS){
			printf("Get counter value error!\n");
			break;
		}else{
			printf("Frequency Value :%d\n",CounterValue[0]);
		}
		CounterValue[0] = 0;
		ret = CNT_SetCounter(CNT_USBCNT,0,CNT_CH0,CounterValue);
		if(ret != ERR_SUCCESS){
			printf("Set counter value error!\n");
			break;
		}
#ifndef OS_UNIX
		Sleep(1000);
#else
		usleep(1000*1000);
#endif
	}
	//Stop counter
	ret = CNT_StopCounter(CNT_USBCNT,0,CNT_CH0);
	if(ret != ERR_SUCCESS){
		printf("Stop counter error!\n");
		return ret;
	}
	return 0;
}

