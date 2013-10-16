/*
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
//to compile type gcc -Wall -Wextra -lm -lpng -O2 -s -o yuv main.c
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>
#include <zlib.h>
uint32_t img_w=640;
uint32_t img_w_2=1280;
uint32_t img_h=480;
uint32_t img_wo=640;
uint32_t img_ho=480;
#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)
#define C(Y) ( (Y) - 16  )
#define D(U) ( (U) - 128 )
#define E(V) ( (V) - 128 )
#define YUV2R(Y, U, V) CLIP(( 298 * C(Y)              + 409 * E(V) + 128) >> 8)
#define YUV2G(Y, U, V) CLIP(( 298 * C(Y) - 100 * D(U) - 208 * E(V) + 128) >> 8)
#define YUV2B(Y, U, V) CLIP(( 298 * C(Y) + 516 * D(U)              + 128) >> 8)
//char buf[1024];
char * buf;
const uint8_t sqrt_tab[]={0,16,23,28,32,36,39,42,45,48,50,53,55,58,60,62,64,66,68,70,71,73,75,77,78,80,81,83,84,86,87,89,90,92,93,94,96,97,98,100,101,102,103,105,106,107,108,109,111,112,113,114,115,116,117,118,119,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,135,136,137,138,139,140,141,142,143,144,145,145,146,147,148,149,150,151,151,152,153,154,155,156,156,157,158,159,160,160,161,162,163,164,164,165,166,167,167,168,169,170,170,171,172,173,173,174,175,176,176,177,178,179,179,180,181,181,182,183,183,184,185,186,186,187,188,188,189,190,190,191,192,192,193,194,194,195,196,196,197,198,198,199,199,200,201,201,202,203,203,204,204,205,206,206,207,208,208,209,209,210,211,211,212,212,213,214,214,215,215,216,217,217,218,218,219,220,220,221,221,222,222,223,224,224,225,225,226,226,227,228,228,229,229,230,230,231,231,232,233,233,234,234,235,235,236,236,237,237,238,238,239,240,240,241,241,242,242,243,243,244,244,245,245,246,246,247,247,248,248,249,249,250,250,251,251,252,252,253,253,254,254,255};
const uint8_t sine_tab[]={0, 2, 3, 5, 6, 8, 9,11,12,14,15,17,18,20,21,23,24,26,27,29,30,32,33,35,36,38,39,41,42,44,45,47,48,50,51,53,54,56,57, 59, 60, 61, 63, 64, 66, 67, 69, 70, 72, 73, 75, 76, 77, 79, 80, 82, 83, 85, 86, 88, 89, 90, 92, 93, 95, 96, 97, 99,100,102,103,104,106,107,108,110,111,113,114,115,117,118,119,121,122,123,125,126,127,129,130,131,132,134,135,136,138,139,140,141,143,144,145,146,148,149,150,151,153,154,155,156,157,159,160,161,162,163,164,166,167,168,169,170,171,172,173,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,207,208,209,210,211,212,213,213,214,215,216,217,217,218,219,220,221,221,222,223,224,224,225,226,226,227,228,228,229,230,230,231,232,232,233,234,234,235,235,236,236,237,238,238,239,239,240,240,241,241,242,242,243,243,244,244,244,245,245,246,246,247,247,247,248,248,248,249,249,249,250,250,250,251,251,251,251,252,252,252,252,253,253,253,253,253,253,254,254,254,254,254,254,254,254,255};
void showHelp()
{
	puts("Yuv422/raw image data to png");
	puts("-f specify filename");
	puts("-n x replace x with the image number you want to convert");
	puts("-h or --help shows this help file");
	puts("-o x replace x with a real positive integer that is less than the filesize of the image in which you are opening\nThis skips x number of bytes");
	puts("-a x replace x with the amount of frames that you wish to averge don't use this if you don't want to average frames");
	puts("-c picks which algorthim you would like to use you can specify either 'y' or 'd' or 'dq' or 'dn' or 'dl' but without the quoes");
	puts("y means yuv422 conversion\nd means to debayer by default debayering conversion is used\ndq means to take debayered data and output quater resolution but it does not to any interopulation instead it takes the 4 one color pixels and makes one\ndn means use neighest neighboor debayer instead of bilinear\nr means rgb565");
	puts("dl is a higher quality algorithm based on https://research.microsoft.com/en-us/um/people/lhe/papers/icassp04.demosaicing.pdf");
	puts("-w specifies width (defaults to 640)");
	puts("-H specifies height (defaults to 480)");
	puts("-sq squrate root curves the image (makes it brighter without clipping highlights using this forumla sqrt(255.0)*sqrt(x)");
	puts("-s sine curves the image using this formula (int)floor((sin((double)x/(255.0/PI*2.075))*255.0)+0.5)");
	puts("-sq and -s can be combined");
}
int savePNG(char * fileName,uint32_t width,uint32_t height,void * ptr){
	//saves a 24bit png with rgb byte order
	png_byte * dat=ptr;//convert to uint8_t
	FILE * fp=fopen(fileName,"wb");
	if (fp==0)
		return 1;
	png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, (png_voidp)0,0,0);
	if (!png_ptr)
		return 1;
	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr){
		png_destroy_write_struct(&png_ptr,(png_infopp)NULL);
		return 1;
	}
	if (setjmp(png_jmpbuf(png_ptr))){
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return 1;
	}
	png_init_io(png_ptr, fp);
	png_set_IHDR(png_ptr, info_ptr, width, height,8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);//must be called before other png_set_*() functions
	png_set_compression_level(png_ptr,Z_BEST_COMPRESSION);
	uint32_t y;
	png_set_user_limits(png_ptr, width, height);
	png_write_info(png_ptr, info_ptr);
	for (y=0;y<height;y++)
		png_write_row(png_ptr, &dat[(y*width*3)]);
	png_write_end(png_ptr, info_ptr);
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);//done with file
	return 0;//will return 0 on success non-zero in error
}
void yuv2rgb(uint8_t * yuvDat,uint8_t * out){
	uint32_t xy;
	for (xy=0;xy<(img_w/2)*img_h;++xy){
		*out++=YUV2R(yuvDat[0],yuvDat[1],yuvDat[3]);
		*out++=YUV2G(yuvDat[0],yuvDat[1],yuvDat[3]);
		*out++=YUV2B(yuvDat[0],yuvDat[1],yuvDat[3]);
		*out++=YUV2R(yuvDat[2],yuvDat[1],yuvDat[3]);
		*out++=YUV2G(yuvDat[2],yuvDat[1],yuvDat[3]);
		*out++=YUV2B(yuvDat[2],yuvDat[1],yuvDat[3]);
		yuvDat+=4;
	}
}
void deBayerHQl(uint8_t *in,uint8_t * out){
	//from https://research.microsoft.com/en-us/um/people/lhe/papers/icassp04.demosaicing.pdf
	uint32_t x,y;
	for (y=0;y<img_h*img_w;y+=2*img_w){
		for (x=0;x<img_w;x+=2){
			/*	B Gb
				Gr R*/
			out[(x*3)+2]=in[x+y];
			out[(x*3)+4]=in[x+y+1];
			out[((x+img_w)*3)+1]=in[x+y+img_w];
			out[((x+img_w)*3)+3]=in[x+y+img_w+1];
		}
		out+=img_w*6;
	}
}
void deBayerBL(uint8_t * in,uint8_t * out){
	uint32_t x,y;
	for (y=0;y<img_h*img_w;y+=2*img_w){
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
		//in+=img_w_2;
	}
	
}
void deBayerV(uint8_t * in,uint8_t * out)
{//from http://www.eie.polyu.edu.hk/~enyhchan/J-TIP-CDemosaicking_using_VarCD.pdf
	
}
void deBayerSSDD(uint8_t * in,uint8_t * out)
{//from http://www.ipol.im/pub/art/2011/bcms-ssdd/
	uint32_t x,y;
	for (y=0;y<img_h;y+=2){
		for (x=0;x<img_w;x+=2){
			/*B Gb
			  Gr R*/
			out[x*3]=in[x];
		}
	}
	
}
void deBayerQ(uint8_t * in,uint8_t * out)
{//generates quater resolution but pixel has real RGB value at each location
	uint32_t x,y;
	for (y=0;y<img_ho;++y){
		for (x=0;x<img_w;x+=2){
			out[(x/2*3)]=in[x+1+img_w];//red
			out[(x/2*3)+1]=(in[x+1]+in[x+img_w])/2;//green
			out[(x/2*3)+2]=in[x];//blue
		}
		out+=img_wo*3;
		in+=img_w*2;
	}
}
void deBayerN(uint8_t * in,uint8_t * out){
	uint32_t x,y;
	for (y=0;y<img_h;y+=2){
		for (x=0;x<img_w;x+=2){
			/* Correct table: (It is slightly different than the most common R G G B tables)
			 B Gb
			 Gr R
			*/ 
			out[(x*3)]=in[x+1+img_w];//red 
			out[(x*3)+1]=in[x+1];//green
			out[(x*3)+2]=in[x];//blue
			
			out[(x*3)+3]=in[x+1+img_w];//red 
			out[(x*3)+4]=in[x+1];//green		
			out[(x*3)+5]=in[x];//blue

			out[((x+img_w)*3)]=in[x+1+img_w];//red 
			out[((x+img_w)*3)+1]=in[x+img_w];//green
			out[((x+img_w)*3)+2]=in[x];//blue

			out[((x+img_w)*3)+3]=in[x+1+img_w];//red 
			out[((x+img_w)*3)+4]=in[x+img_w];//green
			out[((x+img_w)*3)+5]=in[x];//blue
		}
		out+=img_w*6;
		in+=img_w*2;
	}
}
uint8_t readImg(uint32_t numf,uint16_t offset,uint8_t * dat,uint8_t alg,char * fileName){
	FILE * myfile;
	if(fileName==0){
		if(alg!=0)
			sprintf(buf,"F%d.RAW",numf);
		else
			sprintf(buf,"F%d.YUV",numf);
		myfile = fopen(buf,"rb");
	}else
		myfile = fopen(fileName,"rb");
	if (myfile==0){
		if(fileName==0)
			printf("Cannot open file %s\n",buf);
		else
			printf("Cannot open file %s\n",fileName);
		return 1;
	}
	if (offset!=0)
		fseek(myfile,offset,SEEK_SET);
	int error=0;
	if (alg!=0&&alg!=4)
		error=fread(dat,1,(img_w*img_h)-offset,myfile);
	else
		error=fread(dat,1,(img_w_2*img_h)-offset,myfile);
	fclose(myfile);
	if(error==0){
		puts("Error read 0 bytes");
		exit(1);
	}
	return 0;
}
void rgb565torgb888(uint8_t * in,uint8_t * out){
	uint32_t xy;
	for (xy=0;xy<img_w*img_h;++xy){
		// R R R R R G G G   G G G B B B B B
		*out++=*in&248;
		*out++=((*in&7)<<5)|((in[1]&224)>>2);
		++in;
		*out++=*in<<3;
		++in;
	}
}
uint8_t processImg(uint8_t * in,uint8_t * out,uint32_t numf,uint8_t alg,uint16_t offset,uint8_t sqrtUse,uint8_t sineUse,char * fileName){
	if (readImg(numf,offset,in,alg,fileName))
		return 1;
	switch (alg){
	case 5:
		deBayerHQl(in,out);
	break;
	case 4:
		rgb565torgb888(in,out);
	break;
	case 3:
		deBayerBL(in,out);
	break;
	case 2:
		deBayerQ(in,out);//causes low resolution but it's like have a 3cmos sensor or foveon sensor
	break;
	case 1:
		deBayerN(in,out);//nearest neighboor low quality but fast
	break;
	case 0:
		yuv2rgb(in,out);
	break;
	default:
		puts("You must pick a valid algorithm to save the image as");
		return 1;
	}
	if(sineUse){
		uint32_t l=img_wo*img_ho*3;
		while(l--){
			*out=sine_tab[*out];
			++out;
		}
		out-=img_wo*img_ho*3;
	}
	if(sqrtUse){
		uint32_t l=img_wo*img_ho*3;
		while(l--){
			*out=sqrt_tab[*out];
			++out;
		}
	}
	return 0;
}
void avgF(uint16_t numf,uint8_t * inout){
	uint16_t * temp=malloc(img_w*img_h*3*numf*sizeof(uint16_t));
	uint64_t xy;
	uint64_t nl;
	for (xy=0;xy<img_w*img_h*3;xy++)
		temp[xy]=inout[xy];
	for (nl=img_w*img_h*3;nl<numf*img_w*img_h*3;nl+=img_w*img_h*3){
		//printf("Adding %d\n",nl/img_w/img_h/3);
		for (xy=0;xy<=img_w*img_h*3;xy++)
			temp[xy]+=inout[xy+nl];
	}
	for (xy=0;xy<img_w*img_h*3;xy++)
		temp[xy]/=numf;
	for (xy=0;xy<img_w*img_h*3;xy++)
		inout[xy]=temp[xy];
	free(temp);
}
int main(int argc,char ** argv){
	uint8_t useNum=0;
	uint32_t useImg=0;
	uint16_t offset=0;
	uint8_t debayer=3;
	uint16_t numImg=1;
	uint8_t sqrtUse=0;
	uint8_t sineUse=0;
	char * fileName=0;
	buf=malloc(128);
	if (argc>1){
		//handle arguments
		int arg;
		for (arg=0;arg<argc;arg++){
			if (strcmp(argv[arg],"-c") == 0){
				arg++;
				if(strcmp(argv[arg],"y")==0)
					debayer=0;
				else if(strcmp(argv[arg],"d")==0)
					debayer=3;
				else if(strcmp(argv[arg],"dq")==0)
					debayer=2;
				else if(strcmp(argv[arg],"dn")==0)
					debayer=1;
				else if(strcmp(argv[arg],"r")==0)
					debayer=4;
				else if(strcmp(argv[arg],"dl")==0)
					debayer=5;
				else{
					puts("You did not specify a valid algorithm See usage (below)");
					showHelp();
					return 1;
				}
				continue;
			}
			if (strcmp(argv[arg],"-w") == 0){
				arg++;
				img_wo=img_w=atoi(argv[arg]);
				img_w_2=img_w+img_w;
				continue;
			}
			if (strcmp(argv[arg],"-s") == 0){
				sineUse=1;
				continue;
			}
			if (strcmp(argv[arg],"-sq") == 0){
				sqrtUse=1;
				continue;
			}
			if (strcmp(argv[arg],"-H") == 0){
				arg++;
				img_ho=img_h=atoi(argv[arg]);
				continue;
			}
			if (strcmp(argv[arg],"-n") == 0){
				arg++;
				useImg=atoi(argv[arg]);
				if (useNum != 2)
					useNum=1;
				continue;
			}
			if (strcmp(argv[arg],"-f") == 0){
				++arg;
				useNum=3;
				fileName=argv[arg];
				buf=realloc(buf,strlen(fileName)+64);
				continue;
			}
			if (strcmp(argv[arg],"-o") == 0){
				arg++;
				offset=atoi(argv[arg]);
				continue;
			}
			if (strcmp(argv[arg],"-a") == 0){
				arg++;
				useNum=2;
				numImg=atoi(argv[arg]);
				if (numImg <1){
					printf("For argument -a you must specify a number greater than 0 you entered %d\n",numImg);
					return 1;
				}
				continue;
			}
			if ((strcmp(argv[arg],"-h")==0) || (strcmp(argv[arg],"--help")==0)){
				showHelp();
				return 0;
			}
		}
	}
	uint8_t * Dat;//in case some of the file was not saved we use calloc instead of malloc to garentte that the unsaved pixels are set to 0
	if (debayer!=0&&debayer!=4)
		Dat = calloc(img_w*img_h,1);
	else
		Dat = calloc(img_w*img_h,2);
	if(debayer==2){
		img_wo/=2;
		img_ho/=2;
	}
	uint8_t * outImg = malloc(img_wo*img_ho*numImg*3);//all bytes in the array will be overwritten no need for calloc
	if (useNum==1){
		processImg(Dat,outImg,useImg,debayer,offset,sqrtUse,sineUse,0);
		sprintf(buf,"frame %d.png",useImg);
		if (savePNG(buf,img_wo,img_ho,outImg)){
			puts("Error while saving PNG");
			return 1;
		}
	}
	else if (useNum==3){
		processImg(Dat,outImg,useImg,debayer,offset,sqrtUse,sineUse,fileName);
		sprintf(buf,"%s.png",fileName);
		if (savePNG(buf,img_wo,img_ho,outImg)){
			puts("Error while saving PNG");
			return 1;
		}
	}
	else if (useNum==2){
		uint32_t nl;
		for (nl=0;nl<numImg;nl++){
			printf("Reading %d\n",nl);
			processImg(Dat,outImg+(nl*img_wo*img_ho*3),useImg+nl,debayer,offset,sqrtUse,sineUse,0);
		}
		avgF(numImg,outImg);
		sprintf(buf,"frame %d-%d.png",useImg,useImg+numImg-1);
		if (savePNG(buf,img_wo,img_ho,outImg)){
			puts("Error while saving PNG");
			return 1;
		}
	}else{
		uint32_t imgC=0;
		for (;;imgC++){
			printf("Saving image %d\n",imgC);
			if (processImg(Dat,outImg,imgC,debayer,offset,sqrtUse,sineUse,0))
				goto quit;
			sprintf(buf,"frame %d.png",imgC);
			if (savePNG(buf,img_wo,img_ho,outImg)){
				puts("Error while saving PNG");
				return 1;
			}
		}
	}
quit:
	free(Dat);
	free(outImg);
	return 0;
}
