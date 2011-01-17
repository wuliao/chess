#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <string.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include "main.h"
#include <time.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>


struct sockaddr_in server;
int client_sock;

void create_scr_fb(void)
{
    int fd;
    struct fb_var_screeninfo fb_var;
    u32_t *p=NULL;

    fd=open("/dev/fb0",O_RDWR);
    if(fd<0)
    {
        perror("open");
        exit(1);
    }
    if(ioctl(fd,FBIOGET_VSCREENINFO,&fb_var)<0)
    {
        perror("ioctl");
        exit(1);
    }
    fb_v.w=fb_var.xres;  
    fb_v.h=fb_var.yres;  
    fb_v.bpp=fb_var.bits_per_pixel; 
    printf("w = %d\th = %d\tbpp = %d\n",fb_v.w,fb_v.h,fb_v.bpp);
    p = mmap(NULL,fb_v.w*fb_v.h*fb_v.bpp/8,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
    if(p == MAP_FAILED)
    {
        perror("map");
        exit(1);
    }
    *(p)=0x00ff0000;
    fb_v.fb_mem = p;
    close(fd);
}

int fb_one_pixel(int x , int y , u32_t color)
{
    u32_t *p = fb_v.fb_mem;
   
    p[x+y*fb_v.w] = color;
    return 0;
}

        
int main(int argc, const char *argv[])
{
    create_scr_fb();
    CreatBoard();
//    socket(AF_INET,SOCK_DGRAM,0 );
  if((client_sock = socket(AF_INET,SOCK_DGRAM,0 ))<0)
   {
        fprintf(stderr, "%s\n",strerror(errno));           
        exit(EXIT_FAILURE);  
   }
   else
   {        
        printf("creat sock ok!\n");
   }
   
   bzero(&server,sizeof(server));
   server.sin_family = AF_INET;
   server.sin_port = htons(SERVER_PORT);
   server.sin_addr.s_addr = inet_addr(argv[1]);
//   draw_cursor(500,345);
  // client_sock = mouse_doing();    
    mouse_doing();
   // close(client_sock);
    return 0;      
}
