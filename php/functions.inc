<?php
define(CONFIG_FILE, "../conf/community-aid.ini");

define(ACTIVE_CLIENTS, 2);
define(INACTIVE_CLIENTS, 4);
define(ACTIVE_UNASSIGNED_CLIENTS, 6);
define(ALL_CLIENTS, 8);

$last_dow = 6;

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
	
	print '</head><body bgcolor="#BEC8FD"><font face="Verdana, Arial, Helvetica, sans-serif"><p>'; 
}

function printMessage( $message) {
	print '<p><b><font face="Verdana, Arial, Helvetica, sans-serif" size="2">' . 
		$message . '</font></b></p>';
}

function printErrorMessage( $message) {
	print '<p><b><font face="Verdana, Arial, Helvetica, sans-serif" size="2" color="#FF0000">' . 
		$message . '</font></b></p>';
}

//THIS NEEDS TO BE SET
function getTheSalt(){
	return "salt";
}

function salt($passwd){
	return md5($passwd . getTheSalt());
}

function checkValues($fname, $uname, $pass, $pass2){
	$isOK = false;
	if($fname ^ $uname ^ $pass ^ $pass2 == ""){
				messagebox(false, "Adding User",
			 	"You have left a required field blank!<br /> Please fill it in an submit again.");
			}elseif($pass != $pass2){
				messagebox(false, "Adding User", "Password fields to not match. Please Re-enter.");
			}elseif(strlen($pass) <= 5){
				messagebox(false, "Adding User", "Password is too short.<br /> It must be greater than 5 characters");
			}else{
			$isOK=true;
			}
	return $isOK;
}

function checkIsAdmin( $loginname){
	if($loginname == ""){
		return false;
	}else{
		$dbConnect = preLoginDBConnect();
		$sql = "SELECT isadmin FROM operators WHERE loginname='" . $loginname . "'";

		$result = pg_query( $dbConnect, $sql);

		if ( !$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
			printErrorMessage($message);
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

function print_debug( $clean, $settings) {
// if debug flag is set, print the following info
	$s = array();
	$s = get_ca_settings();
	if($s['debug'] >0){
		print "<b>\$_POST:</b><br>";
		print_r( $_POST);
		print "<p>";

		print "<b>\$_GET:</b><br>";
		print_r( $_GET);
		print "<p>";

		print "<b>\$clean:</b><br>";
		print_r( $clean);
		print "<p>";

		print "<b>\$settings:</b><br>";
		print_r( $settings);
		print "<p>";

		print "<b>\$_SERVER:</b><br>";
		print_r( $_SERVER);
		print "<p>";

		print "<b>\$_SESSION:</b><br>";
		print_r( $_SESSION);
		print "<p>";

/*		print "<b>\$GLOBALS:</b><br>";
		print_r( $GLOBALS);
		print "<p>";*/
	}
}
	
function get_ca_settings(){
	$settings = array();
	
	$settings = parse_ini_file(CONFIG_FILE);
	
	if(!isset($settings['force_pdf_when_more_than'])) {
		$settings['force_pdf_when_more_than'] = 300;	
	}

	if(!isset($settings['start_page'])) {
		$settings['start_page'] = 'community-aid.php';	
	}
	
	if(isset($_SERVER['Operator'])) {
		$settings['operator'] = $_SERVER['Operator'];
	}
	else {
		$settings['operator'] = 'caoperator';
	}
	
	if(isset($_SERVER['Operator_Password'])) {
		$settings['operator_password'] = $_SERVER['Operator_Password'];
	}
	else {
		$settings['operator_password'] = 'caoperatorpwd';
	}
	
	if(isset($_SERVER['Admin'])) {
		$settings['admin'] = $_SERVER['Admin'];
	}
	else {
		$settings['admin'] = 'caadmin';
	}
	
	if(isset($_SERVER['Admin_Password'])) {
		$settings['admin_password'] = $_SERVER['Admin_Password'];
	}
	else {
		$settings['admin_password'] = 'caadminpwd';
	}
	
	if(!isset($settings['pghost'])) {
		$settings['pghost'] = 'localhost';
	}
	
	return $settings;
}

function saveIniFile( $clean, $setDefaults = false) {
	$retval = true;
	$inifile = CONFIG_FILE;
	$newConfig = '';

	// we need to reset everything back to default values
	if($setDefaults) {
		$newConfig = "
org = Friendly Call Service\t\t; header option for organisation name
location = Blanchardstown\t\t; header option for project location
start_page=community-aid.php\t\t; identifies system's main page

;****************************
; Database related parameters
;****************************
database = community-aid\t\t\t; name of the database
pghost = localhost\t\t\t; host name of your database server
pgport = 5432\t\t\t; port number the database server listens to

;**************************
; PDF parameters
;**************************
pdf_draw_cell_border = 1\t\t; set to 1, if you need cell borders
\t\t\t\t\t; in the PDF report table

force_pdf_when_more_than = 100\t\t; force PDF output, if report results
\t\t\t\t\t; in more than n number of records
;***************************
; debug parameters
;***************************
debug = 0\t\t; debug set to 1 gives debug output at the top of each page.
debug_pdf = 0\t; debug_pdf set to 1 gives debug output at the bottom of
\t\t\t; PDF report.
\t\t\t; when PDF is chosen for report output, make sure that
\t\t\t; debug = 0. otherwise, you will have error message
\t\t\t; 'FPDF error: Some data has already been
\t\t\t; output to browser, can't send PDF file' and won't see
\t\t\t; the PDF report

;debug_sql_limit = 150\t; if you need a limited output, to save yourself
\t\t\t; some time, set this to the number of records
\t\t\t; you want to limit SELECT statement to.\n";	
	}
	else {
		if(isset( $clean['org']) && ctype_print( $clean['org'])) {
			$newConfig .= "org = " . $clean['org'] . "\n";
		}
		if(isset( $clean['location']) && ctype_print( $clean['location'])) {
			$newConfig .= "location = " . $clean['location'] . "\n";
		}
		if(isset( $clean['start_page']) && ctype_print( $clean['start_page'])) {
			$newConfig .= "start_page = " . $clean['start_page'] . "\n";
		}
		if(isset( $clean['database']) && ctype_print( $clean['database'])) {
			$newConfig .= "database = " . $clean['database'] . "\n";
		}
		if(isset( $clean['pghost']) && ctype_print( $clean['pghost'])) {
			$newConfig .= "pghost = " . $clean['pghost'] . "\n";
		}
		if(isset( $clean['pgport']) && ctype_digit( $clean['pgport'])) {
			$newConfig .= "pgport = " . $clean['pgport'] . "\n";
		}
		if(isset( $clean['pdf_draw_cell_border']) && ctype_digit( $clean['pdf_draw_cell_border'])) {
			if($clean['pdf_draw_cell_border'] > 0) {
				$newConfig .= "pdf_draw_cell_border = 1\n";
			}
			else {
				$newConfig .= "pdf_draw_cell_border = 0\n";
			}
		}
		if(isset( $clean['force_pdf_when_more_than']) && ctype_digit( $clean['force_pdf_when_more_than'])) {
			$newConfig .= "force_pdf_when_more_than = " . $clean['force_pdf_when_more_than'] . "\n";
		}
		if(isset( $clean['debug']) && ctype_digit( $clean['debug'])) {
			if($clean['debug'] > 0) {
				$newConfig .= "debug = 1\n";
			}
			else {
				$newConfig .= "debug = 0\n";
			}
		}
		if(isset( $clean['debug_pdf']) && ctype_digit( $clean['debug_pdf'])) {
			if( $clean['debug_pdf'] > 0) {
				$newConfig .= "debug_pdf = 1\n";
			}
			else {
				$newConfig .= "debug_pdf = 0\n";
			}
		}
		if(isset( $clean['debug_sql_limit']) && ctype_digit( $clean['debug_sql_limit'])) {
			$newConfig .= "debug_sql_limit = " . $clean['debug_sql_limit'] . "\n";
		}
	}
	
	// Let's make sure the file exists and is writable first.
	if (is_writable( $inifile)) {
	    if ( !$fh = fopen( $inifile, 'w')) {
	         printErrorMessage('Cannot open file ' . $inifile . '!');
	         $retval = false;
	    }
	
	    // Write $somecontent to our opened file.
	    if ( fwrite( $fh, $newConfig) === FALSE) {
	        printErrorMessage('Cannot write to file ' . $inifile . '!');
	        $retval = false;
	    }

	    fclose( $fh);
	
	} else {
	    printErrorMessage('File ' . $inifile . ' is not writable!');
	    $retval = false;
	}
	
	return $retval;
}

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

function draw_calls( $clientid) 
{
	$current_week_number = 0;
	$last_week_number = 0;
	
	$dbConnect = dbconnect();
	
    $sql  = "SELECT calls.time as time,
				calls.chat as chat,
				calls.class as class,
				operators.loginname as operator FROM calls, operators
				WHERE clientid={$clientid} AND 
				time > (NOW() - INTERVAL '3 WEEK') AND 
				calls.operatorid=operators.operatorid 
				ORDER BY callid DESC ";
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
    }

    print ("<hr noshade>");
    print('<table border="0" width="100%" cellspacing="0" cellpadding="0">');
    $i=0; // need this to change cell bgcolor
	$firstround = 1;
    while ($myrow = pg_fetch_array($result)) {
// select sclass short name
        $class_ = $myrow["class"];
		$sql  = "SELECT sclass_sname FROM call_sclass WHERE";
		$sql .= " sclass_id='$class_'";
		$res = pg_query( $dbConnect, $sql);
		if (!$res) {
		    $message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		    $message .= 'Whole query: ' . $sql;
		    printErrorMessage($message);
		}
		$class_row = pg_fetch_row($res);
// select sclass short name (end)
	
		$i=$i + 1; // need this to change cell bgcolor
		$time=$myrow["time"];
	
		$regs = 0;
		if ( ereg( "([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})", $time, $regs ) ) {
		    $out="$regs[3]/$regs[2]/$regs[1] $regs[4]:$regs[5]";
		    $current_week_number = date("W",mktime(0,0,0,$regs[2],$regs[3],$regs[1]));
		}
		else {
		    $out="Invalid date format: $time";
		}


		if( $firstround) {
			$last_week_number = $current_week_number;
			$firstround = 0;
		}
		
		if($current_week_number < $last_week_number) {
			print '<tr><td>&nbsp</td></tr>';
			print '<tr><td><font size="1"><b>Week ' . $current_week_number . '</font></td></tr>';
			$last_week_number = $current_week_number;
		}
		
		if($i % 2) {
		    print('<tr bgcolor="#FFFFFF">');
		}
		else {
		    print('<tr bgcolor="#DDDDDD">');
		}
	
	// time column
		print('<td width="20%"> <font size="1" color="#FF0000">');
		print $out;
		print('</font>');
	// class column
		print('</td><td width="10%"><font size="1" color="#00FF00">');
		
		if(!empty($class_row[0])) { 
			print("<a href=\"class.php\" onClick=\"window.open('class.php','','status=yes,toolbar=no,location=no,width=400,height=500');return false;\">$class_row[0]</a>");
		}
		else {
		    print("&nbsp");
		}	
		print('</font>');
	// chat column
		print('</td><td><font size="1" color="#0000FF">');
		
		if(!empty($myrow["chat"])) { 
		    print($myrow["chat"] );
		}
		else {
		    print("&nbsp");
		}
	// operator
		print('</td><td width="10%"><font size="1" color="#0000FF">');
		
		if(!empty($myrow["operator"])) { 
		    print($myrow["operator"] );
		}
		else {
		    print("&nbsp");
		}
		    
		print("</td></tr>");

    } // while
    
    print("</table>");

	dbclose($dbConnect);
}

function draw_classification()
{
	print '<select name="mclass" size="1">';
	
	$dbConnect = dbconnect();

    $sql  = "SELECT m.mclass_id,s.sclass_id,m.mclass_name,s.sclass_name";
    $sql .= " FROM call_mclass AS m,call_sclass AS s";
    $sql .= " WHERE m.mclass_id=s.mclass_id ORDER BY m.mclass_id";
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
    }
    while( $class = pg_fetch_array($result)) {
		printf("<option value=\"%d\">%s -- %s</option>",$class[sclass_id],$class[mclass_name],$class[sclass_name]);
    }
    
    dbclose($dbConnect);
    
    print '</select>';
}

// this is for drop-down list of clients
function draw_clients()
{
	$dbConnect = dbconnect();
	
    $sql = "SELECT clientid,lastname,firstname FROM clients ORDER BY lastname";
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
    }
    while( $client = pg_fetch_array($result)) {
		printf("<option value=\"%d\">%s, %s (%d)</option>",
	    $client[clientid],strtoupper($client[lastname]),
	    strtoupper($client[firstname]),$client[clientid]);
    }
    
    dbclose($dbConnect);
}

function draw_client_details($clientid)
{
    $fs = 2; // font size
    $regs = 0;

	$dbConnect = dbconnect();

    if( $clientid and $dbConnect) {
		$sql  = "SELECT phone1,phone2,alone,dob,medical_notes,alerts,area,
					contact1name,contact1phone,
					contact2name,contact2phone,
					gpname,
					TO_CHAR(timeslot, 'HH24:MI') as timeslot from clients
					WHERE clientid='$clientid' AND active='1'";
		
		$result = pg_query( $dbConnect, $sql);
		if (!$result) {
		    $message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		    $message .= 'Whole query: ' . $sql;
		    printErrorMessage($message);
		}
		else {	
	   	    while ($myrow = pg_fetch_array($result)) {
				if($myrow["phone1"]) {
		        	    $phone1 = $myrow["phone1"]; 
				}
				else {
				    $phone1 = "N/A";
				}
				if($myrow["phone2"]) {
		    	    	    $phone2 = $myrow["phone2"];
				}
				else {
				    $phone2 = "N/A";
				}
				if ( ereg( "([0-9]{4})-([0-9]{2})-([0-9]{2})", $myrow["dob"], $regs ) ) {
				    $dob = "$regs[3]/$regs[2]/$regs[1]";
				}
		       	else {
				    $dob = "Invalid DOB";
				} 
				$area = $myrow['area'];

				$contact1name = $myrow['contact1name'];
				$contact1phone = $myrow['contact1phone'];

				$contact2name = $myrow['contact2name'];
				if(strlen(trim($contact2name)) == 0) $contact2name = 'N/A';

				$contact2phone = $myrow['contact2phone'];
				if(strlen(trim($contact2phone)) == 0) $contact2phone = 'N/A';

				$gpname = $myrow['gpname'];
				if(strlen(trim($gpname)) == 0) $gpname = 'N/A';
				
				$timeslot = $myrow['timeslot'];
				$medical_notes = $myrow["medical_notes"];
		       	$alerts = $myrow["alerts"];
		       	$alone = $myrow["alone"];
		    }
		}
    }    

    $out  = '<table frame="border" width="100%">';
    $out .= '<tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>Phone 1:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $phone1 . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>First Contact:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact1name . "</font></td>";
    $out .= '</tr><tr>';        
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Phone 2:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $phone2 . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>First Contact phone:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact1phone . "</font></td>";
    $out .= '</tr><tr>';    
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>D.O.B. (DD/MM/YYYY):</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $dob . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>Second Contact:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact2name . "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Area:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $area . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>Second Contact phone:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact2phone . "</font></td>";
    $out .= '</tr><tr>';    
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Time slot:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $timeslot . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>GP name:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $gpname . "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Alone:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">"; 
		if( $alone = 't') {
    		$out .= "Yes";
		}
		else {
			$out .= "No";
		}
	$out .= "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Medical notes:</b></font></td>";
    $out .= "<td><font size=" . $fs .">" . $medical_notes . "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Alerts:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $alerts . "</font></td>";
    $out .= '</tr></table>';
    
    print "$out";

	dbclose($dbConnect);
}

// Draw clients list
// left part of the screen
function draw_clients_list($operator, $clientid, $floating_list=false)
{
	$clients = array();		// array to store clientids and the rest
	$calltime = array();	// array to store timeslots
	$nocalltime = array();	// array to store clientid's with no timeslot
	$redcalltime = array();	// array to store timeslots that were missed 
	$redcalltimemsg = array(); // array to hold red messages to append to timeslots
	$redpastcalltime = array();	// array to store timeslots that were missed from yesterday 
	$redpastcalltimemsg = array(); // array to hold redpast messages to append to timeslots
	$nextcall = array();	// array to store nextcalls
	$timeslot_done = array();	// to track if the timeslot call was made
	
	$dbConnect = dbconnect();
	
	$fs = 1; // font size
	
	// getting client list first ...
    $sql  = "SELECT clientid,firstname,lastname,
			TO_CHAR(timeslot, 'HH24:MI') AS timeslot
			FROM clients WHERE clientid IN (SELECT clientid FROM client2operator WHERE 
			operatorid=(SELECT operatorid FROM operators WHERE loginname='{$operator}'))";

    // add floating list to the mix
    if( $floating_list) {
    	$sql .= " OR groupid=1 ";
    }
    
    $sql .= " AND active='t' ORDER BY timeslot, lastname";
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
    }

	while ($myrow = pg_fetch_array($result)) {
		$clients[$myrow['clientid']] = $myrow['lastname'] . ', ' . $myrow['firstname'];
		$calltime[$myrow['clientid']] = $myrow['timeslot'];
	}

	// getting unfinished calls and nextcalltime for it
	$sql = "SELECT clientid,TO_CHAR(nextcalltime,'DD/MM/YYYY HH24:MI:SS') as nextcalltime
			FROM calls WHERE clientid in 
			(SELECT clientid FROM client2operator WHERE 
			operatorid=(SELECT operatorid FROM operators WHERE loginname='{$operator}')) 
			and call_finished='f'";
	
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '\n';
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
    }

	while ($myrow = pg_fetch_array($result)) {
		$nextcall[$myrow['clientid']] = $myrow['nextcalltime'];
	}
	
	// getting the timeslot_done flags for all the clients
	$sql = "SELECT clientid,timeslot_done
			FROM client_timeslot_call WHERE clientid in 
			(SELECT clientid FROM client2operator WHERE 
			operatorid=(SELECT operatorid FROM operators WHERE loginname='{$operator}'))";
	
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '\n';
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
    }

	while ($myrow = pg_fetch_array($result)) {
		$timeslot_done[$myrow['clientid']] = $myrow['timeslot_done'];
	}

	// processing now.....
	$now = time();
	$mnt = mktime(0, 0, 0, date("m"), date("d"), date("Y")); // tonight's midnight

	foreach( $calltime as $cid => $nctime) { // now go through it all and figure out the time....
		$nct = -1;
		if( isset($nextcall[$cid])) {
			if( ereg( "([0-9]{2})/([0-9]{2})/([0-9]{4}) ([0-9]{2}):([0-9]{2}):([0-9]{2})", $nextcall[$cid], $regs )) {
				$nct = mktime($regs[4],$regs[5],$regs[6],$regs[2],$regs[1],$regs[3]); // nextcalltime
			}
/*--worked			if( ereg( "([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})", $nextcall[$cid], $regs )) {
				$nct = mktime($regs[4],$regs[5],$regs[6],$regs[2],$regs[3],$regs[1]); // nextcalltime
			}---worked*/
		}

		$regs = 0;
		if( ereg( "([0-9]{2}):([0-9]{2})", $nctime, $regs )) {
			$tms = mktime($regs[1], $regs[2], 0, date("m"), date("d"), date("Y")); // time slot
		}
		else {
			$tms = 0;
		}

		if( ($mnt - $nct) > 0 && $nct > 0) { // we left unfinished call from yesterday. bad!
			$redpastcalltime[$cid] = $nextcall[$cid];
			$redpastcalltimemsg[$cid] = 'Unfinished call from the past. Call now!';
			unset( $calltime[$cid]);
		}
		else if( $tms < $now && $timeslot_done[$cid] == 'f' ) { // we missed client's usual time slot. bad!
			$redcalltime[$cid] = $nctime;
			$redcalltimemsg[$cid] = 'Missed time slot. Call now!';
			unset( $calltime[$cid]);
		}
		else if( $nct < $now && $nct > 0) {
			$redcalltime[$cid] = date( "H:i", $nct);
			$redcalltimemsg[$cid] = 'Missed call time. Call now!';
			unset( $calltime[$cid]);
		}
		else if( $nct > $now) {
			$calltime[$cid] = date( "H:i", $nct);
		}
		
/* --- worked		if( ($mnt - $nct) > 0 && $nct > 0) { // we left unfinished call from yesterday. bad!
			$calltime[$cid] = '<b><font color="#f11313">Unfinished call from the past. ' . $nextcall[$cid] . ' Call now!</font></b>';
		}
		else if( $tms < $now && $timeslot_done[$cid] == 'f' ) { // we missed client's usual time slot. bad!
			$calltime[$cid] = '<b><font color="#f11313">Missed time slot (' . $nctime . '). Call now!</font></b>';
		}
		else if( $nct < $now && $nct > 0) {
			$calltime[$cid] = '<b><font color="#f11313">Missed call time ('. date( "H:i", $nct) . '). Call now!</font></b>';
		}
		else if( $nct > $now) {
			$calltime[$cid] = date( "H:i", $nct);
		} --- worked */
		
		else if( $tms < $now) {
			$nocalltime[$cid] = '';
			unset( $calltime[$cid]);
		}
//		$calltime[$cid] .= '  --- nct: ' . date("Y-m-d H:i:s", $nct) . '; timeslot: ' . date("Y-m-d H:i:s", $tms) . '; ' . $nextcall[$cid] . '==' . $nct; 
	}
    
	// we need to sort the array now
	asort( $redcalltime);
	asort( $calltime);

	// print it all out ....
	print("<table>");

	// redpasts first
	foreach ( $redpastcalltime as $cid => $ts ) {
		$out  = '<tr><td><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="$fs">';
		$out .= $clients[$cid] . '</a></font></td>';
		$out .= '<td><b><font color="#f11313">' . $redpastcalltime[$cid] . '</font></b></td>';
		$out .= '<td><b><font color="#f11313">' . $redpastcalltimemsg[$cid] . '</font></b></td>';
		
	    print $out . '</tr>';
	}
	
	// reds then
	foreach ( $redcalltime as $cid => $ts ) {
		$out  = '<tr><td><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="$fs">';
		$out .= $clients[$cid] . '</a></font></td>';
		$out .= '<td><b><font color="#f11313">' . $redcalltime[$cid] . '</font></b></td>';
		$out .= '<td><b><font color="#f11313">' . $redcalltimemsg[$cid] . '</font></b></td>';
		
	    print $out . '</tr>';
	}
	
	// normals then
	foreach ( $calltime as $cid => $ts ) {
		$out  = '<tr><td><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="$fs">';
		$out .= $clients[$cid] . '</a></font></td>';
		$out .= '<td>' . $calltime[$cid] . '</td>';
		$out .= '<td>&nbsp</td>';
		
	    print $out . '</tr>';
	}
	
	// notimes first
	foreach ( $nocalltime as $cid => $ts ) {
		$out  = '<tr><td><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="$fs">';
		$out .= $clients[$cid] . '</a></font></td>';
		$out .= '<td>' . $nocalltime[$cid] . '</td>';
		$out .= '<td>&nbsp</td>';
		
	    print $out . '</tr>';
	}
	
/* --- worked	foreach ( $clients as $cid => $name ) {
		$out  = '<tr><td><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="$fs">';
		$out .= $name . '</a></font></td>';
		$out .= '<td>' . $calltime[$cid] . '</td>';
	    
	    print $out . '</tr>';
	}--- worked */

	print("</table>");

	dbclose($dbConnect);
}

function filter_phone_number($phone) {
	$filtered = ereg_replace( " |-", "", $phone);
	if(!ctype_digit($filtered)) {
		$filtered = "N/A";	
	}
	return $filtered;
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

function verifyClientData( &$arClientData, &$clean) {
	$magic = get_magic_quotes_gpc();

	if(isset($arClientData['firstname'])) {
		$firstname = htmlentities(strtoupper($arClientData['firstname']), ENT_QUOTES );
		if( $magic) {
			$clean['firstname'] = stripslashes( $firstname);
		}
		else {
			$clean['firstname'] = $firstname;
		}
	}
	
	if(isset($arClientData['lastname'])) {
		$lastname = htmlentities(strtoupper($arClientData['lastname']), ENT_NOQUOTES );
		if( $magic) {
			$clean['lastname'] = stripslashes( $lastname);
		}
		else {
			$clean['lastname'] = $lastname;
		}
	}
	
	if(isset($arClientData['title'])) {
		$title =  htmlentities($arClientData['title'], ENT_QUOTES );
		if( $magic) {
			$clean['title'] = stripslashes( $title);
		}
		else {
			$clean['title'] = $title;
		}
	}
	
	if(isset($arClientData['gender'])) {
		if(ctype_print($arClientData['gender'])) {
			$clean['gender'] = htmlentities($arClientData['gender'], ENT_QUOTES );
		}
	}
	
	if(isset($arClientData['address'])) {
		if(ctype_print($arClientData['address'])) {
			$address = htmlentities($arClientData['address'], ENT_NOQUOTES );
			if( $magic) {
				$clean['address'] = stripslashes( $address);
			}
			else {
				$clean['address'] = $address;
			}
		}
	}
	
	if(isset($arClientData['area'])) {
		if(ctype_print($arClientData['area'])) {
			$area = htmlentities($arClientData['area'], ENT_NOQUOTES );
			if( $magic) {
				$clean['area'] = stripslashes( $area);
			}
			else {
				$clean['area'] = $area;
			}
		}
	}
	
	if(isset($arClientData['districtid'])) {
		if(ctype_digit($arClientData['districtid'])) {
			$clean['districtid'] = $arClientData['districtid'];
		}
	}
	
	if(isset($arClientData['phone1'])) {
		$clean['phone1'] = filter_phone_number($arClientData['phone1']);
	}
	
	if(isset($arClientData['phone2'])) {
		$clean['phone2'] = filter_phone_number($arClientData['phone2']);
	}
	
	if(isset($arClientData['groupid'])) {
		if(ctype_digit($arClientData['groupid'])) {
			$clean['groupid'] = $arClientData['groupid'];
		}
	}
	
	if(isset($arClientData['dob'])) {
		$reg = 0;
		if(ereg( "([0-3]{1}[0-9]{1})/([0-1]{1}[0-9]{1})/([1-2]{1}[0-9]{3})", $arClientData['dob'], $reg)) {
			$clean['dob'] = $reg[3] . "-" . $reg[2] . "-" . $reg[1];
		}
	}
	
	if(isset($arClientData['gpname'])) {
		if(ctype_print($arClientData['gpname'])) {
			$gpname = htmlentities($arClientData['gpname'], ENT_NOQUOTES );
			if( $magic) {
				$clean['gpname'] = stripslashes( $gpname);
			}
			else {
				$clean['gpname'] = $gpname;
			}
		}
	}
	
	if(isset($arClientData['housetype'])) {
		if(ctype_print($arClientData['housetype'])) {
			$housetype = htmlentities($arClientData['housetype'], ENT_QUOTES );
			if( $magic) {
				$clean['housetype'] = stripslashes( $housetype);
			}
			else {
				$clean['housetype'] = $housetype;
			}
		}
	}
	
	if(isset($arClientData['alone'])) {
		if(strtoupper($arClientData['alone']) == "ON") {
			$clean['alone'] = 't';
		}
		else {
			$clean['alone'] = 'f';
		}
	}
	else {
		$clean['alone'] = 'f';
	}
	
	if(isset($arClientData['medical_notes'])) {
		if(ctype_print($arClientData['medical_notes'])) {
			$medical_notes = htmlentities($arClientData['medical_notes'], ENT_NOQUOTES );
			if( $magic) {
				$clean['medical_notes'] = stripslashes( $medical_notes);
			}
			else {
				$clean['medical_notes'] = $medical_notes;
			}
		}
	}
	
	if(isset($arClientData['alerts'])) {
		if(ctype_print($arClientData['alerts'])) {
			$alerts = htmlentities($arClientData['alerts'], ENT_NOQUOTES );
			if( $magic) {
				$clean['alerts'] = stripslashes( $alerts);
			}
			else {
				$clean['alerts'] = $alerts;
			}
		}
	}
	
	if(isset($arClientData['referrer'])) {
		if( ctype_print($arClientData['referrer'])) {
			$referrer = htmlentities($arClientData['referrer'], ENT_NOQUOTES );
			if( $magic) {
				$clean['referrer'] = stripslashes($referrer);
			}
			else {
				$clean['referrer'] = $referrer;
			}
		}
	}
	
	if(isset($arClientData['referrer_other'])) {
		if( ctype_print($arClientData['referrer_other'])) {
			$referrer = htmlentities($arClientData['referrer_other'], ENT_NOQUOTES );
			if( $magic) {
				$clean['referrer'] = stripslashes($referrer);
			}
			else {
				$clean['referrer'] = $referrer;
			}
		}
	}
	
	if(isset($arClientData['contact1name'])) {
		if(ctype_print($arClientData['contact1name'])) {
			$contact1name = htmlentities($arClientData['contact1name'], ENT_NOQUOTES );
			if( $magic) {
				$clean['contact1name'] = stripslashes($contact1name);
			}
			else {
				$clean['contact1name'] = $contact1name;
			}
		}
	}
	
	if(isset($arClientData['contact1relationship'])) {
		if(ctype_print($arClientData['contact1relationship'])) {
			$contact1relationship = htmlentities($arClientData['contact1relationship'], ENT_QUOTES );
			if( $magic) {
				$clean['contact1relationship'] = stripslashes( $contact1relationship);
			}
			else {
				$clean['contact1relationship'] = $contact1relationship;
			}
		}
	}
	
	if(isset($arClientData['contact1address'])) {
		if(ctype_print($arClientData['contact1address'])) {
			$contact1address = htmlentities($arClientData['contact1address'], ENT_NOQUOTES );
			if( $magic) {
				$clean['contact1address'] = stripslashes( $contact1address);
			}
			else {
				$clean['contact1address'] = $contact1address;
			}
		}
	}
	
	if(isset($arClientData['contact1phone'])) {
		$clean['contact1phone'] = filter_phone_number($arClientData['contact1phone']);
	}
	
	
	if(isset($arClientData['contact2name'])) {
		if(ctype_print($arClientData['contact2name'])) {
			$contact2name = htmlentities($arClientData['contact2name'], ENT_NOQUOTES );
			if( $magic) {
				$clean['contact2name'] = stripslashes($contact2name);
			}
			else {
				$clean['contact2name'] = $contact2name;
			}
		}
	}
	
	if(isset($arClientData['contact2relationship'])) {
		if(ctype_print($arClientData['contact2relationship'])) {
			$contact2relationship = htmlentities($arClientData['contact2relationship'], ENT_QUOTES );
			if( $magic) {
				$clean['contact2relationship'] = stripslashes( $contact2relationship);
			}
			else {
				$clean['contact2relationship'] = $contact2relationship;
			}
		}
	}
	
	if(isset($arClientData['contact2address'])) {
		if(ctype_print($arClientData['contact2address'])) {
			$contact2address = htmlentities($arClientData['contact2address'], ENT_NOQUOTES );
			if( $magic) {
				$clean['contact2address'] = stripslashes( $contact2address);
			}
			else {
				$clean['contact2address'] = $contact2address;
			}
		}
	}
		
	if(isset($arClientData['contact2phone'])) {
		$clean['contact2phone'] = filter_phone_number($arClientData['contact2phone']);
	}
	
	if(isset($arClientData['timeslot'])){
		$clean['timeslot'] = verify_timeslot($arClientData['timeslot']);
	}
}

function verify_timeslot($timeslot)
{
	$ts = "00:00";	
	$vt = explode(":", $timeslot, 2);

if(ctype_digit($vt[0]) && ctype_digit($vt[1])) {
	if(($vt[0]>=0 && $vt[0]<=23) && ($vt[1]>=0 && $vt[1]<=59)){
			$ts = "$vt[0]:$vt[1]";
		}
	}
	
	return $ts; 
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

function getClientsForOperator( $operatorid, $districtid, &$clients) {
	$myrow = '';
	$dbConnect = dbconnect();

	$sql = 'SELECT clients.clientid,clients.firstname,clients.lastname,clients.initials 
			FROM clients LEFT JOIN client2operator ON clients.clientid=client2operator.clientid 
			WHERE client2operator.operatorid=' . $operatorid . ' ';


	if( ctype_digit($districtid) && $districtid > 0) {
		$sql .= ' AND clients.districtid=' . $districtid . ' ';  
	}
	
	$sql .= ' ORDER BY clients.lastname, clients.firstname';

	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			$clients[$myrow['clientid']] = $myrow['lastname'] . ', ' . $myrow['firstname'] . ' ' . $myrow['initials'];
		}
	}
	
	dbclose( $dbConnect);

	return $result;
}

function getClientTimeSlot( $clientid) {
	$myrow = '';
	$timeslot = '&nbsp;';

	if( !ctype_digit( trim($clientid))) {
		return 'Bad client ID';
	}
	
	$dbConnect = dbconnect();

	$sql = "SELECT TO_CHAR(timeslot, 'HH24:MI') as timeslot FROM clients WHERE clientid=" . $clientid;  

	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			$timeslot = $myrow['timeslot'];
		}
	}
	
	dbclose( $dbConnect);
	
	return $timeslot;
}

function getClients( &$clients, $districtid = 0, $what_clients = ALL_CLIENTS) {
	$myrow = '';
	$dbConnect = dbconnect();
	
	$sql = "SELECT clientid,firstname,lastname,initials FROM clients ";

	// if none of the below, we get all clients
	switch( $what_clients) {
	case ACTIVE_CLIENTS:
			$sql .= " WHERE active='t'";
			break;
	case INACTIVE_CLIENTS:
			$sql .= " WHERE active='f'";
			break;
	case ACTIVE_UNASSIGNED_CLIENTS:
			$sql .= " WHERE clientid NOT IN (SELECT clientid FROM client2operator)";
			break;
	}

	// if $districtid=0, we want to see all active clients
	if( ctype_digit($districtid) && $districtid > 0) {
		$sql .= " AND districtid=" . $districtid . " ";  
	}
	
	$sql .= " ORDER BY lastname";
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			if(isset( $myrow['clientid'])) {
				if( ctype_digit($myrow['clientid'])) { 
					$clients[$myrow['clientid']]= htmlentities($myrow['lastname'], ENT_QUOTES) . ', ' .
											htmlentities($myrow['firstname'], ENT_QUOTES) . ' ' .
											htmlentities($myrow['initials'], ENT_QUOTES) . ' ';
				}
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $result;
}

function getActiveClients( &$clients, $districtid = 0, $unassigned_only = 0) {
	if( $unassigned_only == 1 ) {
		return getClients( $clients, $districtid, ACTIVE_UNASSIGNED_CLIENTS);
	}
	else {
		return getClients( $clients, $districtid, ACTIVE_CLIENTS);
	}
}

function getInactiveClients( &$clients, $districtid = 0, $unassigned_only = 0) {
	return getClients( $clients, $districtid, INACTIVE_CLIENTS);
}

function getAllClients( &$clients) {
	return getClients( $clients, 0, ALL_CLIENTS);
}

function deactivate_client( $clientid) {
	$dbConnect = dbconnect();

	$sql = "UPDATE clients SET active='0' WHERE clientid='" . $clientid . "'";
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		printErrorMessage('Invalid query: ' . pg_result_error( $result));
	}
	
	dbclose( $dbConnect);
}

function reactivate_client( $clientid) {
	$dbConnect = dbconnect();

	$sql = "UPDATE clients SET active='1' WHERE clientid='" . $clientid . "'";
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		printErrorMessage('Invalid query: ' . pg_result_error( $result));
	}
	
	dbclose( $dbConnect);
}

function printCallsJavaScript() {
	print '<script type="text/javascript">
		<!-- hide it from old browsers or from those with JavaScript disabled
		function vtslot(time, form)
		{
			var ts = Array();
			var ts = time.value.split(":");
			var hour = Number(ts[0]);
			var min = Number(ts[1]);

			// if both hours and minutes are numeric and within the range => true		
			if(!isNaN(hour) && !isNaN(min)) {
			   if((hour>=0 && hour<=23) && (min>=0 && min<=59)) {
			      return true;
			   }
			}
			
			// if the call is finished => true
			if(form.call_finished.checked) {
				return true;
			}

			// otherwise throw error message
			msg = "Time has to be of 24 hours format (HH:MM)!\n";
			alert( msg);
			return false;
		}
		-->
		</script>';
}

function printAddEditClientJavaScript() 
{
	print '
		<script language="JavaScript1.1">
		<!-- hide it from old browsers or from those with JavaScript disabled
		function vtslot(s)
		{
		   var ts = Array();
		   var ts = s.value.split(":");
		   var hour = Number(ts[0]);
		   var min = Number(ts[1]);

		   if(!isNaN(hour) && !isNaN(min)) {
		      if((hour>=0 && hour<=23) && (min>=0 && min<=59)) {
		         return true;
		      }
		   }
		   alert("Time has to be of 24 hours format (HH:MM)!");
		   return false;
		}
		
		function vphone(phone)
		{
			var result = phone.value.match(/\d+/g);

			if( result != null) {
				return true;
			}
			else {
				if(phone.optional) {
					return true;
				}
				else {
					alert("Phone number should consist of digits only!");
					return false;
				}
			}
		}

		function vdob(dob)
		{
			var msg = "";
			var today = new Date();
			var checked_dob = Array();
			checked_dob = dob.value.split("/");
			
			var day = Number(checked_dob[0]);
			var month = Number(checked_dob[1]);
			var year = Number(checked_dob[2]);

			if(!isNaN(day) && !isNaN(month) && !isNaN(year)) {
				if((day >= 1 && day <= 31) && 
					(month >= 1 && month <= 12) && 
					(year >= 1900 && year <= (today.getYear()+1900))) {
				   return true;
				}
				else {
					msg  = "Day range: 1-31\n";
					msg += "Month range: 1-12\n";
					msg += "Year range: 1900 - ";
					msg += (today.getYear()+1900);
				}
			}
			alert("Date of birth must be of format DD/MM/YYYY!\n\n" + msg);
			return false;
		}
		
		function isEmpty(text, field_name)
		{
			if( text.value.length == 0) {
				alert( field_name + " can not be empty!");
				return false;
			}
			return true;
		}
		
		// A utility function that returns true if a string contains only 
		// whitespace characters
		function isblank(s) {
		    for(var i = 0; i < s.length; i++) {
		        var c = s.charAt(i);
		        if ((c != " ") && (c != "\n") && (c != "\t")) return false;
		    }
		    return true;
		}
		
		function verify(f)
		{
			var msg = "Please, fill all fields marked with red asterisk.\nThe fields are mandatory!\n";
			var empty_fields = "";
			var errors = "";
			var ret;
		
		    // Loop through the elements of the form, looking for all Text and 
		    // Textarea elements that do not have an "optional" property defined. 
		    // Then check for fields that are empty and make a list of them.
		    // Put together error messages for fields that are wrong.
		    for(var i = 0; i < f.length; i++) {
		        var e = f.elements[i];
		        if (((e.type == "text") || (e.type == "textarea")) && !e.optional) {
		            // First check if the field is empty
		            if ((e.value == null) || (e.value == "") || isblank(e.value)) {
		                empty_fields += "\n          " + e.name;
		                continue;
		            }
		        }
		    }

			if( !vdob(f.dob)) {
				errors += "Date of birth format: DD/MM/YYYY\n";
			}

			if( !vphone(f.phone1) || !vphone(f.contact1phone)) {
				errors += "Phone numbers must consist of digits only!\n";
			}

			if(!vphone(f.phone2) && !f.phone2.optional) {
				errors += "Phone numbers must consist of digits only!\n";
			}

			// Now, if there were any errors, display the messages, and
		    // return false to prevent the form from being submitted. 
		    // Otherwise, return true.
		    if (!empty_fields && !errors) return true;
		
		    msg += "______________________________________________________\n\n"
		
		    if (empty_fields) {
		        msg += "- The following required field(s) are empty:" 
		                + empty_fields + "\n";
		        if (errors) msg += "\n";
		    }
		    msg += errors;
		    alert(msg);
		    return false;
		}
		
		-->
		</script>';

}

function printAddEditClientTable( $clean, $edit = 0)
{
	$arDistricts = array(); // array to populate district list
	
	print'<table width="100%" border="0" cellspacing="4" cellpadding="2">
	<tr>
		<td bgcolor="#EEEEEE" colspan="2">
			<font face="verdana, arial, helvetica" size="-1">
			<b>Personal Details</b><br>
			</font>
		</td>
	</tr>
	
	<br>
	<br>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right" width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>First name: </b>
		</font>
		<input type="Text" name="firstname" value="' . $clean['firstname'] . '" size="30" maxlength="30">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Date of Birth: </b>
		</font>
		<input type="Text" name="dob" onchange="return vdob(this);" value="';
		    
	if($clean['dob']) {
		if( ereg( "([1-2]{1}[0-9]{3})-([0-1]{1}[0-9]{1})-([0-9]{2})", $clean['dob'], $reg)) {
			print("$reg[3]/$reg[2]/$reg[1]");
		}
		else {
			print("bad dob");
		}
	}
	
	print '" size="11">
		<font size="1" color="#FF0000">
		<b>* Format DD/MM/YYYY</b>
		</font>
		</td>
	
		<td ALIGN="right" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right" width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Last name: </b>
		</font>
		<input type="Text" name="lastname" value="' . $clean['lastname'] . '" size="30" maxlength="30">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Alone: </b>
		</font>
		<input type="checkbox" name="alone" size="5" maxlength="5"';
				
	if($clean['alone']) {
		print(" checked");
	}
	
	print '>
		</td>
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Address: </b>
		</font>
		<input type="Text" name="address" value="' . $clean['address'] . '" size="30" maxlength="30">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>

		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>';
		
		print '<input type="radio" name="gender" value="female" ';
		if($clean['gender'] == 'female') {
			print 'checked';
		}
		print '>Female</input>';
		
		print '<input type="radio" name="gender" value="male" ';
		if($clean['gender'] == 'male') {
			print 'checked ';
		}
		print '>Male</input>';
		
		print '</b>
		</font>
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Area: </b>
		</font>
		<input type="Text" name="area" value="' . $clean['area'] . '" size="25">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Alerts: </b><br>
		</font>
		<textarea name="alerts" cols="27" rows="4" >' . $clean['alerts'] . '</textarea>
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>District: </b>
		</font>';
			
		if( getDistrictList( $arDistricts)) {
			print '<select name="districtid">';
			
			foreach( $arDistricts as $did => $district_name) {
				if($clean['districtid'] == $did) {
					print '<option value="' . $did . '" selected>' . $district_name . '</option>';
				}
				else {
					print '<option value="' . $did . '">' . $district_name . '</option>';
				}
			}
			
			print '</select>';
		}
		print '<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>House Type: </b>
		</font>
		<select name="housetype">
			<option value="apartment">Apartment</option>
			<option value="bungalow">Bungalow</option>
			<option value="cottage flat">Country House / Cottage</option>
			<option value="detached">Detached House</option>
			<option value="farm">Farm</option>
			<option value="holiday">Holiday Home</option>
			<option value="other">Other</option>
			<option value="semi-detached">Semi-Detached House</option>
			<option value="terraced">Terraced House</option>
			<option value="' . $clean['housetype'] . '" selected>' . $clean['housetype'] . '</option>
		</select>
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Phone Number 1 <font size="1" color="#FF0000">(digits only!)</font>: </b>
		</font>
		<input type="Text" name="phone1" onchange="return vphone(this);" value="' . $clean['phone1'] . '" size="15">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Phone Number 2 <font size="1" color="#FF0000">(digits only!)</font>: </b>
		</font>
		<input type="text" name="phone2" onchange="return vphone(this);" size="14" value="' . $clean['phone2'] . '">
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Group: </b>
		</font>';

		if( getGroupNames( $arGroupNames)) {
			print '<select name="groupid">';
			
			foreach( $arGroupNames as $gid => $group_name) {
				if($clean['groupid'] == $gid) {
					print '<option value="' . $gid . '" selected>' . $group_name . '</option>';
				}
				else {
					print '<option value="' . $gid . '">' . $group_name . '</option>';
				}
			}
			
			print '</select>';
		}

		print '
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%" VALIGN="top">
		</td>
		
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	<table width="100%" border="0" cellspacing="4" cellpadding="2">
	<tr>
		<td bgcolor="#EEEEEE" colspan="2">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Medical Details</b><br>
		</font>
		</td>
	</tr>
	</table>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Referrer: </b>
		</font>
		<select name="referrer">
			<option value="Health Board">Health Board</option>
			<option value="Social Work">Social Work</option>
			<option value="Private">Private</option>
			<option value="Voluntary Group">Voluntary Group</option>
			<option value="Self-Referred">Self Referred</option>
			<option selected value="' . $clean['referrer'] . '">' . $clean['referrer'] . '</option>
		</select>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>GP name: </b>
		</font>
		<input type="text" name="gpname" maxlength="30" size="30" value="' . $clean['gpname'] . '">
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Other: </b>
		</font>
		<input type="text" name="referrer_other" size="25" value="' . $clean['referrer_other'] . '" maxlength="40">
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Medical notes: </b><br>
		</font>
		<textarea name="medical_notes" cols="27" rows="4" >' . $clean['medical_notes'] . '</textarea>
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	<table width="100%" border="0" cellspacing="4" cellpadding="2">
	<tr>
		<td bgcolor="#EEEEEE" colspan="2">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Contact Details</b><br>
		</font>
		</td>
	</tr>
	</table>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Contact name 1: </b>
		</font>
		<input type="text" name="contact1name" value="' . $clean['contact1name'] . '" size="22" maxlength="30">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Phone <font size="1" color="#FF0000">(digits only!)</font>: </b>
		</font>
		<input type="text" name="contact1phone" onchange="return vphone(this);" size="15" maxlength="15" value="' . $clean['contact1phone'] . '">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Address: </b>
		</font>
		<input type="text" name="contact1address" size="32" maxlength="50" value="' . $clean['contact1address'] . '">
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Relationship: </b>
		</font>
		<input type="text" name="contact1relationship" size="20" value="' . $clean['contact1relationship'] . '">
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top"></td>
	</tr>
	</TABLE>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Contact name 2: </b>
		</font>
		<input type="text" name="contact2name" value="' . $clean['contact2name'] . '" size="22" maxlength="30">
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Phone <font size="1" color="#FF0000">(digits only!)</font>: </b>
		</font>
		<input type="text" name="contact2phone" onchange="return vphone(this);" size="15" maxlength="15" value="' . $clean['contact2phone'] . '">
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Address: </b>
		</font>
		<input type="text" name="contact2address" size="32" maxlength="50" value="' . $clean['contact2address'] . '">
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Relationship: </b>
		</font>
		<input type="text" name="contact2relationship" size="20" value="' . $clean['contact2relationship'] . '">
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	<table width="100%" border="0" cellspacing="4" cellpadding="2">
	<tr>
		<td bgcolor="#EEEEEE" colspan="2">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Time of the first call on a day</b><br>
		</font>
		</td>
	</tr>
	</table>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="center"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Time slot: </b>
		</font>
		<input type="text" name="timeslot" onchange="return vtslot(this);" 
			value="' . $clean['timeslot'] . '" size="6" maxlength="5">
		<font size="1" color="#FF0000">
		<b>* 24 hour format (HH:MM)</b>
		</font>
		</input>
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>';

	if( $edit) {
		print '<table width="100%" border="0" cellspacing="4" cellpadding="2">
		<tr>
			<td bgcolor="#EEEEEE" colspan="2">
			<font face="verdana, arial, helvetica" size="-1">
			<b>Why did you edit the client record?</b><br>
			</font>
			</td>
		</tr>
		</table>
		
		<TABLE BORDER=0 WIDTH=100%>
		<tr>
			<td ALIGN="right" width="15%" VALIGN="top">
			</td>
		
			<td ALIGN="left"  width="30%" VALIGN="top">
			<font face="verdana, arial, helvetica" size="-1">
			<b>Change note: </b>
			</font>
			<font size="1" color="#FF0000">
			<b>*</b>
			</font>
			<textarea name="changenote" cols="100" rows="2" ></textarea>
			</td>
		
			<td ALIGN="right" width="10%" VALIGN="top">
			</td>
		</tr>
		</TABLE>';
	}		
}

function addDistrict( $arNewDistricts) {
	$retval = true;
	
	if( count( $arNewDistricts) > 0) {
		$dbConnect = dbconnect();
		
		$sql = 'BEGIN;';
		
		foreach( $arNewDistricts as $district) {
			$sql .= "INSERT INTO districts (district_name) VALUES ('" . trim( $district) . "');";
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
	
	if( $bNoServiceGroups) {
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

function getGroupNamesAsDropDownList( $bNoFloatingList = false)
{
	if( getGroupNames( $arGroupNames, $bNoFloatingList)) {
		print '<select name="groupid">';
		
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

function addGroupToOperator( $groupid, $operatorid) {
	$retval = true;
	$arClientid = array();
	$dbConnect = dbconnect();
	
	$sql = "SELECT clientid FROM clients WHERE groupid=" . pg_escape_string( $groupid);

	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
	while ( $myrow = pg_fetch_array($result)) {
		if(isset( $myrow['clientid'])) {
			if( ctype_digit($myrow['clientid'])) { 
				$arClientid[] = $myrow['clientid'];
			}
		} 
	}
	
	$sql = "BEGIN;";
	$sql .= "DELETE FROM client2operator WHERE clientid IN 
			(SELECT clientid FROM clients WHERE groupid=" . pg_escape_string( $groupid) . ");";
	
	foreach( $arClientid as $cid) {
		$sql .= "INSERT INTO client2operator ( clientid, operatorid) " . 
				" VALUES ("	. pg_escape_string( $cid) .  ", " . pg_escape_string( $operatorid) . ");";
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

function addClient(&$clean) {
	$retval = true;
	$dbConnect = dbconnect();
    
/*	$sql = sprintf("INSERT INTO clients (firstname,lastname,title,gender,address,area,districtid,
			phone1,phone2,housetype,dob,alone, medical_notes,
			contact1name,contact1relationship,contact1address,contact1phone,
			contact2name,contact2relationship, contact2address,contact2phone,
			gpname,referrer,alerts,timeslot,addedby,modifiedby, groupid)
			VALUES ('%s', '%s', '%s', '%s', '%s', '%s', %d, '%s', '%s', '%s', '%s', '%s', '%s',
			'%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s','%s', '%s', '%s', %d, %d, %d)",*/
	$sql = sprintf("SELECT add_client('%s'::varchar, '%s'::varchar, '%s'::varchar,
			'%s'::varchar, '%s'::varchar, '%s'::varchar, %d::bigint, '%s'::varchar, 
			'%s'::varchar, '%s'::varchar, '%s'::date, '%s'::boolean, '%s'::varchar,
			'%s'::varchar, '%s'::varchar, '%s'::varchar, '%s'::varchar, '%s'::varchar,
			'%s'::varchar, '%s'::varchar, '%s'::varchar, '%s'::varchar,'%s'::varchar,
			'%s'::varchar, '%s'::time, %d::bigint, %d::bigint)",  
			pg_escape_string( $clean['firstname']),
			pg_escape_string( $clean['lastname']),
			pg_escape_string( $clean['title']),
			pg_escape_string( $clean['gender']),
			pg_escape_string( $clean['address']),
			pg_escape_string( $clean['area']),
			pg_escape_string( $clean['districtid']), 
			pg_escape_string( $clean['phone1']),
			pg_escape_string( $clean['phone2']),
			pg_escape_string( $clean['housetype']),
			pg_escape_string( $clean['dob']),
			pg_escape_string( $clean['alone']),
			pg_escape_string( $clean['medical_notes']),
			pg_escape_string( $clean['contact1name']),
			pg_escape_string( $clean['contact1relationship']),
			pg_escape_string( $clean['contact1address']),
			pg_escape_string( $clean['contact1phone']),
			pg_escape_string( $clean['contact2name']),
			pg_escape_string( $clean['contact2relationship']),
			pg_escape_string( $clean['contact2address']), 
			pg_escape_string( $clean['contact2phone']),
			pg_escape_string( $clean['gpname']),
			pg_escape_string( $clean['referrer']),
			pg_escape_string( $clean['alerts']),
			pg_escape_string( $clean['timeslot']),
			pg_escape_string( $clean['operatorid']),
/*			pg_escape_string( $clean['operatorid']),*/
			pg_escape_string( $clean['groupid']));

	// run SQL against the DB
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
/*		if( pg_errno() == 1062) {
			print "<font size=\"3\" color=\"#FF0000\"><b>Duplicate entry!</b></font>
					<p>Client with the same name and date of birth already exists on the system.<br>
					Please use Edit Client screen to change details of existing client or check that you entered
					name and date of birth of the new client correctly<p>";
			
			print  '<a href="' . $_SERVER['PHP_SELF'] . '">Add another client</a><p>';
		}
		else {*/
			$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
			printErrorMessage( $message);
			$retval = false;
//		}
	}
	
	dbclose( $dbConnect);
	
	return $retval;
}

function updateClient( &$clean) {
	$retval = true;
	$dbConnect = dbconnect();

	// here if no ID then editing  else adding
	if ( $clean['clientid']) {
	    $sql  = sprintf("UPDATE clients SET firstname='%s',
		    lastname='%s',title='%s',gender ='%s',address='%s',area='%s',
			phone1='%s',phone2='%s',dob='%s',gpname='%s',housetype='%s',
			referrer='%s',alone='%s',medical_notes='%s',alerts='%s',
			contact1name='%s',contact1relationship='%s',
		    contact1address='%s',contact1phone='%s',
			contact2name='%s',contact2relationship='%s',
		    contact2address='%s',contact2phone='%s', 
		    timeslot='%s',changenote='%s',districtid=%d,groupid=%d,modifiedby=%d WHERE clientid=%d",
			pg_escape_string( $clean['firstname']),
			pg_escape_string( $clean['lastname']),
			pg_escape_string( $clean['title']),
			pg_escape_string( $clean['gender']),
			pg_escape_string( $clean['address']),
			pg_escape_string( $clean['area']),
			pg_escape_string( $clean['phone1']),
			pg_escape_string( $clean['phone2']),
			pg_escape_string( $clean['dob']),
			pg_escape_string( $clean['gpname']),
			pg_escape_string( $clean['housetype']),
			pg_escape_string( $clean['referrer']),
			pg_escape_string( $clean['alone']),
			pg_escape_string( $clean['medical_notes']),
			pg_escape_string( $clean['alerts']),
			pg_escape_string( $clean['contact1name']),
			pg_escape_string( $clean['contact1relationship']),
			pg_escape_string( $clean['contact1address']),
			pg_escape_string( $clean['contact1phone']),
			pg_escape_string( $clean['contact2name']),
			pg_escape_string( $clean['contact2relationship']),
			pg_escape_string( $clean['contact2address']),
			pg_escape_string( $clean['contact2phone']),
			pg_escape_string( $clean['timeslot']),
			pg_escape_string( $clean['changenote']),
			pg_escape_string( $clean['districtid']),
			pg_escape_string( $clean['groupid']),
			pg_escape_string( $clean['operatorid']),
			pg_escape_string( $clean['clientid']));

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

function setFirstCallDone( $clientid) {
	$retval = true;
	
	$dbConnect = dbconnect();
	
	$sql = "UPDATE client_timeslot_call SET timeslot_done='t' WHERE clientid=" . $clientid;
	
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}

	dbclose( $dbConnect);
	
	return $retval;
}

function recordTheCall( &$clean) {
	$retval = true;
	
	$dbConnect = dbconnect();

	$sql  = sprintf("INSERT INTO calls (clientid, time, nextcalltime, chat, class, call_finished, operatorid) 
  		    VALUES ( %d, NOW(), CURRENT_DATE + TIME '%s',  '%s', %d, '%s', %d)",
			pg_escape_string( $clean['clientid']), 
			pg_escape_string( $clean['nextcalltime']),
			pg_escape_string( $clean['chat']), 
			pg_escape_string( $clean['mclass']),
			pg_escape_string( $clean['call_finished']), 
			pg_escape_string( $_SESSION['operatorid']));
	
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
	else if( $clean['call_finished'] == 't') { 
		// now we can set all unfinished calls to finished state, so the
		// calculation of the next call time for the client can be correct
		// NOTE: this is quick and dirty workaround for the moment!!!
		$sql  = "UPDATE calls SET call_finished='t' 
				WHERE clientid=" . $clean['clientid'];
		
		$result = pg_query( $dbConnect, $sql);
		if (!$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
			printErrorMessage( $message);
			$retval = false;
		}
	}  
	
	dbclose( $dbConnect);
	
	return $retval;
}

function getClientData( &$clean) {
	$retval = true;
	
	$dbConnect = dbconnect();

	$sql = "SELECT firstname,lastname,
					initials,title,gender,
					address,area,districtid,phone1,phone2,housetype,dob,alone,medical_notes,
					contact1name,contact1relationship,contact1address,contact1phone,
					contact2name,contact2relationship,contact2address,contact2phone,
					gpname,referrer,alerts,groupid,
					TO_CHAR(timeslot, 'HH24:MI') as timeslot FROM clients WHERE (clients.clientid={$clean['clientid']})";
	
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = "Invalid query: " . pg_result_error( $result) . "\n";
		$message .= "Whole query: " . $sql . "\n";
		$retval = false;
	}
	$myrow = pg_fetch_array($result);

/*
 * Cleaning input from the database
 */
	$clean['firstname']				= htmlentities($myrow['firstname'], ENT_QUOTES);
	$clean['lastname']				= htmlentities($myrow['lastname'], ENT_QUOTES);
	$clean['initials']				= htmlentities($myrow['initials'], ENT_QUOTES);
	$clean['title']					= htmlentities($myrow['title'], ENT_QUOTES);
	$clean['gender']				= htmlentities($myrow['gender'], ENT_QUOTES);
	$clean['address']				= htmlentities($myrow['address'], ENT_QUOTES);
	$clean['area']					= htmlentities($myrow['area'], ENT_QUOTES);
	$clean['districtid']			= htmlentities($myrow['districtid'], ENT_QUOTES);
	$clean['phone1']				= htmlentities($myrow['phone1'], ENT_QUOTES);
	$clean['phone2']				= htmlentities($myrow['phone2'], ENT_QUOTES);
	$clean['housetype']				= htmlentities($myrow['housetype'], ENT_QUOTES);
	$clean['dob']					= htmlentities($myrow['dob'], ENT_QUOTES);
	$clean['alone']					= htmlentities($myrow['alone'], ENT_QUOTES);
	$clean['medical_notes']			= htmlentities($myrow['medical_notes'], ENT_QUOTES);
	$clean['contact1name']			= htmlentities($myrow['contact1name'], ENT_QUOTES);
	$clean['contact1relationship']	= htmlentities($myrow['contact1relationship'], ENT_QUOTES);
	$clean['contact1address']		= htmlentities($myrow['contact1address'], ENT_QUOTES);
	$clean['contact1phone']			= htmlentities($myrow['contact1phone'], ENT_QUOTES);
	$clean['contact2name']			= htmlentities($myrow['contact2name'], ENT_QUOTES);
	$clean['contact2relationship']	= htmlentities($myrow['contact2relationship'], ENT_QUOTES);
	$clean['contact2address']		= htmlentities($myrow['contact2address'], ENT_QUOTES);
	$clean['contact2phone']			= htmlentities($myrow['contact2phone'], ENT_QUOTES);
	$clean['gpname']				= htmlentities($myrow['gpname'], ENT_QUOTES);
	$clean['referrer']				= htmlentities($myrow['referrer'], ENT_QUOTES);
	$clean['timeslot']				= htmlentities($myrow['timeslot'], ENT_QUOTES);
	$clean['alerts']				= htmlentities($myrow['alerts'], ENT_QUOTES);
	$clean['groupid']				= htmlentities($myrow['groupid'], ENT_QUOTES);
	
	dbclose($dbConnect);
}

function getClientToDistrictArray( &$aClientToDistrict, $districtid, $unassigned_only = 0) {
	$retval = true;
	
	$dbConnect = dbconnect();

	$sql = "SELECT clients.clientid,districts.district_name FROM clients, districts 
				WHERE clients.districtid=districts.districtid 
				AND clients.active='t'";
	
	if( $unassigned_only == 1) {
		$sql .= " AND clients.clientid NOT IN (SELECT clientid FROM client2operator)";
	}

	if( ctype_digit($districtid) && $districtid > 0) {
		$sql .= " AND clients.districtid=" . $districtid;  
	}
	
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = "Invalid query: " . pg_result_error( $result) . "\n";
		$message .= "Whole query: " . $sql . "\n";
		$retval = false;
	}
	
	while( $myrow = pg_fetch_array($result)) {
		$aClientToDistrict[$myrow['clientid']] = htmlentities($myrow['district_name'], ENT_QUOTES);
	}
	
	dbclose($dbConnect);
	
	return $retval;
}

function getClassificationDictionary() {
	$retval = true;
	$dbConnect = dbconnect();

	$sql  = "SELECT mc.mclass_name,sc.sclass_name,";
	$sql .= "sc.sclass_sname FROM call_mclass AS mc, ";
	$sql .= "call_sclass AS sc WHERE mc.mclass_id = sc.mclass_id";
	
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
		
	$out  = '<font face="Verdana, Arial, Helvetica, sans-serif" size="2">';
	$out .= '<table border="0" width="100%" cellspacing="0" cellpadding="0">';
	$out .= '<tr>';
	$out .= '<td width="40%"><b>Main Class name</b></font></td>';
	$out .= '<td width="40%"><b>Sub Class name</b></td>';
	$out .= '<td><small><b>Short Sub Class name</b></td>';
	$out .= '</tr>';
	
	print($out);
	
	$i=0; // need this to change cell bgcolor
	
	while ($myrow = pg_fetch_array($result)) {
	    // select sclass short name
	    $mclass_name = $myrow[0];
	    $sclass_name = $myrow[1];
	    $sclass_sname = $myrow[2];
	    
	    $i=$i + 1; // need this to change cell bgcolor
	
	    if($i % 2) {
		print('<tr bgcolor="#FFFFFF">');
	    }
	    else {
		print('<tr bgcolor="#DDDDDD">');
	    }
	
	    $out  = "<td>" . $mclass_name . "</td>";
	    $out .= "<td>" . $sclass_name . "</td>";
	    $out .= "<td>" . $sclass_sname . "</td>";
	    $out .= "</tr>";
	    
	    print($out);
	}
	
	return $retval;
}

// pdf stuff begin
require('fpdf.php');

class PDF extends FPDF
{
	var $sql_query = 0;
	var $header = 0;
	var $border = 0;
		
	//Page header
	function Header()
	{
	    //Colors, line width and bold font
	    $this->SetFillColor(17,40,200);
	    $this->SetTextColor(255);
	    $this->SetDrawColor(128,0,0);
	    $this->SetLineWidth(.3);
	    $this->SetFont('','B');
	    //Header
	    $w=array(20,20,40,105);
	    for($i=0;$i<count($this->header);$i++)
	        $this->Cell($w[$i],7,$this->header[$i],1,0,'C',1);
	    $this->Ln();
	}
	
	//Page footer
	function Footer()
	{
	    //Position at 1.5 cm from bottom
	    $this->SetY(-15);
	    //Arial italic 8
	    $this->SetFont('Arial','I',8);
	    //Page number
	    $this->Cell(0,10,'Page '.$this->PageNo().'/{nb}',0,0,'C');
	}
	
	//Colored table
	function ColoredTable($header,$query_result)
	{
	 /*   //Colors, line width and bold font
	    $this->SetFillColor(17,40,200);
	    $this->SetTextColor(255);
	    $this->SetDrawColor(128,0,0);
	    $this->SetLineWidth(.3);
	    $this->SetFont('','B');*/
	    //Header
	    $w=array(20,20,40,105);
	/*    for($i=0;$i<count($header);$i++)
	        $this->Cell($w[$i],7,$header[$i],1,0,'C',1);
	    $this->Ln();*/
	    //Color and font restoration
	    $this->SetFillColor(224,235,255);
	    $this->SetTextColor(0);
	    $this->SetFont('');
	    //Data
	    $fill=0;
	
	
	    while( $row = pg_fetch_array($query_result))
	    {
	    	// call id
	        $this->Cell($w[0],4,$row[0],$this->border,0,'L',$fill);
	        // caller id
	        $this->Cell($w[1],4,$row[1],$this->border,0,'L',$fill);
	        // date & time
	        $this->Cell($w[2],4,$row[2],$this->border,0,'C',$fill);
	        // chat
	        $this->MultiCell($w[3],4,$row[3],$this->border,'L',$fill);
	        $fill=!$fill;
	    }
	    $this->Cell(array_sum($w),0,'','T');
	}
}


// pdf ends

?>
