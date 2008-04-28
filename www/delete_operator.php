<?php
session_start();
require_once("login.inc");
require_once("operator.inc");

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Delete Operator", 0);

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
		if( deleteOperator( $clean)) {
			printMessage( 'Operator ' . $clean['loginname_delete'] . ' deleted!');
		}
		else {
			printErrorMessage( 'Error occured while deleting operator!');
		}
		printMessage( '<a href="' . $_SERVER['PHP_SELF'] . '">Delete another operator</a>');
	}
}
else {	// this part happens if we don't press delete
	if ( $clean['operatorid']) {
		// pull the list of operators
		$operators = array();
		
		if( getOperators( $operators)) {
			print '<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
					<font face="Verdana, Arial, Helvetica, sans-serif">
					<div align="left">
					<table>
					<tr><td><select name="operatorid_delete">';
			
			foreach ( $operators as $oid => $value) {
				print '<option value="' . $oid . '">'
					  . $value . ' (' . $oid . ')' . 
					  '</option>';
			
			}
	
			print '</select></td></tr>
					<tr><td><font face="Verdana, Arial, Helvetica, sans-serif">
					<input type="Submit" name="delete" value="Delete Operator">
					</font>
					</div></td></tr>
					</table></form>';
		}
	}
}

print '</body></html>';

?>