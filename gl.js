const w = 256;
const h = 256;
const black = 0;
const white = 255;
const red = 224;
const green = 28;
const blue = 3;
const cyan = 31;
const magenta = 227;
const yellow = 252;
const no_transparency = 65536;
var canvas;
var c = document.getElementById("canvas");
var ctx = c.getContext("2d");

function randint(min,max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1) + min);
}

function rgb2val(r,g,b) {
    return (r*65536)+(g*256)+b;
}

function rgb8toval(r,g,b) {
    return (r*32)+(g*4)+b;
}

function rgb2rgb8(r,g,b) {
    return rgb8toval(floor(r/32),floor(g/32),floor(b/64));
}

function rgb8valtorgb(v) {
	r = (v >> 5) * 255 / 7
	g = ((v >> 2) & 0x07) * 255 / 7
	b = (v & 0x03) * 255 / 3
	var a = [r,g,b];
	return a;
}

function getpix(image,x,y,iw,ih) {
    return image[y*iw+x];
}

function initCanvas() {
	canvas = Array.from(Array(h), () => new Uint8Array(w));
	c.setAttribute("width",w);
	c.setAttribute("height",h);
    cls();
}

function plot(x,y,c) {
    canvas[y%h][x%w] = c;
}

function draw(iw,ih,img,x,y,tc) {
    for(var iy = 0; iy<ih; iy++) {
        for(var ix = 0; ix<iw; ix++) {
            if(img[iy][ix] != tc) {
                plot(ix+x,iy+y,img[iy][ix]);
            }
        }
    }
}

function binarycolor(c,bg,fg) {
    if(c == 0) {
        return bg;
    } else {
        return fg;
    }
}

function draw_binary(iw,ih,data,x,y,bg,fg,tc) {
    var img = Array.from(Array(ih), () => new Array(iw));
    for(var iy = 0; iy<ih; iy++) {
       for(var ix = 0; ix<iw; ix++) {
            img[iy][ix] = binarycolor(getpix(data,ix,iy,iw,ih),bg,fg);
       }
    }
    draw(iw,ih,img,x,y,tc);
}

function getLifePix(x,y) {
	if (canvas[y] == undefined) {
		return 0;
	} else if (canvas[y][x] == undefined) {
		return 0;
	} else {
		return canvas[y][x];
	}
}

function getNeighbors(x,y) {
    var n = 0;
    if(getLifePix(x-1,y+1) == white){n=n+1;}
    if(getLifePix(x,y+1) == white){n=n+1;}
    if(getLifePix(x+1,y+1) == white){n=n+1;}
    if(getLifePix(x-1,y) == white){n=n+1;}
    if(getLifePix(x+1,y) == white){n=n+1;}
    if(getLifePix(x-1,y-1) == white){n=n+1;}
    if(getLifePix(x,y-1) == white){n=n+1;}
    if(getLifePix(x+1,y-1) == white){n=n+1;}
    return n;
}

function initLife() {
    for(var y = 0; y<h; y++) {
       for(var x = 0; x<w; x++) {
            canvas[y][x] = (randint(0,1))*white;
       }
    }
}

function updatePix(x,y) {
    var n = getNeighbors(x,y);
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

function updateLife() {
    for(var y = 0; y<h; y++) {
      for(var x = 0; x<w; x++) {
            updatePix(x,y);
        }
    }
}

function munch() {
    for(var y = 0; y<h; y++) {
        for(var x = 0; x<w; x++) {
            var r = (((x&y)+t))*5;
            canvas[y][x] = r;
        }
    }
}

function cls() {
    for(var y = 0; y<h; y++) {
        for(var x = 0; x<w; x++) {
            canvas[y][x] = 0;
        }
    }
}

function getdata() {
	var i = new ImageData(w,h);
	var data = i.data;
	var p = 0;
	for(var y = 0; y<h; y++) {
        for(var x = 0; x<w; x++) {
			rgb = rgb8valtorgb(canvas[y][x]);
            data[p] = rgb[0];
			data[p+1] = rgb[1];
			data[p+2] = rgb[2];
			data[p+3] = 255;
			p += 4;
        }
    }
	i.data = data;
	return i;
}

function render() {
	var data = getdata();
	ctx.putImageData(data,0,0);
}

initCanvas();
setInterval(render,1000/60);