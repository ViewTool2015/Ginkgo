/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2013. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_RING_DEVICE                2       /* control type: ring, callback function: RingDeviceCallback */
#define  PANEL_COMMANDBUTTON_INIT         3       /* control type: command, callback function: InitDeviceCallback */
#define  PANEL_COMMANDBUTTON_START        4       /* control type: command, callback function: StartMeasureCallback */
#define  PANEL_COMMANDBUTTON_QUIT         5       /* control type: command, callback function: QuitCallback */
#define  PANEL_TIMER_SCAN_DEVICE          6       /* control type: timer, callback function: ScanDeviceCallback */
#define  PANEL_TEXTMSG                    7       /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMER_START                8       /* control type: timer, callback function: MeasureCallback */
#define  PANEL_LED_10                     9       /* control type: LED, callback function: (none) */
#define  PANEL_LED_9                      10      /* control type: LED, callback function: (none) */
#define  PANEL_LED_8                      11      /* control type: LED, callback function: (none) */
#define  PANEL_LED_7                      12      /* control type: LED, callback function: (none) */
#define  PANEL_LED_6                      13      /* control type: LED, callback function: (none) */
#define  PANEL_LED_5                      14      /* control type: LED, callback function: (none) */
#define  PANEL_LED_4                      15      /* control type: LED, callback function: (none) */
#define  PANEL_LED_3                      16      /* control type: LED, callback function: (none) */
#define  PANEL_LED_2                      17      /* control type: LED, callback function: (none) */
#define  PANEL_LED_1                      18      /* control type: LED, callback function: (none) */
#define  PANEL_LED_BUSY                   19      /* control type: LED, callback function: (none) */
#define  PANEL_COMMAND                    20      /* control type: string, callback function: (none) */
#define  PANEL_TEXTMSG_2                  21      /* control type: textMsg, callback function: (none) */
#define  PANEL_PICTURE                    22      /* control type: picture, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK InitDeviceCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MeasureCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RingDeviceCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ScanDeviceCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StartMeasureCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
