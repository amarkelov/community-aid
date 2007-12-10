<?php
session_start();
require("functions.inc");

$settings = get_ca_settings();

if ($settings['debug'] > 0) {
	print_debug( $clean, $settings);
}
	
if (isset($_SESSION['s_username'])) {
	header("Location: ".$_GET['f'] ."?a"); //already logged in
}else{
	if(isset($_POST['password']))
	{
		if (isset($_POST['loginname'])) {
			$loginname = $_POST['loginname'];
		
			$dbConnect = preLoginDBConnect();
			
			$sql = "SELECT operatorid,loginname,fullname,saltypwd FROM operators" .
				" WHERE loginname='" . $loginname . "'";
			$result = pg_query( $dbConnect, $sql);
	
			if ( !$result) {
				$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
				die($message);
			}
				
			$data = pg_fetch_array( $result);
			
			pg_free_result( $result); 

			if ( $data['saltypwd'] !== salt($_POST['password'])) { 
				session_destroy();
				sleep(3);
				header("Location: " . $_GET['f']. "?f"); //failed login
				exit;
			}
			else {
				$_SESSION['s_username'] = $data['loginname'];
				$_SESSION['operatorid'] = $data['operatorid'];
				$_SESSION['fullname'] = $data['fullname'];
			}
			
			dbclose( $dbConnect);
		}
	}
}
if (isset($_GET['f']) ){
	header("Location: ".$_GET['f']);  // return to page we came from
	
}else{
	header("Location: " . $settings['start_page']); // dont know where to go!
} 
?>   
