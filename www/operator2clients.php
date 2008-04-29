<?php
session_start();
require_once("login.inc");
require_once("operator.inc");
require_once("client.inc");
require_once("groups.inc");

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Assign Operator to Client", 0);

// if debug flag is set, print the following info
if ($settings['debug'] > 0) {
	require_once("functions.inc");
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
else if(isset($_POST['edit'])) {
	$clean['edit'] = $_POST['edit'];
}
else if(isset($_POST['group'])) {
	$clean['group'] = $_POST['group'];
}

if(isset($_POST['operatorid_edit'])){
	if(ctype_digit($_POST['operatorid_edit'])) {
		$clean['operatorid_edit'] = $_POST['operatorid_edit'];
	} 
}
if(isset($_POST['unassigned_only'])) {
	if(isset($_POST['unassigned_only'])) {
		switch(strtoupper($_POST['unassigned_only'])) {
			case "ON":
				$clean['unassigned_only'] = 1;
				break;
			default:
				$clean['unassigned_only'] = 0;
				break;
		}
	}
}

if(isset($_SESSION['operatorid'])){
	if(ctype_digit($_SESSION['operatorid'])) {
		$clean['operatorid'] = $_SESSION['operatorid'];
	}
}

if(isset($_POST['districtid'])){
	if(ctype_digit($_POST['districtid'])) {
		$clean['districtid'] = $_POST['districtid'];
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
 
if ($clean['submit']) {
	if ($clean['operatorid_edit']) {
		$assigned = array();
		
		//assign_client( $clean['operatorid'], ...);
		if( is_array($_POST['assigned'])) {
			$assigned = $_POST['assigned'];
		}
		
		if( addClientToOperator( $assigned, $clean['operatorid_edit'])) {
			printMessage( 'Clients assigned to operator!');
		}
		else {
			printErrorMessage( 'Error occured while assigning client to operator');
		}
		
		printMessage('<a href="' . $_SERVER['PHP_SELF'] . '">Assign client(s) to another operator</a>');
	}
}
else if( $clean['group']) {
	if ( $clean['operatorid_edit']) {
		if ( isset( $clean['groupid'])) {
			if( addGroupToOperator( $clean['groupid'], $clean['operatorid_edit'])) {
				printMessage( 'Clients assigned to operator!');
			}
			else {
				printErrorMessage( 'Error occured while assigning client to operator');
			}
			
			printMessage('<a href="' . $_SERVER['PHP_SELF'] . '">Assign client(s) to another operator</a>');
		}
	}
}
else {	// this part happens if we don't press submit
	if ( $clean['operatorid']) {
		// pull the list of operators
		$operators = array();
		$arDistricts = array();

		if( getOperators( $operators)) {
			print '<font face="Verdana, Arial, Helvetica, sans-serif" size="2">
					<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
					<div align="left">';

			print 'Choose the Operator you want to assign clients to<br><br>
				Operator: <select name="operatorid_edit">';
			
			foreach ( $operators as $oid => $value) {
				print '<option value="' . $oid . '">'
					  . $value . ' (' . $oid . ')' . 
					  '</option>';
			
			}
			print '</select><br><br>';
			print 'Choose the group of clients you want to assign to the operator<br><br>
				Group: ';

			/* all groups except 'N/A' and 'Floating list' */
			getGroupNamesAsDropDownList( "groupid", true);
			
			print '<br><br><input type="Submit" name="group" value="Assign the Group to the Operator">
			    	</div></form></font>';
		}
	}
}

print '</body></html>';

?>