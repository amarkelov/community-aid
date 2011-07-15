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

function verify_timeslot($timeslot)
{
	$ts = "00:00";	
	$vt = explode(":", $timeslot, 2);

	if(ctype_digit($vt[0]) && ctype_digit($vt[1])) {
		if(($vt[0]>=0 && $vt[0]<=23) && ($vt[1]>=0 && $vt[1]<=59)) {
			$ts = "$vt[0]:$vt[1]";
		}
	}
	
	return $ts; 
}

function filter_phone_number($phone) {
	$filtered = preg_replace( "/ |-/", "", $phone);
	if(!ctype_digit($filtered)) {
		$filtered = "N/A";	
	}
	return $filtered;
}

function array2string( $array) {
	$key = 0;
	$val = 0;
	$list = "";
	$i = 0;
	
	while( list($key, $val) = each($array)) {
	    if( $i > 0) {
			$list .= "," . $val;
	    }
	    else {
	    	/* it's the first item */
	    	$list = $val;
	    }
	    $i++;
	}
	
	return $list;
}

?>
