<?php
session_start();
require 'functions.inc';

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Assign Operator to Client", 0);

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
else if(isset($_POST['edit'])) {
	$clean['edit'] = $_POST['edit'];
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
else if( $clean['edit']) {
	if ( $clean['operatorid_edit']) {
		$aActiveClients = array();
		$aAssignedClients = array();
		$aOperatorNames = array();
		$aClientToDistrict = array();
		
		if( getActiveClients( $aActiveClients, $clean['districtid'], $clean['unassigned_only'])) {
			if( count( $aActiveClients) > 0) {
				getClientToDistrictArray( &$aClientToDistrict, $clean['districtid'], $clean['unassigned_only']);
			
				if( getClientsForOperator( $clean['operatorid_edit'], $clean['districtid'], $aAssignedClients)) {
					print '<b>You are now assigning clients for operator: ' . getOperatorName( $clean['operatorid_edit']) . '</b>
							<p><font face="Verdana, Arial, Helvetica, sans-serif" size="2">
							To assign client to operator, use the checkboxes to the left from
							the client names. Checked checkbox means the client will be assigned
							to the operator. When finished, press "Submit" button.</font></p>';
				
					print '<div align="left"><form method="post" action="' . $_SERVER['PHP_SELF'] . '">
							<font face="Verdana, Arial, Helvetica, sans-serif" size="2">
							<table frame="border" rules="rows" width="50%">';
					
					print '<tr>
							<td></td>
							<td><b>Client ID</b></td>
							<td><b>Client Name</b></td>
							<td align="center"><b>District</b></td>
							<td align="center"><b>Client\'s timeslot</b></td>
							<td align="center"><b>Client assigned to</b></td>
						   </tr>';
					
					foreach( $aActiveClients as $cid => $value) {
						print '<tr><td witdth="10%" valign="top">
							    <input type="checkbox" name="assigned['. $cid . ']"';
						if(isset($aAssignedClients[$cid])) {
							print 'checked';
						}
						print '></input></td>
								<td width="10%" valign="top">' . $cid . '</td>
								<td width="30%" valign="top">' . strtoupper( $value) . '</td>
								<td width="10%" valign="top" align="center">' . $aClientToDistrict[$cid] . '</td>
								<td width="30%" valign="top" align="center">' . getClientTimeSlot( $cid) . '</td>';
						
						print '<td width="100%" valign="top" align="center">';
						getOperatorNameAssignedToClient( $cid, &$aOperatorNames);
						
						if( count($aOperatorNames)) {
							
							foreach( $aOperatorNames as $operator_name => $cid) {
								print $operator_name . "<br>";
							}
							// clean it up
							$aOperatorNames = array();
						}
						else {
							print '&nbsp;';
						}
						print '</td></tr>';
					}
					
					
					print '<input type="hidden" name="operatorid_edit" value="' . $clean['operatorid_edit'] . '" />
							</table>
							<p>
							<input type="Submit" name="submit" value="Submit">
							</font>
							</div>
							</form>';
				}
			}
			else { // there is no unassigned clients
				printMessage('There is no unassigned clients from ' 
								. getDistrictName( $clean['districtid']) 
								. ' area on the system at the moment.');
				printMessage('<p><a href="' . $_SERVER['PHP_SELF'] . '">Assign client(s) to another operator</a></p>');
			}
		}
	}
}
else if( $clean['group']) {
	if ( $clean['operatorid_edit']) {
		if ( isset( $clean['groupid'])) {
			if( addGroupToOperator( $clean['operatorid_edit'], $clean['groupid']) {
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
			print '</select>';

			print '<br><br><hr noshade><br>';
			
			print 'You can filter out clients by different criteria and press "Assign Client" button
					to see the list and choose clients you want to assign to the selected operator<br><br>';
			print 'District:</td><td>';
					
			if( getDistrictList( $arDistricts)) {
				print '<select name="districtid">';
				print '<option value="0">All districts</option>';
				foreach( $arDistricts as $did => $district_name) {
						print '<option value="' . $did . '">' . $district_name . '</option>';
				}
				print '</select>';
			}
			print '<br><input type="checkbox" name="unassigned_only">Show unassigned clients only</input>
					<br><br><input type="Submit" name="edit" value="Assign clients">';

			print '<hr noshade><br>';
			print 'Or you can quickly assign clients, who belong to the chosen group, to selected operator 
				by pressing "Assign the Group to the Operator"<br><br>
				Group: ';

			getGroupNamesAsDropDownList();
			
			print '<br><br><input type="Submit" name="group" value="Assign the Group to the Operator">
			    	</div></form></font>';
		}
	}
}

print '</body></html>';

?>