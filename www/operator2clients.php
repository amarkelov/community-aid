<?php
session_start();
require 'functions.inc';

$clean = array();
$mysql = array();

$settings = get_gmp_settings();

print '<html><head>
		<title>Assign Operator to Client --  Friendly Call Service -- ' . $settings['location'] . '</title>
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

if(isset($_POST['submit'])) {
	$clean['submit'] = $_POST['submit'];
}
else if(isset($_POST['edit'])) {
	$clean['edit'] = $_POST['edit'];
}
if(isset($_POST['operatorid_edit'])){
	$clean['operatorid_edit'] = $_POST['operatorid_edit']; 
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
		//assign_client( $clean['operatorid'], ...);
		if( is_array($_POST['assigned'])) {
			$assigned = array();
			$assigned = $_POST['assigned'];

			if( addClientToOperator( $assigned, $clean)) {
				print '<b>Clients assigned to operator!</b><p>';
						
			}
			else {
				print '<b><font color="#FF0000">Error occured while assigning client to operator!</font></b><p>';
			}
			
			print '<a href="' . $_SERVER['PHP_SELF'] . '">Assign client(s) to another operator</a><p>';
		}
	}
}
else if( $clean['edit']) {
	if ( $clean['operatorid_edit']) {
		$aActiveClients = array();
		$aAssignedClients = array();
		$aOperatorNames = array();
		
		if( getClientsForOperator( $clean['operatorid_edit'], $aAssignedClients)) {
			if( getActiveClients( $aActiveClients)) {	
				print '<b>You are now assigning clients for operator: ' . getOperatorName( $clean['operatorid_edit']) . '</b>
						<p>To assign client to operator, use the checkboxes to the left from
						the client names. Checked checkbox means the client will be assigned
						to the operator.<br>When finished, press "Submit" button.</p>';
			
				print '<div align="left"><form method="post" action="' . $_SERVER['PHP_SELF'] . '">
						<table frame="border" rules="rows" width="50%">';
				
				print '<tr>
						<td></td>
						<td><b>Client ID</b></td>
						<td><b>Client Name</b></td>
						<td><b>Client\'s timeslot</b></td>
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
							<td width="10%" valign="top" align="center">' . getClientTimeSlot( $cid) . '</td>';
					
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
					print '</td>';
				}
				
				
				print '<input type="hidden" name="operatorid_edit" value="' . $clean['operatorid_edit'] . '" />
						</table>
						<font face="Verdana, Arial, Helvetica, sans-serif">
						<input type="Submit" name="submit" value="Submit">
						</font>
						</div>
						</form>';
			}			
		}
	}
}
else {	// this part happens if we don't press submit
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
					<input type="Submit" name="edit" value="Assign clients">
					</font>
					</div></td></tr>';
			
			print '</table></form>';
		}
	}
}

print '</body></html>';

?>