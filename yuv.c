#include <stdint.h>
#include "defines.h"
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )
#define YUV2R(Y, U, V) CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)

void yuv2rgb(uint8_t * yuvDat,uint8_t * out,int alg,uint32_t img_w,uint32_t img_h){
	uint32_t xy;
	int y1,y2;
	switch(alg){
		case ALG_YUV_0:
			y1=0;
			y2=2;
		break;
		case ALG_YUV_1:
			y2=0;
			y1=2;
		break;
	}
	for (xy=0;xy<(img_w/2)*img_h;++xy){
		*out++=YUV2R(yuvDat[y1],yuvDat[1],yuvDat[3]);
		*out++=YUV2G(yuvDat[y1],yuvDat[1],yuvDat[3]);
		*out++=YUV2B(yuvDat[y1],yuvDat[1],yuvDat[3]);
		*out++=YUV2R(yuvDat[y2],yuvDat[1],yuvDat[3]);
		*out++=YUV2G(yuvDat[y2],yuvDat[1],yuvDat[3]);
		*out++=YUV2B(yuvDat[y2],yuvDat[1],yuvDat[3]);
		yuvDat+=4;
	}
}
