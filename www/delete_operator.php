<?php
session_start();
require 'functions.inc';

$clean = array();
$mysql = array();

$settings = get_gmp_settings();

print '<html><head>
		<title>Delete Operator --  Friendly Call Service -- ' . $settings['location'] . '</title>
		<meta http-equiv="expires" content="-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		</head>
		<body bgcolor="#BEC8FD"><p>';
 
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

if(isset($_POST['delete'])) {
	$clean['delete'] = $_POST['delete'];

	if(isset($_POST['operatorid_delete'])) {
		if(ctype_digit($_POST['operatorid_delete'])) {
			$clean['operatorid_delete'] = 	$_POST['operatorid_delete'];	
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
 
if ($clean['delete']) {
	if ( $clean['operatorid']) {
		if( $clean['operatorid_delete'] != 1) {
			$loginname = getOperatorLoginName($clean['operatorid_delete']);
			if( ctype_alnum($loginname)) {
				$clean['loginname_delete'] = $loginname;
			}
			 
			$dbConnect = dbconnect();
			
			$sql = 'DELETE FROM operators WHERE loginname="' . $clean['loginname_delete'] . '"';
					
			$result = mysql_query( $sql, $dbConnect);
			if ( !$result) {
				$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
				die($message);
			}
			else {
				print '<b>Operator ' . $clean['loginname_delete'] . ' deleted!</b><p>
						<a href="' . $PHP_SELF . '">Delete another operator</a><p>';
			}
			
			dbclose( $dbConnect);
		}
		else {
			print '<b>You cannot delete default System Administrator!</b><p>
					<a href="' . $PHP_SELF . '">Delete another operator</a><p>';
		
		}
	}
}
else {	// this part happens if we don't press delete
	if ( $clean['operatorid']) {
		// pull the list of operators
		$operators = array();
		
		if( getOperators( $operators)) {
			print '<form method="post" action="' . $PHP_SELF . '">
					<font face="Verdana, Arial, Helvetica, sans-serif">
					<div align="left">
					<table>';
	
			print '<tr><td><select name="operatorid_delete">';
			
			foreach ( $operators as $oid => $value) {
				print '<option value="' . $oid . '">'
					  . $value . ' (' . $oid . ')' . 
					  '</option>';
			
			}
	
			print '</select></td></tr>';
			print '<tr><td><font face="Verdana, Arial, Helvetica, sans-serif">
					<input type="Submit" name="delete" value="Delete Operator">
					</font>
					</div></td></tr>';
			
			print '</table></form>';
		}
	}
}

print '</body></html>';

?>