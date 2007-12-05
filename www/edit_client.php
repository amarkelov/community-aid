<?php
session_start();
require 'functions.inc';

$clean = array();
$mysql = array();

$settings = get_ca_settings();

// Page Header ...
printHeader( "Edit Client", 0, "printAddEditClientJavaScript");

// if debug flag is set, print the following info
if ($settings['debug'] > 0) {
	print_debug();
}

// START LOG IN CODE
$doWeExit = displayLogin(basename($_SERVER['PHP_SELF']), true);
if($doWeExit == true){
	exit;
}
// END LOG IN CODE

/*
 * Start filtering input
 */
 
if(isset($_POST['clientid'])) {
	if(ctype_digit($_POST['clientid'])) { 
		$clean['clientid'] = $_POST['clientid'];
	}
}

if(isset($_POST['submit'])) {
	$clean['submit'] = $_POST['submit'];
}

if(isset($_POST['edit'])) {
	$clean['edit'] = $_POST['edit'];
}

if(isset($_SESSION['operatorid'])) {
	$clean['operatorid'] = $_SESSION['operatorid'];
}

if($settings['debug'] == 1){
	print "<b>\$clean on entry:</b><br>";
	print_r( $clean);
	print "<p>";
}

verifyClientData( $_POST, $clean);

/*
 * End of filtering input
 */
 
if ($clean['submit']) {
	// if debug flag is set, print the following info
	if ($settings['debug'] > 0) {
		print_debug( $clean, $settings);
	}

	// fix DOB
    if( $dob) {
		if( ereg("([0-9]{2})/([0-1]{1}[0-9]{1})/([1-2]{1}[0-9]{3})", $dob, $reg)) {
		    $dob="$reg[3]-$reg[2]-$reg[1]";
		}
    }

    // here if no ID then editing  else adding
	if ( $clean['clientid']) {
	    $sql  = sprintf("UPDATE clients SET firstname='%s',
		    lastname='%s',title='%s',gender ='%s',address='%s',area='%s',
			phone1='%s',phone2='%s',dob='%s',gpname='%s',housetype='%s',
			referrer='%s',alone=%d,ailments='%s',note='%s',
			contact1name='%s',contact1relationship='%s',
		    contact1address='%s',contact1phone1='%s',
			contact2name='%s',contact2relationship='%s',
		    contact2address='%s',contact2phone1='%s', 
		    timeslot='%s',changenote='%s',districtid=%d,modifiedby=%d WHERE clientid=%d",
			mysql_real_escape_string( $clean['firstname'], $dbConnect)
			mysql_real_escape_string( $clean['lastname'], $dbConnect),
			mysql_real_escape_string( $clean['title'], $dbConnect),
			mysql_real_escape_string( $clean['gender'], $dbConnect),
			mysql_real_escape_string( $clean['address'], $dbConnect),
			mysql_real_escape_string( $clean['area'], $dbConnect),
			mysql_real_escape_string( $clean['phone1'], $dbConnect),
			mysql_real_escape_string( $clean['phone2'], $dbConnect),
			mysql_real_escape_string( $clean['dob'], $dbConnect),
			mysql_real_escape_string( $clean['gpname'], $dbConnect),
			mysql_real_escape_string( $clean['housetype'], $dbConnect),
			mysql_real_escape_string( $clean['referrer'], $dbConnect),
			mysql_real_escape_string( $clean['alone'], $dbConnect),
			mysql_real_escape_string( $clean['ailments'], $dbConnect),
			mysql_real_escape_string( $clean['note'], $dbConnect),
			mysql_real_escape_string( $clean['contact1name'], $dbConnect),
			mysql_real_escape_string( $clean['contact1relationship'], $dbConnect),
			mysql_real_escape_string( $clean['contact1address'], $dbConnect),
			mysql_real_escape_string( $clean['contact1phone1'], $dbConnect),
			mysql_real_escape_string( $clean['contact2name'], $dbConnect),
			mysql_real_escape_string( $clean['contact2relationship'], $dbConnect),
			mysql_real_escape_string( $clean['contact2address'], $dbConnect),
			mysql_real_escape_string( $clean['contact2phone1'], $dbConnect),
			mysql_real_escape_string( $clean['timeslot'], $dbConnect),
			mysql_real_escape_string( $clean['reason'], $dbConnect),
			mysql_real_escape_string( $clean['districtid'], $dbConnect),
			mysql_real_escape_string( $clean['operatorid'], $dbConnect),
			mysql_real_escape_string( $clean['clientid'], $dbConnect));

		// run SQL against the DB
		$dbConnect = dbconnect();
		
		$result = mysql_query($sql);
		if ( !$result) {
			$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
			die($message);
		}
	
		dbclose( $dbConnect);
		
		echo "Record updated/edited!<p>";
		print  '<a href="' . $_SERVER[PHP_SELF] . '">Choose another client to edit</a><p>';
	}
} // if ($submit)
else if( $clean['edit']) {
	print '<form name="edit_client"
				onSubmit="
				this.title.optional = true;
				this.phone2.optional = true;
				this.gpname.optional = true;
				this.contact1relationship.optional = true;
				this.contact1address.optional = true;
				this.contact2name.optional = true;
				this.contact2relationship.optional = true;
				this.contact2address.optional = true;
				this.contact2phone1.optional = true;
				this.note.optional = true;
				this.referrer_other.optional = true;
				return verify(this);"
				method="post" 
				action="' . $_SERVER['PHP_SELF'] . '">
			<font face="Verdana, Arial, Helvetica, sans-serif">';
	
	if ($clean['clientid']) {
	
	    // editing so select a record
	
		$sql = "SELECT firstname,lastname,initials,title,gender,address,area,phone1,phone2,housetype,
				dob,alone,ailments,contact1name,contact1relationship,contact1address,contact1phone1,
				contact1phone2,contact2name,contact2relationship,contact2address,contact2phone1,
				contact2phone2,gpname,referrer,note,districtid,
				TIME_FORMAT(timeslot,'%H:%i') as timeslot
				FROM clients WHERE clientid='" . $clean['clientid'] . "'";
		
		$dbConnect = dbconnect();
		
		$result = mysql_query($sql, $dbConnect);
		if ( !$result) {
			$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
			die($message);
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
		$clean['area']					= htmlentities($myrow['area'], ENT_QUOTES);
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
		$clean['districtid']			= htmlentities($myrow['districtid'], ENT_QUOTES);
	
	//	$clientid = $myrow["clientid"];
	  
		dbclose($dbConnect);

		// if debug flag is set, print the following info
		if ($settings['debug'] > 0) {
			print_debug( $clean, $settings);
		}
				
		print '<input type=hidden name="clientid" value="' . $clean['clientid']. '">';
	
	} // if ($clean['clientid'])
	
	print '</font>';

	//<!------------------->
	//<!----HTML LAYOUT---->
	//<!------------------->
	
	printAddEditClientTable( $clean);	
	
	print '<br>
	
	<div align="center">
	<font face="Verdana, Arial, Helvetica, sans-serif">
	<input type="Submit" name="submit" value="Submit">
	</font>
	</div>
	
	</form>
	</body>
	</html>';
}
else {

	// this part happens if we don't press submit
	if (!$clean['clientid']) {
		// pull the list of active clients
		$clients = array();
		
		if( getAllClients( $clients)) {
			print '<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
					<font face="Verdana, Arial, Helvetica, sans-serif">
					<div align="left">
					<table>';
	
			print '<tr><td><select name="clientid">';
			
			foreach ( $clients as $cid => $value) {
				print '<option value="' . $cid . '">'
					  . $value . ' (' . $cid . ')' . 
					  '</option>';
			
			}
	
			print '</select></td></tr>';
			print '<tr><td><font face="Verdana, Arial, Helvetica, sans-serif">
					<input type="Submit" name="edit" value="Edit client">
					</font>
					</div></td></tr>';
			
			print '</table></form>';
		}
	}
}
	
?>
