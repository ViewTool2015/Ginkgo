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
#define  PANEL_CHECKBOX_CH0               5       /* control type: radioButton, callback function: (none) */
#define  PANEL_CHECKBOX_CH7               6       /* control type: radioButton, callback function: (none) */
#define  PANEL_CHECKBOX_CH6               7       /* control type: radioButton, callback function: (none) */
#define  PANEL_CHECKBOX_CH5               8       /* control type: radioButton, callback function: (none) */
#define  PANEL_CHECKBOX_CH4               9       /* control type: radioButton, callback function: (none) */
#define  PANEL_CHECKBOX_CH3               10      /* control type: radioButton, callback function: (none) */
#define  PANEL_CHECKBOX_CH2               11      /* control type: radioButton, callback function: (none) */
#define  PANEL_CHECKBOX_CH1               12      /* control type: radioButton, callback function: (none) */
#define  PANEL_TEXTMSG                    13      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_2                  14      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_3                  15      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_4                  16      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_5                  17      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_6                  18      /* control type: textMsg, callback function: (none) */
#define  PANEL_TEXTMSG_7                  19      /* control type: textMsg, callback function: (none) */
#define  PANEL_STRING_CH7                 20      /* control type: string, callback function: (none) */
#define  PANEL_STRING_CH6                 21      /* control type: string, callback function: (none) */
#define  PANEL_STRING_CH5                 22      /* control type: string, callback function: (none) */
#define  PANEL_STRING_CH4                 23      /* control type: string, callback function: (none) */
#define  PANEL_STRING_CH3                 24      /* control type: string, callback function: (none) */
#define  PANEL_STRING_CH2                 25      /* control type: string, callback function: (none) */
#define  PANEL_STRING_CH1                 26      /* control type: string, callback function: (none) */
#define  PANEL_STRING_CH0                 27      /* control type: string, callback function: (none) */
#define  PANEL_TEXTMSG_8                  28      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMER                      29      /* control type: timer, callback function: TimerCallback */
#define  PANEL_RING_DEVICE                30      /* control type: ring, callback function: SelectDeviceCallback */
#define  PANEL_TEXTMSG_9                  31      /* control type: textMsg, callback function: (none) */
#define  PANEL_TIMER_MEASURE              32      /* control type: timer, callback function: TimerMeasureCallback */


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
