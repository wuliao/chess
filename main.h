#ifndef _MAIN_
#define _MAIN_

#include <stdio.h>
typedef unsigned int u32_t;
typedef unsigned int u8_t;
#define P_NUM  24
#define V_NUM  30
typedef struct
{ int w;
	int h;
	int bpp;
	void *fb_mem;
} v_info_t;
    v_info_t fb_v;
extern int fb_one_pixel(int x,int y,u32_t color);
extern void CreatBoard(void);
#endif
