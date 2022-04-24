<?php
require "images.php";
define("black",0);
define("white",255);
define("red",224);
define("green",28);
define("blue",3);
define("cyan",31);
define("magenta",227);
define("yellow",252);
define("no_transparency",65535);
error_reporting(E_ALL ^ E_WARNING);

function initcanvas(int $w,int $h) {
	define("w",$w);
	define("h",$h);
	$GLOBALS["canvas"] = array_fill(0, h, array_fill(0, w, 0));
}

function rgb2val(int $r,int $g,int $b) {
    return ($r*65536)+($g*256)+$b;
}

function rgb8toval(int $r,int $g,int $b) {
    return ($r*32)+($g*4)+$b;
}

function rgb2rgb8(int $r,int $g,int $b) {
    return rgb8toval(floor($r/32),floor($g/32),floor($b/64));
}

function minvalue(int $v) {
	return min($v,255);
}

function getpix($image,int $x,int $y,int $iw,int $ih) {
    return $image[$y*$iw+$x];
}

function plot(int $x,int $y,int $c) {
	global $canvas,$t;
    $canvas[$y%h][$x%w] = $c;
}

function draw(int $iw,int $ih,$image,int $x,int $y,int $tc) {
    for($iy = 0; $iy<$ih; $iy++) {
        for($ix = 0; $ix<$iw; $ix++) {
            if($image[$iy][$ix] != $tc) {
                plot($ix+$x,$iy+$y,getpix($image,$ix,$iy,$iw,$ih));
            }
        }
    }
}

function binarycolor(int $c,int $bg,int $fg) {
    if($c == 0) {
        return $bg;
    } else {
        return $fg;
    }
}

function draw_binary(int $iw,int $ih,$data,int $x,int $y,int $bg,int $fg,int $tc) {
    $img = array_fill(0, $ih, array_fill(0, $iw, 0));
    for($i = 0; $i<count($data); $i++) {
       $img[$i] = binarycolor($data[$i],$bg,$fg);
    }
    draw($iw,$ih,$img,$x,$y,$tc);
}

function getNeighbors(int $x,int $y) {
	global $canvas,$t;
	$n = 0;
	if($canvas[$x-1][$y+1] == white){$n=$n+1;}
	if($canvas[$x][$y+1] == white){$n=$n+1;}
	if($canvas[$x+1][$y+1] == white){$n=$n+1;}
	if($canvas[$x-1][$y] == white){$n=$n+1;}
	if($canvas[$x+1][$y] == white){$n=$n+1;}
	if($canvas[$x-1][$y-1] == white){$n=$n+1;}
	if($canvas[$x][$y-1] == white){$n=$n+1;}
	if($canvas[$x+1][$y-1] == white){$n=$n+1;}
	return $n;
}

function initLife() {
	global $canvas,$t;
    for($y = 0; $y<h; $y++) {
       for($x = 0; $x<w; $x++) {
            $canvas[$y][$x] = rand(0,1)*white;
       }
    }
}

function updatePix(int $x,int $y) {
	global $canvas,$t;
    $n = getNeighbors($x,$y);
    if($canvas[$y][$x] == white && $n<3) {
        $canvas[$y][$x] = black;
    }
    if($canvas[$y][$x] == white && $n>4) {
        $canvas[$y][$x] = black;
    }
    if($canvas[$y][$x] == white && ($n==2 || $n==3)) {
        $canvas[$y][$x] = white;
    }
    if($canvas[$y][$x] == black && $n==3) {
        $canvas[$y][$x] = white;
    }
    if($canvas[$y][$x] > 5 && $canvas[$y][$x] < 255) {
        $canvas[$y][$x] = white;
    }
}

function update() {
    for($y = 0; $y<h; $y++) {
      for($x = 0; $x<w; $x++) {
            updatePix($x,$y);
        }
    }
}

function munch() {
	global $canvas,$t;
    for($y = 0; $y<h; $y++) {
        for($x = 0; $x<w; $x++) {
            $r = ((($x&$y)+$t))*5;
            $canvas[$y][$x] = $r&255;
        }
    }
}

function render() {
	global $canvas,$t;
    for($y = 0; $y<h; $y++) {
        for($x = 0; $x<w; $x++) {
            echo chr($canvas[$y][$x]);
        }
    }
}

function putheader() {
	header("Content-Type: image/bmp");
	echo "BM";
	echo pack("V*",4294967295);
	echo pack("v*",0);
	echo pack("v*",0);
	echo pack("V*",1078);
	echo pack("V*",40);
	echo pack("V*",w);
	echo pack("V*",h);
	echo pack("v*",1);
	echo pack("v*",8);
	echo pack("V*",0);
	echo pack("V*",0);
	echo pack("V*",w);
	echo pack("V*",h);
	echo pack("V*",256);
	echo pack("V*",0);
	foreach (range(0,255) as $i) {
		echo chr(minvalue(round(($i>>5)*36.5)));
		echo chr(minvalue(round((($i>>2)&7)*36.5)));
		echo chr(minvalue(($i&3)*85));
		echo chr(0);
	}
}

srand(time());
?>
