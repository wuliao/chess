#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <math.h>
#include "main.h"
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
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

typedef struct{
    char dest;
    char souce;
    char type;
    char len;
    int x1;
    int y1;
}msg_t;
char buffer[BUFFER_SIZE];
struct sockaddr_in server;
//char *p = (msg_t *)buffer;
socklen_t server_len;
int client_sock;
int n,nely;
u32_t color;
fd_set allset;
extern v_info_t fb_v;
int mmx, mmy;

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
    int fd, flag = 0,a;
    int mx = 512;
    int my = 384;
    
    fd = open("/dev/input/mice",O_RDWR);
    if(fd < 0)
    {
        perror("opean");
        exit(1);
     } 
        draw_cursor(mx,my);
        print_choice(); 
        printf("%d %d \n",my ,mx);
        a = fd > client_sock ? fd : client_sock;
        
        while(1)
        {   
            FD_ZERO(&allset);
            FD_SET(client_sock, &allset);    
            FD_SET(fd,&allset);
            nely = select(fd+1,&allset,NULL,NULL,NULL);   
                
            if(nely < 0)
            {   
                perror("select error");
                exit(EXIT_FAILURE);
            }
           if(flag == 0)
           {
            color = 0x000000ff;
           }
           if(FD_ISSET(fd,&allset))
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
                switch(mevent.button)
                {
                    case 1:
                            
                            //    save_shape(mx,  my);
                                //restore_shape(mx,my);
                                if(flag == 0)
                                {   
                                    if(chessman(mx , my, color) == 0)
                                    {
                                        msg_t *p = (msg_t *)buffer;
                                        p -> x1 = (mx-STARTING_X+SPACE/2)/SPACE;
                                        p -> y1 = (my-STARTING_Y+SPACE/2)/SPACE;
                                        server_len = sizeof(server);
                                        n = sendto(client_sock,buffer,BUFFER_SIZE,0,(struct sockaddr *)&server,server_len);
                                        flag = 1;
                           
                                        printf("%d\n",n);
                                        if(n < 0)
                                        {
                                            perror("sendto");
                                            exit(EXIT_FAILURE);
                                        }
                                        chess_count(mx,my);
                                        check_all();
                                    }        
                                }
                                 
                             break;
                    case 2 : break;
                    case 3 : break;
                    default : break;
                }
                draw_cursor(mx , my);
              } 
           } 
                
                if ( FD_ISSET(client_sock,&allset))
                {
                    server_len = sizeof(server);
                    if(flag == 1)
                    {
                        n = recvfrom(client_sock,buffer,BUFFER_SIZE,0,(struct sockaddr *)&server,&server_len);
                        color = 0x0000ff00;
                
                        if(n>0)
                        {
                            msg_t *p = (msg_t *)buffer;
                            mmx = p->x1*SPACE + STARTING_X;
                            mmy = p->y1*SPACE + STARTING_Y;
                            chessman(mmx,mmy, color);
                            chess_count(mmx,mmy); check_all(); }
                            flag = 0;
                    }
                }
            }
     return 0;

}
