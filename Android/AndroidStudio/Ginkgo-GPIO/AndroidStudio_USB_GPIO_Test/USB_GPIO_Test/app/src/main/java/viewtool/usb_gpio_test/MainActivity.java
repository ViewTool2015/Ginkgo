package viewtool.usb_gpio_test;

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

import com.viewtool.Ginkgo.GinkgoDriver;
import com.viewtool.Ginkgo.ControlGPIO;

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

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        printView =(EditText) findViewById(R.id.printTextView);
        config_usb();
        Button button =(Button)findViewById(R.id.start);
        button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                printView.clearComposingText();
                printView.setText("");
                int ret;
                // Scan device
                mUsbDevice = mGinkgoDriver.ControlGPIO.VGI_ScanDevice();
                if(mUsbDevice == null){
                    printView.append("No device connected!\n");
                    return;
                }
                // Open device
                ret = mGinkgoDriver.ControlGPIO.VGI_OpenDevice();
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Open device error!\n");
                    return;
                }else{
                    printView.append("Open device success!\n");
                }

                // Set gpio 7&8to output
                ret = mGinkgoDriver.ControlGPIO.VGI_SetOutput((short)((1<<7)|(1<<8)));
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Set output error!\n");
                    return;
                }else{
                    printView.append("Set output success!\n");
                }

                // Set gpio7&8
                ret = mGinkgoDriver.ControlGPIO.VGI_SetPins((short)((1<<7)|(1<<8)));
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Set pin high error!\n");
                    return;
                }else{
                    printView.append("Set pin high success!\n");
                }

                // Reset gpio7&8
                ret = mGinkgoDriver.ControlGPIO.VGI_ResetPins((short)((1<<7)|(1<<8)));
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Reset pin low error!\n");
                    return;
                }else{
                    printView.append("Reset pin low success!\n");
                }

                // Set gpio4&5 to input
                ret = mGinkgoDriver.ControlGPIO.VGI_SetInput((short)((1<<4)|(1<<5)));
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Set pin input error!\n");
                    return;
                }else{
                    printView.append("Set pin input success!\n");
                }

                // Get gpio4&5 value
                short[] ReadData = new short[1];
                ret = mGinkgoDriver.ControlGPIO.VGI_ReadDatas((short)((1<<4)|(1<<5)), ReadData);
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Get pin data error!\n");
                    return;
                }else{
                    printView.append("Get pin data success!\n");
                    if((ReadData[0]&(1<<4)) != 0)
                    {
                        printView.append("GPIO_4 is high-level\n");
                    }
                    else
                    {
                        printView.append("GPIO_4 is high-level\n");
                    }

                    if((ReadData[0]&(1<<5)) != 0)
                    {
                        printView.append("GPIO_5 is high-level\n");
                    }
                    else
                    {
                        printView.append("GPIO_5 is high-level\n");
                    }
                }

                // Set GPIO_4 and GPIO_5 to OD(Bi-directional, need pull-up resistor)
                //ret = mGinkgoDriver.ControlGPIO.VGI_SetOpenDrain((short)((1<<4)|(1<<5)));
                // Close device
                ret = mGinkgoDriver.ControlGPIO.VGI_CloseDevice();
                if ( ret != mGinkgoDriver.ErrorType.ERR_SUCCESS)
                {
                    printView.append("Close device error!");
                    return;
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

    private void config_usb() {
        mUsbManager = (UsbManager) getSystemService(MainActivity.USB_SERVICE);
        pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(
                ACTION_USB_PERMISSION), 0);
        if((mUsbManager != null)&&(pendingIntent != null)){
            mGinkgoDriver = new GinkgoDriver(mUsbManager, pendingIntent);
        }
    }
}
