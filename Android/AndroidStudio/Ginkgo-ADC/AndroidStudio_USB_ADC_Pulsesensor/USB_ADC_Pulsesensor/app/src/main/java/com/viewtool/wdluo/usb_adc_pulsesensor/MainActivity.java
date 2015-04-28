package com.viewtool.wdluo.usb_adc_pulsesensor;

import android.app.Activity;
import android.app.AlertDialog;
import android.app.PendingIntent;
import android.content.DialogInterface;
import android.content.Intent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;

import com.viewtool.Ginkgo.GinkgoDriver;




public class MainActivity extends Activity {
    // permissions
    private static final String ACTION_USB_PERMISSION = "com.viewtool.wdluo.usb_adc_pulsesensor.USB_PERMISSION";
    GinkgoDriver mGinkgoDriver;
    UsbManager mUsbManager;
    UsbDevice mUsbDevice;
    PendingIntent pendingIntent;
    fft myFFT = new fft(1024*16);
    fft.compx[] s = new fft.compx[myFFT.pointNum];
    int measureIndex = 0;
    WaveSurfaceView waveSurfaceView;
    GetAdcDataThread getAdcDataThread;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        for (int i = 0; i < myFFT.pointNum; i++) {
            s[i] = myFFT. new compx();
            s[i].imag = 0;
            s[i].real = 0;
        }
        config_usb();
        setContentView(R.layout.activity_main);
        waveSurfaceView = (WaveSurfaceView)findViewById(R.id.surfaceView);
        Button button =(Button)findViewById(R.id.startButton);
        button.setOnClickListener(new Button.OnClickListener(){
            @Override
            public void onClick(View view) {
                // TODO Auto-generated method stub
                Button button = (Button)view;
                String buttonText = button.getText().toString();
                int ret;
                if(buttonText.equals("Start")){
                    //Scan device
                    mUsbDevice = mGinkgoDriver.ControlADC.VAI_ScanDevice();
                    if(mUsbDevice == null){
                        warring("No device connected!");
                        return;
                    }
                    //Open device
                    ret = mGinkgoDriver.ControlADC.VAI_OpenDevice();
                    if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                        warring("Open device error!");
                        return;
                    }
                    //Config device
                    ret = mGinkgoDriver.ControlADC.VAI_InitADC((byte)0x01,(short)0);
                    if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                        warring("Config device error!");
                        return;
                    }
                    getAdcDataThread = new GetAdcDataThread();
                    getAdcDataThread.setRunning(true);
                    getAdcDataThread.start();
                    button.setText("Stop");
                }else{
                    getAdcDataThread.setRunning(false);
                    try {
                        getAdcDataThread.join();
                    }catch(InterruptedException e){
                    }
                    button.setText("Start");
                }
            }
        });
    }
    private void warring(String str){
        new AlertDialog.Builder((this))
                .setTitle("Alerting Message")
                .setMessage(str)
                .setNegativeButton("Close", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        //do nothing - it will close on its own
                    }
                }).show();
    }
    /**
     * Config USB monitor
     */
    private void config_usb() {
        // TODO Auto-generated method stub
        mUsbManager = (UsbManager) getSystemService(MainActivity.USB_SERVICE);
        pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(
                ACTION_USB_PERMISSION), 0);
        if((mUsbManager != null)&&(pendingIntent != null)){
            mGinkgoDriver = new GinkgoDriver(mUsbManager, pendingIntent);
        }else{
            warring("Device can not support usb otg!");
        }
    }

    //Thread to get adc data
    public class GetAdcDataThread extends Thread  {
        private boolean _run = false;
        int ret;

        short[] adcDataTmp = new short[1];
        public void setRunning(boolean run) {
            _run = run;
        }
        @Override
        public void run() {
            // TODO Auto-generated method stub
            while (_run) {
                try {
                    Thread.sleep(5);
                    ret = mGinkgoDriver.ControlADC.VAI_ReadDatas((short)1,adcDataTmp);
                    if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){

                    }
                    s[measureIndex].real = adcDataTmp[0]*3.3/4096;
                    waveSurfaceView.setWaveData((short)(adcDataTmp[0]>>2),measureIndex);
                    measureIndex++;
                    if(measureIndex >= 1024) {
                        measureIndex = 0;
                        for (int i = 0; i < myFFT.pointNum; i++) {
                            s[i].imag = 0;
                        }
                        for (int i = 1024; i < myFFT.pointNum; i++) {
                            s[i].real = 0;
                        }
                        myFFT.calculate_fft(s);                       // FFT

                        for (int i = 1; i < myFFT.pointNum/2; i++) {  //complex number.real part
                            s[i].real = Math.sqrt(s[i].real * s[i].real + s[i].imag * s[i].imag);
                            //waveSurfaceView.fftData[i] = s[i].real;
                        }

                        // Amplitude maximum points
                        double t;
                        int index = 0;
                        double heartRate;
                        double maxValue=0;
                        int maxIndex=0;
                        int windowSize = myFFT.pointNum*30/(60*200);
                        for (int i = (myFFT.pointNum*50/(60*200)); i < (myFFT.pointNum/2)-windowSize; i++) {
                            t = 0;
                            for(int j=0;j<windowSize;j++){
                                if (s[i+j].real > t) {
                                    t = s[i+j].real;
                                    index = i+j;
                                }
                            }
                            if((index == i)||(index == (i+windowSize))){
                                continue;
                            }else{
                                heartRate = 60*200*index/myFFT.pointNum;
                                if((heartRate > 50)&&(heartRate < 150)) {
                                    if (t > maxValue) {
                                        maxValue = t;
                                        maxIndex = index;
                                    }
                                }
                            }
                        }
                        heartRate = 60*200*maxIndex/myFFT.pointNum;
                        waveSurfaceView.heartRate = heartRate;
                    }
                    //waveSurfaceView.heartRate = 456;
                } catch (Exception e) {
                    // TODO Auto-generated catch block
                    e.printStackTrace();
                    //System.out.println("thread error...");
                }
            }
        }
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
}
