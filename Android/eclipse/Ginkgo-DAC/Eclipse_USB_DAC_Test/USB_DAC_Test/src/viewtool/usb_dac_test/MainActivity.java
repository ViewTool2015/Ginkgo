package viewtool.usb_dac_test;

import android.app.Activity;
import android.app.PendingIntent;
import android.hardware.usb.UsbDevice;
import android.hardware.usb.UsbManager;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.content.Intent;
import android.widget.EditText;

import com.viewtool.Ginkgo.ErrorType;
import com.viewtool.Ginkgo.GinkgoDriver;


public class MainActivity extends Activity {
    private static final String ACTION_USB_PERMISSION = "viewtool.usb_dac_test.USB_PERMISSION";
    EditText printView;
    GinkgoDriver mGinkgoDriver;
    UsbManager mUsbManager;
    UsbDevice mUsbDevice;
    PendingIntent pendingIntent;
    int StatusMod;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        printView =(EditText) findViewById(R.id.printTextView);
        config_usb();
        StatusMod = 0;
        final Button button =(Button)findViewById(R.id.start);
        button.setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View arg0) {
                int ret;
                short[] WaveDataBuff = new short[1024];
                byte dac_channel;
                short WaveDataNum;
                short period ;
                switch (StatusMod)
                {
                    case 0:
                        printView.clearComposingText();
                        printView.setText("");

                        //Scan device
                        mUsbDevice = mGinkgoDriver.ControlDAC.VDA_ScanDevice();
                        if(mUsbDevice == null){
                            printView.append("No device connected!\n");
                            return;
                        }
                        //Open device
                        ret = mGinkgoDriver.ControlDAC.VDA_OpenDevice();
                        if(ret != ErrorType.ERR_SUCCESS){
                            printView.append("Open device error!\n");
                            return;
                        }else{
                            printView.append("Open device success!\n");
                        }
                        button.setText("DC");
                        StatusMod =1;
                        break;
                    case 1:
                        // Configuration DAC to output dc voltage
                        WaveDataBuff[0] = 1532;//Vout = (3.3V/4095)*1532 = 1.23V
                        printView.append(String.format("Output DC Voltage:%f v\n",WaveDataBuff[0]*3.3/4095));
                        // init data
                        dac_channel = 0; WaveDataNum = 1; period = 0;
                        ret = mGinkgoDriver.ControlDAC.VDA_InitDAC(dac_channel, WaveDataBuff, WaveDataNum, period);
                        if(ret != ErrorType.ERR_SUCCESS){
                            printView.append("Config DAC error!\n");
                            return;
                        }
                        // Start DAC to output
                        ret = mGinkgoDriver.ControlDAC.VDA_StartDAC(dac_channel);
                        if(ret != ErrorType.ERR_SUCCESS){
                            printView.append("Config DAC error!\n");
                            return;
                        }
                        printView.append("press \"quare wave\" to continue\n");
                        button.setText("quare wave");
                        StatusMod =2;
                        break;
                    case 2:
                        // Configuration DAC to Quare wave
                        for(int i = 0; i < 64; i++)
                        {
                            WaveDataBuff[i] = 0xfff;
                            WaveDataBuff[i + 64] = 0x00;
                        }
                        printView.append("Output Square Wave\n");
                        // init data
                        dac_channel = 0; WaveDataNum = 128; period = 100;
                        ret = mGinkgoDriver.ControlDAC.VDA_InitDAC(dac_channel, WaveDataBuff, WaveDataNum, period);
                        if(ret != ErrorType.ERR_SUCCESS){
                            printView.append("Config DAC error!\n");
                            return;
                        }
                        // Start DAC to output
                        ret = mGinkgoDriver.ControlDAC.VDA_StartDAC(dac_channel);
                        if(ret != ErrorType.ERR_SUCCESS){
                            printView.append("Start DAC error!\n");
                            return;
                        }
                        printView.append("press \"sawtooth wave \" to continue\n");
                        button.setText("sawtooth wave ");
                        StatusMod =3 ;
                        break;
                    case 3:
                        // Configuration DAC to sawtooth wave
                        for(int i = 0; i < 128; i++)
                        {
                            WaveDataBuff[i] = (short)(i * 0xfff /128);
                        }
                        printView.append("Output sawtooth wave\n");
                        // init data
                        dac_channel = 0; WaveDataNum = 128; period = 100;
                        ret = mGinkgoDriver.ControlDAC.VDA_InitDAC(dac_channel, WaveDataBuff, WaveDataNum, period);
                        if(ret != ErrorType.ERR_SUCCESS){
                            printView.append("Config DAC error!\n");
                            return;
                        }
                        // Start DAC to output
                        ret = mGinkgoDriver.ControlDAC.VDA_StartDAC(dac_channel);
                        if(ret != ErrorType.ERR_SUCCESS){
                            printView.append("Start DAC error!\n");
                            return;
                        }
                        printView.append("press \"triangle wave \" to continue\n");
                        button.setText("triangle wave ");
                        StatusMod =4 ;
                        break;
                    case 4:
                        // Configuration DAC to triangle wave
                        for(int i = 0; i < 64; i++)
                        {
                            WaveDataBuff[i] = (short)(i*0xFFF/64);
                            WaveDataBuff[64 + i] = (short)((64 - i) * 0xFFF / 64);
                        }
                        printView.append("Output triangle wave\n");
                        // init data
                        dac_channel = 0; WaveDataNum = 128; period = 100;
                        ret = mGinkgoDriver.ControlDAC.VDA_InitDAC(dac_channel, WaveDataBuff, WaveDataNum, period);
                        if(ret != ErrorType.ERR_SUCCESS){
                            printView.append("Config DAC error!\n");
                            return;
                        }
                        // Start DAC to output
                        ret = mGinkgoDriver.ControlDAC.VDA_StartDAC(dac_channel);
                        if(ret != ErrorType.ERR_SUCCESS){
                            printView.append("Start DAC error!\n");
                            return;
                        }
                        printView.append("press \"Sine wave \" to continue\n");
                        button.setText("Sine wave ");
                        StatusMod =5;
                        break;
                    case 5:
                        // Configuration DAC to sine wave
                        for(int i = 0; i < 128; i++)
                        {
                            WaveDataBuff[i] = (short)( 0xFFF*(1+  Math.sin(2*Math.PI*i/128))/2.0);
                        }
                        printView.append("Output sine wave\n");
                        // init data
                        dac_channel = 0; WaveDataNum = 128; period = 100;
                        ret = mGinkgoDriver.ControlDAC.VDA_InitDAC(dac_channel, WaveDataBuff, WaveDataNum, period);
                        if(ret != ErrorType.ERR_SUCCESS){
                            printView.append("Config DAC error!\n");
                            return;
                        }
                        // Start DAC to output
                        ret = mGinkgoDriver.ControlDAC.VDA_StartDAC(dac_channel);
                        if(ret != ErrorType.ERR_SUCCESS){
                            printView.append("Start DAC error!\n");
                            return;
                        }
                        printView.append("press \"Start \" to continue\n");
                        button.setText("Start ");
                        StatusMod =0;
                        break;
                    default:
                        printView.append("It's a bad code\n");
                        break;
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
    
    private void config_usb() {
        mUsbManager = (UsbManager) getSystemService(MainActivity.USB_SERVICE);
        pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(
                ACTION_USB_PERMISSION), 0);
        if((mUsbManager != null)&&(pendingIntent != null)){
            mGinkgoDriver = new GinkgoDriver(mUsbManager, pendingIntent);
        }
    }
    
}
