package viewtool.usb_can_test;

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

import com.viewtool.Ginkgo.ControlCAN;
import com.viewtool.Ginkgo.GinkgoDriver;

public class MainActivity extends Activity {
    private static final String ACTION_USB_PERMISSION = "viewtool.usb_can_test.USB_PERMISSION";

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
            public void onClick(View view) {
                printView.clearComposingText();
                printView.setText("");
                int ret;


                // Scan device
                mUsbDevice = mGinkgoDriver.ControlCAN.VCI_ScanDevice();
                if(mUsbDevice == null){
                    printView.append("No device connected");
                    return;
                }

                // Open device
                ret = mGinkgoDriver.ControlCAN.VCI_OpenDevice();
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Open device error!\n");
                    printView.append(String.format("Error code: %d\n",ret));
                    return;
                }else{
                    printView.append("Open device success!\n");
                }

                ControlCAN.VCI_INIT_CONFIG_EX CAN_InitEx = mGinkgoDriver.ControlCAN.new VCI_INIT_CONFIG_EX();
                CAN_InitEx.CAN_ABOM = 0;//Automatic bus-off management
                // Loop back
                CAN_InitEx.CAN_Mode = 1;
                //1Mbps
                CAN_InitEx.CAN_BRP = 6;
                CAN_InitEx.CAN_BS1 = 3;
                CAN_InitEx.CAN_BS2 = 2;
                CAN_InitEx.CAN_SJW = 1;
                CAN_InitEx.CAN_NART = 1;//No automatic retransmission
                CAN_InitEx.CAN_RFLM = 0;//Receive FIFO locked mode
                CAN_InitEx.CAN_TXFP = 0;//Transmit FIFO priority
                CAN_InitEx.CAN_RELAY = 0;
                ret = mGinkgoDriver.ControlCAN.VCI_InitCANEx((byte)0, CAN_InitEx);
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Init device failed!\n");
                    printView.append(String.format("Error code: %d\n",ret));
                    return;
                }else{
                    printView.append("Init device success!\n");
                }
                //Set filter
                ControlCAN.VCI_FILTER_CONFIG CAN_FilterConfig = mGinkgoDriver.ControlCAN.new VCI_FILTER_CONFIG();
                CAN_FilterConfig.FilterIndex = 0;
                CAN_FilterConfig.Enable = 1;//Enable
                CAN_FilterConfig.ExtFrame = 0;
                CAN_FilterConfig.FilterMode = 0;
                CAN_FilterConfig.ID_IDE = 0;
                CAN_FilterConfig.ID_RTR = 0;
                CAN_FilterConfig.ID_Std_Ext = 0;
                CAN_FilterConfig.MASK_IDE = 0;
                CAN_FilterConfig.MASK_RTR = 0;
                CAN_FilterConfig.MASK_Std_Ext = 0;
                ret = mGinkgoDriver.ControlCAN.VCI_SetFilter((byte)0, CAN_FilterConfig);
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Set filter failed!\n");
                    printView.append(String.format("Error code: %d\n",ret));
                    return;
                }else{
                    printView.append("Set filter success!\n");
                }

                // Start CAN
                ret = mGinkgoDriver.ControlCAN.VCI_StartCAN((byte)0);
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Start CAN failed!\n");
                    printView.append(String.format("Error code: %d\n",ret));
                    return;
                }else{
                    printView.append("Start CAN success!\n");
                }

                ControlCAN.VCI_CAN_OBJ CAN_SendData[] =  new ControlCAN.VCI_CAN_OBJ[2];
                for (int i = 0; i < CAN_SendData.length; i++) {
                    CAN_SendData[i] = mGinkgoDriver.ControlCAN.new VCI_CAN_OBJ();
                    CAN_SendData[i].DataLen = 8;
                    CAN_SendData[i].Data = new byte[8];
                    for (int j = 0; j < CAN_SendData[i].DataLen; j++) {
                        CAN_SendData[i].Data[j] = (byte) (i + j);
                }
                CAN_SendData[i].ExternFlag = 0;
                CAN_SendData[i].RemoteFlag = 0;
                CAN_SendData[i].ID = 0x123 + i;
                CAN_SendData[i].SendType = 2;
                CAN_SendData[i].SendType = 0;
                }
                ret = mGinkgoDriver.ControlCAN.VCI_Transmit((byte)0, CAN_SendData, CAN_SendData.length);
                if(ret != mGinkgoDriver.ErrorType.ERR_SUCCESS){
                    printView.append("Send CAN data failed!\n");
                    printView.append(String.format("Error code: %d\n",ret));
                    return;
                }else{
                    printView.append("Send CAN data success!\n");
                }

                try {
                    Thread.sleep(100);
                }catch (InterruptedException e) {
                    return;
                }

                // CAN read data
                for (int i = 0; i < CAN_SendData.length; i++)
                {
                    CAN_SendData[i] = mGinkgoDriver.ControlCAN.new VCI_CAN_OBJ();
                    CAN_SendData[i].Data = new byte[8];
                }

                int ReadDataNum;
                int DataNum = mGinkgoDriver.ControlCAN.VCI_GetReceiveNum((byte)0);
                if(DataNum > 0)
                {
                    ReadDataNum = mGinkgoDriver.ControlCAN.
                            VCI_Receive((byte)0, CAN_SendData, CAN_SendData.length);
                    for(int i = 0; i < ReadDataNum; i++)
                    {
                        printView.append("");
                        printView.append("--CAN_ReceiveData.RemoteFlag = "
                                + String.format("%d", CAN_SendData[i].RemoteFlag) + "\n");
                        printView.append("--CAN_ReceiveData.ExternFlag = "
                                + String.format("%d", CAN_SendData[i].ExternFlag) + "\n");
                        printView.append("--CAN_ReceiveData.ID = 0x"
                                + String.format("%x", CAN_SendData[i].ID) + "\n");
                        printView.append("--CAN_ReceiveData.DataLen = "
                                + String.format("%d", CAN_SendData[i].DataLen) + "\n");
                        printView.append("--CAN_ReceiveData.Data:");
                        for(int j = 0; j < CAN_SendData[i].DataLen; j++){
                            printView.append(String.format("%x",CAN_SendData[i].Data[j]) + " ");
                        }
                        printView.append("--CAN_ReceiveData.TimeStamp = "+ String.format("%d", CAN_SendData[i].TimeStamp));
                    }
                }

                //Stop receive can data
                mGinkgoDriver.ControlCAN.VCI_ResetCAN((byte)0);
//                mGinkgoDriver.ControlCAN.VCI_CloseDevice();
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
