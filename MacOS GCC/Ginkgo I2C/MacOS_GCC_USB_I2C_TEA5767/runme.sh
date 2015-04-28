#!/bin/sh
echo $PWD
install_name_tool -change "bin/Release/Ginkgo_Driver.dylib" "@executable_path/lib/macos/libGinkgo_Driver.dylib" USB_I2C_TEA5767
export LD_LIBRARY_PATH="$PWD/lib/macos:$LD_LIBRARY_PATH"
echo $LD_LIBRARY_PATH
./USB_I2C_TEA5767
