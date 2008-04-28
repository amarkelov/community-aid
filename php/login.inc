<?php
require_once("db.inc");
require_once("messages.inc");
require_once("header.inc");

//THIS NEEDS TO BE SET
function getTheSalt(){
	return "salt";
}

function salt($passwd){
	return md5($passwd . getTheSalt());
}

function checkIsAdmin( $loginname){
	if($loginname == "") {
		return false;
	}
	else{
		$dbConnect = preLoginDBConnect();
		$sql = "SELECT isadmin FROM operators WHERE loginname='" . $loginname . "'";

		$result = pg_query( $dbConnect, $sql);

		if ( !$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
			printErrorMessage($message);
			return false;
		}
		
		$data = pg_fetch_array( $result);
		if( $data['isadmin'] == 't'){
		  	return true;
		}

		dbclose($dbConnect);
	}
	
	return false;
}

/* 
 * $page Page calling this method
 * $check4Admin shoule be either true, false or null;
 * returns true or false. True idicates the page should stop executing after this method
 *
 */
function displayLogin($page, $check4Admin, $quiet = FALSE){
	$settings = array();
	$settings = get_ca_settings();
	
	if (!isset($page)){
		$page = $settings['start_page'];
	}

	$out  = '<font face="verdana, arial, helvetica" size="2">';
	$out .= '<table width="100%" border="0" cellpadding="2"><tr>';

	if(!isset($_SESSION['s_username'])){   //we haven't logged in yet   
		$out .= '<br />	<div align="left">';
		
		// check if login had failed due to wrong user name or password
		if( array_key_exists('f', $_GET)) {
			$out .= '<b><font color="#f11313">User name or password is incorrect!</font></b>';
		}
		
		$out .= '<form id="login.php" method="post" action="login.php?f=' . $page . '">
		<table>
		<tr>
		<td>Username: </td>
		<td><input name="loginname" type="text" id="loginname" size="20" maxlength="64" /><br/></td>
		</tr>
		<tr>
		<td>Password: </td>
		<td><input name="password" type="password" size="20" maxlength="64" /><br/></td>
		</tr>
		</table>
		<input name="Submit" type="submit" value="Login" />
		</form></div></tr></td></table></body></html>';
		print $out;
		return true; //we need to exit code
	}
	else {  // we have logged in 
		if($check4Admin and !checkIsAdmin($_SESSION['s_username'])){
			//if logged in, need to check for admin and IS NOT admin 
			$out .= '<div align=left>Sorry you do not have sufficent priviledges to access this page<br /><br/>
					<a href="' . $settings['start_page'] . '">Click here to return to main page</a></div></td></tr></table>';
			print $out;
			return true;
		}
		else {
			if( !$quiet) {
				$out .= '<td  valign="top"><div align="left">
				Logged in as <b>' . $_SESSION['s_username'] .'</b>. 
				<a href="logout.php?f=' . $page . '">logout</a>';
				
				// display link back to main menu only if we are not on the main page already
				if( ltrim($_SERVER['PHP_SELF'], '/') != $settings['start_page']) {
					$out .= '<br><br><a href="' . $settings['start_page'] . '">Click here to return to main menu</a>';
				}	
				
				$out .= '</div></td>';
			}
		}
	}
	
	if( !$quiet) {
		$out .= '<td><div align="right">
				<a href="' . $settings['start_page'] . '"><img src="/images/header.png" border="0" alt="Friendly Call Service"></a>';
		$out .= '<br>';
		$out .= '<br><b>Registered clients todate:</b> ' . getNumberOfRegisteredClients();
		$out .= '<br><b>Active clients today:</b> ' . getNumberOfActiveClients();
		$out .= '</div></td></tr></table></font>';
		
		print $out;
	}
}

function login( $loginname, $password) {
	if(isset($password)) {
		if (isset($loginname)) {
			$dbConnect = preLoginDBConnect();
			
			$sql = "SELECT operatorid,loginname,fullname,saltypwd,deleted FROM operators" .
				" WHERE loginname='" . $loginname . "'";

			$result = pg_query( $dbConnect, $sql);
			if ( !$result) {
			    $message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
			    $message .= 'Whole query: ' . $sql;
			    printErrorMessage($message);
			}
				
			$data = pg_fetch_array( $result);
			
			pg_free_result( $result); 

			if ( $data['saltypwd'] !== salt($password) || $data['deleted'] == 't') { 
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

?>