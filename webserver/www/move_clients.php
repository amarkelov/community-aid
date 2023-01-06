<?php
session_start();
require_once(dirname(dirname(__FILE__)) . "/php/login.inc");
require_once(dirname(dirname(__FILE__)) . "/php/groups.inc");

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Move clients between groups", 0);

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

if(isset($_POST['new_groupid'])){
	if(ctype_digit($_POST['new_groupid'])) {
		$clean['new_groupid'] = $_POST['new_groupid'];
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
			<b><font size="+1">Move clients</font><b><br><br>
			<table>
			<tr>
			<td>From group: </td><td>';

	getGroupNamesAsDropDownList("groupid");
	
	print '</tr><tr><td>To Group: </td><td>';
	getGroupNamesAsDropDownList("new_groupid");
	
	print '</td></tr></table>
			<br><br><input type="Submit" name="submit" value="Move clients">
	    	</div></form></font>';

	if ($clean['submit']) {
		print '<font face="Verdana, Arial, Helvetica, sans-serif" size="2">';
		
		if ( $clean['groupid'] == $clean['new_groupid']) {
			printErrorMessage( 'Source and destination groups are the same!');
		}
		else {
			if ( moveClientsToGroup( $clean['groupid'], $clean['new_groupid'])) {
				printMessage( 'Clients moved from '. getGroupName( $clean['groupid']) .
								' to ' . getGroupName( $clean['new_groupid']) . ' successfuly!');
			}
			else {
				printErrorMessage( 'Error occured while moving clients 
								from '. getGroupName( $clean['groupid']) .
								' to ' . getGroupName( $clean['new_groupid']) . '!');
			}
			
//			printMessage('<a href="' . $_SERVER['PHP_SELF'] . '">Assign client(s) to another operator</a>');
		}
	}
}
print '</body></html>';

?>