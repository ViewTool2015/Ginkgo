using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Runtime.InteropServices;

namespace Timer
{
    class timer
    {
        [DllImport("kernel32.dll")]
        public extern static short QueryPerformanceCounter(ref long x);
        [DllImport("kernel32.dll")]
        public extern static short QueryPerformanceFrequency(ref long x);
    }
}
