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
				$saltypwd = salt($_POST['password']);
			
				$dbConnect = preLoginDBConnect();
				
				$query = mysql_query("SELECT operatorid,loginname,fullname,saltypwd FROM operators".
					" WHERE loginname='$loginname'") or die("Error Logging into the Server");
					
				$data = mysql_fetch_array($query);
				
				mysql_free_result($query); 

				if ( $data['saltypwd'] != $saltypwd) { 
					$saltypwd="";
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
