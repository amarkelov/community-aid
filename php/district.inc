<?php
require_once("db.inc");
require_once("messages.inc");

function addDistrict( $arNewDistricts) {
	$retval = true;
	
	if( count( $arNewDistricts) > 0) {
		$dbConnect = dbconnect();
		
		$sql = 'BEGIN;';
		
		foreach( $arNewDistricts as $district) {
			$sql .= "INSERT INTO districts (district_name) 
						VALUES ('" . pg_escape_string(trim( $district)) . "');";
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

function updateDistrictName( $districtid, $new_district_name) {
	$retval = true;
	
	if( $districtid > 0 && isset( $new_district_name)) {
		$dbConnect = dbconnect();
		
		$sql = "UPDATE districts SET district_name='" . trim( $new_district_name) 
				. "' WHERE districtid=" . $districtid;
		
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

function getDistrictList( &$arDistricts, $getNullOne = false)
{
	$myrow = '';
	$dbConnect = dbconnect();
	
	if( $getNullOne) {
		$sql = "SELECT districtid,district_name FROM districts";
	}
	else {
		$sql = "SELECT districtid,district_name FROM districts WHERE districtid > 0";
	}
	
	$sql .= ' ORDER BY district_name';
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			if(isset( $myrow['districtid'])) {
				if( ctype_digit($myrow['districtid'])) { 
					$arDistricts[$myrow['districtid']]= htmlentities($myrow['district_name'], ENT_QUOTES);
				}
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $result;
}

function getDistrictName( $districtid)
{
	$row = array();
	$dbConnect = dbconnect();
	
	$sql = "SELECT district_name FROM districts WHERE districtid=" . $districtid;

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
	
	return $row['district_name'];
}

?>