#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>
#include <io.h>
#include <time.h>
#include "images.h"
#define w 100
#define h 100
#define black 0
#define white 255
#define red 224
#define green 28
#define blue 3
#define cyan 31
#define magenta 227
#define yellow 252
#define no_transparency 65536
#define arrayLength(array) (sizeof((array))/sizeof((array)[0]))
unsigned char canvas[h][w];
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

int getpix(unsigned char image[],int x,int y,int iw,int ih) {
    return image[y*iw+x];
}

void initCanvas() {
    for(int i = 0; i<h; i++) {
        for(int j = 0; j<w; j++) {
            canvas[i][j] = 0;
        }
    }
}

void plot(int x,int y,int c) {
    canvas[y%h][x%w] = c;
}

void draw(int iw,int ih,unsigned char image[ih][iw],int x,int y,int tc) {
    for(int iy = 0; iy<ih; iy++) {
        for(int ix = 0; ix<iw; ix++) {
            if(image[iy][ix] != tc) {
                plot(ix+x,iy+y,image[iy][ix]);
            }
        }
    }
}

unsigned char binarycolor(unsigned char c,unsigned char bg,unsigned char fg) {
    if(c == 0) {
        return bg;
    } else {
        return fg;
    }
}

void draw_binary(int iw,int ih,unsigned char data[],int x,int y,int bg,int fg,int tc) {
    unsigned char img[ih][iw];
    for(int y = 0; y<ih; y++) {
       for(int x = 0; x<iw; x++) {
            img[y][x] = binarycolor(getpix(data,x,y,iw,ih),bg,fg);
       }
    }
    draw(iw,ih,img,x,y,tc);
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
        draw_binary(lukey_width,lukey_height,lukey_bits,t%w,t%h,magenta,cyan,cyan);
        render();
        t=t+1;
    }
}
