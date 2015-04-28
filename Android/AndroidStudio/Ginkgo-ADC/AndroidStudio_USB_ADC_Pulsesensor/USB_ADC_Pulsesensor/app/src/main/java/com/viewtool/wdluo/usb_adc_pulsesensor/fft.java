package com.viewtool.wdluo.usb_adc_pulsesensor;

/**
 * Created by wdluo on 2015/3/13.
 */
public class fft {
    int pointNum;
    double[] sin_tab;
    fft(int PointNum){
        pointNum = PointNum;
        sin_tab = new double[pointNum>>2];
        for(int i=0;i<(pointNum>>2);i++){
            sin_tab[i] = Math.sin(2*Math.PI*i/pointNum);
        }
    }

    /*******************************************************************
     * Function: struct compx EE(struct compx b1,struct compx b2)
     * Description: The complex multiplication
     * Input: complex b1, complex b2
     * Output: The product of b1 and b2
     *******************************************************************/
    compx EE(compx a,compx b)
    {
        compx c = new compx();
        c.real=a.real*b.real-a.imag*b.imag;
        c.imag=a.real*b.imag+a.imag*b.real;
        return(c);
    }
    /*******************************************************************
     * Function: void sin_tab(float pi)
     * Description: calculate sin via look-up table
     * Input: 0~2*PI,
     * Output: sine of pi
     *******************************************************************/
    double sin_tab(double pi)
    {
        int n;
        double a=0;
        n=(int)(pi*pointNum/2/Math.PI);

        if(n>=0&&n<=pointNum/4)
            a=sin_tab[n];
        else if(n>pointNum/4&&n<pointNum/2)
        {
            n-=pointNum/4;
            a=sin_tab[pointNum/4-n];
        }
        else if(n>=pointNum/2&&n<3*pointNum/4)
        {
            n-=pointNum/2;
            a=-sin_tab[n];
        }
        else if(n>=3*pointNum/4&&n<3*pointNum)
        {
            n=pointNum-n;
            a=-sin_tab[n];
        }
        return a;
    }

    /*******************************************************************
     * Function: void cos_tab(float pi)
     * Description: calculate cos via look-up table
     * Input: 0~2*PI,
     * Output: cos of pi
     *******************************************************************/
    double cos_tab(double pi)
    {
        double a,pi2;
        pi2=pi+Math.PI/2;
        if(pi2>2*Math.PI)
            pi2-=2*Math.PI;
        a=sin_tab(pi2);
        return a;
    }

    /*******************************************************************
     * Function: void FFT(struct compx *xin)
     * Description: FFT
     * Input: point of complex xin
     * Output: none
     *******************************************************************/
    void calculate_fft(compx[] xin)
    {
        int f,m,nv2,nm1,i,k,l,j=0;
        compx u = new compx();
        compx w = new compx();
        compx v;

        nv2=pointNum/2;
        nm1=pointNum-1;
        for(i=0;i<nm1;i++)
        {
            if(i<j)
            {
                v=xin[j];
                xin[j]=xin[i];
                xin[i]=v;
            }
            k=nv2;
            while(k<=j)
            {
                j=j-k;
                k=k/2;
            }
            j=j+k;
        }

        int le,lei,ip;
        f=pointNum;
        for(l=1;(f=f/2)!=1;l++);
        for(m=1;m<=l;m++)
        {
            le=2<<(m-1);
            lei=le/2;
            u.real=1.0;
            u.imag=0.0;
            w.real=Math.cos(Math.PI/lei);
            w.imag=-Math.sin(Math.PI/lei);
            //w.real=cos_tab(Math.PI/lei);
            //w.imag=-sin_tab(Math.PI/lei);
            for(j=0;j<=lei-1;j++)
            {
                for(i=j;i<=pointNum-1;i=i+le)
                {
                    ip=i+lei;
                    v=EE(xin[ip],u);
                    xin[ip].real=xin[i].real-v.real;
                    xin[ip].imag=xin[i].imag-v.imag;
                    xin[i].real=xin[i].real+v.real;
                    xin[i].imag=xin[i].imag+v.imag;
                }
                u=EE(u,w);
            }
        }
    }
    public class compx {
        public double real = 0;
        public double imag = 0;
    };
}
