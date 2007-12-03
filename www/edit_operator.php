<?php
session_start();
require 'functions.inc';

$clean = array();
$mysql = array();

$settings = get_ca_settings();

print '<html><head>
		<title>Edit Operator --  Friendly Call Service -- ' . $settings['location'] . '</title>
		<meta http-equiv="expires" content="-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		</head>
		<body bgcolor="#BEC8FD">
		<font face="Verdana" size="2">';
 
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

	if(isset($_POST['loginname'])) {
		if( ctype_alnum( $_POST['loginname'])) {
			$clean['loginname'] = $_POST['loginname'];
		}
	}
	if(isset($_POST['fullname'])) {
		if( ctype_print($_POST['fullname'])) {
			$clean['fullname'] = $_POST['fullname'];
		}
	}
	if(isset($_POST['password'])){
		$clean['password'] = $_POST['password']; 
	}
	if(isset($_POST['isAdmin'])) {
		if(strtoupper($_POST['isAdmin']) == "ON") {
			$clean['isAdmin'] = 1;
		}
		else {
			$clean['isAdmin'] = 0;
		}
	}
	if(isset($_POST['isSnr'])) {
		if(strtoupper($_POST['isSnr']) == "ON") {
			$clean['isSnr'] = 1;
		}
		else {
			$clean['isSnr'] = 0;
		}
	}
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
			print '<b>Operator ' . $clean['loginname'] . ' (' . $clean['fullname'] . ') changed!</b><p>';
		}

		print '<a href="' . $_SERVER['PHP_SELF'] . '">Edit another operator</a><p>';
	}
}
else  if( $clean['edit']){
	if ( $clean['operatorid_edit']) {
		print '<div align="left">
			<table>
			<form method="post" action="' . $_SERVER['PHP_SELF'] . '" onsubmit="return(this.password = SHA1(this.password + \"' . getTheSalt() . '\"));">
			<tr><td>Operator\'s login name: </td>
				<td><input name="loginname" type="text" size="20" maxlength="64" value="' . getOperatorLoginName($clean['operatorid_edit']) .'" /></td></tr>
			<tr><td>Operator\'s full name: </td>
				<td><input name="fullname" type="text" size="20" maxlength="64" value="' . getOperatorName($clean['operatorid_edit']) .'"/></td></tr>
			<tr><td>Password: </td>
				<td><input name="password" type="password" size="20" maxlength="64" /></td></tr>
			<tr><td>Administrator: </td>';
		
		if(isOperatorAdmin($clean['operatorid_edit'])) {
			print '<td><input type="checkbox" name="isAdmin" size="5" maxlength="5" checked></td>';
		}
		else {
			print '<td><input type="checkbox" name="isAdmin" size="5" maxlength="5"></td>';
		}
		
		print '<td><<<< Check the checkbox if you want to give the operator administrator privileges</td></tr>
			   <tr><td>Senior Operator: </td>';
		
		if(isOperatorSnr($clean['operatorid_edit'])) {
			print '<td><input type="checkbox" name="isSnr" size="5" maxlength="5" checked></td>';
		}
		else {
			print '<td><input type="checkbox" name="isSnr" size="5" maxlength="5"></td>';
		}
		
		print '<td><<<< Check the checkbox if you want to make the operator the Senior Operator</td>
			</tr>
			<tr><td><input name="submit" type="submit" value="Submit" /></td></tr>
			<input type="hidden" name="operatorid_edit" value="' . $clean['operatorid_edit'] . '" />
			</form></div></tr></td></table>';
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