<?php
session_start();
require_once("login.inc");
require_once("groups.inc");

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Print group list", 0);

// if debug flag is set, print the following info
if ($settings['debug'] > 0) {
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

if(isset($_POST['submit'])) {
	$clean['submit'] = $_POST['submit'];
}

if(isset($_SESSION['operatorid'])){
	if(ctype_digit($_SESSION['operatorid'])) {
		$clean['operatorid'] = $_SESSION['operatorid'];
	}
}

if(isset($_POST['groupid'])){
	if(ctype_digit($_POST['groupid'])) {
		$clean['groupid'] = $_POST['groupid'];
	}
}

if($settings['debug'] == 1){
	print "<b>\$clean:</b><br>";
	print_r( $clean);
	print "<p>";
}
/*
 * End of filtering input
 */
 
if( $clean['operatorid']) {
	print '<font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
			<div align="left">
			<font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			Choose the group of clients you want to print<br><br>
			Group: ';

	getGroupNamesAsDropDownList();
	
	print '<br><br><input type="Submit" name="submit" value="Print the Group list">
	    	</div></form></font>';

	if ($clean['submit']) {
		printMessage('Group name: ' . getGroupName( $clean['groupid']));
		print '<font face="Verdana, Arial, Helvetica, sans-serif" size="2">
				<table frame="border" rules="rows" width="50%">';
		
		printGroupClients( $clean['groupid']);
		
		print '</table>';
	}
}
print '</body></html>';

?>