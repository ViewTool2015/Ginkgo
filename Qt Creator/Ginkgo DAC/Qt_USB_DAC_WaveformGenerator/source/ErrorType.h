#ifndef _ERRORTYPE_H_
#define _ERRORTYPE_H_

#include <stdint.h>
#ifndef OS_UNIX
#include <Windows.h>
#else
#include <unistd.h>
#ifndef WINAPI
#define WINAPI
#endif
#endif

//Generic error code
#define	ERR_SUCCESS					(0)		// no error
#define	ERR_PARAMETER_NULL			(-1)	// parameter input is null
#define	ERR_INPUT_DATA_TOO_MUCH		(-2)	// parameter input number is too much
#define	ERR_INPUT_DATA_TOO_LESS		(-3)	// parameter input number is not enough
#define	ERR_INPUT_DATA_ILLEGALITY	(-4)	// parameter input format is invalid
#define	ERR_USB_WRITE_DATA			(-5)	// USB writing error
#define	ERR_USB_READ_DATA			(-6)	//USB reading error
#define	ERR_READ_NO_DATA			(-7)	//no data return on reading
#define	ERR_OPEN_DEVICE				(-8)	//open device failed
#define	ERR_CLOSE_DEVICE			(-9)	//close device failed
#define	ERR_EXECUTE_CMD				(-10)	//device command execution failed
#define ERR_SELECT_DEVICE			(-11)	//device selection failed
#define	ERR_DEVICE_OPENED			(-12)	//device is opened
#define	ERR_DEVICE_NOTOPEN			(-13)	//device not opened
#define	ERR_BUFFER_OVERFLOW			(-14)	//buffer is overflow
#define	ERR_DEVICE_NOTEXIST			(-15)	//device is not existing
#define	ERR_LOAD_KERNELDLL			(-16)	//load dll failed
#define ERR_CMD_FAILED				(-17)	//command execution failed
#define	ERR_BUFFER_CREATE			(-18)	//memory is not enough


#endif
