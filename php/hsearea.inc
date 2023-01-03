<?php
require_once(dirname(__FILE__) . "/db.inc");
require_once(dirname(__FILE__) . "/messages.inc");


function getAllHSEAreas( &$arDistricts, $getNullOne = false)
{
	$myrow = '';
	$dbConnect = dbconnect();
	
	if( $getNullOne) {
		$sql = "SELECT hseareaid,hsearea_name FROM hseareas";
	}
	else {
		$sql = "SELECT hseareaid,hsearea_name FROM hseareas WHERE hseareaid > 0";
	}
	
	$sql .= ' ORDER BY hseareaid';
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			if(isset( $myrow['hseareaid'])) {
				if( ctype_digit($myrow['hseareaid'])) { 
					$arDistricts[$myrow['hseareaid']]= htmlentities($myrow['hsearea_name'], ENT_QUOTES);
				}
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $result;
}

function getClientByHSEArea( $hseareaid)
{
	$myrow = '';
	$dbConnect = dbconnect();
	$result = -1;
	$out = '';
	$count = 0;
	
	if( ctype_digit($hseareaid)) {
		$sql = "SELECT lastname,firstname,address FROM clients WHERE hseareaid=" . $hseareaid . " ORDER BY hseareaid";
	}
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			$out .= "<tr>";
			$out .= "<td>" . htmlentities($myrow['lastname'], ENT_QUOTES) . "</td>";
			$out .= "<td>" . htmlentities($myrow['firstname'], ENT_QUOTES) . "</td>";
			$out .= "<td>" . htmlentities($myrow['address'], ENT_QUOTES) . "</td>";
			$out .= "</tr>";
			$count++;
		}
				
		printMessage("Records found: " . $count);
		print "<table>" . $out . "</table>";
	}
	
	dbclose( $dbConnect);
	
	return $result = $count;
}
?>