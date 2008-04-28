<?php
require_once("db.inc");
require_once("messages.inc");

function checkIsSnr( $operatorid){
	$retval = false;
	
	$dbConnect = dbconnect();
	
	// let's see who is the senior operator at the moment
	$sql = "SELECT operatorid FROM operators WHERE issnr='t'";
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			if(isset( $myrow['operatorid'])) {
				if( ctype_digit($myrow['operatorid'])) { 
					if( $operatorid == $myrow['operatorid']){
						$retval = true;
					}
					else {
						$retval = false;
					}
				}
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $retval;
}

function transferClientToSnrOperator( $clientid) {
	$retval = true;
	$myrow = array();
	$snrID = -1;
	
	$snrID = getSnrOperatorID();
	
	$dbConnect = dbconnect();
	
	// we are not going to change anything if $snrID is not available
	if( $snrID > 0) {
		$sql = "DELETE FROM client2operator WHERE clientid=" . $clientid;
		$result = pg_query( $dbConnect, $sql);
		if ( !$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
			printErrorMessage( $message); 
			$retval = false;
		}
	
		$sql = 'INSERT INTO client2operator VALUES ('. $clientid . ','. $snrID . ')';
		
		$result = pg_query( $dbConnect, $sql);
		if ( !$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
			printErrorMessage( $message);
			$retval = false;
		}
	}
	
	dbclose( $dbConnect);
	
	return $retval;
}

function getSnrOperatorID() {
	$snrID = -1;
	
	$dbConnect = dbconnect();
	
	// let's see who is the senior operator at the moment
	$sql = "SELECT operatorid FROM operators WHERE issnr='t'";
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		$retval = false;
	}
	else {
		$myrow = pg_fetch_array($result);
		if(isset( $myrow['operatorid'])) {
			if( ctype_digit($myrow['operatorid'])) { 
				$snrID = $myrow['operatorid'];
			}
		} 
	}
	dbclose( $dbConnect);
	
	return $snrID;
}

function setSnrOperator( $operatorid) {
	$retval = true;
	
	$dbConnect = dbconnect();

	$sql = "UPDATE operators SET issnr='t' WHERE operatorid=" . $operatorid;
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
	
	dbclose( $dbConnect);
	
	return $retval;
}

function clearSnrOperator( $operatorid) {
	$retval = true;
	
	$dbConnect = dbconnect();

	$sql = "UPDATE operators SET issnr='f' WHERE operatorid=" . $operatorid;
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
	
	dbclose( $dbConnect);
	
	return $retval;
}

function addOperator( &$clean) {
	$retval = true;
	$snrID = -1;
	
	$snrID = getSnrOperatorID();
	
	$dbConnect = dbconnect();
	
	$sql = "SELECT * FROM operators WHERE loginname='" . $clean['loginname'] . "'";
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = pg_last_error( $dbConnect) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
	
	$row = pg_fetch_array($result);
	if(isset($row['loginname'])) {
		printErrorMessage( 'Operator with name ' . $row['loginname'] . ' already exists on the system. 
							Please use a different login name');
		$retval = false;
	}
	else {
		$sql = sprintf("INSERT INTO operators (loginname,fullname,saltypwd,addedby,modifiedby,isadmin,issnr)
				VALUES ('%s', '%s', MD5('%s'), %d, %d, '%s', '%s')", 
				pg_escape_string( $clean['loginname']),
				pg_escape_string( $clean['fullname']), 
				pg_escape_string( $clean['password'] . getTheSalt()),
				pg_escape_string( $clean['operatorid']),
				pg_escape_string( $clean['operatorid']),
				pg_escape_string( $clean['isadmin']),
				pg_escape_string( $clean['issnr']));
				
		$result = pg_query( $dbConnect, $sql);
		if ( !$result) {
			$message  = pg_last_error( $dbConnect) . '<br>' . 'Query: ' . $sql;
			printErrorMessage( $message);
			$retval = false;
		}
	}

	dbclose( $dbConnect);
	
	return $retval;
}

function updateOperator( &$clean) {
	$snrID = -1;
	$retval = true;
	
	$snrID = getSnrOperatorID();
	
	$dbConnect = dbconnect();
	
	$sql = "UPDATE operators SET loginname='" . $clean['loginname'] . "',
			fullname='" . $clean['fullname'] . "',";
	
	if( strlen($clean['password']) > 0) {
		$sql .= "saltypwd=MD5('" . $clean['password'] . getTheSalt() . "'),";
	}
	
	$sql .= "isadmin='" . $clean['isadmin'] . "'," .
			"issnr='" . $clean['issnr'] . "',
			modified=NOW(),
			modifiedby=" . $clean['operatorid'] . " 
			WHERE operatorid=" . $clean['operatorid_edit'];

	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}

	dbclose( $dbConnect);
	
	// check if there is no more Senior Operator on the system
	// and make Administrator the Senior by default
	if( $clean['issnr'] == 'f' and $snrID == $clean['operatorid_edit']) {
		// set admin (operatorid=1) as Senior
		setSnrOperator(1);
	}
	else {
		// we need to clear current Senior
		if( $snrID > 0 && $snrID != $clean['operatorid_edit']) {
			clearSnrOperator($snrID);
		}
	}
	
	return $retval;
}

function deleteOperator( &$clean) {
	$retval = true;
	
	if( $clean['operatorid_delete'] != 1) {
		$loginname = getOperatorLoginName($clean['operatorid_delete']);
		if( ctype_alnum($loginname)) {
			$clean['loginname_delete'] = $loginname;
		}
		 
		$dbConnect = dbconnect();
		
		$sql = "DELETE FROM operators WHERE loginname='" . $clean['loginname_delete'] . "'";
				
		$result = pg_query( $dbConnect, $sql);
		if ( !$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
			printErrorMessage($message);
			$retval = false;
		}
		
		dbclose( $dbConnect);
	}
	else {
		print '<b>You cannot delete default System Administrator!</b><p>
				<a href="' . $_SERVER['PHP_SELF'] . '">Delete another operator</a><p>';
		$retval = false;	
	}
	
	return $retval;
}

function isOperatorSnr( $operatorid) {
	$myrow = '';
	$issnr = false;
	
	$dbConnect = dbconnect();

	$sql = "SELECT issnr FROM operators WHERE operatorid='" . $operatorid . "'";
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			if( $myrow['issnr'] == 't') {
				$issnr = true;
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $issnr;
}

function isOperatorAdmin( $operatorid) {
	$myrow = '';
	$isadmin = false;
	
	$dbConnect = dbconnect();

	$sql = "SELECT isadmin FROM operators WHERE operatorid='" . $operatorid . "'";
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			if( $myrow['isadmin'] == 't') {
				$isadmin = true;
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $isadmin;
}

function getOperators( &$operators) {
	$myrow = '';
	$dbConnect = dbconnect();

	$sql = "SELECT operatorid,fullname FROM operators WHERE deleted='f' ORDER BY operatorid";
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			if(isset( $myrow['operatorid'])) {
				if( ctype_digit($myrow['operatorid'])) { 
					$operators[$myrow['operatorid']]= htmlentities($myrow['fullname'], ENT_QUOTES);
				}
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $result;
}

function getOperatorName( $operatorid) {
	$myrow = '';
	$sOperatorName = '';
	
	$dbConnect = dbconnect();

	$sql = "SELECT fullname FROM operators WHERE operatorid='" . $operatorid . "'";
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			if(isset( $myrow['fullname'])) {
				$sOperatorName = $myrow['fullname'];
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $sOperatorName;
}

function getOperatorNameAssignedToClient( $clientid, &$aOperatorNames) {
	$myrow = '';

	if( !ctype_digit( trim($clientid))) {
		return false;
	}
	
	$dbConnect = dbconnect();

	$sql = "SELECT loginname FROM operators WHERE operatorid IN 
			(SELECT operatorid FROM client2operator WHERE clientid='" . $clientid . "');";
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			if(isset( $myrow['loginname'])) {
				$aOperatorNames[ $myrow['loginname']] = $clientid;
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return true;
}

function getOperatorLoginName( $operatorid) {
	$myrow = '';
	$sOperatorName = '';
	
	$dbConnect = dbconnect();

	$sql = "SELECT loginname FROM operators WHERE operatorid='" . $operatorid . "'";
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			if(isset( $myrow['loginname'])) {
				$sOperatorName = $myrow['loginname'];
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $sOperatorName;
}

function getOperatorPassword( $operatorid) {
	$myrow = '';
	$sOperatorPwd = '';
	
	$dbConnect = dbconnect();

	$sql = "SELECT saltypwd FROM operators WHERE operatorid='" . $operatorid . "'";
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			if(isset( $myrow['saltypwd'])) {
				$sOperatorPwd = $myrow['saltypwd'];
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $sOperatorPwd;
}

function verifyOperatorData( &$arOperatorData, &$clean) {
	$clean['submit'] = $arOperatorData['submit'];

	if(isset($arOperatorData['loginname'])) {
		if( ctype_alnum( $arOperatorData['loginname'])) {
			$clean['loginname'] = $arOperatorData['loginname'];
		}
	}
	if(isset($arOperatorData['fullname'])) {
		if( ctype_print($arOperatorData['fullname'])) {
			$fullname = htmlentities( $arOperatorData['fullname'], ENT_NOQUOTES);
			if( get_magic_quotes_gpc()) {
				$clean['fullname'] = stripslashes($fullname);
			}
			else {
				$clean['fullname'] = $fullname;
			} 
		}
	}
	if(isset($arOperatorData['password'])){
		$clean['password'] = $arOperatorData['password']; 
	}
	if(isset($arOperatorData['isadmin'])) {
		if(strtoupper($arOperatorData['isadmin']) == "ON") {
			$clean['isadmin'] = 't';
		}
		else {
			$clean['isadmin'] = 'f';
		}
	}
	else {
		$clean['isadmin'] = 'f';
	}
	
	if(isset($arOperatorData['issnr'])) {
		if(strtoupper($arOperatorData['issnr']) == "ON") {
			$clean['issnr'] = 't';
		}
		else {
			$clean['issnr'] = 'f';
		}
	}
	else {
		$clean['issnr'] = 'f';
	}
}

function addClientToOperator( &$arAssigned, $operatorid) {
	$retval = true;
	$dbConnect = dbconnect();
	
	$sql = "BEGIN;";
	$sql .= "DELETE FROM client2operator WHERE operatorid=" . $operatorid . ";";
	
	foreach($arAssigned as $cid => $value) {
		$sql .= 'INSERT INTO client2operator VALUES ';
		$sql .= '('. $cid . ',' . pg_escape_string( $operatorid) . ');';
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
?>