package viewtool.usb_spi_slavewriteread;

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
    private static final String ACTION_USB_PERMISSION = "viewtool.usb_spi_slavewriteread.USB_PERMISSION";
    EditText printView;
    GinkgoDriver mGinkgoDriver;
    UsbManager mUsbManager;
    UsbDevice mUsbDevice;
    PendingIntent pendingIntent;
    GetDataThread getDataThread;
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
                SPI_Config.ClockSpeed = 1000000;
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
                double StartTime, EndTime;

                byte spiindex = 0;
                short WriteLen = 10000;
                short ReadLen = 10000;
                int i;
                // Write test
                StartTime = System.currentTimeMillis();
                printView.append(String.format("%f", StartTime));
                for(i = 0; i <400; i++){
                    ret = mGinkgoDriver.ControlSPI.VSI_WriteBytes(spiindex, write_buffer, WriteLen);
                    if(ret != ErrorType.ERR_SUCCESS){
                        printView.append("Write data error!\n");
                        return;
                    }
                }
                EndTime = System.currentTimeMillis();
                printView.append(String.format("%f", EndTime));
                printView.append(String.format("Write Data Numbers: %d Bytes\n", i * 10000));
                printView.append(String.format("Write Data Elapsed Time: %d ms \n", (int)(EndTime - StartTime)));
                printView.append(String.format("Write Data Speed: %f KByte/s\n", ((i * 10000) / 1024) / ((EndTime - StartTime)*0.001)));

                // Read test
                StartTime = System.currentTimeMillis();
                printView.append(String.format("%f", StartTime));
                for(i = 0; i <400; i++){
                    ret = mGinkgoDriver.ControlSPI.VSI_ReadBytes(spiindex, read_buffer, ReadLen);
                    if(ret != ErrorType.ERR_SUCCESS){
                        printView.append("Read data error!\n");
                        return;
                    }
                }
                EndTime = System.currentTimeMillis();
                printView.append(String.format("%f", EndTime));
                printView.append(String.format("Read Data Numbers: %d Bytes\n", i * 10000));
                printView.append(String.format("Read Data Elapsed Time: %d ms \n", (int)(EndTime - StartTime)));
                printView.append(String.format("Read Data Speed: %f KByte/s\n", ((i * 10000) / 1024) / ((EndTime - StartTime)*0.001)));

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
        mUsbManager = (UsbManager) getSystemService(MainActivity.USB_SERVICE);
        pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(
                ACTION_USB_PERMISSION), 0);
        if((mUsbManager != null)&&(pendingIntent != null)){
            mGinkgoDriver = new GinkgoDriver(mUsbManager, pendingIntent);
        }
    }

    public class GetDataThread extends Thread{
        int ret;
        byte[] read_buffer = new byte[10240];
        int[] read_data_num ;
        @Override
        public  void run(){
            ret = mGinkgoDriver.ControlSPI.VSI_SlaveReadBytes(read_buffer, read_data_num,100);
            if(ret != ErrorType.ERR_SUCCESS){
                printView.append("Slave write data error!\n");
                return;
            }
            else if (read_data_num[0] > 0)
            {
                printView.append("Read data ");
                for ( int i = 0; i < read_data_num[0]; i++)
                {
                    printView.append(String.format("%x",read_buffer[i]));
                }
                printView.append("\n");
            }
        }
    }
}
