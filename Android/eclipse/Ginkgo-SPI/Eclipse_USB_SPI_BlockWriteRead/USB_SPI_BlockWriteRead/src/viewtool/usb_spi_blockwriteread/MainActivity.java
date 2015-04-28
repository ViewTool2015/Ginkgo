package viewtool.usb_spi_blockwriteread;

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
import com.viewtool.Ginkgo.ControlSPI;


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
        printView =(EditText) findViewById(R.id.printTextView);
        config_usb();
        Button button =(Button)findViewById(R.id.start);
        button.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View arg0) {
                printView.clearComposingText();
                printView.setText("");
                int ret;

                //Scan device
                mUsbDevice = mGinkgoDriver.ControlSPI.VSI_ScanDevice();
                if (mUsbDevice == null) {
                    printView.append("No device connected!\n");
                    return;
                }
                //Open device
                ret = mGinkgoDriver.ControlSPI.VSI_OpenDevice();
                if (ret != ErrorType.ERR_SUCCESS) {
                    printView.append("Open device error!\n");
                    return;
                } else {
                    printView.append("Open device success!\n");
                }
                // Get product information
                ControlSPI.VSI_BOARD_INFO BoardInfo = mGinkgoDriver.ControlSPI.new VSI_BOARD_INFO();
                ret = mGinkgoDriver.ControlSPI.VSI_ReadBoardInfo(BoardInfo);
                if (ret != ErrorType.ERR_SUCCESS) {
                    printView.append("Read board information error!\n");
                    return;
                } else {
                    printView.append("Product Name:" + new String(BoardInfo.ProductName) + "\n");
                    printView.append("Firmware Version:" + String.format("V%d.%d.%d\n", BoardInfo.FirmwareVersion[1], BoardInfo.FirmwareVersion[2], BoardInfo.FirmwareVersion[3]));
                    printView.append("Hardware Version:" + String.format("V%d.%d.%d\n", BoardInfo.HardwareVersion[1], BoardInfo.HardwareVersion[2], BoardInfo.HardwareVersion[3]));
                    printView.append("Serial Number:\n");
                    for (int i = 0; i < 12; i++) {
                        printView.append(String.format("%02X", BoardInfo.SerialNumber[i]));
                    }
                    printView.append("\n");
                }
                try {
                    Thread.currentThread();
					Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
                //Config device
                ControlSPI.VSI_INIT_CONFIG SPI_Config = mGinkgoDriver.ControlSPI.new VSI_INIT_CONFIG();
                SPI_Config.ControlMode = 1;
                SPI_Config.MasterMode = 1;
                SPI_Config.ClockSpeed = 1000000;
                SPI_Config.CPHA = 0;
                SPI_Config.CPOL = 0;
                SPI_Config.LSBFirst = 0;
                SPI_Config.TranBits = 8;
                ret = mGinkgoDriver.ControlSPI.VSI_InitSPI(SPI_Config);
                if (ret != ErrorType.ERR_SUCCESS) {
                    printView.append("Config device error!\n");
                    return;
                } else {
                    printView.append("Config device success!\n");
                }

                byte[] write_buffer = new byte[10240];
                byte[] read_buffer = new byte[10240];
                byte spiindex = 0;
                short WriteLen = 0;
                short ReadLen = 0;
                byte BlockNum = 5;
                byte BlockSize = 2;
                int IntervalTime = 100;
                // Block mode write data, data will be send out BlockNum time(s), send out BlockSize bytes of data every time
                // Set CS to low before send data and set CS to high after send data complete when every time send data
                // Each time send data, interval is IntervalTime us, that is CS set to high and hold IntervalTime us
                for (byte i = 0; i < BlockNum; i++ )
                {
                    for (Byte j = 0; j < BlockSize; j++ )
                    {
                        write_buffer[i * BlockSize + j] = (byte)(i * j);
                    }
                }
                ret = mGinkgoDriver.ControlSPI.VSI_BlockWriteBytes(spiindex, write_buffer, BlockSize, BlockNum, IntervalTime);
                if (ret != ErrorType.ERR_SUCCESS) {
                    printView.append("Block write data error!\n");
                    return;
                } else {
                    printView.append("Block write data success!\n");
                }

                // Block mode read data: same as write data
                ret = mGinkgoDriver.ControlSPI.VSI_BlockReadBytes(spiindex, write_buffer, BlockSize, BlockNum, IntervalTime);
                if (ret != ErrorType.ERR_SUCCESS) {
                    printView.append("Block write data error!\n");
                    return;
                } else {
                    printView.append("Block write data success!\n");
                    printView.append("Read data(Hex):\n");
                    for (int i = 0; i < BlockNum; i++)
                    {
                        for (int j = 0; j < BlockSize; j++)
                        {
                            printView.append(String.format("%x ", read_buffer[i * BlockSize + j]));
                        }
                    }
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

    private void config_usb() {
        mUsbManager = (UsbManager) getSystemService(MainActivity.USB_SERVICE);
        pendingIntent = PendingIntent.getBroadcast(this, 0, new Intent(
                ACTION_USB_PERMISSION), 0);
        if((mUsbManager != null)&&(pendingIntent != null)){
            mGinkgoDriver = new GinkgoDriver(mUsbManager, pendingIntent);
        }
    }
}
