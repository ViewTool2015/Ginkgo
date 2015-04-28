package viewtool.usb_i2c_at24c02_sctl;

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

import com.viewtool.Ginkgo.ErrorType;
import com.viewtool.Ginkgo.GinkgoDriver;
import com.viewtool.Ginkgo.ControlI2C;

  /*
  Hardware Connection  (This is for your reference only)
  AT24C02        Ginkgo USB-I2C Adapter
  1.A0      <-->  GND(Pin19/Pin20)
  2.A1	    <-->  GND(Pin19/Pin20)
  3.A2      <-->  GND(Pin19/Pin20)
  4.GND     <-->  GND(Pin19/Pin20)
  5.SDA	    <-->  SI2C_SDA0(Pin6)
  6.SCL	    <-->  SI2C_SCL0 (Pin4)
  7.WP	    <-->  GND(Pin19/Pin20)
  8.VCC	    <-->  VCC(Pin1/Pin2)
  */

public class MainActivity extends Activity {
    // permissions
    private static final String ACTION_USB_PERMISSION = "viewtool.usb_i2c_at24c02_sctl.USB_PERMISSION";
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
        Button button =(Button)findViewById(R.id.start);
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
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Open device error!\n");
                    return;
                }else{
                    printView.append("Open device success!\n");
                }

                // Config device
                ControlI2C.VII_INIT_CONFIG I2C_Config =  mGinkgoDriver.ControlI2C.new VII_INIT_CONFIG();
                I2C_Config.AddrType     = 7;      // 7-7bit mode, 10-10bit mode
                I2C_Config.ClockSpeed   = 400000; // 400k
                I2C_Config.ControlMode  = 2;      // 1- Hardware mode  2- Software mode
                I2C_Config.MasterMode   = 1;      // 1- master mode    0- slave mode
                I2C_Config.SubAddrWidth = 1;      // 0~4 Byte(s) sub-address
                byte i2cindex = 0;
                ret = mGinkgoDriver.ControlI2C.VII_InitI2C(i2cindex, I2C_Config);
                if (ret != ErrorType.ERR_SUCCESS){
                    printView.append("Config device error!\n");
                    printView.append(String.format("%d",ret));
                    return;
                }else{
                    printView.append("Config device success!\n");
                }

                //Config timing,Unit of time for microseconds
                ControlI2C.VII_TIME_CONFIG I2C_TimeConfig = mGinkgoDriver.ControlI2C.new VII_TIME_CONFIG();
                I2C_TimeConfig.tSU_STA = 5;
                I2C_TimeConfig.tHD_STA = 4;
                I2C_TimeConfig.tLOW = 5;
                I2C_TimeConfig.tHIGH = 5;
                I2C_TimeConfig.tSU_DAT = 1;
                I2C_TimeConfig.tSU_STO = 4;
                I2C_TimeConfig.tBuf = 5;
                ret = mGinkgoDriver.ControlI2C.VII_TimeConfig( i2cindex, I2C_TimeConfig);
                if (ret != ErrorType.ERR_SUCCESS){
                    printView.append("Config time error!\n");
                    printView.append(String.format("%d",ret));
                    return;
                }else{
                    printView.append("Config time success!\n");
                }

                // Write data
                byte[] write_buffer = new byte[8];
                for(int i = 0;i < 8; i++){
                    write_buffer[i] = (byte)i;
                }
                i2cindex = 0;
                short addr = 0xa0;
                int subaddr = 0;
                short len = 8;
                ret = mGinkgoDriver.ControlI2C.VII_WriteBytes(i2cindex, addr, subaddr, write_buffer, len);
                if (ret != ErrorType.ERR_SUCCESS){
                    printView.append("Write bytes error!\n");
                    printView.append(String.format("Error code: %d\n",ret));
                    for(int i = 0;i < 8; i++) {
                        printView.append(String.format("%02X ", write_buffer[i]));
                    }
                    return;
                }else{
                    printView.append("Write bytes success\n");
                }

                // Delay to ensure write complete
                try {
                    Thread.sleep(500);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                //Read data
                byte[] read_buffer = new byte[8];
                i2cindex = 0;
                addr = 0xa0;
                subaddr = 0;
                len = 8;
                ret = mGinkgoDriver.ControlI2C.VII_ReadBytes(i2cindex, addr, subaddr, read_buffer, len);
                if (ret != ErrorType.ERR_SUCCESS){
                    printView.append("Read data error!\n");
                    printView.append(String.format("Error code: %d\n",ret));
                    for(int i = 0;i < 8; i++) {
                        printView.append(String.format("%02X ", read_buffer[i]));
                    }
                    return;
                }else{
                    String read_data_str = new String("");
                    printView.append("Read data :\n");
                    for(int i = 0; i < 8; i++){
                        read_data_str += String.format("%02X ",read_buffer[i]);
                    }
                    printView.append(read_data_str);
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