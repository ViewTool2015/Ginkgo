Imports Microsoft.VisualBasic
Imports System.Runtime.InteropServices

Module timer
    ' Function
    Declare Function QueryPerformanceCounter Lib "kernel32.dll" (ByRef x As Long) As Int16
    Declare Function QueryPerformanceFrequency Lib "kernel32.dll" (ByRef x As Long) As Int16

End Module


