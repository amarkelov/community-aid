<?php
function printMessage( $message) {
	print '<p><b><font face="Verdana, Arial, Helvetica, sans-serif" size="2">' . 
		$message . '</font></b></p>';
}

function printErrorMessage( $message) {
	print '<p><b><font face="Verdana, Arial, Helvetica, sans-serif" size="2" color="#FF0000">' . 
		$message . '</font></b></p>';
}
?>