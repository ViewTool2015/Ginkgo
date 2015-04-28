package viewtool.usb_spi_writefromfile;

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
import java.io.*;
import android.os.Environment;

import com.viewtool.Ginkgo.ErrorType;
import com.viewtool.Ginkgo.GinkgoDriver;
import com.viewtool.Ginkgo.ControlSPI;

public class MainActivity extends Activity {
    // permissions
    private static final String ACTION_USB_PERMISSION = "com.viewtool.usb_spi_writefromfile.USB_PERMISSION";
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
                SPI_Config.ClockSpeed = 36000000;
                SPI_Config.CPHA = 0;
                SPI_Config.CPOL = 0;
                SPI_Config.LSBFirst = 0;
                SPI_Config.TranBits = 8;
                ret = mGinkgoDriver.ControlSPI.VSI_InitSPI(SPI_Config);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Config device error!\n");
                    return;
                }else{
                    printView.append("Config device success!\n");
                }


                byte[] write_buffer = new byte[10240];
                byte spiindex = 0;
                short WriteLen = 0;
                String ReadBuff = new String();

                // insert SD card or not
                if(Environment.getExternalStorageState().equals(
                        android.os.Environment.MEDIA_MOUNTED)){
                    File SDCardRootDir = Environment.getExternalStorageDirectory();
                    printView.append("sd card insert!\n");
                    printView.append(SDCardRootDir.toString()+"\n");
                    File fp = new File(SDCardRootDir, "data.txt");
                    FileInputStream fs;
                    String[] strarray;

                    try{
                     fs = new FileInputStream(fp);
					BufferedReader br =new BufferedReader(new InputStreamReader(fs));
                        do{
                         ReadBuff = br.readLine();
                         printView.append(ReadBuff + "\n");
                         strarray = ReadBuff.split(" ");
                         for(int i = 0; i < strarray.length; i++){
                             printView.append(strarray[i].toString() + "  ");
                             int x = Integer.parseInt(strarray[i]);
                             printView.append(String.format("%d", x) + "\n");
                             write_buffer[i] = (byte)x;
                         }
                         WriteLen = (short)write_buffer.length;
                         ret = mGinkgoDriver.ControlSPI.VSI_WriteBytes(spiindex, write_buffer,WriteLen);
                         if(ret != ErrorType.ERR_SUCCESS){
                             printView.append("Write error!\n");
                             return;
                         }else{
                             printView.append("Write success!\n");
                         }

                     }
                     while(ReadBuff != null);

                     }
                    catch (FileNotFoundException e) {
                        e.printStackTrace();
                        printView.append("file not found");
                    }
                    catch(Exception e)
                    {
                        e.printStackTrace();
                    }
                }else{
                    printView.append("No sd card insert!\n");
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
        mUsbManager = (UsbManager) getSystemService(MainActivity.USB_SERVICE);
        pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(
                ACTION_USB_PERMISSION), 0);
        if((mUsbManager != null)&&(pendingIntent != null)){
            mGinkgoDriver = new GinkgoDriver(mUsbManager, pendingIntent);
        }
    }
}
