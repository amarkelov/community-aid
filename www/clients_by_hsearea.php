<?php
session_start();
require_once(dirname(dirname(__FILE__)) . "/php/login.inc");
require_once(dirname(dirname(__FILE__)) . "/php/hsearea.inc");
require_once(dirname(dirname(__FILE__)) . "/php/client.inc");
require_once(dirname(dirname(__FILE__)) . "/php/functions.inc");

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Clients by HSE Area", 0);

// if debug flag is set, print the following info
if ($settings['debug'] > 0) {
	require_once(dirname(dirname(__FILE__)) . "/php/functions.inc");
	print_debug();
}

// START LOG IN CODE
$doWeExit = displayLogin(basename($_SERVER['PHP_SELF']), true);
if($doWeExit == true){
	exit;
}
// END LOG IN CODE

/*
 * Start filtering input
 */
 
if(isset($_POST['hseareaid'])) {
	if(ctype_digit($_POST['hseareaid'])) { 
		$clean['hseareaid'] = $_POST['hseareaid'];
	}
}

if(isset($_POST['submit'])) {
	$clean['submit'] = $_POST['submit'];
}

if(isset($_SESSION['operatorid'])) {
	$clean['operatorid'] = $_SESSION['operatorid'];
}

if($settings['debug'] == 1){
	print "<b>\$clean on entry:</b><br>";
	print_r( $clean);
	print "<p>";
}

/*
 * End of filtering input
 */
 
if ($clean['hseareaid']) {
	if( !getClientByHSEArea( $clean['hseareaid'])) {	
		printErrorMessage( 'No clients registered in this area!');
	}
	printMessage( '<a href="' . $_SERVER['PHP_SELF'] . '">Choose another HSE Area</a>');
} // if ($submit)
else {

	// this part happens if we don't press submit
	if (!$clean['hseareaid']) {
		// pull the list of active clients
		$areas = array();
		
		if( getAllHSEAreas( $areas)) {
			print '<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
					<font face="Verdana, Arial, Helvetica, sans-serif">
					<div align="left">
					<table>';
	
			print '<tr><td>HSE Area name:</td><td><select name="hseareaid">';
			
			foreach ( $areas as $aid => $value) {
				print '<option value="' . $aid . '">'
					  . $value . ' (' . $aid . ')' . 
					  '</option>';
			
			}
	
			print '</select></td></tr>';
			print '<tr><td><font face="Verdana, Arial, Helvetica, sans-serif">
					<input type="Submit" name="getclients" value="Get clients list">
					</font>
					</div></td></tr>';
			
			print '</table></form>';
		}
	}
}
	
?>
