#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <io.h>
#include <time.h>
#define w 500
#define h 500
#define black 0
#define white 255
#define no_transparency 65536
unsigned char canvas[h][w];
unsigned char smiley[8][8] = {
   {0,255,255,255,255,255,255,0},
   {255,255,255,255,255,255,255,255},
   {255,255,0,255,255,0,255,255},
   {255,255,255,255,255,255,255,255},
   {255,0,255,255,255,255,0,255},
   {255,255,0,0,0,0,255,255},
   {255,255,255,255,255,255,255,255},
   {0,255,255,255,255,255,255,0},
};
int t;

unsigned int rgb2val(unsigned char r,unsigned char g,unsigned char b) {
   return (r*65536)+(g*256)+b;
}

unsigned char rgb8toval(unsigned char r,unsigned char g,unsigned char b) {
   return (r*32)+(g*4)+b;
}

unsigned char rgb2rgb8(unsigned char r,unsigned char g,unsigned char b) {
   return rgb8toval(floor(r/32),floor(g/32),floor(b/64));
}

void initCanvas() {
   for(int i = 0; i<h; i++) {
      for(int j = 0; j<w; j++) {
         canvas[i][j] = 0;
      }
   }
}

void plot(int x,int y,int c) {
   canvas[y][x] = c;
}

void draw(int iw,int ih,unsigned char image[ih][iw],int x,int y,int tc) {
   for(int iy = 0; iy<ih; iy++) {
      for(int ix = 0; ix<iw; ix++) {
        if(image[iy][ix] != tc) {
          canvas[iy+y][ix+x] = image[iy][ix];
        }
      }
   }
}

int getNeighbors(int x,int y) {
   int n = 0;
   if(canvas[x-1][y+1] == white){n=n+1;}
   if(canvas[x][y+1] == white){n=n+1;}
   if(canvas[x+1][y+1] == white){n=n+1;}
   if(canvas[x-1][y] == white){n=n+1;}
   if(canvas[x+1][y] == white){n=n+1;}
   if(canvas[x-1][y-1] == white){n=n+1;}
   if(canvas[x][y-1] == white){n=n+1;}
   if(canvas[x+1][y-1] == white){n=n+1;}
   return n;
}

void initLife() {
   for(int y = 0; y<h; y++) {
      for(int x = 0; x<w; x++) {
         canvas[y][x] = (rand()%2)*white;
      }
   }
}

void updatePix(int x,int y) {
   int n = getNeighbors(x,y);
   if(canvas[y][x] == white && n<3) {
      canvas[y][x] = black;
   }
   if(canvas[y][x] == white && n>4) {
      canvas[y][x] = black;
   }
   if(canvas[y][x] == white && (n==2 || n==3)) {
      canvas[y][x] = white;
   }
   if(canvas[y][x] == black && n==3) {
      canvas[y][x] = white;
   }
   if(canvas[y][x] > 5 && canvas[y][x] < 255) {
      canvas[y][x] = white;
   }
}

void update() {
   for(int y = 0; y<h; y++) {
      for(int x = 0; x<w; x++) {
         updatePix(x,y);
      }
   }
}

void munch() {
   for(int y = 0; y<h; y++) {
      for(int x = 0; x<w; x++) {
         int r = (((x&y)+t))*5;
         canvas[y][x] = r;
      }
   }
}

void render() {
   for(int y = 0; y<h; y++) {
      for(int x = 0; x<w; x++) {
         putchar(canvas[y][x]);
      }
   }
}

int main() {
   srand(time(NULL));
   #ifdef O_BINARY
   setmode(fileno(stdout), O_BINARY);
   #endif
   initCanvas();
   initLife();
   while(1) {
    munch();
    draw(8,8,smiley,t%w,t%h,black);
    update();
    render();
    t=t+1;
   }
   return 0;
}
