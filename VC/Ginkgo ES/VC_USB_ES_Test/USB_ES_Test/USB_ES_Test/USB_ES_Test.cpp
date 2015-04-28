  /*
  ******************************************************************************
  * @file     : USB_ES_Test.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 10:06
  * @brief    : USB_ES_Test demo
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
#include "EasyScale.h"


int _tmain(int argc, _TCHAR* argv[])
{
	VEI_ES_TIMING ESTime;
	VEI_ES_OBJ ESData;
	int ret;
	int DevNum = VEI_ScanDevice(1);
	if(DevNum > 0){
		printf("Have %d device connected!\n",DevNum);
	}else{
		printf("No device connected!\n");
	}
	VEI_OpenDevice(0,0,0);

	ESTime.ESDelay = 100;
	ESTime.ESDetect = 200;
	ESTime.ESWin = 1000;
	ESTime.HighBit_THigh = 5;
	ESTime.HighBit_TLow = 0;
	ESTime.LowBit_THigh = 0;
	ESTime.LowBit_TLow = 5;
	ESTime.TACKN = 0;
	ESTime.TEos = 2;
	ESTime.TStart = 2;
	ESTime.TValACK = 0;

	ret = VEI_InitES(0,0,0xFFFF,0x0000,&ESTime);
	if(ret){
		printf("Init ES Error!\n");
	}else{
		printf("Init ES success!\n");
	}

	ESData.Data = 0X55;
	ESData.RFA = 0;
	ESData.DevAddr = 0x72;
	ESData.SubAddr = 1;
	ESData.Data = 0x3;
	ret = VEI_Transmit(0,0,0xFFFF,&ESData,1);
	if(ret){
		printf("Write ES data Error!\n");
	}else{
		printf("Write ES data success!\n");
	}
	return 0;
}

