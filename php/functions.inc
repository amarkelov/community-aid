<?php

define(CONFIG_FILE, "../www/gmp.ini");

define(ACTIVE_CLIENTS, 2);
define(INACTIVE_CLIENTS, 4);
define(ALL_CLIENTS, 8);

$last_dow = 6;


 //THIS NEEDS TO BE SET
function getTheSalt(){
	return "salt";
}

function salt($passwd){
	return sha1($passwd . getTheSalt());
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

function addUser($uname, $fname, $pass, $isadmin){
	$success = false;
	$link = dbConnect();
	if(checkIsAdmin($_SESSION['s_username'])){
			$sql = "INSERT INTO `operators` (`operatorid`, `loginname`, `fullname`,".
					" `saltypwd`, `isAdmin`) VALUES ('', '$uname', '$fname', '"
					.salt($pass)."','$isadmin')";
			$query = mysql_query($sql, $link);
			
			if (mysql_errno($link)=='0'){
				//Print Success
				$success = true;
			}else{
				//echo mysql_error($link);
			}
		}
		dbclose($link);
	
	return $success;
}

function checkIsAdmin( $loginname){
	if($loginname == ""){
		return false;
	}else{
		$link = preLoginDBConnect();
		$sql = mysql_query("SELECT isAdmin FROM operators WHERE loginname='$loginname'") 
				or die ("Error connecting to database");
		$data = mysql_fetch_array($sql);
		dbclose($link);
		if($data['isAdmin']){
		  	return true;
		}
	}
	return false;
}

function deleteUser($loginname){
	$success = false;
	if(checkIsAdmin($_SESSION['s_username'])){
		$link = dbConnect();
		$sql = mysql_query("DELETE FROM operators WHERE loginname='$loginname' LIMIT 1") or die (mysql_error($link));
		//$data = mysql_fetch_array($sql);
		if (mysql_errno($link)=='0'){
		$success = true;
		}
		dbclose($link);
	}
	return $success;
}

function updateUser($loginname, $newloginname, $newfullname, $newisadmin){
	$success = false;
	if(checkIsAdmin($_SESSION['s_username'])){
		$link = dbConnect();
		$sql = mysql_query("UPDATE operators SET loginname = '$newloginname',
							`fullname` = '$newfullname', `isAdmin` = '$newisadmin'
							WHERE `loginname` = '$loginname' LIMIT 1") 
				or die (mysql_error($link));		//not the best idea for deployment
		//$data = mysql_fetch_array($sql);
		if (mysql_errno($link)=='0'){
		$success = true;
		}
		dbclose($link);
	}
	return $success;
}


/* 
 * $page Page calling this method
 * $check4Admin shoule be either true, false or null;
 * returns true or false. True idicates the page should stop executing after this method
 *
 */
function displayLogin($page, $check4Admin){
	$settings = array();
	$settings = get_gmp_settings();
	
	if (!isset($page)){
		$page = $settings['start_page'];
	}

	$out = '<table width="100%" border="0" cellpadding="2"><tr>';

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
		echo $out;
		return true; //we need to exit code
	}
	else {  // we have logged in 
		if($check4Admin and !checkIsAdmin($_SESSION['s_username'])){
			//if logged in, need to check for admin and IS NOT admin 
			$out .= '<div align=left>Sorry you do not have sufficent priviledges to access this page<br /><br/>
					<a href=gmp.php>Click here to return to main page</a></div></td></tr></table>';
			echo $out;
			return true;
			echo "I shouldn't have got here";
		}
		else {
			$out .= '<td  valign="top"><div align="left">
			Logged in as <b>' . $_SESSION['s_username'] .'</b>. 
			<a href="logout.php?f=' . $page . '">logout</a>';
			
			// display link back to main menu only if we are not on the main page already
			if( ltrim($_SERVER['PHP_SELF'], '/') != $settings['start_page']) {
				$out .= '<br><br><a href=gmp.php>Click here to return to main menu</a>';
			}	
			
			$out .= '</div></td>';
		}
	}
	$out .= '<td><div align="right">
			<a href="/gmp.php"><img src="/images/header.png" border="0" alt="Friendly Call Service"></a>';
	$out .= '<br>';
	$out .= '<br><b>Registered clients todate:</b> ' . getNumberOfRegisteredClients();
	$out .= '<br><b>Active clients today:</b> ' . getNumberOfActiveClients();
	$out .= '</div></td></tr></table>';
	
	echo $out;
}

function getNumberOfRegisteredClients() {
	$iNumberOfClients = 0;
	$dbConnect = dbconnect();

	$sql = "SELECT COUNT(clientid) FROM clients";
	
	$result = mysql_query( $sql, $dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		die($message);
	}

	$myrow = mysql_fetch_row($result);
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
	
	$result = mysql_query( $sql, $dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		die($message);
	}

	$myrow = mysql_fetch_row($result);
	if($myrow != FALSE) {
		$iNumberOfClients = $myrow[0];
	}
	
	dbclose( $dbConnect);
	
	return $iNumberOfClients;
}

function print_debug( $clean, $settings) {
// if debug flag is set, print the following info
	$s = array();
	$s = get_gmp_settings();
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
	
function get_gmp_settings(){
	$settings = array();
	
	$settings = parse_ini_file(CONFIG_FILE);
	
	if(!isset($settings['force_pdf_when_more_than'])) {
		$settings['force_pdf_when_more_than'] = 300;	
	}

	if(!isset($settings['start_page'])) {
		$settings['start_page'] = 'gmp.php';	
	}
	
	if(isset($_SERVER['Operator'])) {
		$settings['operator'] = $_SERVER['Operator'];
	}
	else {
		$settings['operator'] = 'gmoperator';
	}
	
	if(isset($_SERVER['Operator_Password'])) {
		$settings['operator_password'] = $_SERVER['Operator_Password'];
	}
	else {
		$settings['operator_password'] = 'gmoperatorpwd';
	}
	
	if(isset($_SERVER['Admin'])) {
		$settings['admin'] = $_SERVER['Admin'];
	}
	else {
		$settings['admin'] = 'gmadmin';
	}
	
	if(isset($_SERVER['Admin_Password'])) {
		$settings['admin_password'] = $_SERVER['Admin_Password'];
	}
	else {
		$settings['admin_password'] = 'gmadminpwd';
	}
	
	if(isset($_SERVER['DbHost'])) {
		$settings['dbhost'] = $_SERVER['DbHost'];
	}
	else {
		$settings['dbhost'] = 'localhost';
	}
	
	return $settings;
}

function preLoginDBConnect()
{
	$settings = array();
	
	$settings = get_gmp_settings();
	
	$dbLoginName = $settings['operator'];
	$dbLoginPwd = $settings['operator_password'];
		
	$dbConnect = mysql_connect($settings['dbhost'], $dbLoginName, $dbLoginPwd);
	if (!$dbConnect) {
		die('Could not connect: ' . mysql_error());
	}
	
	//mysql_select_db($db, $dbConnect);
	$db_selected = mysql_select_db($settings['database'], $dbConnect);
	if($settings['debug'] > 0) {
		if (!$db_selected) {
		    die ('Can\'t use ' . $settings['database']. ': ' . mysql_error());
		}
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
	
	$settings = get_gmp_settings();
	
	if( !checkIsAdmin( $dbLoginName)) {
		$dbLoginName = $settings['operator'];
		$dbLoginPwd = $settings['operator_password'];
	}
	else {
		$dbLoginName = $settings['admin'];
		$dbLoginPwd = $settings['admin_password'];
	}
		
	$dbConnect = mysql_connect($settings['dbhost'], $dbLoginName, $dbLoginPwd);
	if (!$dbConnect) {
		die('Could not connect: ' . mysql_error());
	}
	
	//mysql_select_db($db, $dbConnect);
	$db_selected = mysql_select_db($settings['database'], $dbConnect);
	if($settings['debug'] > 0) {
		if (!$db_selected) {
		    die ('Can\'t use ' . $settings['database']. ': ' . mysql_error());
		}
	}
	
	return $dbConnect;
}

function dbclose( $dbConnect)
{
	mysql_close($dbConnect);
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
				WHERE clientid='{$clientid}' AND 
				time > (NOW() - INTERVAL 3 WEEK) AND 
				calls.operatorid=operators.operatorid 
				ORDER BY callid DESC ";
    
    $result = mysql_query( $sql, $dbConnect);
    if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $sql;
		die($message);
    }

    print ("<hr noshade>");
    print('<table border="0" width="100%" cellspacing="0" cellpadding="0">');
    $i=0; // need this to change cell bgcolor
	$firstround = 1;
    while ($myrow = mysql_fetch_array($result)) {
// select sclass short name
        $class_ = $myrow["class"];
		$sql  = "SELECT sclass_sname FROM call_sclass WHERE";
		$sql .= " sclass_id='$class_'";
		$res = mysql_query( $sql, $dbConnect);
		if (!$res) {
		    $message  = 'Invalid query: ' . mysql_error() . "\n";
		    $message .= 'Whole query: ' . $sql;
		    die($message);
		}
		$class_row = mysql_fetch_row($res);
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
	$dbConnect = dbconnect();

//      $sql = "SELECT mclass_id,mclass_name FROM call_mclass";
    $sql  = "SELECT m.mclass_id,s.sclass_id,m.mclass_name,s.sclass_name";
    $sql .= " FROM call_mclass AS m,call_sclass AS s";
    $sql .= " WHERE m.mclass_id=s.mclass_id ORDER BY m.mclass_id";
    
    $result = mysql_query($sql, $dbConnect);
    if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $sql;
		die($message);
    }
    while( $class = mysql_fetch_array($result)) {
		printf("<option value=\"%d\">%s -- %s</option>",$class[sclass_id],$class[mclass_name],$class[sclass_name]);
    }
    
    dbclose($dbConnect);
}

// this is for drop-down list of clients
function draw_clients()
{
	$dbConnect = dbconnect();
	
    $sql = "SELECT clientid,lastname,firstname FROM clients ORDER BY lastname";
    
    $result = mysql_query($sql, $dbConnect);
    if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $sql;
		die($message);
    }
    while( $client = mysql_fetch_array($result)) {
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
		$sql  = "SELECT phone1,phone2,alone,dob,ailments,note,area,
					contact1name,contact1phone1,
					contact2name,contact2phone1,
					gpname,
					TIME_FORMAT(timeslot,'%H:%i') as timeslot from clients
					WHERE clientid='$clientid' AND active='1'";
		
		$result = mysql_query( $sql, $dbConnect);
		if (!$result) {
		    $message  = 'Invalid query: ' . mysql_error() . "\n";
		    $message .= 'Whole query: ' . $sql;
		    die($message);
		}
		else {	
	   	    while ($myrow = mysql_fetch_array($result)) {
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
				$contact1phone1 = $myrow['contact1phone1'];

				$contact2name = $myrow['contact2name'];
				if(strlen(trim($contact2name)) == 0) $contact2name = 'N/A';

				$contact2phone1 = $myrow['contact2phone1'];
				if(strlen(trim($contact2phone1)) == 0) $contact2phone1 = 'N/A';

				$gpname = $myrow['gpname'];
				if(strlen(trim($gpname)) == 0) $gpname = 'N/A';
				
				$timeslot = $myrow['timeslot'];
				$ailments = $myrow["ailments"];
		       	$note = $myrow["note"];
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
    $out .= "<td><font size=" . $fs . ">" . $contact1phone1 . "</font></td>";
    $out .= '</tr><tr>';    
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>D.O.B. (DD/MM/YYYY):</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $dob . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>Second Contact:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact2name . "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Area:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $area . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>Second Contact phone:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact2phone1 . "</font></td>";
    $out .= '</tr><tr>';    
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Time slot:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $timeslot . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>GP name:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $gpname . "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Alone:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">"; 
		if( $alone = 1) {
    		$out .= "Yes";
		}
		else {
			$out .= "No";
		}
	$out .= "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Medical notes:</b></font></td>";
    $out .= "<td><font size=" . $fs .">" . $ailments . "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Alerts:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $note . "</font></td>";
    $out .= '</tr></table>';
    
    print "$out";

	dbclose($dbConnect);
}

// Draw clients list
// left part of the screen
function draw_clients_list($operator, $clientid)
{
	$clients = array();		// array to store clientids and the rest
	$calltime = array();	// array to store timeslots
	$nextcall = array();	// array to store nextcalls
	$timeslot_done = array();	// to track if the timeslot call was made
 
	$dbConnect = dbconnect();
	
	$fs = 1; // font size
	
	// getting client list first ...
    $sql  = "SELECT clientid,firstname,lastname,
			TIME_FORMAT(timeslot,'%H:%i') AS timeslot
			FROM clients WHERE clientid IN (SELECT clientid FROM client2operator WHERE 
			operatorid=(SELECT operatorid FROM operators WHERE loginname='{$operator}'))
			AND active='1' ORDER BY timeslot, lastname";
    
    $result = mysql_query( $sql, $dbConnect);
    if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		die($message);
    }

	while ($myrow = mysql_fetch_array($result)) {
		$clients[$myrow['clientid']] = $myrow['lastname'] . ', ' . $myrow['firstname'];
		$calltime[$myrow['clientid']] = $myrow['timeslot'];
	}

	// getting unfinished calls and nextcalltime for it
	$sql = "SELECT clientid,DATE_FORMAT(nextcalltime,'%Y-%m-%d %H:%i:%s') as nextcalltime
			FROM calls WHERE clientid in 
			(SELECT clientid FROM client2operator WHERE 
			operatorid=(SELECT operatorid FROM operators WHERE loginname='{$operator}')) 
			and call_finished='0'";
	
    $result = mysql_query( $sql, $dbConnect);
    if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . '\n';
		$message .= 'Whole query: ' . $sql;
		die($message);
    }

	while ($myrow = mysql_fetch_array($result)) {
		$nextcall[$myrow['clientid']] = $myrow['nextcalltime'];
	}
	
	// getting the timeslot_done flags for all the clients
	$sql = "SELECT clientid,timeslot_done
			FROM client_timeslot_call WHERE clientid in 
			(SELECT clientid FROM client2operator WHERE 
			operatorid=(SELECT operatorid FROM operators WHERE loginname='{$operator}'))";
	
    $result = mysql_query( $sql, $dbConnect);
    if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . '\n';
		$message .= 'Whole query: ' . $sql;
		die($message);
    }

	while ($myrow = mysql_fetch_array($result)) {
		$timeslot_done[$myrow['clientid']] = $myrow['timeslot_done'];
	}

	// processing now.....
	$now = time();
	$mnt = mktime(0, 0, 0, date("m"), date("d"), date("Y")); // tonight's midnight

	foreach( $calltime as $cid => $nctime) { // now go through it all and figure out the time....
		$nct = -1;
		if( isset($nextcall[$cid])) {
			if( ereg( "([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})", $nextcall[$cid], $regs )) {
				$nct = mktime($regs[4],$regs[5],$regs[6],$regs[2],$regs[3],$regs[1]); // nextcalltime
			}
		}

		$regs = 0;
		if( ereg( "([0-9]{2}):([0-9]{2})", $nctime, $regs )) {
			$tms = mktime($regs[1], $regs[2], 0, date("m"), date("d"), date("Y")); // time slot
		}
		else {
			$tms = 0;
		}

		if( ($mnt - $nct) > 0 && $nct > 0) { // we left unfinished call from yesterday. bad!
			$calltime[$cid] = '<b><font color="#f11313">Unfinished call from the past. ' . $nextcall[$cid] . ' Call now!</font></b>';
		}
		else if( $tms < $now && $timeslot_done[$cid] == 0 ) { // we missed client's usual time slot. bad!
			$calltime[$cid] = '<b><font color="#f11313">Missed time slot (' . $nctime . '). Call now!</font></b>';
		}
		else if( $nct > $now) {
			$calltime[$cid] = date( "H:i", $nct);
		}
		else if( $nct < $now && $nct > 0) {
			$calltime[$cid] = '<b><font color="#f11313">Missed call time ('. date( "H:i", $nct) . '). Call now!</font></b>';
		}
		
		else if( $tms < $now) {
			$calltime[$cid] = '';
		}
//		$calltime[$cid] .= '  --- nct: ' . date("Y-m-d H:i:s", $nct) . '; timeslot: ' . date("Y-m-d H:i:s", $tms) . '; ' . $nextcall[$cid] . '==' . $nct; 
	}
    
	// we need to sort the array now
	

	// print it all out ....
	print("<table>");

	foreach ( $clients as $cid => $name ) {
		$out  = '<tr><td><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="$fs">';
		$out .= $name . '</a></font></td>';
		$out .= '<td>' . $calltime[$cid] . '</td>';
	    
	    print $out . '</tr>';
	}

	print("</table>");

	dbclose($dbConnect);
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

	$sql = "SELECT operatorid,fullname FROM operators ORDER BY fullname";
	
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		die($message);
	}
	else {
		while ( $myrow = mysql_fetch_array($result)) {
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
	
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		die($message);
	}
	else {
		while ( $myrow = mysql_fetch_array($result)) {
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
	
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		die($message);
	}
	else {
		while ( $myrow = mysql_fetch_array($result)) {
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
	
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		die($message);
	}
	else {
		while ( $myrow = mysql_fetch_array($result)) {
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
	
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		die($message);
	}
	else {
		while ( $myrow = mysql_fetch_array($result)) {
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
	$isAdmin = false;
	
	$dbConnect = dbconnect();

	$sql = "SELECT isAdmin FROM operators WHERE operatorid='" . $operatorid . "'";
	
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		print $message;
	}
	else {
		while ( $myrow = mysql_fetch_array($result)) {
			if(isset( $myrow['isAdmin'])) {
				$isAdmin = $myrow['isAdmin'];
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $isAdmin;
}

function isOperatorSnr( $operatorid) {
	$myrow = '';
	$isSnr = false;
	
	$dbConnect = dbconnect();

	$sql = "SELECT isSnr FROM operators WHERE operatorid='" . $operatorid . "'";
	
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		print $message;
	}
	else {
		while ( $myrow = mysql_fetch_array($result)) {
			if(isset( $myrow['isSnr'])) {
				$isSnr = $myrow['isSnr'];
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $isSnr;
}

function getClientsForOperator( $operatorid, &$clients) {
	$myrow = '';
	$dbConnect = dbconnect();

	$sql = 'SELECT clients.clientid,clients.firstname,clients.lastname,clients.initials 
			FROM clients LEFT JOIN client2operator ON clients.clientid=client2operator.clientid 
			WHERE client2operator.operatorid="' . $operatorid . '"  
			ORDER BY clients.lastname, clients.firstname';

	$result = mysql_query( $sql, $dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		die($message);
	}
	else {
		while ( $myrow = mysql_fetch_array($result)) {
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

	$sql = "SELECT TIME_FORMAT(timeslot,'%H:%i') as timeslot FROM clients WHERE clientid='" . $clientid . "'";  

	$result = mysql_query( $sql, $dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		die($message);
	}
	else {
		while ( $myrow = mysql_fetch_array($result)) {
			$timeslot = $myrow['timeslot'];
		}
	}
	
	dbclose( $dbConnect);
	
	return $timeslot;
}

function getClients( &$clients, $what_clients) {
	$myrow = '';
	$dbConnect = dbconnect();
	
	switch( $what_clients) {
	case ACTIVE_CLIENTS:
			$sql = "SELECT clientid,firstname,lastname,initials FROM clients WHERE active='1' ORDER BY lastname";
			break;
	case INACTIVE_CLIENTS:
			$sql = "SELECT clientid,firstname,lastname,initials FROM clients WHERE active='0' ORDER BY lastname";
			break;
	case ALL_CLIENTS:
			$sql = "SELECT clientid,firstname,lastname,initials FROM clients ORDER BY lastname";
			break;
	}
	
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		die($message);
	}
	else {
		while ( $myrow = mysql_fetch_array($result)) {
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

function getActiveClients( &$clients) {
	return getClients( $clients, ACTIVE_CLIENTS);
}

function getInactiveClients( &$clients) {
	return getClients( $clients, INACTIVE_CLIENTS);
}

function getAllClients( &$clients) {
	return getClients( $clients, ALL_CLIENTS);
}

function deactivate_client( $clientid) {
	$dbConnect = dbconnect();

	$sql = "UPDATE clients SET active='0' WHERE clientid='" . $clientid . "'";
	$result = mysql_query($sql);
	if (!$result) {
		die('Invalid query: ' . mysql_error());
	}
	
	dbclose( $dbConnect);
}

function reactivate_client( $clientid) {
	$dbConnect = dbconnect();

	$sql = "UPDATE clients SET active='1' WHERE clientid='" . $clientid . "'";
	$result = mysql_query($sql);
	if (!$result) {
		die('Invalid query: ' . mysql_error());
	}
	
	dbclose( $dbConnect);
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

			if( !vphone(f.phone1) || !vphone(f.contact1phone1)) {
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

function printAddEditClientTable( $clean)
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
		<textarea name="note" cols="27" rows="4" >' . $clean['note'] . '</textarea>
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
		<textarea name="ailments" cols="27" rows="4" >' . $clean['ailments'] . '</textarea>
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
		<input type="text" name="contact1phone1" onchange="return vphone(this);" size="15" maxlength="15" value="' . $clean['contact1phone1'] . '">
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
		<input type="text" name="contact2phone1" onchange="return vphone(this);" size="15" maxlength="15" value="' . $clean['contact2phone1'] . '">
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
	
		<td ALIGN="right"  width="30%" VALIGN="top">
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
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>';

}

function getDistrictList( &$arDistricts)
{
	$myrow = '';
	$dbConnect = dbconnect();
	
	$sql = "SELECT districtid,district_name,comments FROM districts";

	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		die($message);
	}
	else {
		while ( $myrow = mysql_fetch_array($result)) {
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

function addClientToOperator( &$arAssigned, &$clean) {
	$retval = true;
	$dbConnect = dbconnect();
	
	$sql = 'DELETE FROM client2operator WHERE operatorid="' . $clean['operatorid_edit'] . '"';
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		$retval = false;
	}

	$sql = 'INSERT INTO client2operator VALUES ';
	
	foreach($arAssigned as $cid => $value) {
		$sql .= '('. $cid . ','. $clean['operatorid_edit'] . '),';
	}
	
	// trim trailing comma
	$out = $sql;
	$sql = rtrim( $out, ",");
	
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		$retval = false;
	}
	
	dbclose( $dbConnect);
	
	return $retval;
}

function checkIsSnr( $operatorid){
	$retval = false;
	
	$dbConnect = dbconnect();
	
	// let's see who is the senior operator at the moment
	$sql = 'SELECT operatorid FROM operators WHERE isSnr="1"';
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		$retval = false;
	}
	else {
		while ( $myrow = mysql_fetch_array($result)) {
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
		$sql = 'DELETE FROM client2operator WHERE clientid="' . $clientid . '"';
		$result = mysql_query( $sql,$dbConnect);
		if ( !$result) {
			$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
			print $message; 
			$retval = false;
		}
	
		$sql = 'INSERT INTO client2operator VALUES ('. $clientid . ','. $snrID . ')';
		
		$result = mysql_query( $sql,$dbConnect);
		if ( !$result) {
			$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
			print $message;
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
	$sql = 'SELECT operatorid FROM operators WHERE isSnr="1"';
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		$retval = false;
	}
	else {
		$myrow = mysql_fetch_array($result);
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

	$sql = 'UPDATE operators SET isSnr="1" WHERE operatorid="' . $operatorid . '"';
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		print $message;
		$retval = false;
	}
	
	dbclose( $dbConnect);
	
	return $retval;
}

function clearSnrOperator( $operatorid) {
	$retval = true;
	
	$dbConnect = dbconnect();

	$sql = 'UPDATE operators SET isSnr="0" WHERE operatorid="' . $operatorid . '"';
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		print $message;
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
	
	$sql = 'INSERT INTO operators (loginname,fullname,saltypwd,addedby,isAdmin,isSnr)
			VALUES ("' . $clean['loginname'] . '","' . $clean['fullname'] . 
			'",SHA1("' . $clean['password'] . getTheSalt() . '"),"' 
			. $clean['operatorid'] . '","' . $clean['isAdmin'] . '","' . $clean['isSnr'] . '")';
			
	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		if( mysql_errno() == 1062) {
			print "<font size=\"3\" color=\"#FF0000\"><b>Duplicate entry!</b></font>
					<p>Operator with the same name already exists on the system.<br>
					Please use a different login name<p>";
			$retval = false;
		}
		else {
			$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
			print $message;
			$retval = false;
		}
	}

	// this bit really has to be a part of transaction
	if($retval) {
		// strip current Senior Operator off her duty
		if( $clean['isSnr'] == 1) {
			clearSnrOperator( $snrID);
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
	
	$sql = 'UPDATE operators SET loginname="' . $clean['loginname'] . '",
			fullname="' . $clean['fullname'] . '",';
	
	if( strlen($clean['password']) > 0) {
		$sql .= 'saltypwd=SHA1("' . $clean['password'] . getTheSalt() . '"),';
	}
	
	$sql .= 'isAdmin="' . $clean['isAdmin'] . '",' .
			'isSnr="' . $clean['isSnr'] . '",
			modified=NOW(),
			modifiedby="' . $clean['operatorid'] . '" 
			WHERE operatorid="' . $clean['operatorid_edit'] . '"';

	$result = mysql_query( $sql,$dbConnect);
	if ( !$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		print $message;
		$retval = false;
	}

	dbclose( $dbConnect);
	
	// check if there is no more Senior Operator on the system
	// and make Administrator the Senior by default
	if( $clean['isSnr'] == 0) {
		if( $snrID == $clean['operatorid_edit']) {
			// set admin (operatorid=1) as Senior
			setSnrOperator(1);
		}
	}
	else {
		// we need to clear current Senior
		if( $snrID > 0 && $snrID != $clean['operatorid_edit']) {
			clearSnrOperator($snrID);
		}
	}
	
	return $retval;
}

function addClient(&$clean) {
	$retval = true;
	$dbConnect = dbconnect();
    
	$sql = 'INSERT INTO clients (firstname,lastname,title,gender,address,area,districtid,
			phone1,phone2,housetype,dob,alone, ailments,contact1name,contact1relationship,
			contact1address,contact1phone1,contact2name,contact2relationship, contact2address,
			contact2phone1,gpname,referrer,note,referrer_other,timeslot,addedby,modifiedby)
			VALUES ("'. $clean['firstname'] . '", "' . $clean['lastname'] . '", "' . $clean['title'] . '", "'
			 . $clean['gender'] . '", "' . $clean['address'] . '", "' . $clean['area'] . '", "'
			 . $clean['districtid'] . '", "' 
			 . $clean['phone1'] . '", "'  . $clean['phone2'] . '", "' . $clean['housetype'] . '", "'
			 . $clean['dob'] . '", "' . $clean['alone'] . '", "' . $clean['ailments'] . '", "'
			 . $clean['contact1name'] . '", "' . $clean['contact1relationship'] . '", "'
			 . $clean['contact1address'] . '", "' . $clean['contact1phone1'] . '", "' . $clean['contact2name'] . '", "'
			 . $clean['contact2relationship'] . '", "' . $clean['contact2address'] . '", "' 
			 . $clean['contact2phone1'] . '", "' . $clean['gpname'] . '", "' . $clean['referrer'] . '", "'
			 . $clean['note'] . '", "' . $clean['referrer_other'] . '", "'
			 . $clean['timeslot'] . '", "' . $_SESSION['operatorid'] . '", "' . $_SESSION['operatorid'] . '")';

	// run SQL against the DB
	$result = mysql_query($sql);
	if (!$result) {
		if( mysql_errno() == 1062) {
			print "<font size=\"3\" color=\"#FF0000\"><b>Duplicate entry!</b></font>
					<p>Client with the same name and date of birth already exists on the system.<br>
					Please use Edit Client screen to change details of existing client or check that you entered
					name and date of birth of the new client correctly<p>";
			
			print  '<a href="' . $_SERVER['PHP_SELF'] . '">Add another client</a><p>';
		}
		else {
			$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
			print $message;
			$retval = false;
		}
	}
	
	dbclose( $dbConnect);
	
	return $retval;
}

function setFirstCallDone( $clientid) {
	$retval = true;
	
	$dbConnect = dbconnect();
	
	$sql = 'UPDATE client_timeslot_call SET timeslot_done="1" WHERE clientid="' . $clientid . '"';
	
	$result = mysql_query($sql);
	if (!$result) {
		$message  = "Invalid query: " . mysql_error() . "\n";
		$message .= "Whole query: " . $sql . "\n";
		print $message;
		$retval = false;
	}

	dbclose( $dbConnect);
	
	return $retval;
}

function recordTheCall( &$clean) {
	$retval = true;
	
	$dbConnect = dbconnect();

	$sql  = "INSERT INTO calls (clientid, time, nextcalltime, chat, class, call_finished, operatorid) 
  		    VALUES ('" . $clean['clientid'] . "', 
					NOW(), 
					TIMESTAMP( CURDATE() , '" . $clean['nextcalltime'] . "'),
					'" . $clean['chat'] . "', 
					'" . $clean['mclass'] . "',
					'" . $clean['call_finished'] . "', 
					'" . $_SESSION['operatorid'] . "') ";
	
	$result = mysql_query($sql, $dbConnect);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
		$retval = false;
	}
	else if( $clean['call_finished'] == 1) { 
		// now we can set all unfinished calls to finished state, so the
		// calculation of the next call time for the client can be correct
		// NOTE: this is quick and dirty workaround for the moment!!!
		$sql  = "UPDATE calls SET call_finished='1' 
				WHERE clientid='" . $clean['clientid'] . "'";
		
		$result = mysql_query($sql, $dbConnect);
		if (!$result) {
			$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
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
					address,phone1,phone2,housetype,dob,alone,ailments,
					contact1name,contact1relationship,contact1address,contact1phone1,contact1phone2,
					contact2name,contact2relationship,contact2address,contact2phone1,contact2phone2,
					gpname,referrer,note,referrer_other,
					TIME_FORMAT(timeslot,'%H:%i') as timeslot FROM clients WHERE (clients.clientid={$clean['clientid']})";
	
	$result = mysql_query($sql, $dbConnect);
	if (!$result) {
		$message  = "Invalid query: " . mysql_error() . "\n";
		$message .= "Whole query: " . $sql . "\n";
		$retval = false;
	}
	$myrow = mysql_fetch_array($result);

/*
 * Cleaning input from the database
 */
	$clean['firstname']				= htmlentities($myrow['firstname'], ENT_QUOTES);
	$clean['lastname']				= htmlentities($myrow['lastname'], ENT_QUOTES);
	$clean['initials']				= htmlentities($myrow['initials'], ENT_QUOTES);
	$clean['title']					= htmlentities($myrow['title'], ENT_QUOTES);
	$clean['gender']				= htmlentities($myrow['gender'], ENT_QUOTES);
	$clean['address']				= htmlentities($myrow['address'], ENT_QUOTES);
	$clean['phone1']				= htmlentities($myrow['phone1'], ENT_QUOTES);
	$clean['phone2']				= htmlentities($myrow['phone2'], ENT_QUOTES);
	$clean['housetype']				= htmlentities($myrow['housetype'], ENT_QUOTES);
	$clean['dob']					= htmlentities($myrow['dob'], ENT_QUOTES);
	$clean['alone']					= htmlentities($myrow['alone'], ENT_QUOTES);
	$clean['ailments']				= htmlentities($myrow['ailments'], ENT_QUOTES);
	$clean['contact1name']			= htmlentities($myrow['contact1name'], ENT_QUOTES);
	$clean['contact1relationship']	= htmlentities($myrow['contact1relationship'], ENT_QUOTES);
	$clean['contact1address']		= htmlentities($myrow['contact1address'], ENT_QUOTES);
	$clean['contact1phone1']		= htmlentities($myrow['contact1phone1'], ENT_QUOTES);
	$clean['contact1phone2']		= htmlentities($myrow['contact1phone2'], ENT_QUOTES);
	$clean['contact2name']			= htmlentities($myrow['contact2name'], ENT_QUOTES);
	$clean['contact2relationship']	= htmlentities($myrow['contact2relationship'], ENT_QUOTES);
	$clean['contact2address']		= htmlentities($myrow['contact2address'], ENT_QUOTES);
	$clean['contact2phone1']		= htmlentities($myrow['contact2phone1'], ENT_QUOTES);
	$clean['contact2phone2']		= htmlentities($myrow['contact2phone2'], ENT_QUOTES);
	$clean['gpname']				= htmlentities($myrow['gpname'], ENT_QUOTES);
	$clean['referrer']				= htmlentities($myrow['referrer'], ENT_QUOTES);
	$clean['timeslot']				= htmlentities($myrow['timeslot'], ENT_QUOTES);
	$clean['note']					= htmlentities($myrow['note'], ENT_QUOTES);
	$clean['referrer_other']		= htmlentities($myrow['referrer_other'], ENT_QUOTES);

	dbclose($dbConnect);
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
	
	
	    while( $row = mysql_fetch_array($query_result))
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
