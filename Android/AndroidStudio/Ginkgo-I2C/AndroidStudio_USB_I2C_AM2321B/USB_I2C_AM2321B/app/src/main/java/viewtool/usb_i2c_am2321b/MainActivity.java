package viewtool.usb_i2c_am2321b;

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
    // permission
    private static final String ACTION_USB_PERMISSION = "viewtool.usb_i2c_am2321b.USB_PERMISSION";
    EditText printView;
    GinkgoDriver mGinkgoDriver;
    UsbManager mUsbManager;
    UsbDevice mUsbDevice;
    PendingIntent pendingIntent;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        config_usb();
        printView = (EditText)findViewById(R.id.printTextView);
        Button button = (Button)findViewById((R.id.start));
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View arg0) {
                printView.clearComposingText();
                printView.setText("");
                int ret;
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
                    return;
                }else{
                    printView.append("Open device success!\n");
                }

                // Config device
                ControlI2C.VII_INIT_CONFIG I2C_Config =  mGinkgoDriver.ControlI2C.new VII_INIT_CONFIG();
                I2C_Config.AddrType     = 7;      // 7-7bit mode, 10-10bit mode
                I2C_Config.ClockSpeed   = 100000; // 400k
                I2C_Config.ControlMode  = 1;      // 1- Hardware mode  2- Software mode
                I2C_Config.MasterMode   = 1;      // 1- master mode    0- slave mode
                I2C_Config.SubAddrWidth = 0;      // 0~4 Byte(s) sub-address
                byte i2cindex = 0;
                ret = mGinkgoDriver.ControlI2C.VII_InitI2C(i2cindex, I2C_Config);
                if (ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Config device error!\n");
                    printView.append(String.format("%d",ret));
                    return;
                }else{
                    printView.append("Config device success!\n");
                }
                byte[] write_buffer = new byte[8];
                byte[] read_buffer  = new byte[8];

                while(true){
                    short addr = 0xb8;
                    int subaddr = 0;
                    short len = 1;
                    mGinkgoDriver.ControlI2C.VII_WriteBytes(i2cindex, addr, subaddr, write_buffer, len);

                    write_buffer[0] = 0x03;
                    write_buffer[1] = 0x00;
                    write_buffer[2] = 0x04;
                    len = 3;
                    ret = mGinkgoDriver.ControlI2C.VII_WriteBytes(i2cindex, addr, subaddr, write_buffer, len);
                    if (ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                        printView.append("Write bytes error!\n");
                        printView.append(String.format("Error code: %d\n",ret));
                        for(int i = 0;i < 8; i++) {
                            printView.append(String.format("%02X ", write_buffer[i]));
                        }
                        return;
                    }else{
                        printView.append("Write bytes success\n");
                    }

                    // Read out temperature and humidity
                    len = 8;
                    ret = mGinkgoDriver.ControlI2C.VII_WriteBytes(i2cindex, addr, subaddr, write_buffer, len);
                    if (ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                        printView.append("Read bytes error!\n");
                        printView.append(String.format("Error code: %d\n",ret));
                        for(int i = 0;i < 8; i++) {
                            printView.append(String.format("%02X ", write_buffer[i]));
                        }
                        return;
                    }else{
                        double t = ((read_buffer[4] << 8) | read_buffer[5]) / 10.0;
                        double h = ((read_buffer[2] << 8) | read_buffer[3]) / 10.0;
                        printView.append("temperature:"+ String.format("%f", t) + " ℃" + "\n");
                        printView.append("temperature:"+ String.format("%f", h) + " %" + "\n");
                    }


                }

            }
        });
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
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
}
