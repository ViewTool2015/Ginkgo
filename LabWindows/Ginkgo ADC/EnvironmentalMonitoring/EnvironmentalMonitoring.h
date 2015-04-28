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
#define  PANEL_LIGHT                      2       /* control type: scale, callback function: (none) */
#define  PANEL_CARBON_GAS                 3       /* control type: scale, callback function: (none) */
#define  PANEL_RING_DEVICE                4       /* control type: ring, callback function: RingDeviceCallback */
#define  PANEL_COMMANDBUTTON_INIT         5       /* control type: command, callback function: InitDeviceCallback */
#define  PANEL_COMMANDBUTTON_START        6       /* control type: command, callback function: StartMeasureCallback */
#define  PANEL_COMMANDBUTTON_QUIT         7       /* control type: command, callback function: QuitCallback */
#define  PANEL_TIMER_SCAN_DEVICE          8       /* control type: timer, callback function: ScanDeviceCallback */
#define  PANEL_COAL_GAS                   9       /* control type: scale, callback function: (none) */
#define  PANEL_NATURAL_GAS                10      /* control type: scale, callback function: (none) */
#define  PANEL_ALCOHOL                    11      /* control type: scale, callback function: (none) */
#define  PANEL_SMOKE                      12      /* control type: scale, callback function: (none) */
#define  PANEL_TEXTMSG                    13      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMER_START                14      /* control type: timer, callback function: MeasureCallback */


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
