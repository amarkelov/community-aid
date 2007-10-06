<?php
session_start();
require 'functions.inc';

$clean = array();
$mysql = array();

$settings = get_gmp_settings();

print '<html><head>
		<title>Edit Client List -- Good Morning ' . $settings['location'] . '</title>
		<meta http-equiv="expires" content="-1">
		<meta http-equiv="Cache-Control" content="no-cache">';

printAddEditClientJavaScript();

print '</head>
		<body bgcolor="#BEC8FD"><p>';

// START LOG IN CODE
	$doWeExit = displayLogin(basename($_SERVER['PHP_SELF']), true);
	if($doWeExit == true){
		exit;
	}
// END LOG IN CODE

/*
 * Start filtering input
 */
 
function filter_phone_number($phone) {
	$filtered = "N/A";
	if(ctype_digit($phone)) {
		$filtered = $phone;
	}
	return $filtered;
}

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

if(isset($_POST['firstname'])) {
	$clean['firstname'] = htmlentities(strtoupper($_POST['firstname']), ENT_QUOTES );
}

if(isset($_POST['lastname'])) {
	if(ctype_print($_POST['lastname'])) {
		$clean['lastname'] = htmlentities(strtoupper($_POST['lastname']), ENT_QUOTES );
	}
}

if(isset($_POST['title'])) {
	$clean['title'] = htmlentities($_POST['title'], ENT_QUOTES );
}

if(isset($_POST['gender'])) {
	if(ctype_print($_POST['gender'])) {
		$clean['gender'] = htmlentities($_POST['gender'], ENT_QUOTES );
	}
}

if(isset($_POST['address'])) {
	if(ctype_print($_POST['address'])) {
		$clean['address'] = htmlentities($_POST['address'], ENT_QUOTES );
	}
}

if(isset($_POST['area'])) {
	if(ctype_print($_POST['area'])) {
		$clean['area'] = htmlentities($_POST['area'], ENT_QUOTES );
	}
}

if(isset($_POST['phone1'])) {
	$clean['phone1'] = filter_phone_number($_POST['phone1']);
}

if(isset($_POST['phone2'])) {
	$clean['phone2'] = filter_phone_number($_POST['phone2']);
}

if(isset($_POST['dob'])) {
	$reg = 0;
	if(ereg( "([0-2]{1}[0-9]{1})/([0-1]{1}[0-9]{1})/([1-2]{1}[0-9]{3})", $_POST['dob'], $reg)) {
		$clean['dob'] = $reg[3] . "-" . $reg[2] . "-" . $reg[1];
	}
}

if(isset($_POST['gpname'])) {
	if(ctype_print($_POST['gpname'])) {
		$clean['gpname'] = htmlentities($_POST['gpname'], ENT_QUOTES );
	}
}

if(isset($_POST['housetype'])) {
	if(ctype_print($_POST['housetype'])) {
		$clean['housetype'] = htmlentities($_POST['housetype'], ENT_QUOTES );
	}
}

if(isset($_POST['referrer'])) {
	$clean['referrer'] = htmlentities($_POST['referrer'], ENT_QUOTES );
}

if(isset($_POST['alone'])) {
	if(strtoupper($_POST['alone']) == "ON") {
		$clean['alone'] = 1;
	}
	else {
		$clean['alone'] = 0;
	}
}

if(isset($_POST['ailments'])) {
	if(ctype_print($_POST['ailments'])) {
		$clean['ailments'] = htmlentities($_POST['ailments'], ENT_QUOTES );
	}
}

if(isset($_POST['note'])) {
	if(ctype_print(strtr($_POST['note'],"\n\t\r", "   "))) {
		$clean['note'] = htmlentities($_POST['note'], ENT_QUOTES );
	}
}

if(isset($_POST['referrer_other'])) {
	if(ctype_print($_POST['referrer_other'])) {
		$clean['referrer_other'] = htmlentities($_POST['referrer_other'], ENT_QUOTES );
	}
}

if(isset($_POST['contact1name'])) {
	if(ctype_print($_POST['contact1name'])) {
		$clean['contact1name'] = htmlentities($_POST['contact1name'], ENT_QUOTES );
	}
}

if(isset($_POST['contact1relationship'])) {
	if(ctype_print($_POST['contact1relationship'])) {
		$clean['contact1relationship'] = htmlentities($_POST['contact1relationship'], ENT_QUOTES );
	}
}

if(isset($_POST['contact1address'])) {
	if(ctype_print($_POST['contact1address'])) {
		$clean['contact1address'] = htmlentities($_POST['contact1address'], ENT_QUOTES );
	}
}

if(isset($_POST['contact1phone1'])) {
	$clean['contact1phone1'] = filter_phone_number($_POST['contact1phone1']);
}


if(isset($_POST['contact2name'])) {
	if(ctype_print($_POST['contact2name'])) {
		$clean['contact2name'] = htmlentities($_POST['contact2name'], ENT_QUOTES );
	}
}

if(isset($_POST['contact2relationship'])) {
	if(ctype_print($_POST['contact2relationship'])) {
		$clean['contact2relationship'] = htmlentities($_POST['contact2relationship'], ENT_QUOTES );
	}
}

if(isset($_POST['contact2address'])) {
	if(ctype_print($_POST['contact2address'])) {
		$clean['contact2address'] = htmlentities($_POST['contact2address'], ENT_QUOTES );
	}
}

if(isset($_POST['contact2phone1'])) {
	$clean['contact2phone1'] = filter_phone_number($_POST['contact2phone1']);
}

if(isset($_POST['timeslot'])){
	$clean['timeslot'] = verify_timeslot($_POST['timeslot']);
}

if(isset($_POST['reason'])) {
	if(ctype_print($_POST['reason'])) {
		$clean['reason'] = htmlentities($_POST['reason'], ENT_QUOTES );
	}
}

if(isset($_POST['districtid'])) {
	if(ctype_digit($_POST['districtid'])) {
		$clean['districtid'] = $_POST['districtid'];
	}
}

if(isset($_SESSION['operatorid'])) {
	$clean['operatorid'] = $_SESSION['operatorid'];
}

if($settings['debug'] == 1){
	print "<b>\$clean on entry:</b><br>";
	print_r( $clean);
	print "<p>";
}
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
	    $sql  = "UPDATE clients SET firstname='" . $clean['firstname'] ."',
		    lastname='" . $clean['lastname'] . "',title='" . $clean['title'] . "',gender ='" . $clean['gender'] . "',
		    address='" . $clean['address'] . "', area='" . $clean['area'] . "',phone1='" . $clean['phone1'] . "',
		    phone2='" . $clean['phone2'] . "',dob='" . $clean['dob'] . "',gpname='" . $clean['gpname'] . "',
		    housetype='" . $clean['housetype'] . "',referrer='" . $clean['referrer'] . "',
		    alone='" . $clean['alone'] . "',ailments='" . $clean['ailments'] . "',note='" . $clean['note'] . "',
		    referrer_other='" . $clean['referrer_other'] . "',
		    contact1name='" . $clean['contact1name'] . "',
		    contact1relationship='" . $clean['contact1relationship'] . "',
		    contact1address='" . $clean['contact1address'] . "',
		    contact1phone1='" . $clean['contact1phone1'] . "',contact2name='" . $clean['contact2name'] . "',
		    contact2relationship='" . $clean['contact2relationship'] . "',
		    contact2address='" . $clean['contact2address'] . "',
		    contact2phone1='" . $clean['contact2phone1'] . "', 
		    timeslot='" . $clean['timeslot'] . "', changenote='" . $clean['reason'] . "', 
			districtid='" . $clean['districtid'] . "',
		    modifiedby='" . $clean['operatorid'] . "' WHERE clientid='" . $clean['clientid'] . "'";

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
				contact2phone2,gpname,referrer,referrer_other,note,districtid,
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
		$clean['referrer_other']		= htmlentities($myrow['referrer_other'], ENT_QUOTES);
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