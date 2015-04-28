/*
Hardware Connection
W25Q32         Ginkgo USB-SPI Adapter
1.CS      <-->  SPI_SEL0(Pin11)
2.DO	    <-->  SPI_MISO(Pin15)
3.WP      <-->  VCC(Pin2)
4.GND     <-->  GND(Pin19/Pin20)
5.DI	    <-->  SPI_MOSI(Pin17)
6.CLK	    <-->  SPI_SCK(Pin13)
7.HOLD    <-->  VCC(Pin2)
8.VCC	    <-->  VCC(Pin2)
*/

package viewtool.usb_spi_w25q32;

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
                ControlSPI.VSI_INIT_CONFIG SPI_Config = mGinkgoDriver.ControlSPI.new VSI_INIT_CONFIG();
                SPI_Config.ControlMode = 1;
                SPI_Config.MasterMode = 1;
                SPI_Config.ClockSpeed = 1125000;
                SPI_Config.CPHA = 0;
                SPI_Config.CPOL = 0;
                SPI_Config.LSBFirst = 0;
                SPI_Config.TranBits = 8;
                SPI_Config.SelPolarity = 0;
                ret = mGinkgoDriver.ControlSPI.VSI_InitSPI(SPI_Config);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Config device error!\n");
                    return;
                }else{
                    printView.append("Config device success!\n");
                }

                byte[] write_buffer = new byte[10240];
                byte[] read_buffer = new byte[10240];
                byte spiindex = 0;
                short WriteLen = 0;
                short ReadLen = 0;

                // Get JEDEC ID
                write_buffer[0] = (byte)0x9f;//SPI Flash get JEDEC ID command
                WriteLen = 1;
                ReadLen  =3;
                ret = mGinkgoDriver.ControlSPI.VSI_WriteReadBytes(spiindex, write_buffer, WriteLen, read_buffer, ReadLen);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Get JEDEC ID error!\n");
                    printView.append(String.format("%d \n", ret));
                    return;
                }else{
                    printView.append("JEDEC ID is :");
                    int JEDEC_ID = (((int)read_buffer[0] << 16) & 0x00ffffff)| ((int)read_buffer[1] << 8) | (int)read_buffer[2];
                    printView.append(String.format("%x\n",JEDEC_ID));
                }

                // Send write enable
                write_buffer[0] = 0x06;
                WriteLen = 1;
                ret = mGinkgoDriver.ControlSPI.VSI_WriteBytes(spiindex, write_buffer, WriteLen);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("write enable error!\n");
                    printView.append(String.format("%d \n", ret));
                    return;
                }

                // Erase first sector
                write_buffer[0] = 0x20;//SPI Flash sector erase command
                write_buffer[1] = 0x00;
                write_buffer[2] = 0x00;
                write_buffer[3] = 0x00;
                WriteLen = 4;
                ret = mGinkgoDriver.ControlSPI.VSI_WriteBytes(spiindex, write_buffer, WriteLen);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Erase first sector error!\n");
                    printView.append(String.format("%d \n", ret));
                    return;
                }

                // Check the operation to complete
                do{
                    write_buffer[0] = 0x05;
                    WriteLen = 1;
                    ReadLen  = 1;
                    ret = mGinkgoDriver.ControlSPI.VSI_WriteReadBytes(spiindex, write_buffer, WriteLen, read_buffer, ReadLen);
                }while(((read_buffer[0]&0x01) == 0x01)&&(ret == ErrorType.ERR_SUCCESS));

                // Send write enable
                write_buffer[0] = 0x06;
                WriteLen = 1;
                ret = mGinkgoDriver.ControlSPI.VSI_WriteBytes(spiindex, write_buffer, WriteLen);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("write enable error!\n");
                    printView.append(String.format("%d \n", ret));
                    return;
                }

                // Page Program
                write_buffer[0] = 0x02;//SPI Flash write data command
                write_buffer[1] = 0x00;//address
                write_buffer[2] = 0x00;
                write_buffer[3] = 0x00;
                WriteLen = 4+ 256;
                for (int i = 0; i < 256; i++)
                {
                    write_buffer[4 + i] = (byte)i;
                }
                ret = mGinkgoDriver.ControlSPI.VSI_WriteBytes(spiindex, write_buffer, WriteLen);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Page Program error!\n");
                    printView.append(String.format("%d \n", ret));
                    return;
                }else{
                    printView.append("Page Program success!\n");
                }

                // Check the operation to complete
                do{
                    write_buffer[0] = 0x05;
                    WriteLen = 1;
                    ReadLen  = 1;
                    ret = mGinkgoDriver.ControlSPI.VSI_WriteReadBytes(spiindex, write_buffer, WriteLen, read_buffer, ReadLen);
                }while(((read_buffer[0]&0x01) == 0x01)&&(ret == ErrorType.ERR_SUCCESS));

                // Read Data
                write_buffer[0] = 0x03;//SPI Flash read data command
                write_buffer[1] = 0x00;//
                write_buffer[2] = 0x00;//
                write_buffer[3] = 0x00;//
                WriteLen = 4;
                ReadLen  = 256;
                ret = mGinkgoDriver.ControlSPI.VSI_WriteReadBytes(spiindex, write_buffer, WriteLen, read_buffer, ReadLen);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Get data error!\n");
                    printView.append(String.format("%d \n", ret));
                    return;
                }else{
                    for(int i = 0; i < 256; i++)
                    {
                        printView.append(String.format("%x ", read_buffer[i]));
                        if (((i + 1) % 16) == 0)
                        {
                            printView.append(String.format("\n"));
                        }
                    }
                }

                // Close device
                ret = mGinkgoDriver.ControlSPI.VSI_CloseDevice();
                if (ret != ErrorType.ERR_SUCCESS) {
                    printView.append("close device error!\n");
                    return;
                }else{
                    printView.append("close device success!\n");
                }
            }
        });

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

    private void config_usb() {
        // TODO Auto-generated method stub
        mUsbManager = (UsbManager) getSystemService(MainActivity.USB_SERVICE);
        pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(
                ACTION_USB_PERMISSION), 0);
        if((mUsbManager != null)&&(pendingIntent != null)){
            mGinkgoDriver = new GinkgoDriver(mUsbManager, pendingIntent);
        }
    }
}
