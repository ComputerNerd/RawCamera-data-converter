#include "pixels.h"
int getBayerPixel(uint8_t*in,int x,int y,uint32_t w,uint32_t h){
	if(x<0)
		x=0;
	if(y<0)
		y=0;
	if(x>=w)
		x=w-1;
	if(y>=h)
		y=h-1;
	return in[x+(y*w)];
}
int getBayerPixelCol(uint8_t*in,int x,int y,uint32_t w,uint32_t h,int color){
	//x and y specify which square to pick.
	x*=2;
	y*=2;
	if(x<0)
		x=0;
	if(y<0)
		y=0;
	if(x>=w)
		x=w-1;
	if(y>=h)
		y=h-1;
	switch(color){
		case RED:
			return in[(y*w)+w+x+1];
		break;
		case GREEN_RED:
			return in[(y*w)+w+x];
		break;
		case GREEN_BLUE:
			return in[(y*w)+x+1];
		break;
		case BLUE:
			return in[(y*w)+x];
		break;
	}
}
void setPixelBounds(uint8_t*out,int x,int y,uint32_t w,uint32_t h,int channel,uint8_t val){
	if(x<0)
		return;
	if(y<0)
		return;
	if(x>=w)
		return;
	if(y>=h)
		return;
	out[(y*w*3)+(x*3)+channel]=val;
}
