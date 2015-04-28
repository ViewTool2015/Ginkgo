package viewtool.usb_cnt_counter;

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

import com.viewtool.Ginkgo.ControlSPI;
import com.viewtool.Ginkgo.GinkgoDriver;
import com.viewtool.Ginkgo.ControlCNT;

import java.util.Timer;
import java.util.TimerTask;
import java.util.logging.Handler;
import java.util.logging.LogRecord;

public class MainActivity extends Activity {
    private static final String ACTION_USB_PERMISSION = "com.viewtool.wdluo.usb_spi_test.USB_PERMISSION";
    Timer timer;
    EditText printView;
    GinkgoDriver mGinkgoDriver;
    UsbManager mUsbManager;
    UsbDevice mUsbDevice;
    PendingIntent pendingIntent;
    int i = 0;
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
            public void onClick(View arg0) {
                int ret;

                //timer.cancel();
                printView.clearComposingText();
                printView.setText("11");

                //Scan device
                mUsbDevice = mGinkgoDriver.ControlCNT.CNT_ScanDevice();
                if(mUsbDevice == null){
                    printView.append("No device connected!\n");
                    return;
                }

                //Open device
                ret = mGinkgoDriver.ControlCNT.CNT_OpenDevice();
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

                //Config counter
                ControlCNT.CNT_INIT_CONFIG CNT_Config = mGinkgoDriver.ControlCNT.new CNT_INIT_CONFIG();
                CNT_Config.CounterBitWide = 32;
                CNT_Config.CounterMode  = 0;
                CNT_Config.CounterPolarity = 0;
                ret = mGinkgoDriver.ControlCNT.CNT_InitCounter((byte)0, CNT_Config);
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Config device error!\n");
                    return;
                }else{
                    printView.append("Config device success!\n");
                }

                // Start Counter
                ret = mGinkgoDriver.ControlCNT.CNT_StartCounter((byte)(0 << 1));
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Start Counter error!\n");
                    return;
                }else{
                    printView.append("Start Counter success!\n");
                }

                //
                timer.schedule(new TimerTask() {
                    @Override
                    public void run() {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                int ret;
                                int[] CntValue = new int[1];
                                printView.append("1s\n");
                                //get counter value
                                ret = mGinkgoDriver.ControlCNT.CNT_GetCounter((byte)(1<<0), CntValue);
                                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                                    printView.append("Get counter value error!\n");
                                    return;
                                }else{
                                    printView.append(String.format("Counter Value %d \n", CntValue[0]));
                                }
                            }
                        });
                    }
                },1000,1000);


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
