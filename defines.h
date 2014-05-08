//Argument defines

//2 bytes per pixel modes

#define ALG_YUV_0 0
#define ALG_YUV_1 1
#define ALG_RGB565 2

#define BPP2AMT 2 //How many are 2 bytes per pixel

//1 byte per pixel modes

#define ALG_DEBAYER_HQ 3
#define ALG_DEBAYER_BL 4
#define ALG_DEBAYER_Q 5
#define ALG_DEBAYER_N 6

#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)
