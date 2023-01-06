<?php
function preLoginDBConnect()
{
	$settings = array();
	
	$settings = get_ca_settings();
	
	$connectstr = "host=" . $settings['pghost'] 
					. " port=" . $settings['pgport'] . " dbname=" . $settings['database'] 
					. " user=" . $settings['operator'] . " password=" . $settings['operator_password'];

	$dbConnect = pg_connect($connectstr);
	if (!$dbConnect) {
		printErrorMessage('Could not connect: ' . pg_result_error( $result));
	}
	
	return $dbConnect;
}

function dbconnect()
{
	$dbLoginName = '';
	
	if( isset($_SESSION['s_username'])) {
		$dbLoginName = $_SESSION['s_username'];
	}
	
	$settings = array();
	
	$settings = get_ca_settings();
	
	if( !checkIsAdmin( $dbLoginName)) {
		$dbLoginName = $settings['operator'];
		$dbLoginPwd = $settings['operator_password'];
	}
	else {
		$dbLoginName = $settings['admin'];
		$dbLoginPwd = $settings['admin_password'];
	}

	$connectstr = "host=" . $settings['pghost'] 
					. " port=" . $settings['pgport'] . " dbname=" . $settings['database'] 
					. " user=" . $dbLoginName . " password=" . $dbLoginPwd;
	$dbConnect = pg_connect($connectstr);
	if (!$dbConnect) {
		printErrorMessage('Could not connect: ' . pg_result_error( $result));
	}
	
	return $dbConnect;
}

function dbclose( $dbConnect)
{
	pg_close($dbConnect);
}

?>