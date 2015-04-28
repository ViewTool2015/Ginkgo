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
#define  PANEL_TIMER_MEASURE              7       /* control type: timer, callback function: TimerMeasureCallback */
#define  PANEL_GRAPH                      8       /* control type: graph, callback function: (none) */
#define  PANEL_TEXTMSG                    9       /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_9                  10      /* control type: textMsg, callback function: (none) */
#define  PANEL_us                         11      /* control type: textMsg, callback function: (none) */
#define  PANEL_SAMPLE_TIME                12      /* control type: numeric, callback function: SampleTimeChangedCallback */


     /* Control Arrays: */

#define  CTRLARRAY                        1

     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK InitDeviceButtonCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QuitButtonCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SampleTimeChangedCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SelectDeviceCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK StartButtonCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TimerCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TimerMeasureCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
