package com.viewtool.wdluo.usb_spi_test;

import android.app.Activity;
import android.app.PendingIntent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.content.Intent;
import android.widget.EditText;

import com.viewtool.Ginkgo.ErrorType;
import com.viewtool.Ginkgo.GinkgoDriver;
import com.viewtool.Ginkgo.ControlSPI;


public class MainActivity extends Activity {
    // permissions
    private static final String ACTION_USB_PERMISSION = "com.viewtool.wdluo.usb_spi_test.USB_PERMISSION";
    EditText printView;
    GinkgoDriver mGinkgoDriver;
    UsbManager mUsbManager;
    UsbDevice mUsbDevice;
    PendingIntent pendingIntent;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        printView =(EditText) findViewById(R.id.printTextView);
        config_usb();
        Button button =(Button)findViewById(R.id.start);
        button.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View arg0) {
                // TODO Auto-generated method stub
                printView.clearComposingText();
                printView.setText("");
                int ret;
                //Scan device
                mUsbDevice = mGinkgoDriver.ControlSPI.VSI_ScanDevice();
                if(mUsbDevice == null){
                    printView.append("No device connected!\n");
                    return;
                }
                //Open device
                ret = mGinkgoDriver.ControlSPI.VSI_OpenDevice();
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Open device error!\n");
                    return;
                }else{
                    printView.append("Open device success!\n");
                }
                // Get product information
                ControlSPI.VSI_BOARD_INFO BoardInfo = mGinkgoDriver.ControlSPI.new VSI_BOARD_INFO();
                ret = mGinkgoDriver.ControlSPI.VSI_ReadBoardInfo(BoardInfo);
                if (ret != ErrorType.ERR_SUCCESS){
                    printView.append("Read board information error!\n");
                    return ;
                }else{
                    printView.append("Product Name:"+new String(BoardInfo.ProductName)+"\n");
                    printView.append("Firmware Version:"+String.format("V%d.%d.%d\n",BoardInfo.FirmwareVersion[1],BoardInfo.FirmwareVersion[2],BoardInfo.FirmwareVersion[3]));
                    printView.append("Hardware Version:"+String.format("V%d.%d.%d\n",BoardInfo.HardwareVersion[1],BoardInfo.HardwareVersion[2],BoardInfo.HardwareVersion[3]));
                    printView.append("Serial Number:\n");
                    for(int i=0;i<12;i++){
                        printView.append(String.format("%02X",BoardInfo.SerialNumber[i]));
                    }
                    printView.append("\n");
                }
                try {
                    Thread.currentThread();
					Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                //Config device
                //Initialize device(Default:Master Mode, Hardware SPI, Half-Duplex)
                ControlSPI.VSI_INIT_CONFIG SPI_Config = mGinkgoDriver.ControlSPI.new VSI_INIT_CONFIG();
                ret = mGinkgoDriver.ControlSPI.VSI_InitSPI(SPI_Config);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Config device error!\n");
                    return;
                }else{
                    printView.append("Config device success!\n");
                }
                //Write 16 Bytes
                byte[] write_buffer = new byte[16];
                for(int i=0;i<write_buffer.length;i++){
                    write_buffer[i]=(byte)i;
                }
                ret = mGinkgoDriver.ControlSPI.VSI_WriteBytes((byte)0, write_buffer, (short)write_buffer.length);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Write bytes error!\n");
                    return;
                }else{
                    printView.append("Write bytes success!\n");
                }
                //Read 16 Bytes
                byte[] read_buffer = new byte[16];
                ret = mGinkgoDriver.ControlSPI.VSI_ReadBytes((byte)0, read_buffer, (short)read_buffer.length);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Read bytes error!\n");
                    return;
                }else{
                    printView.append("Read bytes(Hex):\n");
                    printView.append(Bytes2HexString(read_buffer));
                    printView.append("\n");
                }
                // Write and read data(Master Mode, Hardware SPI, Half-Duplex),
                // Write 5-byte data and read 8-byte data immediately with chip select
                for (int i = 0; i < 5; i++){
                    write_buffer[i] = (byte)i;
                }
                ret = mGinkgoDriver.ControlSPI.VSI_WriteReadBytes((byte)0, write_buffer, (short)5, read_buffer, (short)8);
                if (ret != ErrorType.ERR_SUCCESS){
                    printView.append("WriteRead data error!\n");
                    return;
                }else{
                    printView.append("Read bytes(Hex):\n");
                    printView.append(Bytes2HexString(read_buffer));
                    printView.append("\n");
                }
                try {
                    Thread.currentThread();
					Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                // Initialize device(Master Mode, Software SPI, Half-Duplex)
                // function VSI_WriteBytes£¬VSI_ReadBytes£¬VSI_WriteReadBytes can be support in software SPI mode
                // Hardware SPI cannot support function VSI_WriteBits,VSI_ReadBits,VSI_WriteReadBits, but can be used in 1-wired mode
                SPI_Config.ControlMode = 2;
                SPI_Config.MasterMode = 1;
                SPI_Config.ClockSpeed = 100000;
                SPI_Config.CPHA = 0;
                SPI_Config.CPOL = 0;
                SPI_Config.LSBFirst = 0;
                SPI_Config.TranBits = 8;
                SPI_Config.SelPolarity = 0;
                ret = mGinkgoDriver.ControlSPI.VSI_InitSPI(SPI_Config);
                if (ret != ErrorType.ERR_SUCCESS){
                    printView.append("Config device error!\n");
                    return;
                }else{
                    printView.append("Config device success!\n");
                }
                // Write data with binary, up to 10240 bits
                byte[] write_bits = "10110100100101".getBytes();
                ret = mGinkgoDriver.ControlSPI.VSI_WriteBits(0, write_bits);// Write 14-bits data
                if (ret != ErrorType.ERR_SUCCESS){
                    printView.append("Write bits error!\n");
                    return;
                }else{
                    printView.append("Write bits success!\n");
                }

                // Read data with binary, up to 10240 bits
                byte[] read_buffer_bin = new byte[10240];
                ret = mGinkgoDriver.ControlSPI.VSI_ReadBits(0, read_buffer_bin, 19);// Read 19-bits data
                if (ret != ErrorType.ERR_SUCCESS){
                    printView.append("Read bit error!\n");
                    return;
                }else{
                    printView.append("Read bits:");
                    printView.append(new String(read_buffer_bin));
                    printView.append("\n");
                }
                // Read and write data with binary
                write_bits = "000011110101001".getBytes();
                ret = mGinkgoDriver.ControlSPI.VSI_WriteReadBits(0,write_bits,read_buffer_bin, 25);// Read 25-bits data
                if (ret != ErrorType.ERR_SUCCESS){
                    printView.append("Write bit error!\n");
                    return;
                }else{
                    printView.append("Read bits:");
                    printView.append(new String(read_buffer_bin));
                    printView.append("\n");
                }
                try {
                    Thread.currentThread();
					Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                // Initialize device(Slave Mode, Hardware SPI, Full-Duplex)
                SPI_Config.ControlMode = 0;
                SPI_Config.MasterMode = 0;  // Slave Mode
                SPI_Config.CPHA = 0;        // Clock Polarity and Phase must be same to master
                SPI_Config.CPOL = 0;
                SPI_Config.LSBFirst = 0;
                SPI_Config.TranBits = 8;    // Support 8bit mode only
                SPI_Config.SelPolarity = 0;
                ret = mGinkgoDriver.ControlSPI.VSI_InitSPI(SPI_Config);
                if (ret != ErrorType.ERR_SUCCESS){
                    printView.append("Config device error!\n");
                    return;
                }else{
                    printView.append("Config device success!\n");
                }
                // Write data in slave mode(call function VSI_SlaveWriteBytes), data will not send out via MISO pin immediately until chip select by master,
                //  function VSI_SlaveWriteBytes return immediately after called, the data stored in adapter memory buffer
                for (int i = 0; i < 8; i++){
                    write_buffer[i] = (byte)i;
                }
                ret = mGinkgoDriver.ControlSPI.VSI_SlaveWriteBytes(write_buffer, 8);
                if (ret != ErrorType.ERR_SUCCESS){
                    printView.append("Slave write data error!\n");
                    return;
                }else{
                    printView.append("Slave write data success!\n");
                }
                // Write data in slave mode: last parameter(100) is waiting time(in us),
                //  function will return immediately if time-out(no matter whether read the data or not)
                // Function judge received number of data via parameter read_data_num
                // ATTENTION: After call function VSI_SlaveWriteBytes,
                //  slave will receive the data when master read data(slave in full-duplex)
                //  master can call function VSI_SlaveReadBytes to discard received data after read the data complete from slave
                int[] read_data_num = {0};
                ret = mGinkgoDriver.ControlSPI.VSI_SlaveReadBytes(read_buffer, read_data_num,100);
                if (ret != ErrorType.ERR_SUCCESS){
                    printView.append("Read data error!\n");
                    return;
                }else{
                    if (read_data_num[0] > 0){
                        printView.append(String.format("Read data num: %d\n",read_data_num));
                        printView.append("Read data(Hex):");
                        for (int i = 0; i < read_data_num[0]; i++){
                            printView.append(String.format("%02X ",read_buffer[i]));
                        }
                    }else{
                        printView.append("No data!\n");
                    }
                }
            }
        });
    }

    /**
     * Config USB monitor
     */
    private void config_usb() {
        // TODO Auto-generated method stub
        mUsbManager = (UsbManager) getSystemService(MainActivity.USB_SERVICE);
        pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(
                ACTION_USB_PERMISSION), 0);
        if((mUsbManager != null)&&(pendingIntent != null)){
            mGinkgoDriver = new GinkgoDriver(mUsbManager, pendingIntent);
        }
    }
    /**
     * Turn a byte array to hexadecimal string
     * @param b byte array
     * @return hexadecimal string
     */
    public static String Bytes2HexString(byte[] b) {
        String ret = "";
        for (int i = 0; i < b.length; i++) {
            String hex = Integer.toHexString(b[i] & 0xFF);
            if (hex.length() == 1) {
                hex = '0' + hex;
            }
            ret += hex.toUpperCase() + " ";

        }
        return ret;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
