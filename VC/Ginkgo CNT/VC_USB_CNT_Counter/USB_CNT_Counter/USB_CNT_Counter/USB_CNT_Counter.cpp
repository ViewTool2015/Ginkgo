  /*
  ******************************************************************************
  * @file     : USB_CNT_Counter.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 9:58
  * @brief    : USB_CNT_Counter demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

#include "stdafx.h"
#include "ControlCNT.h"


int _tmain(int argc, _TCHAR* argv[])
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
			printf("Counter Value :%d\n",CounterValue[0]);
		}
		Sleep(1000);
	}
	//Stop counter
	ret = CNT_StopCounter(CNT_USBCNT,0,CNT_CH0);
	if(ret != ERR_SUCCESS){
		printf("Stop counter error!\n");
		return ret;
	}
	return 0;
}

