<?php
require "gl.php";
ob_start();
initcanvas(200,200);
if(!isset($_GET["l"])) {
	initlife();
	draw_binary(lukey_width,lukey_height,lukey_bits,0,0,black,white,no_transparency);
} else {
	draw(200,200,array_map("ord",str_split($_GET["l"])),0,0,no_transparency);
}
update();
render();
$image = ob_get_contents();
ob_clean();
putheader();
echo $image;
header("Refresh: 0.1; URL=?l=".urlencode($image));
?>
