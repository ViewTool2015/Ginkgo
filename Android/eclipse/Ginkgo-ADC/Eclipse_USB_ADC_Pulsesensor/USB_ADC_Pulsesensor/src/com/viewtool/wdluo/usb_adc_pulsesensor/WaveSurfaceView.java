package com.viewtool.wdluo.usb_adc_pulsesensor;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * Created by wdluo on 2015/3/12.
 */
public class WaveSurfaceView extends SurfaceView implements SurfaceHolder.Callback{
    public short[] adcData = new short[1024];
    public double[] fftData = new double[1024];
    int screenWidth = this.getWidth();  // screen Width
    int screenHeight = this.getHeight();  // screen Height
    int WaveX=28,WaveY=1024;
    public double heartRate = 0;
    private WaveFormThread plot_thread;
    public WaveSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        // TODO Auto-generated constructor stub
        getHolder().addCallback(this);
        plot_thread = new WaveFormThread(getHolder(),this);
    }
    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        // TODO Auto-generated method stub
        plot_thread.setRunning(true);
        plot_thread.start();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        // TODO Auto-generated method stub
        boolean retry = true;
        plot_thread.setRunning(false);
        while (retry){
            try{
                plot_thread.join();
                retry = false;
            }catch(InterruptedException e){
            }
        }
    }
    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width,
                               int height) {
        // TODO Auto-generated method stub

    }
    @Override
    public void onDraw(Canvas canvas) {
        // TODO Auto-generated method stub
        PlotPoints(canvas);
    }

    /**
     *
     * @param canvas
     */
    public void PlotPoints(Canvas canvas){
        Paint paint = new Paint();
        paint.setColor(Color.GREEN);
        // set Stroke Width
        paint.setStrokeWidth(3);
        // set Text font Size
        paint.setTextSize(200);
        if(canvas != null) {
            canvas.drawColor(Color.BLACK);//Clear screen
            for (int i = 0; i < (adcData.length - 1); i+=1) {
                // draw
                canvas.drawLine(WaveX+i, WaveY-adcData[i], WaveX+i+1, WaveY-adcData[i+1], paint);
            }
            canvas.drawText(String.format("%.0f",heartRate), 450, WaveY+300, paint);
        }

    }

    public void setWaveData(short data,int index){
        if(index >= adcData.length){
            index = adcData.length-1;
        }
        adcData[index] = data;
    }
    /**
     *
     * @return
     */
    public double getHeartRate() {
        synchronized (this){
            return heartRate;
        }
    }

    /**
     *
     * @param HeartRate
     */
    public void setHeartRate(double HeartRate) {
        synchronized (this){
            heartRate = HeartRate;
        }
    }
    public class WaveFormThread extends Thread {
        private SurfaceHolder holder;
        private WaveSurfaceView plot_area;
        private boolean _run = false;

        public WaveFormThread(SurfaceHolder surfaceHolder, WaveSurfaceView view) {
            holder = surfaceHolder;
            plot_area = view;
        }

        public void setRunning(boolean run) {
            _run = run;
        }

        @Override
        public void run() {
            Canvas c;
            while(_run) {
                c = null;
                try {
                    c = holder.lockCanvas(null);
                    synchronized (holder) {
                        plot_area.PlotPoints(c);
                    }
                } finally {
                    if(c!=null) {
                        holder.unlockCanvasAndPost(c);
                    }
                }
            }
        }
    }
}
