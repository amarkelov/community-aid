<?php
require_once("db.inc");
require_once("messages.inc");

function addGroup( $arNewGroups) {
	$retval = true;
	
	if( count( $arNewGroups) > 0) {
		$dbConnect = dbconnect();
		
		$sql = 'BEGIN;';
		
		foreach( $arNewGroups as $group) {
			$sql .= "INSERT INTO groups (group_name) VALUES ('" . trim( $group) . "');";
		}
		
		$sql .= "COMMIT";
		
		$result = pg_query( $dbConnect, $sql);
		if ( !$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
			printErrorMessage($message);
			$retval = false;
		}
		
		dbclose( $dbConnect);
	}
	
	return $retval;
}

function updateGroupName( $groupid, $new_group_name) {
	$retval = true;
	
	if( $groupid > 0 && isset( $new_group_name)) {
		$dbConnect = dbconnect();
		
		$sql = "UPDATE groups SET group_name='" . trim( $new_group_name) 
				. "' WHERE groupid=" . $groupid;
		
		$result = pg_query( $dbConnect, $sql);
		if ( !$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
			printErrorMessage($message);
			$retval = false;
		}
		
		dbclose( $dbConnect);
	}
	
	return $retval;
}

function getGroupName( $groupid)
{
	$row = array();
	$dbConnect = dbconnect();
	
	$sql = "SELECT group_name FROM groups WHERE groupid=" . $groupid;

	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
	else {
		$row = pg_fetch_array($result);
	}
	
	dbclose( $dbConnect);
	
	return $row['group_name'];
}

function getGroupNames( &$arGroupNames, $bNoFloatingList = false)
{
	$myrow = '';
	$dbConnect = dbconnect();
	
	$sql = "SELECT groupid,group_name FROM groups";
	
	if( $bNoFloatingList) {
		$sql .= " WHERE groupid > 1";
	}
	
	$sql .= " ORDER BY groupid";
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
		$retval = false;
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			if(isset( $myrow['groupid'])) {
				if( ctype_digit($myrow['groupid'])) { 
					$arGroupNames[$myrow['groupid']]= htmlentities($myrow['group_name'], ENT_QUOTES);
				}
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $result;
}

function getGroupNamesAsDropDownList( $selectName = "groupid", $bNoFloatingList = false)
{
	$arGroupNames = array();
	
	if( getGroupNames( $arGroupNames, $bNoFloatingList)) {
		print '<select name="' . $selectName . '" onchange="document.forms[0].groupid_cb.checked=true;" multiple>';
		
		foreach( $arGroupNames as $gid => $group_name) {
			print '<option value="' . $gid . '">' . $group_name . '</option>';
		}
		
		print '</select>';
	}
}

function printGroupClients( $groupid) {
	$retval = true;
	$dbConnect = dbconnect();

	$sql = "SELECT clientid,firstname,lastname,TO_CHAR(timeslot, 'HH24:MI')
			 AS timeslot,districts.district_name FROM clients LEFT JOIN districts 
			 ON clients.districtid=districts.districtid 
			 WHERE groupid=" . pg_escape_string( $groupid) . " ORDER BY lastname";

	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
	else {
		print '<tr>
			<td><b>Client ID</b></td>
			<td><b>Client Name</b></td>
			<td><b>District</b></td>
			<td align="center"><b>Client\'s timeslot</b></td>
			</tr>';		
		
		while ( $row = pg_fetch_array($result)) {
			if(isset( $row['clientid'])) {
				$out .= '<tr>';
				$out .= '<td>' . htmlentities($row['clientid'], ENT_QUOTES) . '</td>';
				$out .= '<td>' . htmlentities($row['lastname'], ENT_QUOTES) . ', '
							   . htmlentities($row['firstname'], ENT_QUOTES) . '</td>';
				$out .= '<td>' . htmlentities($row['district_name'], ENT_QUOTES) . '</td>';
				$out .= '<td align="center">' . htmlentities($row['timeslot'], ENT_QUOTES) . '</td>';
				$out .= '</tr>';
			} 
		}
		
		print $out;
	}
	
	dbclose( $dbConnect);
	
	return $retval;
}

function addGroupToOperator( $groupid, $operatorid) {
	$retval = true;
	$dbConnect = dbconnect();
	
	$sql = "BEGIN;";
	$sql .= "DELETE FROM group2operator WHERE 
				operatorid=" . pg_escape_string( $operatorid) . ";";

	foreach( $groupid as $gid) {
		$sql .= "INSERT INTO group2operator ( groupid, operatorid) " . 
					" VALUES ("	. pg_escape_string( $gid) .  ", " . pg_escape_string( $operatorid) . ");";
	}
	
	$sql .= "COMMIT";

	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
	
	dbclose( $dbConnect);
	
	return $retval;
}

function moveClientsToGroup( $from_groupid, $to_groupid) {
	$retval = true;
	$dbConnect = dbconnect();

	$sql = "UPDATE clients SET groupid=" . pg_escape_string( $to_groupid) . 
			" WHERE groupid=" . pg_escape_string( $from_groupid);

	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
			
	dbclose( $dbConnect);
	
	return $retval;
}

?>