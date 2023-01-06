<?php
session_start();
require_once(dirname(dirname(__FILE__)) . "/php/login.inc");
require_once(dirname(dirname(__FILE__)) . "/php/operator.inc");
require_once(dirname(dirname(__FILE__)) . "/php/client.inc");
require_once(dirname(dirname(__FILE__)) . "/php/groups.inc");

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Assign Operator to Group of clients", 0);

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

if(isset($_POST['operatorid_edit'])){
	if(ctype_digit($_POST['operatorid_edit'])) {
		$clean['operatorid_edit'] = $_POST['operatorid_edit'];
	} 
}

if(isset($_SESSION['operatorid'])){
	if(ctype_digit($_SESSION['operatorid'])) {
		$clean['operatorid'] = $_SESSION['operatorid'];
	}
}

if(isset($_POST['groupid'])){
	$clean['groupid'] = array();
	
	foreach($_POST['groupid'] as $id => $gid) {
		if(ctype_digit($gid)) {
			$clean['groupid'][] = $gid;
		}
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
 
if( $clean['submit']) {
	if ( $clean['operatorid_edit']) {
		if ( isset( $clean['groupid'])) {
			if( addGroupToOperator( $clean['groupid'], $clean['operatorid_edit'])) {
				printMessage( 'Group(s) assigned to operator!');
			}
			else {
				printErrorMessage( 'Error occured while assigning group(s) to operator');
			}
			
			printMessage('<a href="' . $_SERVER['PHP_SELF'] . '">Assign group(s) to another operator</a>');
		}
	}
}
else {	// this part happens if we don't press submit
	if ( $clean['operatorid']) {
		// pull the list of operators
		$operators = array();

		if( getOperators( $operators)) {
			print '<font face="Verdana, Arial, Helvetica, sans-serif" size="2">
					<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
					<div align="left" style="vertical-align:top">';

			print 'Choose the Operator you want to assign clients to<br><br>
				Operator: <select name="operatorid_edit">';
			
			foreach ( $operators as $oid => $value) {
				print '<option value="' . $oid . '">'
					  . $value . ' (' . $oid . ')' . 
					  '</option>';
			
			}
			print '</select><br><br>';
			print 'Choose the group of clients you want to assign to the operator (multiple choice is available)<br><br>
				Group: ';

			/* all groups except 'N/A' and 'Floating list' */
			getGroupNamesAsDropDownList( "groupid[]", true);
			
			print '<br><br><input type="Submit" name="submit" value="Assign the Group(s) to the Operator">
			    	</div></form></font>';
		}
	}
}

print '</body></html>';

?>