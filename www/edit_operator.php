<?php
session_start();
require 'functions.inc';

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Edit Operator", 0);

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
else if(isset($_POST['edit'])) {
	$clean['edit'] = $_POST['edit'];

	if(isset($_POST['password'])){
		if( strlen($_POST['password']) > 0) {
			$clean['password'] = $_POST['password'];
		} 
	}
}

if(isset($_POST['operatorid_edit'])){
	if( ctype_digit($_POST['operatorid_edit'])) {
		$clean['operatorid_edit'] = $_POST['operatorid_edit'];
	}
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
	if ($clean['operatorid_edit']) {
		if( updateOperator( $clean)){
			printMessage('Operator ' . $clean['loginname'] . ' (' . $clean['fullname'] . ') updated!');
		}
		else {
			printErrorMessage('Error occured while updating operator!');
		}

		printMessage('<a href="' . $_SERVER['PHP_SELF'] . '">Edit another operator</a>');
	}
}
else  if( $clean['edit']){
	if ( $clean['operatorid_edit']) {
		print '<div align="left"><font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			<table>
			<form method="post" action="' . $_SERVER['PHP_SELF'] . '" onsubmit="return(this.password = SHA1(this.password + \"' . getTheSalt() . '\"));">
			<tr><td>Operator\'s login name: </td>
				<td><input name="loginname" type="text" size="20" maxlength="64" value="' . getOperatorLoginName($clean['operatorid_edit']) .'"></td></tr>
			<tr><td>Operator\'s full name: </td>
				<td><input name="fullname" type="text" size="20" maxlength="64" value="' . getOperatorName($clean['operatorid_edit']) .'"></td></tr>
			<tr><td>Password: </td>
				<td><input name="password" type="password" size="20" maxlength="64"></td></tr>
			<tr><td>Administrator: </td>';
		
		if(isOperatorAdmin($clean['operatorid_edit'])) {
			print '<td><input type="checkbox" name="isadmin" size="5" maxlength="5" checked></td>';
		}
		else {
			print '<td><input type="checkbox" name="isadmin" size="5" maxlength="5"></td>';
		}
		
		print '<td><<<< Check the checkbox if you want to give the operator administrator privileges</td></tr>
			   <tr><td>Senior Operator: </td>';
		
		if(isOperatorSnr($clean['operatorid_edit'])) {
			print '<td><input type="checkbox" name="issnr" size="5" maxlength="5" checked></td>';
		}
		else {
			print '<td><input type="checkbox" name="issnr" size="5" maxlength="5"></td>';
		}
		
		print '<td><<<< Check the checkbox if you want to make the operator the Senior Operator</td>
			</tr>
			<tr><td><input name="submit" type="submit" value="Submit" /></td></tr>
			<input type="hidden" name="operatorid_edit" value="' . $clean['operatorid_edit'] . '">
			</form></tr></td></table></div></font>';
	}
}
else {
	if ( $clean['operatorid']) {
		// pull the list of operators
		$operators = array();
		
		if( getOperators( $operators)) {
			print '<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
					<font face="Verdana, Arial, Helvetica, sans-serif">
					<div align="left">
					<table>';
	
			print '<tr><td><select name="operatorid_edit">';
			
			foreach ( $operators as $oid => $value) {
				print '<option value="' . $oid . '">'
					  . $value . ' (' . $oid . ')' . 
					  '</option>';
			
			}
	
			print '</select></td></tr>';
			print '<tr><td><font face="Verdana, Arial, Helvetica, sans-serif">
					<input type="Submit" name="edit" value="Edit Operator">
					</font>
					</div></td></tr>';
			
			print '</table></form>';
		}
	}

}

print '</body></html>';

?>