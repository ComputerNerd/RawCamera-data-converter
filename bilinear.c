#include <stdint.h>
#include "pixels.h"
void deBayerBL(uint8_t * in,uint8_t * out,uint32_t img_w,uint32_t img_h){
	int32_t x,y;
	for (y=0;y<img_h/2;++y){
		for (x=0;x<img_w/2;++x){
				/* The following pattern is assumed for this code:
				 * B Gb
				 * Gr R*/

				setPX(x*2,y*2,RED_RGB,(getBP(x,y,RED)+getBP(x-1,y,RED)+getBP(x,y-1,RED)+getBP(x-1,y-1,RED))/4);
				setPX(x*2,y*2,GREEN_RGB,(getBP(x,y,GREEN_BLUE)+getBP(x-1,y,GREEN_BLUE)+getBP(x,y,GREEN_RED)+getBP(x,y-1,GREEN_RED))/4);
				setPX(x*2,y*2,BLUE_RGB,getBP(x,y,BLUE));
				
				setPX(x*2+1,y*2,RED_RGB,(getBP(x,y,RED)+getBP(x,y-1,RED))/2);
				setPX(x*2+1,y*2,GREEN_RGB,getBP(x,y,GREEN_BLUE));
				setPX(x*2+1,y*2,BLUE_RGB,(getBP(x,y,BLUE)+getBP(x+1,y,BLUE))/2);

				setPX(x*2,y*2+1,RED_RGB,(getBP(x,y,RED)+getBP(x-1,y,RED))/2);
				setPX(x*2,y*2+1,GREEN_RGB,getBP(x,y,GREEN_RED));
				setPX(x*2,y*2+1,BLUE_RGB,(getBP(x,y,BLUE)+getBP(x,y+1,BLUE))/2);

				setPX(x*2+1,y*2+1,RED_RGB,getBP(x,y,RED));
				setPX(x*2+1,y*2+1,GREEN_RGB,(getBP(x,y,GREEN_BLUE)+getBP(x,y+1,GREEN_BLUE)+getBP(x,y,GREEN_RED)+getBP(x+1,y,GREEN_RED))/4);
				setPX(x*2+1,y*2+1,BLUE_RGB,(getBP(x,y,BLUE)+getBP(x+1,y,BLUE)+getBP(x,y+1,BLUE)+getBP(x+1,y+1,BLUE))/4);
		}
	}
}
