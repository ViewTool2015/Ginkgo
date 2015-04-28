#!/bin/sh
echo $PWD
export LD_LIBRARY_PATH="$PWD/lib/linux_32bit:$LD_LIBRARY_PATH"
echo $LD_LIBRARY_PATH
./USB_CAN_Test
