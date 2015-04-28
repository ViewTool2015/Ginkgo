
#include "cie.h"
#include <math.h>
#include <QtGui>


CCIE::CCIE(void)
{
}

CCIE::~CCIE(void)
{
}
void CCIE::RGB2XYZ(float r, float g, float b, float &x, float &y, float &z)
{
	r/=255.0f;g/=255.0f;b/=255.0f;
	
	if(r<=0.04045)	r=r/12.92f;
	else r=float(pow(((r+0.055)/1.055),2.4));

	if(g<=0.04045)  g=g/12.92f;
	else g=float(pow(((g+0.055)/1.055),2.4));
	
	if(b<=0.04045)  b=b/12.92f;
	else b=float(pow(((b+0.055)/1.055),2.4));
	
	x =  41.24f * r + 35.76f * g + 18.05f * b;
	y =  21.26f * r + 71.52f * g + 7.2f * b;
	z =  1.93f  * r + 11.92f * g + 95.05f * b;
}

void CCIE::XYZ2Luv(float x, float y, float z, float &L, float &u, float &v)
{
	//定义X0,Y0,X0为D65 CIE 标准光源的色度坐标
	float X0=95.05f,Y0=100.00f,Z0=108.90f;
	float fx=x/X0,fy=y/Y0,fz=z/Z0,frx,fry,frz;
	float k =0.333;

	if(fx>0.008856) frx=float(pow(fx,k));
	else frx=7.787f*fx+16.0f/116.0f;

	if(fy>0.008856) fry=float(pow(fy,k));
	else fry=7.787f*fy+16.0f/116.0f;

	if(fz>0.008856) frz=float(pow(fz,k));
	else frz=7.787f*fz+16.0f/116.0f;

	if(fx<=0.008856 && fy<=0.008856 && fz<=0.008856)
	{
		L=903.3f*fy;
		u=3893.5f*(fx-fy);
		v=1557.4f*(fy-fz);
	}
	else
	{
		L=116*fry-16;
		u=500*(frx-fry);
		v=200*(fry-frz);
	}
//	if(fy>0.008856) L=116*fry-16;
//	else L=903.3f*fy;
}

void CCIE::XYZ2RGB(float x, float y, float z, int &r, int &g, int &b)
{
	double dr,dg,db;
	dr =  0.032406  * x - 0.015371 * y - 0.0049895 * z;
	dg = -0.0096891 * x + 0.018757 * y + 0.00041914* z;
	db =  0.00055708* x - 0.0020401* y + 0.01057   * z;

	if(dr<=0.00313) dr=dr*12.92;
	else dr=exp(log(dr)/2.4)*1.055-0.055;

	if(dg<=0.00313) dg=dg*12.92;
	else dg=exp(log(dg)/2.4)*1.055-0.055;

	if(db<=0.00313) db=db*12.92;
	else db=exp(log(db)/2.4)*1.055-0.055;

	dr=dr*255;
	dg=dg*255;
	db=db*255;
	
    dr= min(255.0f,max(0.0f,dr));
    dg= min(255.0f,max(0.0f,dg));
    db= min(255.0f,max(0.0f,db));

    r=int(dr+0.5);
    g=int(dg+0.5);
    b=int(db+0.5);
}

double CCIE::min(double a,double b)
{
    return (a>b)?b:a;
}

double CCIE::max(double a,double b)
{
    return (a>b)?a:b;
}

void CCIE::RGB2Luv(float r, float g, float b, float &L, float &u, float &v)
{
	float x,y,z;

	RGB2XYZ(r,g,b,x,y,z);
	XYZ2Luv(x,y,z,L,u,v);
}

void CCIE::Lab2LHC(float L, float a, float b, float &c, float &h)
{
	c=float(sqrt(a*a+b*b));
	h=float(180/3.1415927*atan2(b,a));
	if(h<0) h+=360;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//Function: 球体模型的转换公式
//Parameter:
//Notes:
//Date:
////////////////////////////////////////////////////////////////////////////////////////////////
void CCIE::RGB2HSV_Sphere(int r, int g, int b, float &h, float &s, float &v)
{
	float fr=float(r)/255,fg=float(g)/255,fb=float(b)/255;

	float max=fr>fg?fr:fg;max=max>fb?max:fb;
	float min=fr<fg?fr:fg;min=min<fb?min:fb;
	
	fr=(max-fr)/(max-min);
	fg=(max-fg)/(max-min);
	fb=(max-fb)/(max-min);
	
	//v
	v=(max+min)/2;
	//s
	if(max==min) s=0.0f;
	else if(v<0.5) s=float(max-min)/(max+min);
	else s=float(max-min)/(2-max-min);
	//h
	if(max==min) h=0.0f;
	else if(r==max) h=60*(2+fb-fg);
	else if(g==max) h=60*(4+fr-fb);
	else h=60*(6+fg-fr);

	while(h>360 ) h=h-360;
	while(h<0) h=h+360;
}

void CCIE::HSV2RGB_Sphere(float h, float s, float v, int &ir, int &ig, int &ib)
{
	if(h<0 || h>360 || s<0 || s>1 || v<0 || v>1) return;

	float max,min,r,g,b;
	if(v<=0.5) max=v*(1+s);
	else max=s+v-s*v;
	min=2*v-max;

	//r
	if(h>=0 && h<60) r=min+(max-min)*(h/60);
	else if(h>=60 && h<180) r=max;
	else if(h>=180 && h<240) r=min+(max-min)*((240-h)/60);
	else r=min;
	//g
	if(h>=0 && h<120) g=min;
	else if(h>=120 && h<180) g=min+(max-min)*((h-120)/60);
	else if(h>=180 && h<300) g=max;
	else g=min+(max-min)*((360-h)/60);
	//b
	if(h>=0 && h<60) b=max;
	else if(h>=60 && h<120) b=min+(max-min)*((120-h)/60);
	else if(h>=120 && h<240) b=min;
	else if(h>=240 && h<300) b=min+(max-min)*((h-240)/60);
	else b=max;

	ir=int(r*255+0.5);ig=int(g*255+0.5);ib=int(b*255+0.5);
	if(ir<0) ir=0;if(ir>255) ir=255;
	if(ig<0) ig=0;if(ig>255) ig=255;
	if(ib<0) ib=0;if(ib>255) ib=255;
}

void CCIE::Luv2RGB(float L, float u, float v, int &r, int &g, int &b)
{
	float x,y,z;
	Luv2XYZ(L,u,v,x,y,z);
	XYZ2RGB(x,y,z,r,g,b);
}

void CCIE::Luv2XYZ(float L, float u, float v, float &x, float &y, float &z)
{
	float frx,fry,frz;
	float X0=95.05f,Y0=100.00f,Z0=108.90f;

	fry=(L+16)/116;
	frx=u/500+fry;
	frz=fry-v/200;

	if(frx<=0.2069 && fry<=0.2069 && frz<=0.2069)
	{
		y=L/903.3f*Y0;
		x=(u/3893.5f+L/903.3f)*X0;
		z=(L/903.3f-v/1557.4f)*Z0;
	}
	else
	{
		if(frx<=0.2069)	x=(frx-16.0f/116.0f)/7.787f*X0;
		else x=float(pow(frx,3))*X0;

		if(fry<=0.2069)	y=(fry-16.0f/116.0f)/7.787f*X0;
		else y=float(pow(fry,3))*X0;
		
		if(frz<=0.2069)	z=(frz-16.0f/116.0f)/7.787f*X0;
		else z=float(pow(frz,3))*X0;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////
//Function: 圆柱体模型的转换公式
//Parameter:
//Notes:
//Date:
////////////////////////////////////////////////////////////////////////////////////////////////
void CCIE::RGB2HSV_Cylinder(int r, int g, int b, float &h, float &s, float &v)
{
	int max,min;
	float triangle;
	
	min=(r<g)?r:g;
	min=(min<b)?min:b;
	max=(r>g)?r:g;
	max=(max>b)?max:b;
	
	v=(r+g+b)/float(sqrt(3.0));
	
	if(max==min)
	{
		h=0.0f;
		s=0.0f;
	}
	else
	{
		s=1-(3.0f*min/(r+g+b));
		triangle=float(acos((double(0.5*((r-g)+(r-b))))/(sqrt(double((r-g)*(r-g)+(r-b)*(g-b))))));
		if(g>=b)
			h=triangle;
		else
			h=float(2*3.14159265-triangle);
	}
	
	h=float(h*180.0/3.14159265);
}

void CCIE::HSV2RGB_Cylinder(float h, float s, float v, int &r, int &g, int &b)
{
	float v1,v2,v3,fr,fg,fb;
	
	v1=float(cos(double(h*3.14159265/180))/cos(double((60-h)*3.14159265/180)));
	v2=float(cos(double((h-120)*3.14159265/180))/cos(double((180-h)*3.14159265/180)));
	v3=float(cos(double((h-240)*3.14159265/180))/cos(double((300-h)*3.14159265/180)));
	
	if(h>=0 && h<120)
	{
		fr=float(v*(1+s*v1)/sqrt(3.0));
		fb=float(v*(1-s)/sqrt(3.0));
		fg=float(v*sqrt(3.0)-fr-fb);
		if(fg<=0)
			fg=0;
	}
	else if(h>=120 && h<240)
	{
		fg=float(v*(1+s*v2)/sqrt(3.0));
		fr=float(v*(1-s)/sqrt(3.0));
		fb=float(v*sqrt(3.0)-fr-fg);
		if(fb<=0)
			fb=0;
	}
	else if(h>=240 && h<360)
	{
		fb=float(v*(1+s*v3)/sqrt(3.0));
		fg=float(v*(1-s)/sqrt(3.0));
		fr=float(v*sqrt(3.0)-fb-fg);
		if(fr<=0)
			fr=0;
	}
	
	if(fr>=255)
		fr=255;
	if(fg>=255)
		fg=255;
	if(fb>=255)
		fb=255;

	r=int(fr+0.5);
	g=int(fg+0.5);
	b=int(fb+0.5);
}
