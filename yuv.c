#include <stdint.h>
#include "defines.h"
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )
#define YUV2R(Y, U, V) CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)
static uint8_t clipp(double x){
	if(x<0.0)
		return 0;
	else if(x>255.0)
		return 255;
	else
		return x;
}
/*static uint8_t*YUV2RGB(double y,double u,double v,uint8_t*out){
	*out++=clipp(y+(1.371*(u-128.0)));
	*out++=clipp(y-(0.698*(u-128.0)-(0.336*(v-128.0))));
	*out++=clipp(y+(1.732*(v-128.0)));
	return out;
}*/
static uint8_t*YUV2RGB(double Y,double U,double V,uint8_t*out){
	*out++=clipp(Y + 1.4075 * (V - 128));
	*out++=clipp(Y - 0.3455 * (U - 128) - (0.7169 * (V - 128)));
	*out++=clipp(Y + 1.7790 * (U - 128));
	return out;
}
/*static uint8_t*YUV2RGB(double Y,double U,double V,uint8_t*out){
	*out++=clipp(Y+(1.402*(U-128.0)));
	*out++=clipp(Y-(.34414*(V-128))-(.71414*(U-128.0)));
	*out++=clipp(Y+(1.772*(V-128.0)));
	return out;
}*/
void yuv2rgb(uint8_t * yuvDat,uint8_t * out,int alg,uint32_t img_w,uint32_t img_h){
	uint32_t xy;
	int y1,y2,y3,y4;
	switch(alg){
		case ALG_YUV_0:
			y1=0;
			y2=2;
			y3=1;
			y4=3;
		break;
		case ALG_YUV_1:
			y1=2;
			y2=0;
			y3=1;
			y4=3;
		break;
		case ALG_YUV_2:
			y1=0;
			y2=2;
			y3=3;
			y4=1;
		break;
		case ALG_YUV_3:
			y1=2;
			y2=0;
			y3=3;
			y4=1;
		break;
	}
	for (xy=0;xy<(img_w/2)*img_h;++xy){
		/* *out++=YUV2R(yuvDat[y1],yuvDat[y3],yuvDat[y4]);
		*out++=YUV2G(yuvDat[y1],yuvDat[y3],yuvDat[y4]);
		*out++=YUV2B(yuvDat[y1],yuvDat[y3],yuvDat[y4]);
		*out++=YUV2R(yuvDat[y2],yuvDat[y3],yuvDat[y4]);
		*out++=YUV2G(yuvDat[y2],yuvDat[y3],yuvDat[y4]);
		*out++=YUV2B(yuvDat[y2],yuvDat[y3],yuvDat[y4]);*/
		out=YUV2RGB(yuvDat[y1],yuvDat[y3],yuvDat[y4],out);
		out=YUV2RGB(yuvDat[y2],yuvDat[y3],yuvDat[y4],out);
		yuvDat+=4;
	}
}
