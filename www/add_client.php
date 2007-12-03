<?php
session_start();
require 'functions.inc';

$clean = array();
$mysql = array();

$settings = get_ca_settings();

print '<html><head>
		<title>Add Client --  Friendly Call Service -- ' . $settings['location'] . '</title>
		<meta http-equiv="expires" content="-1">
		<meta http-equiv="Cache-Control" content="no-cache">';

printAddEditClientJavaScript();

print '</head>
		<body bgcolor="#BEC8FD"><p>';

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
 
function filter_phone_number($phone) {
	$filtered = ereg_replace( " |-", "", $phone);
	if(ctype_digit($phone)) {
		$filtered = $phone;
	}
	else {
		$filtered = "N/A";	
	}
	return $filtered;
}

if(isset($_POST['clientid'])) {
	if(ctype_digit($_POST['clientid'])) { 
		$clean['clientid'] = $_POST['clientid'];
	}
}
elseif(isset($_GET['clientid']) && !isset($_POST['submit'])) {
	if(ctype_digit($_GET['clientid'])) {
		$clean['clientid'] = $_GET['clientid'];
	}
}

if(isset($_GET['delete'])) {
	if(strtoupper($_GET['delete']) == "YES") {
		$clean['delete'] = $_GET['delete'];
	}
}

if(isset($_POST['submit'])) {
	$clean['submit'] = $_POST['submit'];
}

if(isset($_POST['firstname'])) {
	$clean['firstname'] = htmlentities(strtoupper($_POST['firstname']), ENT_QUOTES );
}

if(isset($_POST['lastname'])) {
/*	$pos = strpos($_POST['lastname'], "'");
	if($pos == 1) {
		if(list($fh,$sh) = split("'", $_POST['lastname'])) {
			if( strtoupper($fh) == "O" && ctype_alpha($sh)) {
				$clean['lastname'] = htmlentities(strtoupper($_POST['lastname']), ENT_QUOTES );
			}
		}
	}
	else if(ctype_alpha($_POST['lastname'])) {*/
		$clean['lastname'] = htmlentities(strtoupper($_POST['lastname']), ENT_QUOTES );
//	}
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

if(isset($_POST['districtid'])) {
	if(ctype_digit($_POST['districtid'])) {
		$clean['districtid'] = $_POST['districtid'];
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
	$string = $_POST['gpname'];
	if(ctype_alpha(ereg_replace( " |\'|-", "", $string))) {
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
	if(ctype_print($_POST['note'])) {
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
	if(ctype_alnum($_POST['contact2name'])) {
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

if($settings['debug'] == 1){
	print "<b>\$clean:</b><br>";
	print_r( $clean);
	print "<p>";
}
/*
 * End of filtering input
 */
 
if ($clean['submit']) {
	if( addClient($clean)) {
		print "Record Added!<p>";
	}
	print  '<a href="' . $_SERVER['PHP_SELF'] . '">Add another client</a><p>';
} // if ($submit)
else {
	print '<form name="add_client" 
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
				action="' . $_SERVER['PHP_SELF'] . '">';
	
	//<!------------------->
	//<!----HTML LAYOUT---->
	//<!------------------->
	
	printAddEditClientTable( $clean);
	
	print '<br>
	
	<div align="center">
	<font face="Verdana, Arial, Helvetica, sans-serif">
	<input type="Submit" name="submit" value="Add client">
	</font>
	</div>
	
	</form>
	</body>
	</html>';
}
?>
