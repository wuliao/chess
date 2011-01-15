#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "main.h"

typedef struct {
        int dx;
        int dy;
        int dz;
        int button;
}m_event;

#define C_WIDTH  10
#define C_HEIGHT 17
#define T__      0xffffffff
#define BORD     0x0
#define X__      0xffff   

 
#define   STARTING_X  100
#define   STARTING_Y  40
#define   SPACE       30
extern v_info_t fb_v;
static u32_t cursor_pixel[C_WIDTH*C_HEIGHT] ={
      BORD,  T__,T__,T__,T__,T__,T__,T__,T__,T__,
      BORD, BORD,T__,T__,T__,T__,T__,T__,T__,T__, 
      BORD,X__, BORD,T__,T__,T__,T__,T__,T__,T__,
      BORD,X__,X__ ,BORD, T__,T__,T__,T__,T__,T__,
      BORD,X__,X__,X__ ,BORD, T__,T__,T__,T__,T__,
      BORD,X__,X__,X__,X__, BORD,T__,T__,T__,T__,
      BORD,X__,X__,X__,X__,X__ ,BORD, T__,T__,T__,
      BORD,X__,X__,X__,X__,X__,X__, BORD,T__,T__,
      BORD,X__,X__,X__,X__,X__,X__,X__, BORD,T__,
      BORD,X__,X__,X__,X__,X__,X__,X__,X__, BORD,
      BORD,X__,X__,X__,X__,X__,BORD,BORD,BORD,BORD, 
      BORD,X__,X__,BORD,X__,X__,BORD,T__,T__,T__,
      BORD,X__,BORD,T__,BORD,X__,X__,BORD,T__,T__, 
      BORD,BORD,T__,T__,BORD,X__,X__,BORD,T__,T__,
      T__,T__,T__,T__,T__,BORD,X__,X__,BORD,T__,
      T__,T__,T__,T__,T__,BORD,X__,X__,BORD,T__,
      T__,T__,T__,T__,T__,T__,BORD,BORD,T__,T__
};

static u32_t shape_save[C_WIDTH*C_HEIGHT];
char board[V_NUM*P_NUM];
char who = 1;
u32_t color_choice = 0x000000ff;

int save_shape(int x, int y)
{
    int i = 0 ;
    int j = 0 ;
    u32_t *p = fb_v.fb_mem;

    for(i=0;i<C_HEIGHT;i++)
    {
        for(j = 0 ; j < C_WIDTH;j++)
        {
            shape_save[j+i*C_WIDTH] = p[x+j+(y+i)*fb_v.w];
        }
    }
    return 0;
}
int restore_shape(int x,int y)
{
    int i = 0;
    int j = 0;
    
    for(i=0;i<C_HEIGHT;i++ )
    {
        for(j=0;j<C_WIDTH;j++)
        {
            fb_one_pixel(x+j,y+i,shape_save[j+i*C_WIDTH]);
        }
    }
    return 0;
}

int draw_cursor(int x,int y)  
{
    int i = 0;
    int j = 0;
    
    save_shape(x, y);
    for(i =0;i <C_HEIGHT;i++)
    {
        for(j = 0;j< C_WIDTH;j++)
        {
            if(cursor_pixel[j+i*C_WIDTH] != T__ )
            {
                fb_one_pixel( x+j, y+i,cursor_pixel[j+i*C_WIDTH]);     
            }    
        }
    }
    return 0;
}
int get_m_info(int fd,m_event * event)
{

    char buf[8] = {0};
    int n;
    n = read (fd ,buf,8);
    if(n <= 0)
    {
        return 0;
    }
    event->button = (buf[0] & 0x07);
    event->dx = buf[1];
    event->dy = -buf[2];
    event->dz = buf[3];

    return n;
}

int check_five(int x,int y)
{
    int i = 0;
    int j =0;
    char counter = 0;
    char storage = 0;
    char nx = 0;
    char ny = 0;

    char n_x[4]= {0,1,1,1};
    char n_y[4]= {1,0,1,-1};
    
    storage = board[x + y*V_NUM];
    if(storage == 0)
    {
        return 0;
    }
    for(j=0; j<4;j++ )
    {
    
        counter = 1;
        nx = x;
        ny = y;

        for(i = 1;i< 5; i++)
        {
            nx += n_x[j];
            ny += n_y[j];
            if(board[nx+ny*V_NUM] == storage)
            {
                
                counter++;
            }
            else
            {
                break;
            }
        }
        if(counter == 5)
        {   
            return counter;
        }
     }   
    return 0;
}
int chess_count(int x, int y)
{
    int i=x;
    int j=y;

    i = (i-STARTING_X)/SPACE;
    j = (j-STARTING_Y)/SPACE;

    if(((x-STARTING_X)%SPACE) >(SPACE/2))
    {
        i++;
    }
    if(((y-STARTING_Y)%SPACE) >(SPACE/2))
    {
        j++;
    }
    board[i+j*V_NUM] = who;
    return 0;
}


int check_all(void)
{
    int i = 0;
    int j = 0;

    for(i = 0;i < P_NUM;i++)
    {
        for(j = 0;j < V_NUM; j++)
        {
            if(check_five(i,j) != 0)
            {
                printf("%d won!\n",who);
                return 1;
            }
        }
    }
    return 0;
}
int print_choice(void)
{
    fb_circle(40,100,20 ,0x000000ff);
    fb_circle(40,200,20 ,0x00ff0000);
    return 0;
}


int mouse_doing(void)
{
    m_event mevent;
    int fd;
    int mx = 512;
    int my = 384;
    
    fd = open("/dev/input/mice",O_RDWR|O_NONBLOCK);
    if(fd < 0)
    {
        perror("opean");
        exit(1);
     } 
        draw_cursor(mx,my);
        print_choice(); 
        printf("%d %d \n",my ,mx);
        while(1)
        {
            if(get_m_info(fd ,&mevent) > 0)
            {
                
                restore_shape(mx,my);
                print_choice();
                mx += mevent.dx;
                my += mevent.dy;
                mx = ((mx >= 0 ) ? mx : 0);
                my = ((my >= 0 ) ? my : 0);

                if(mx >= (fb_v.w-C_WIDTH))
                {
                    mx = (fb_v.w-C_WIDTH);
                }
                if(my >= (fb_v.h-C_HEIGHT))
                {
                    my = (fb_v.h-C_HEIGHT);
                }
                draw_cursor(mx , my);
                switch(mevent.button)
                {
                    case 1:
                             restore_shape(mx,my);
                             if(chessman(mx , my) == 0)
                             {
                                chess_count(mx,my);
                                check_all();
                             }
                             draw_cursor(mx , my);
                             break;
                    case 2 : break;
                    case 3 : break;
                    default : break;
                }
             } 
             usleep(1000);
          }
     return 0;

}
