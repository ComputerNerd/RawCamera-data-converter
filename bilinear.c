#include <stdint.h>
void deBayerBL(uint8_t * in,uint8_t * out,uint32_t img_w,uint32_t img_h){
	uint32_t img_w_2=img_w*2;
	uint32_t x,y;
	for (y=0;y<img_h*img_w;y+=img_w_2){
		for (x=0;x<img_w;x+=2){
			/*	B Gb
				Gr R*/
			if(y!=0){
				if(x!=0)
					out[x*3]=(in[y+x+1+img_w]+in[y+x+1-img_w]+in[y+x-1+img_w]+in[y+x-1-img_w])/4;//red
				else
					out[x*3]=(in[y+x+1+img_w]+in[y+x+1-img_w])/2;//red
			}else{
				if(x!=0)
					out[x*3]=(in[y+x+1+img_w]+in[y+x-1+img_w])/2;//red
				else
					out[x*3]=in[y+x+1+img_w];//red
			}
			if(y!=0){
				if(x!=0)
					out[(x*3)+1]=(in[x+y-img_w]+in[x+y-1]+in[x+y+1]+in[x+y+img_w])/4;
				else
					out[(x*3)+1]=(in[x+y-img_w]+in[x+y+img_w]+in[x+y+1])/3;
			}else{
				if(x!=0)
					out[(x*3)+1]=(in[y+x-1]+in[y+x+1]+in[y+x+img_w])/3;
				else
					out[(x*3)+1]=(in[y+x+1]+in[y+x+img_w])/2;//green
			}
			out[(x*3)+2]=in[y+x];//blue
			if(y!=0)
				out[(x*3)+3]=(in[y+x+1+img_w]+in[y+x+1-img_w])/2;//red
			else
				out[(x*3)+3]=in[y+x+1+img_w];
			out[(x*3)+4]=in[y+x+1];//green
			out[(x*3)+5]=(in[x+y]+in[x+y+2])/2;//blue
			
			if(x!=0)
				out[((x+img_w)*3)]=(in[x+y+img_w+1]+in[x+y+img_w-1])/2;//red
			else
				out[((x+img_w)*3)]=in[x+y+img_w+1];//red
			out[((x+img_w)*3)+1]=in[y+x+img_w];//green
			out[((x+img_w)*3)+2]=(in[x+y]+in[x+y+img_w_2])/2;//get blue
			
			out[((x+img_w)*3)+3]=in[y+x+1+img_w];//red
			out[((x+img_w)*3)+4]=(in[x+y+1]+in[x+y+1+img_w_2]+in[x+y+img_w]+in[x+y+img_w+2])/4;//green
			out[((x+img_w)*3)+5]=(in[x+y]+in[x+y+2]+in[x+y+img_w_2]+in[x+y+2+img_w_2])/4;
		}
		out+=img_w*6;
	}
}
