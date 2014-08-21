#include <stdint.h>
#include <math.h>
#include "pixels.h"
void Adaptive_Color_Plane_Interpolation(uint8_t * in,uint8_t * out,uint32_t img_w,uint32_t img_h){// http://www.ece.ncsu.edu/imaging/Publications/2002/demosaicking-JEI-02.pdf
	uint32_t x,y;
	for (y=0;y<img_h/2;++y){
		for (x=0;x<img_w/2;++x){
			//Start by estimating green at red
			int a=abs(-getBP(x-1,y,RED)+(2*getBP(x,y,RED))-getBP(x+1,y,RED))+abs(getBP(x,y,GREEN_RED)-getBP(x+1,y,GREEN_RED));
			int B=abs(-getBP(x-1,y-1,RED)+(2*getBP(x,y,RED))-getBP(x+1,y+1,RED))+abs(getBP(x,y-1,GREEN_BLUE)-getBP(x,y,GREEN_BLUE));
			setPX(x*2,y*2,BLUE_RGB,getBP(x,y,BLUE));
			setPX(x*2+1,y*2,GREEN_RGB,getBP(x,y,GREEN_BLUE));
			setPX(x*2,y*2+1,GREEN_RGB,getBP(x,y,GREEN_RED));
			setPX(x*2+1,y*2+1,RED_RGB,getBP(x,y,RED));
		}
	}
}
