<?php
session_start();
require 'functions.inc';

$clean = array();
$mysql = array();

$settings = get_ca_settings();

print '<html><head>
		<title>Add Operator --  Friendly Call Service -- ' . $settings['location'] . '</title>
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
			print '<b>Operator ' . $clean['loginname'] . ' (' . $clean['fullname'] . ') added!</b><p>
					<a href="' . $_SERVER['PHP_SELF'] . '">Add another operator</a><p>';
		}
	}
}
else {	// this part happens if we don't press submit
	if ( $clean['operatorid']) {
		print '<div align="left">
			<table>
			<form method="post" action="' . $_SERVER['PHP_SELF'] . '" onsubmit="return(this.password = SHA1(this.password + \"' . getTheSalt() . '\"));">
			<tr>
				<td>Operator\'s login name: </td><td><input name="loginname" type="text" size="20" maxlength="64" /></td>
			</tr>
			<tr>
				<td>Operator\'s full name: </td><td><input name="fullname" type="text" size="20" maxlength="64" /></td>
			</tr>
			<tr>
				<td>Password: </td><td><input name="password" type="password" size="20" maxlength="64" /></td>
			</tr>
			<tr>
				<td>Administrator: </td><td><input type="checkbox" name="isAdmin" size="5" maxlength="5"></td>
				<td><<<< Check the checkbox if you want to give the operator administrator privileges</td>
			</tr>
			<tr>
				<td>Senior Operator: </td><td><input type="checkbox" name="isSnr" size="5" maxlength="5"></td>
				<td><<<< Check the checkbox if you want to make the operator the Senior Operator</td>
			</tr>
			<tr>
				<td><input name="submit" type="submit" value="Add Operator" /></td>
			</tr>
			</form></div></tr></td></table>';
	}
}

print '</body></html>';

?>