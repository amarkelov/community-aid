<?php
session_start();
require 'functions.inc';

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Add Operator", 0);

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
	verifyOperatorData( $_POST, $clean);
}

if(isset($_SESSION['operatorid'])){
	if(ctype_digit($_SESSION['operatorid'])) {
		$clean['operatorid'] = $_SESSION['operatorid'];
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
 
if ($clean['submit']) {
	if ($clean['operatorid']) {
		
		if( addOperator($clean)) {
			printMessage( 'Operator ' . $clean['loginname'] . ' (' . $clean['fullname'] . ') added!');
		}
		else {
			printErrorMessage( 'Error occured while adding operator');
		}
		printMessage( '<a href="' . $_SERVER['PHP_SELF'] . '">Add another operator</a>');
	}
}
else {	// this part happens if we don't press submit
	if ( $clean['operatorid']) {
		print '<div align="left"><font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			<form method="post" action="' . $_SERVER['PHP_SELF'] . '" onsubmit="return(this.password = SHA1(this.password + \"' . getTheSalt() . '\"));">
			<table>
			<tr>
				<td>Operator\'s login name: </td><td><input name="loginname" type="text" size="20" maxlength="64"></td>
			</tr>
			<tr>
				<td>Operator\'s full name: </td><td><input name="fullname" type="text" size="20" maxlength="64"></td>
			</tr>
			<tr>
				<td>Password: </td><td><input name="password" type="password" size="20" maxlength="64"></td>
			</tr>
			<tr>
				<td>Administrator: </td><td><input type="checkbox" name="isadmin" size="5" maxlength="5"></td>
				<td><<<< Check the checkbox if you want to give the operator administrator privileges</td>
			</tr>
			<tr>
				<td>Senior Operator: </td><td><input type="checkbox" name="issnr" size="5" maxlength="5"></td>
				<td><<<< Check the checkbox if you want to make the operator the Senior Operator</td>
			</tr>
			<tr>
				<td><input name="submit" type="submit" value="Add Operator"></td>
			</tr>
			</table></form></font></div>';
	}
}

print '</body></html>';

?>