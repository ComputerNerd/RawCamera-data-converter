#include <stdio.h>
#ifdef USE_MINIZ
#define MINIZ_NO_STDIO
#define MINIZ_NO_ARCHIVE_APIS
#define MINIZ_NO_TIME
#define MINIZ_NO_ZLIB_APIS
#include "miniz.c"
#else
#include <png.h>
#include <zlib.h>
#endif
#include <stdint.h>
int savePNG(char * fileName,uint32_t width,uint32_t height,void * ptr){
	//saves a 24bit png with rgb byte order
	#ifdef USE_MINIZ
		size_t png_data_size = 0;
		void *pPNG_data = tdefl_write_image_to_png_file_in_memory_ex(ptr,width,height,3,&png_data_size,10,MZ_FALSE);
		if (!pPNG_data)
			fputs("tdefl_write_image_to_png_file_in_memory_ex() failed!\n",stderr);
		else{
			FILE *pFile = fopen(fileName, "wb");
			fwrite(pPNG_data, 1, png_data_size, pFile);
			fclose(pFile);
		}
		mz_free(pPNG_data);
	#else
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
		png_set_user_limits(png_ptr, width, height);
		png_write_info(png_ptr, info_ptr);
		uint32_t y;
		for (y=0;y<height;++y)
			png_write_row(png_ptr, &dat[(y*width*3)]);
		png_write_end(png_ptr, info_ptr);
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);//done with file
	#endif
	return 0;//will return 0 on success non-zero in error
}
