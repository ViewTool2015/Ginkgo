#ifndef _YEELINK_H_
#define _YEELINK_H_

#include <stdint.h>
#ifndef OS_LINUX
#include <Windows.h>
#endif

// Error Code 
#define	ERR_SUCCESS					(0)		// No error
#define	ERR_PARAMETER_NULL			(-1)	// void pointer 
#define	ERR_START_SOCKET			(-2)	// Start Socket failed
#define	ERR_GET_IP_ADDR				(-3)	// Get IP address failed
#define	ERR_CONNECT_FAILD			(-4)	// Connect remote host failed
#define	ERR_HTTP_RESPONSE			(-5)	// HTTP response failed
#define	ERR_USERNAME_PASS			(-6)	// ID or password error
#define	ERR_REQUEST					(-7)	// Request Method error
#define ERR_UNKNOWN					(-100)	// Unknown Error

#ifdef __cplusplus
extern "C"
{
#endif
int32_t WINAPI Yeelink_GetApiKey(const char *pUserName,const char *pPassword);
int32_t WINAPI Yeelink_PostData(const char *pDeviceId,const char *pSensorId,const char *pValue);
int32_t WINAPI Yeelink_GetData(const char *pDeviceId,const char *pSensorId,char *pValue);
#ifdef __cplusplus
}
#endif

#endif
