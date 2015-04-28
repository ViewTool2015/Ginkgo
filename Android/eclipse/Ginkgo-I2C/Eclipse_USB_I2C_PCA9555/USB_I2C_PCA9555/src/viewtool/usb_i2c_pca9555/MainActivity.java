package viewtool.usb_i2c_pca9555;

import android.app.Activity;
import android.app.PendingIntent;
import android.content.Intent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import com.viewtool.Ginkgo.GinkgoDriver;
import com.viewtool.Ginkgo.ControlI2C;
public class MainActivity extends Activity {
    // permissions
    private static final String ACTION_USB_PERMISSION = "viewtool.usb_i2c_pca9555.USB_PERMISSION";

    EditText printView;
    GinkgoDriver mGinkgoDriver;
    UsbManager mUsbManager;
    UsbDevice mUsbDevice;
    PendingIntent pendingIntent;
    GetSensorDataThread getSensorData;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        config_usb();
        printView = (EditText)findViewById(R.id.printTextView);
        Button button =(Button)findViewById(R.id.start);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View arg0) {
                printView.clearComposingText();
                printView.setText("");
                int ret;
                byte[] write_buffer = new byte[8];
                byte[] read_buffer  = new byte[16];
                byte i2cindex = 0;
                short addr = 0x40;
                int subaddr = 0;
                short len = 8;

                // Scan device
                mUsbDevice = mGinkgoDriver.ControlI2C.VII_ScanDevice();
                if(mUsbDevice == null){
                    printView.append("No device connected");
                    return;
                }

                // Open device
                ret = mGinkgoDriver.ControlI2C.VII_OpenDevice();
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Open device error!\n");
                    printView.append(String.format("Error code: %d\n",ret));
                    return;
                }else{
                    printView.append("Open device success!\n");
                }

                // Config device
                ControlI2C.VII_INIT_CONFIG I2C_Config =  mGinkgoDriver.ControlI2C.new VII_INIT_CONFIG();
                I2C_Config.AddrType     = 7;      // 7-7bit mode, 10-10bit mode
                I2C_Config.ClockSpeed   = 400000; // 400k
                I2C_Config.ControlMode  = 1;      // 1- Hardware mode  2- Software mode
                I2C_Config.MasterMode   = 1;      // 1- master mode    0- slave mode
                I2C_Config.SubAddrWidth = 1;      // 0~4 Byte(s) sub-address

                ret = mGinkgoDriver.ControlI2C.VII_InitI2C(i2cindex, I2C_Config);
                if (ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Config device error!\n");
                    printView.append(String.format("%d",ret));
                    return;
                }else{
                    printView.append("Config device success!\n");
                }

                // Set all ports to output mode
                subaddr = 0x06;
                write_buffer[0] = 0x00;
                write_buffer[1] = 0x00;
                len = 2;
                ret = mGinkgoDriver.ControlI2C.VII_WriteBytes(i2cindex, addr, subaddr, write_buffer, len);
                if (ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Write bytes error!\n");
                    printView.append(String.format("Error code: %d\n",ret));
                    for(int i = 0;i < 8; i++) {
                        printView.append(String.format("%02X ", write_buffer[i]));
                    }
                    return;
                }else{
                    printView.append("All port set to output\n");
                }

                // todo add thread
                getSensorData = new GetSensorDataThread();
                getSensorData.start();

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
    /**
     * Config USB monitor
     */
    private void config_usb() {
        mUsbManager = (UsbManager) getSystemService(MainActivity.USB_SERVICE);
        pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(
                ACTION_USB_PERMISSION), 0);
        if((mUsbManager != null)&&(pendingIntent != null)){
            mGinkgoDriver = new GinkgoDriver(mUsbManager, pendingIntent);
        }
    }

    public class GetSensorDataThread extends Thread {
        int ret;
        byte[] write_buffer = new byte[8];
        byte[] read_buffer  = new byte[8];
        byte i2cindex = 0;
        short addr = 0x40;
        int subaddr = 0;
        short len = 8;
        int x,y,z;

        @Override
        public void run(){
            while(true) {
                try {

                    Thread.currentThread().sleep(1000);
                    // Control all the port output low level
                    subaddr = 0x02;
                    write_buffer[0] = 0x00;
                    write_buffer[1] = 0x00;
                    len = 2;
                    ret = mGinkgoDriver.ControlI2C.VII_WriteBytes(i2cindex, addr, subaddr, write_buffer, len);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            printView.append("Write data :\n");
                            for (int i = 0; i < 8; i++) {
                                printView.append(String.format("%02X ", write_buffer[i]));
                            }
                        }
                    });
                    if (ret != mGinkgoDriver.ErrorType.ERR_SUCCESS) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                printView.append("Write data error!\n");
                                printView.append(String.format("Error code: %d\n", ret));
                                for (int i = 0; i < 8; i++) {
                                    printView.append(String.format("%02X ", write_buffer[i]));
                                }
                            }
                        });
                    }
                    Thread.currentThread().sleep(1000);
                    // Control of all port output high level
                    subaddr = 0x02;
                    write_buffer[0] = (byte)0xff;
                    write_buffer[1] = (byte)0xff;
                    len = 2;
                    ret = mGinkgoDriver.ControlI2C.VII_WriteBytes(i2cindex, addr, subaddr, write_buffer, len);
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            printView.append("Write data :\n");
                            for (int i = 0; i < 8; i++) {
                                printView.append(String.format("%02X ", write_buffer[i]));
                            }
                        }
                    });
                    if (ret != mGinkgoDriver.ErrorType.ERR_SUCCESS) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                printView.append("Write data error!\n");
                                printView.append(String.format("Error code: %d\n", ret));
                                for (int i = 0; i < 8; i++) {
                                    printView.append(String.format("%02X ", write_buffer[i]));
                                }
                            }
                        });
                    }

                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }

    }
}
