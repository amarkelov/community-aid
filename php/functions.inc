<?php
require_once 'config.inc';

$last_dow = 6;

function print_debug( $clean, $settings) {
// if debug flag is set, print the following info
	$s = array();
	$s = get_ca_settings();
	if($s['debug'] >0){
		print "<b>\$_POST:</b><br>";
		print_r( $_POST);
		print "<p>";

		print "<b>\$_GET:</b><br>";
		print_r( $_GET);
		print "<p>";

		print "<b>\$clean:</b><br>";
		print_r( $clean);
		print "<p>";

		print "<b>\$settings:</b><br>";
		print_r( $settings);
		print "<p>";

		print "<b>\$_SERVER:</b><br>";
		print_r( $_SERVER);
		print "<p>";

		print "<b>\$_SESSION:</b><br>";
		print_r( $_SESSION);
		print "<p>";

/*		print "<b>\$GLOBALS:</b><br>";
		print_r( $GLOBALS);
		print "<p>";*/
	}
}

?>
