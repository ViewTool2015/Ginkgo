
// Ginkgo_i2c_slave.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CGinkgo_i2c_slaveApp:
// See Ginkgo_i2c_slave.cpp for the implementation of this class
//

class CGinkgo_i2c_slaveApp : public CWinApp
{
public:
	CGinkgo_i2c_slaveApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CGinkgo_i2c_slaveApp theApp;