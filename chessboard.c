#include <stdio.h> 
#include "main.h"

#define   STARTING_X  100
#define   STARTING_Y  40
#define   SPACE       30

void  CreatBoard(void)
{   
    u32_t k=0 ,i=0,j =0;

    for(k=0; k < (P_NUM - 1)* SPACE ;k++ )
    {
       for(j = 0; j < (V_NUM - 1) * SPACE; j++ )
       {
            fb_one_pixel( STARTING_X  + j , STARTING_Y + k ,0x0000ff00);
       }     
    }

    for(i = 0;i < P_NUM; i++)
    {
        fb_line(STARTING_X , STARTING_Y + i*SPACE , STARTING_X +(V_NUM - 1)*SPACE , STARTING_Y +i*SPACE  , 0x000000ff);
    }

    for(i= 0; i< V_NUM; i++)
    {
        fb_line(STARTING_X + i*SPACE , STARTING_Y  , STARTING_X + i*SPACE , STARTING_Y +(P_NUM -1 )*SPACE  , 0x000000ff);
    }
}

void setdot(int x, int y )
{
    int y1 =((y - STARTING_Y)/SPACE)*SPACE + STARTING_Y,      y2 = ((y - STARTING_Y)/SPACE)*SPACE + STARTING_Y +SPACE;
    int x1 =((x - STARTING_X)/SPACE)*SPACE + STARTING_X ,     x2 = ((x - STARTING_X)/SPACE)*SPACE + STARTING_X +SPACE;
    
    if((x2 -x) > (x - x1) )
    {
        x = x1;
    }
    else
    {
        x = x2;    
    }
    if( (y2 - y) > (y - y1) )
    {
        y = y1 ;
    }
    else
    {
        y = y2 ;
    }
    fb_one_pixel(x , y, 0x00ff0000);
}
