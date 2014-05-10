#include "pixels.h"
int getBayerPixel(uint8_t*in,int x,int y,uint32_t w,uint32_t h,int color){
	//x and y specify which square to pick.
	if(x<0)
		x=0;
	if(y<0)
		y=0;
	if(x>=w/2)
		x=w/2-1;
	if(y>=h/2)
		y=h/2-1;
	switch(color){
		case RED:
			return in[(y*w*2)+w+(x*2)+1];
		break;
		case GREEN_RED:
			return in[(y*w*2)+w+(x*2)];
		break;
		case GREEN_BLUE:
			return in[(y*w*2)+(x*2)+1];
		break;
		case BLUE:
			return in[(y*w*2)+(x*2)];
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
