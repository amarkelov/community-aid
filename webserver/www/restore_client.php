<?php
session_start();
require_once(dirname(dirname(__FILE__)) . "/php/login.inc");
require_once(dirname(dirname(__FILE__)) . "/php/client.inc");

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Re-activate Client", 0);

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
 
if(isset($_POST['clientid'])) {
	if(ctype_digit($_POST['clientid'])) { 
		$clean['clientid'] = $_POST['clientid'];
	}
}

if(isset($_POST['submit'])) {
	$clean['submit'] = $_POST['submit'];
}


if($settings['debug'] == 1){
	print "<b>\$clean:</b><br>";
	print_r( $clean);
	print "<p>";
}
/*
 * End of filtering input
 */
 
if ($clean['submit']) {
	// here if no ID then editing  else adding
	if ($clean['clientid']) {
		if( reactivateClient( $clean['clientid'])) {	
			printMessage('Client set active!');
		}
		else {
			printErrorMessage( 'Error occured while setting client active!');
		}
		printMessage( '<a href="' . $_SERVER['PHP_SELF'] . '">Choose another client to reactivate</a>');
	}
}
else {	// this part happens if we don't press submit

	// pull the list of active clients
	$clients = array();
	
	if( getInactiveClients( $clients)) {
		if( sizeof( $clients) > 0) {
			print '<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
					<font face="Verdana, Arial, Helvetica, sans-serif">
					<div align="left">
					<table>';
	
			print '<tr><td><select name="clientid">';
			
			foreach ( $clients as $cid => $value) {
				print '<option value="' . $cid . '">'
					  . $value . ' (' . $cid . ')' . 
					  '</option>';
			
			}
	
			print '</select></td></tr>';
			print '<tr><td><font face="Verdana, Arial, Helvetica, sans-serif">
					<input type="Submit" name="submit" value="Re-activate client">
					</font>
					</div></td></tr>';
			
			print '</table></form>';
		}
		else {
			printMessage( 'No inactive clients in the system at the moment.');
		}
	}
}

print '</body></html>';

?>