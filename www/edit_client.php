<?php

require 'functions.inc';

$clean = array();
$mysql = array();

$settings = get_gmp_settings();

?>

<html>
<head>
<title>Edit Client List -- Good Morning <?php echo $settings['location'] ?></title>
<meta http-equiv="expires" content="-1">
<meta http-equiv="Cache-Control" content="no-cache">
<script language="JavaScript">
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
-->
</script>
</head>
<body bgcolor="#BEC8FD">

<p>

<?php
// if debug flag is set, print the following info
if($settings['debug'] == 1){
	print "<b>\$_POST:</b><br>";
	print_r( $_POST);
	print "<p>";

	print "<b>\$_GET:</b><br>";
	print_r( $_GET);
	print "<p>";

	print "<b>\$settings:</b><br>";
	print_r( $settings);
	print "<p>";

}

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

if(isset($_POST['houseno'])) {
	if(ctype_digit($_POST['houseno'])) {
		$clean['houseno'] = $_POST['houseno'];
	}
}

if(isset($_POST['postcode'])) {
	if(ctype_digit($_POST['postcode'])) {
		$clean['postcode'] = $_POST['postcode'];
	}
}

if(isset($_POST['street'])) {
	if(ctype_alpha($_POST['street'])) {
		$clean['street'] = htmlentities($_POST['street'], ENT_QUOTES );
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
	if(ctype_alpha($_POST['gpname'])) {
		$clean['gpname'] = htmlentities($_POST['gpname'], ENT_QUOTES );
	}
}

if(isset($_POST['housing'])) {
	if(ctype_alpha($_POST['housing'])) {
		$clean['housing'] = htmlentities($_POST['housing'], ENT_QUOTES );
	}
}

if(isset($_POST['housetype'])) {
	if(ctype_alpha($_POST['housetype'])) {
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
	if(ctype_alnum($_POST['ailments'])) {
		$clean['ailments'] = htmlentities($_POST['ailments'], ENT_QUOTES );
	}
}

if(isset($_POST['note'])) {
	if(ctype_alnum($_POST['note'])) {
		$clean['note'] = htmlentities($_POST['note'], ENT_QUOTES );
	}
}

if(isset($_POST['description2'])) {
	if(ctype_alnum($_POST['description2'])) {
		$clean['description2'] = htmlentities($_POST['description2'], ENT_QUOTES );
	}
}

if(isset($_POST['contact1name'])) {
	if(ctype_alnum($_POST['contact1name'])) {
		$clean['contact1name'] = htmlentities($_POST['contact1name'], ENT_QUOTES );
	}
}

if(isset($_POST['contact1relationship'])) {
	if(ctype_alnum($_POST['contact1relationship'])) {
		$clean['contact1relationship'] = htmlentities($_POST['contact1relationship'], ENT_QUOTES );
	}
}

if(isset($_POST['contact1address'])) {
	if(ctype_alnum($_POST['contact1address'])) {
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
	if(ctype_alnum($_POST['contact2relationship'])) {
		$clean['contact2relationship'] = htmlentities($_POST['contact2relationship'], ENT_QUOTES );
	}
}

if(isset($_POST['contact2address'])) {
	if(ctype_alnum($_POST['contact2address'])) {
		$clean['contact2address'] = htmlentities($_POST['contact2address'], ENT_QUOTES );
	}
}

if(isset($_POST['contact2phone1'])) {
	$clean['contact2phone1'] = filter_phone_number($_POST['contact2phone1']);
}

if(isset($_POST['list'])) {
	if(ctype_alpha($_POST['list'])) {
		$clean['list'] = htmlentities($_POST['list'], ENT_QUOTES );
	}
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
 
$dbConnect = dbconnect();

if ($clean['submit']) {
    // fix DOB
    if( $dob) {
		if( ereg("([0-9]{2})/([0-1]{1}[0-9]{1})/([1-2]{1}[0-9]{3})", $dob, $reg)) {
		    $dob="$reg[3]-$reg[2]-$reg[1]";
		}
    }

	// here if no ID then editing  else adding
	if ($clean['clientid']) {
	    $sql  = "UPDATE clients SET firstname='" . $clean['firstname'] ."',";
	    $sql .= "lastname='" . $clean['lastname'] . "',title='" . $clean['title'] . "',houseno='" . $clean['houseno'] . "',";
	    $sql .= "postcode='" . $clean['postcode'] ."',street='" . $clean['street'] . "',phone1='" . $clean['phone1'] . "',";
	    $sql .= "phone2='" . $clean['phone2'] . "',dob='" . $clean['dob'] . "',gpname='" . $clean['gpname'] . "',";
	    $sql .= "housing='" . $clean['housing'] . "',housetype='" . $clean['housetype'] . "',referrer='" . $clean['referrer'] . "',";
	    $sql .= "alone='" . $clean['alone'] . "',ailments='" . $clean['ailments'] . "',note='" . $clean['note'] . "',";
	    $sql .= "description2='" . $clean['description2'] . "',";
	    $sql .= "contact1name='" . $clean['contact1name'] . "',";
	    $sql .= "contact1relationship='" . $clean['contact1relationship'] . "',";
	    $sql .= "contact1address='" . $clean['contact1address'] . "',";
	    $sql .= "contact1phone1='" . $clean['contact1phone1'] . "',contact2name='" . $clean['contact2name'] . "',";
	    $sql .= "contact2relationship='" . $clean['contact2relationship'] . "',";
	    $sql .= "contact2address='" . $clean['contact2address'] . "',";
	    $sql .= "contact2phone1='" . $clean['contact2phone1'] . "',list='" . $clean['list'] . "',";
	    $sql .= "timeslot='" . $clean['timeslot'] . "' WHERE clientid='" . $clean['clientid'] . "'";

	}
	else {
		$sql = "INSERT INTO clients (firstname,lastname,title,houseno,postcode,street,";
		$sql .= "phone1,phone2,housetype,dob,alone, ailments,contact1name,contact1relationship,";
		$sql .= "contact1address,contact1phone1,contact2name,contact2relationship, contact2address,";
		$sql .= "contact2phone1,gpname,referrer,housing,note,description2,list,timeslot)";
		$sql .= "VALUES ('" . $clean['firstname'] . "', '" . $clean['lastname'] . "','" . $clean['title'] . "', ";
		$sql .= "'" . $clean['houseno'] . "', '" . $clean['postcode'] . "', '" . $clean['street'] . "', '" . $clean['phone1'] . "',";
		$sql .= "'" . $clean['phone2'] . "', '" . $clean['housetype'] . "', '" . $clean['dob'] . "', '" . $clean['alone'] . "',";
		$sql .= "'" . $clean['ailments'] . "', '" . $clean['contact1name'] . "', '" . $clean['contact1relationship'] . "',";
		$sql .= "'" . $clean['contact1address'] . "', '" . $clean['contact1phone1'] . "','" . $clean['contact2name'] . "',";
		$sql .= "'" . $clean['contact2relationship'] . "', '" . $clean['contact2address'] . "', '" . $clean['contact2phone1'] . "',";
		$sql .= "'" . $clean['gpname'] . "', '" . $clean['referrer'] . "', '" . $clean['housing'] . "', '" . $clean['note'] . "',";
		$sql .= "'" . $clean['description2'] . "', '" . $clean['list'] . "', '" . $clean['timeslot'] . "')";

	}

	// run SQL against the DB
	$result = mysql_query($sql);
	if (!$result) {
		die('Invalid query: ' . mysql_error());
	}

	// log entry
/*	$logstr = "*** CLIENT RECORD AMENDED *** \n" . $reason;
	$callclass = "ADMIN";
	$sql  = "INSERT INTO calls (clientid, chat, class) ";
	$sql .= "VALUES ('$clientid', '$logstr', '$callclass')";
	$result = mysql_query($sql);
	if (!$result) {
		die('Invalid query: ' . mysql_error());
	}*/

	echo "Record updated/edited!<p>";
	print  "<a href=\" $PHP_SELF \">ADD/Select a RECORD</a><p>";

} // if ($submit)
elseif ($clean['delete']) {

// delete a record - actually move to black list

	$sql = "UPDATE clients SET list='black' WHERE clientid='" . $clean['clientid'] . "'";
	$result = mysql_query($sql);
	if (!$result) {
		die('Invalid query: ' . mysql_error());
	}

	echo "Client inactivated!p>";
	print  "<a href=\" $PHP_SELF \">ADD/Select a RECORD</a><p>";

}
else {

	// this part happens if we don't press submit
	if (!$clean['clientid']) {

	// print the list if there is not editing
	$result = mysql_query("SELECT clientid,firstname,lastname,list FROM clients ORDER BY lastname",$dbConnect);
	if (!$result) {
		die('Invalid query: ' . mysql_error());
	}


	print("<table>");
	while ($myrow = mysql_fetch_array($result)) {
		$out =  "<tr><td><img src=\"/images/" . $myrow["list"] . ".png\"";
		$out .= " width=\"16\" height=\"16\" border=\"0\"></td>";
		$out .= sprintf("<td><a href=\"%s?clientid=%s\">%s, %s</a></td>", 
						$PHP_SELF, $myrow["clientid"], $myrow["lastname"],$myrow["firstname"]);
		$out .= sprintf("<td><a href=\"%s?clientid=%s&delete=yes\">(Remove)</a></td></tr>", 
						$PHP_SELF, $myrow["clientid"]);

		print( $out);
	}
	print("</table>");
}

 ?>
<p>
<font face="Verdana, Arial, Helvetica, sans-serif" >
<a href="<?php echo $PHP_SELF?>">ADD/SELECT ANOTHER RECORD</a> <br>
</font>

<form onSubmit="return(vtslot(this.timeslot));" method="post" action="<?php echo $PHP_SELF?>">
<font face="Verdana, Arial, Helvetica, sans-serif">

<?php

if ($clean['clientid']) {

    // editing so select a record

	$sql = "SELECT * FROM clients WHERE (clients.clientid='" . $clean['clientid'] . "')";
	$result = mysql_query($sql, $dbConnect);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
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
	$clean['houseno']				= htmlentities($myrow['houseno'], ENT_QUOTES);
	$clean['postcode']				= htmlentities($myrow['postcode'], ENT_QUOTES);
	$clean['street']				= htmlentities($myrow['street'], ENT_QUOTES);
	$clean['phone1']				= htmlentities($myrow['phone1'], ENT_QUOTES);
	$clean['phone2']				= htmlentities($myrow['phone2'], ENT_QUOTES);
	$clean['housetype']				= htmlentities($myrow['housetype'], ENT_QUOTES);
	$clean['dob']					= htmlentities($myrow['dob'], ENT_QUOTES);
	$clean['startdate']				= htmlentities($myrow['startdate'], ENT_QUOTES);
	$clean['leavedate']				= htmlentities($myrow['leavedate'], ENT_QUOTES);
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
	$clean['housing']				= htmlentities($myrow['housing'], ENT_QUOTES);
	$clean['timeslot']				= htmlentities($myrow['timeslot'], ENT_QUOTES);
	$clean['list']					= htmlentities($myrow['list'], ENT_QUOTES);
	$clean['note']					= htmlentities($myrow['note'], ENT_QUOTES);
	$clean['description1']			= htmlentities($myrow['description1'], ENT_QUOTES);
	$clean['description2']			= htmlentities($myrow['description2'], ENT_QUOTES);
	$clean['done']					= htmlentities($myrow['done'], ENT_QUOTES);

//	$clientid = $myrow["clientid"];
  
	dbclose($dbConnect);
	
    ?>
  <input type=hidden name="clientid" value="<?php echo $clean['clientid'] ?>">
  <?php

} // if ($clean['clientid'])

//<!------------------->
//<!----HTML LAYOUT---->
//<!------------------->


    ?> </font>

<table width="100%" border="0" cellspacing="4" cellpadding="2">
<tr>
	<td bgcolor="#EEEEEE" colspan="2">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Edit Personal Details</b><br>
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
	<input type="Text" name="firstname" value="<?php echo $clean['firstname'] ?>" size="30" maxlength="30">
	</td>

	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="left" width="30%" VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Date of Birth: </b>
	</font>
	<input type="Text" name="dob" value="
	    <?php
		if($clean['dob']) {
			if( ereg( "([1-2]{1}[0-9]{3})-([0-1]{1}[0-9]{1})-([0-9]{2})", $clean['dob'], $reg)) {
				print("$reg[3]/$reg[2]/$reg[1]");
			}
			else {
				print("bad dob");
			}
		}
	  	?>
	 " size="11">
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
	<input type="Text" name="lastname" value="<?php echo $clean['lastname'] ?>" size="30" maxlength="30">
	</td>

	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="left" width="30%"  VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Alone: </b>
	</font>
	<input type="checkbox" name="alone" size="5" maxlength="5"
			<?php
				if($clean['alone']) {
					print(" checked");
				}
			?>
	>
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
	<b>House #: </b>
	</font>
	<input type="Text" name="houseno" value="<?php echo $clean['houseno'] ?>" size="30" maxlength="30">
	</td>

	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="left" width="30%"  VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Note: </b>
	</font>
	<textarea name="note" cols="27" rows="4" ><?php echo $clean['note'] ?> </textarea>
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
	<b>Street: </b>
	</font>
	<input type="Text" name="street" value="<?php echo $clean['street'] ?>" size="30">
	</td>

	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="left" width="30%"  VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Landlord: </b></font>
    <select name="housing">
         <option value="FCC">FCC</option>
         <option value="Owner Occupier">Owner Occupier</option>
         <option value="HA">HA - add note</option>
         <option value="other">Other</option>
         <option value="<?php echo $clean['housing'] ?>" selected><?php echo $clean['housing'] ?></option>
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
	<b>Post Code: </b>
	</font>
	<input type="Text" name="postcode" value="<?php echo $clean['postcode'] ?>" size="20">
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
		<option value="<?php echo $clean['housetype'] ?>" selected><?php echo $clean['housetype'] ?></option>
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
	<b>Phone Number 1: </b>
	</font>
	<input type="Text" name="phone1" value="<?php echo $clean['phone1'] ?>" size="15">
	</td>

	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="left" width="30%"  VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Phone Number 2: </b></font><input type="text" name="phone2" size="14" value="<?php echo $clean['phone2'] ?>">
	</td>

	<td ALIGN="left" width="15%"  VALIGN="top">
	</td>
</tr>
</TABLE>

<table width="100%" border="0" cellspacing="4" cellpadding="2">
<tr>
	<td bgcolor="#EEEEEE" colspan="2">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Edit Medical Details</b><br>
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
		<option selected value="<?php echo $clean['referrer'] ?>"><?php echo $clean['referrer'] ?></option>
	</select>
	</td>

	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="left" width="30%"  VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	<b>GP name: </b>
	</font>
	<input type="text" name="gpname" maxlength="30" size="30" value="<?php echo $clean['gpname'] ?>">
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
	<input type="text" name="description2" size="25" value="<?php echo $clean['description2']  ?>" maxlength="40">
	</td>

	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="left" width="30%"  VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Ailments: </b>
	</font>
	<input type="text" name="ailments" size="30" value="<?php echo $clean['ailments']?>" maxlength="150">
	</td>

	<td ALIGN="left" width="15%"  VALIGN="top">
	</td>
</tr>
</TABLE>

<table width="100%" border="0" cellspacing="4" cellpadding="2">
<tr>
	<td bgcolor="#EEEEEE" colspan="2">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Edit Home Help Details</b><br>
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
	<input type="text" name="contact1name" value="<?php echo $clean['contact1name']  ?>" size="22" maxlength="30">
	</td>

	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="left" width="30%"  VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Phone: </b>
	</font>
	<input type="text" name="contact1phone1" size="15" maxlength="15" value="<?php echo $clean['contact1phone1'] ?>">
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
	<input type="text" name="contact1address" size="32" maxlength="50" value="<?php echo $clean['contact1address'] ?>">
	</td>

	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="left" width="30%"  VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Relationship: </b>
	</font>
	<input type="text" name="contact1relationship" size="20" value="<?php echo $clean['contact1relationship'] ?>">
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
	<input type="text" name="contact2name" value="<?php echo $clean['contact2name']  ?>" size="22" maxlength="30">
	</td>

	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="left" width="30%"  VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Phone: </b>
	</font>
	<input type="text" name="contact2phone1" size="15" maxlength="15" value="<?php echo $clean['contact2phone1'] ?>">
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
	<input type="text" name="contact2address" size="32" maxlength="50" value="<?php echo $clean['contact2address'] ?>">
	</td>

	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="left" width="30%"  VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Relationship: </b>
	</font>
	<input type="text" name="contact2relationship" size="20" value="<?php echo $clean['contact2relationship'] ?>">
	</td>

	<td ALIGN="left" width="15%"  VALIGN="top">
	</td>
</tr>
</TABLE>

<table width="100%" border="0" cellspacing="4" cellpadding="2">
<tr>
	<td bgcolor="#EEEEEE" colspan="2">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Enter Change Details</b><br>
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
	<b>Timeslot: </b>
	</font>
	<input type="text" name="timeslot" value="
		<?php
		if($timeslot) {
			if(ereg("([0-9){2}):([0-9]{2}):([0-9]{2})", $clean['timeslot'], $reg)) {
				print("$reg[1]:$reg[2]");
			}
		}
		?>
	" size="6" maxlength="5">
	<font size="1" color="#FF0000">
	<b>24 hour format (HH:MM)</b>
	</font>
	</td>

	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="left" width="30%"  VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Colour: </b>
	</font>
	<select name="list">
		<option value="<?php echo $clean['list'] ?>" selected><?php echo $clean['list']  ?></option>
		<option value="black">black</option>
		<option value="blue">blue</option>
		<option value="cyan">cyan</option>
		<option value="green">green</option>
		<option value="grey">grey</option>
		<option value="magenta">magenta</option>
		<option value="olive">olive</option>
		<option value="red">red</option>
		<option value="yellow">yellow</option>
	</select>
	</td>

	<td ALIGN="left" width="15%"  VALIGN="top">
	</td>
</tr>
</TABLE>

<TABLE BORDER=0 WIDTH=100%>
<tr>
	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="center"  width="90%" VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	<b>Give Reason for Change and your initials: (e.g. &quot;New Contact - zz&quot;)</b>
	</font>
	<input type="text" name="reason" size="50" maxlength="50">
	</td>

	<td ALIGN="right" width="10%" VALIGN="top">
	</td>

	<td ALIGN="left" width="30%"  VALIGN="top">
	<font face="verdana, arial, helvetica" size="-1">
	</td>

	<td ALIGN="left" width="15%"  VALIGN="top">
	</td>
</tr>
</TABLE>

<br>

<div align="center">
<font face="Verdana, Arial, Helvetica, sans-serif">
<input type="Submit" name="submit" value="Enter information">
</font>
</div>

</form>
<?php
}
?>

</body>
</html>
