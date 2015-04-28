package viewtool.usb_uart_test;

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

import com.viewtool.Ginkgo.ControlUART;
import com.viewtool.Ginkgo.GinkgoDriver;
import com.viewtool.Ginkgo.ControlCNT;

import java.util.Timer;


public class MainActivity extends Activity {
    private static final String ACTION_USB_PERMISSION = "com.viewtool.wdluo.usb_spi_test.USB_PERMISSION";
    Timer timer;
    EditText printView;
    GinkgoDriver mGinkgoDriver;
    UsbManager mUsbManager;
    UsbDevice mUsbDevice;
    PendingIntent pendingIntent;
    GetData getData;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);


        printView =(EditText) findViewById(R.id.printTextView);
        timer = new Timer();
        config_usb();

        Button button =(Button)findViewById(R.id.start);
        button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View view) {
                printView.clearComposingText();
                printView.setText("");
                int ret;

                //Scan device
                mUsbDevice = mGinkgoDriver.ControlUART.UART_ScanDevice();
                if(mUsbDevice == null){
                    printView.append("No device connected!\n");
                    return;
                }

                //Open device
                ret = mGinkgoDriver.ControlUART.UART_OpenDevice();
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Open device error!\n");
                    return;
                }else{
                    printView.append("Open device success!\n");
                }

                try {
                    Thread.currentThread().sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                ControlUART.UART_INIT_CONFIG UARTConfig = mGinkgoDriver.ControlUART. new UART_INIT_CONFIG();
                UARTConfig.BaudRate = 115200;
                UARTConfig.Parity = 0;
                UARTConfig.RS485Mode = 485;
                UARTConfig.StopBits = 0;
                UARTConfig.WordLength = 8;
                ret = mGinkgoDriver.ControlUART.UART_InitDevice((byte)(0), UARTConfig);
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Config device error!\n");
                    return;
                }else{
                    printView.append("Config device success!\n");
                }

                getData = new GetData();
                getData.start();

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

    public  class GetData extends Thread{
        int ret;
        byte index = 0;

        short[] len = new short[1];
        @Override
        public void run(){
            while(true){
                byte[] read_buffer = new byte[64];
                ret = mGinkgoDriver.ControlUART.UART_ReadBytes(index, read_buffer, len);
                    if(len[0] > 0){
                        final String tt = new String(read_buffer);
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                printView.append(tt + "\n");
                            }
                        });
                        ret = mGinkgoDriver.ControlUART.UART_WriteBytes(index, read_buffer, len[0]);


                        len[0] = 0;
                    }
                try {
                    Thread.currentThread().sleep(300);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }


            }

        }
    }
}
