<?php
require_once("db.inc");
require_once("messages.inc");
require_once("config.inc");

function printHeader( $pageTitle, $refreshRate = 0, $javaScriptFuncName = NULL) {
	$settings = array();
	$settings = get_ca_settings();
	
	print '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
			"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
			<html xmlns="http://www.w3.org/1999/xhtml">
			<head>
				<title>' . $pageTitle . ' -- '  . $settings['org'] . ' -- ' . $settings['location'] . '</title>
				<meta http-equiv="expires" content="-1">
				<meta http-equiv="Cache-Control" content="no-cache">';
	
	if( $refreshRate > 0) {
		print '<meta http-equiv="refresh" content="' . $refreshRate . '" url="http://' 
					. $HTTP_ENV_VARS["HOSTNAME"]
					. $_SERVER['PHP_SELF'] . '">';
	}
	
	if( isset( $javaScriptFuncName)) {
		$javaScriptFuncName();
	}
	
	print '</head>
			<body bgcolor="#BEC8FD">
			<font face="Verdana, Arial, Helvetica, sans-serif">
			<p>'; 
}

function getNumberOfRegisteredClients() {
	$iNumberOfClients = 0;
	$dbConnect = dbconnect();

	$sql = "SELECT COUNT(clientid) FROM clients";
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}

	$myrow = pg_fetch_row($result);
	if($myrow != FALSE) {
		$iNumberOfClients = $myrow[0];
	}
	
	dbclose( $dbConnect);
	
	return $iNumberOfClients;
}

function getNumberOfActiveClients() {
	$iNumberOfClients = 0;
	$dbConnect = dbconnect();

	$sql = "SELECT COUNT(clientid) FROM clients WHERE active='1'";
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}

	$myrow = pg_fetch_row($result);
	if($myrow != FALSE) {
		$iNumberOfClients = $myrow[0];
	}
	
	dbclose( $dbConnect);
	
	return $iNumberOfClients;
}

?>