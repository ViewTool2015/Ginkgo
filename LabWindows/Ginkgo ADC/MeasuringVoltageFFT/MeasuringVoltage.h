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
#define  PANEL_QuitButton                 2       /* control type: command, callback function: QuitButtonCallback */
#define  PANEL_InitDeviceButton           3       /* control type: command, callback function: InitDeviceButtonCallback */
#define  PANEL_StartButton                4       /* control type: command, callback function: StartButtonCallback */
#define  PANEL_TIMER                      5       /* control type: timer, callback function: TimerCallback */
#define  PANEL_RING_DEVICE                6       /* control type: ring, callback function: SelectDeviceCallback */
#define  PANEL_TEXTMSG_9                  7       /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMER_MEASURE              8       /* control type: timer, callback function: TimerMeasureCallback */
#define  PANEL_GRAPH                      9       /* control type: graph, callback function: (none) */
#define  PANEL_TEXTMSG                    10      /* control type: textMsg, callback function: (none) */
#define  PANEL_STRING                     11      /* control type: string, callback function: (none) */
#define  PANEL_TEXTMSG_2                  12      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK InitDeviceButtonCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitButtonCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SelectDeviceCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StartButtonCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TimerCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TimerMeasureCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
