#define RED 0
#define GREEN_RED 1
#define GREEN_BLUE 2
#define BLUE 3
#include <stdint.h>
int getBayerPixel(uint8_t*in,int x,int y,uint32_t w,uint32_t h,int color);
#define RED_RGB 0
#define GREEN_RGB 1
#define BLUE_RGB 2
void setPixelBounds(uint8_t*out,int x,int y,uint32_t w,uint32_t h,int channel,uint8_t val);
