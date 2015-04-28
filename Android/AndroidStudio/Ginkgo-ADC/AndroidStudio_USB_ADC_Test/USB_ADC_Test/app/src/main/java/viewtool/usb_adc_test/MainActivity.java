package viewtool.usb_adc_test;

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

public class MainActivity extends Activity {
    // permissions
    private static final String ACTION_USB_PERMISSION = "com.viewtool.usb_adc_test.USB_PERMISSION";
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
                mUsbDevice = mGinkgoDriver.ControlADC.VAI_ScanDevice();
                if(mUsbDevice == null){
                    printView.append("No device connected!\n");
                    return;
                }
                // Open device
                ret = mGinkgoDriver.ControlADC.VAI_OpenDevice();
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Open device error!\n");
                    return;
                }else{
                    printView.append("Open device success!\n");
                }

                // Initialize ADC_CH0 channel
                byte channel = 1<<0;
                short period = 0;
                ret = mGinkgoDriver.ControlADC.VAI_InitADC(channel, period);// Cycle can be set to 0 if each channel transfer a byte of data at a time.
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Initialize ADC error!\n");
                    return;
                }else{
                    printView.append("Initialize ADC success!\n");
                }
                // Get voltage values of ADC_CH0
                short[] adc_datas = new short[4096];
                short dataNum = 1;
                ret = mGinkgoDriver.ControlADC.VAI_ReadDatas(dataNum, adc_datas);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Read ADC data error!\n");
                    return;
                }else{
                    printView.append(String.format("ADC_CH0 = %.3f \n", adc_datas[0]*3.3/4095));
                }

                // Initialize ADC_CH0 & ADC_CH1
                channel = (1<<0)|(1<<1);
                period = 0;
                ret = mGinkgoDriver.ControlADC.VAI_InitADC(channel, period);// Cycle can be set to 0 if each channel transfer a byte of data at a time.
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Initialize ADC error!\n");
                    return;
                }else{
                    printView.append("Initialize ADC success!\n");
                }
                // Get voltage values of ADC_CH0 & ADC_CH1
                dataNum = 1;
                ret = mGinkgoDriver.ControlADC.VAI_ReadDatas(dataNum, adc_datas);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Read ADC data error!\n");
                    return;
                }else{
                    printView.append(String.format("ADC_CH0 = %.3f \n", adc_datas[0]*3.3/4095));
                    printView.append(String.format("ADC_CH0 = %.3f \n", adc_datas[1]*3.3/4095));
                }

                // Initialize ADC_CH0 & ADC_CH1, each channel sampling interval is set to 1000us
                channel = (1<<0)|(1<<1);
                period = 1000;
                ret = mGinkgoDriver.ControlADC.VAI_InitADC(channel, period);// Cycle can be set to 0 if each channel transfer a byte of data at a time.
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Initialize ADC error!\n");
                    return;
                }else{
                    printView.append("Initialize ADC success!\n");
                }
                // Get voltage values of ADC_CH0 & ADC_CH1
                dataNum = 10;
                ret = mGinkgoDriver.ControlADC.VAI_ReadDatas(dataNum, adc_datas);
                if(ret != ErrorType.ERR_SUCCESS){
                    printView.append("Read ADC data error!\n");
                    return;
                }else{
                    for (int i = 0; i < dataNum; i++)
                        printView.append(String.format("ADC_CH0[%d] = %.3f \n",i, adc_datas[i*2]*3.3/4095));
                    for (int i = 0; i < dataNum; i++)
                        printView.append(String.format("ADC_CH1[%d] = %.3f \n",i, adc_datas[i*2+1]*3.3/4095));
                }

                new GetDataThread().start();
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

    public class GetDataThread extends Thread {
        int ret = 0;
        short dataNum = 0;
        short[] adc_datas = new short[4096];
        byte SamplingCnt = 0;
        public void run() {
            dataNum = 10;
            while(true){
                ret = mGinkgoDriver.ControlADC.VAI_ReadDatas(dataNum, adc_datas);
                if(ret != ErrorType.ERR_SUCCESS){
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            printView.append("Read ADC data error!\n");
                        }
                    });

                    return;
                }

                if(++SamplingCnt == 50){
                    SamplingCnt = 0;
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            printView.append(String.format("ADC_CH0 = %.3f \n", adc_datas[0]*3.3/4095));
                        }
                    });

                }

                try {
                    Thread.sleep(20);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }

    }
}
