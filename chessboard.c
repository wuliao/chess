#include <stdio.h> 
#include "main.h"

#define   STARTING_X  100
#define   STARTING_Y  40
#define   SPACE       30

void  CreatBoard(void)
{   
    u32_t k=0 ,i=0,j =0;

    for(k=0; k < (P_NUM )* SPACE ;k++ )
    {
       for(j = 0; j < (V_NUM ) * SPACE; j++ )
       {
            fb_one_pixel( STARTING_X - SPACE/2  + j , STARTING_Y - SPACE/2 + k ,0X00F4A460);
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
extern u32_t color_choice;
extern char who;

int choose_color(int x,int y)
{
    if((x>=20) && (x<=60))
    {
        if((y>=80) && (y<=120))
        {
            color_choice = 0x000000ff;
            who = 1;
        }
        if((y>=180) && (y<=220))
        {
            color_choice = 0x00ff0000;
            who = 2;
        }
    }
    return 0;
}

int chessman(int x,int y, u32_t color_choice)
{
/*      
      if((x < 100) || (x >(30*29 +100)))
      {
        choose_color(x,y);
        return 1;
      }
      if((y < 40) || y >(30*23 + 40))
      {
        choose_color(x,y);
        return 1;
      }*/
        
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
        fb_circle( x , y, 13, color_choice);
    return 0; 
}
