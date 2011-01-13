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
extern int fb_line(int x1, int y1, int x2, int y2, u32_t color);
extern int fb_circle(int x, int y, int r, u32_t color);
extern void chessman(int x,int y);
extern int draw_cursor(int x,int y);
extern int save_shape(int x, int y);
extern int restore_shape(int x,int y);
extern int mouse_doing(void);
#endif
