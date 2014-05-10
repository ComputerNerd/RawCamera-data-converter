//Argument defines

//2 bytes per pixel modes

#define ALG_YUV_0 0
#define ALG_YUV_1 1
#define ALG_YUV_2 2
#define ALG_YUV_3 3
#define ALG_RGB565 4

#define BPP2AMT 4 //How many are 2 bytes per pixel

//1 byte per pixel modes

#define ALG_DEBAYER_HQ 5
#define ALG_DEBAYER_BL 6
#define ALG_DEBAYER_Q 7
#define ALG_DEBAYER_N 8
#define ALG_ADAPTIVE 9

#define CLIP(X) ( (X) > 255 ? 255 : (X) < 0 ? 0 : X)
