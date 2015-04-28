package viewtool.usb_pwm_test;

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
import android.widget.MultiAutoCompleteTextView;

import com.viewtool.Ginkgo.GinkgoDriver;
import com.viewtool.Ginkgo.ControlPWM;


public class MainActivity extends Activity {
    // permissions
    private static final String ACTION_USB_PERMISSION = "viewtool.usb_pwm_test.USB_PERMISSION";
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
            public void onClick(View view) {
                printView.clearComposingText();
                printView.setText("");
                int ret;
                //Scan device
                mUsbDevice = mGinkgoDriver.ControlPWM.VPI_ScanDevice();
                if(mUsbDevice == null){
                    printView.append("No device connected!\n");
                    return;
                }

                //Open device
                ret = mGinkgoDriver.ControlPWM.VPI_OpenDevice();
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Open device error!\n");
                    return;
                }else{
                    printView.append("Open device success!\n");
                }

                byte pwm_channel;
                pwm_channel = ControlPWM.PWM_CHANNEL.VPI_PWM_CH0;
                // Attention: PWM_CH(2n) and PWM_CH(2n+1) cannot separate set
                // Each channel duty ratio can separate set
                // Close  device

                ControlPWM.VPI_INIT_CONFIG PWM_Config = mGinkgoDriver.ControlPWM.new VPI_INIT_CONFIG();
                PWM_Config.PWM_ChannelMask = pwm_channel;
                PWM_Config.PWM_Frequency = 10000;
                PWM_Config.PWM_Mode = 0;
                PWM_Config.PWM_Polarity = 0;
                PWM_Config.PWM_Pulse = 50;
                ret = mGinkgoDriver.ControlPWM.VPI_InitPWM(PWM_Config);
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Initialize pwm error!\n");
                    return;
                }else{
                    printView.append("Initialize pwm success!\n");
                }
                // Start pwm ch0
                ret = mGinkgoDriver.ControlPWM.VPI_StartPWM(pwm_channel);
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Start pwm error!\n");
                    return;
                }else{
                    printView.append("Start pwm success!\n");
                }
                // ret = mGinkgoDriver.ControlPWM.VPI_StopPWM(pwm_channel);



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
