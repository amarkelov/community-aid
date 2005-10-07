<html>
<head>
<title>Edit Client List -- Good Morning Blanchardstown</title>
<meta http-equiv="expires" content="-1">
<meta http-equiv="Cache-Control" content="no-cache">
</head>
<body bgcolor="#BEC8FD">

<p>
<?php

require 'functions.inc';

$db = "gmpDb";
//$db = $HTTP_HOST . "Db";
$dbConnect = mysql_connect("localhost", "gmadmin", "old290174");
if (!$dbConnect) {
	die('Could not connect: ' . mysql_error());
}

mysql_select_db($db,$dbConnect);

if ($submit) {
    // fix DOB
    if( $dob) {
	if( ereg("([0-9]{2})/([0-1]{1}[0-9]{1})/([1-2]{1}[0-9]{3})", $dob, $reg)) {
	    $dob="$reg[3]-$reg[2]-$reg[1]";
	}
    }

	// here if no ID then editing  else adding
	if ($clientid) {
	    // convert on or off into 1 or 0
	    if(strtoupper($alone) == 'ON') {
		$alone = 1;
	    }
	    else {
		$alone = 0;
	    }

	    $sql  = "UPDATE clients SET firstname='$firstname',";
	    $sql .= "lastname='$lastname',title='$title',houseno='$houseno',";
	    $sql .= "postcode='$postcode',street='$street',phone1='$phone1',";
	    $sql .= "phone2='$phone2',dob='$dob',gpname='$gpname',";
	    $sql .= "housing='$housing',housetype='$housetype',referrer='$referrer',";
	    $sql .= "alone='$alone',ailments='$ailments',note='$note',";
	    $sql .= "description1='$description1',description2='$description2',";
	    $sql .= "contact1name='$contact1name',";
	    $sql .= "contact1relationship='$contact1relationship',";
	    $sql .= "contact1address='$contact1address',";
	    $sql .= "contact1phone1='$contact1phone1',contact2name='$contact2name',";
	    $sql .= "contact2relationship='$contact2relationship',";
	    $sql .= "contact2address='$contact2address',";
	    $sql .= "contact2phone1='$contact2phone1',list='$list',";
	    $sql .= "timeslot='$timeslot' WHERE clientid=$clientid";

	}
	else {

		$sql = "INSERT INTO clients (firstname,lastname,title,houseno,postcode,street,phone1,phone2,housetype,dob,alone, ailments,contact1name,contact1relationship,contact1address,contact1phone1,contact2name,contact2relationship, contact2address,contact2phone1,gpname,referrer,housing,note,description1,description2,list,timeslot)
	VALUES ('$firstname', '$lastname','$title', '$houseno', '$postcode', '$street', '$phone1', '$phone2', '$housetype', '$dob', '$alone', '$ailments', '$contact1name', '$contact1relationship', '$contact1address', '$contact1phone1',' $contact2name', '$contact2relationship', '$contact2address', '$contact2phone1','$gpname', '$referrer', '$housing', '$note', '$description1', '$description2', '$list', '$timeslot')";

	}

	// run SQL against the DB
	$result = mysql_query($sql);
	if (!$result) {
		die('Invalid query: ' . mysql_error());
	}

	// log entry
	$logstr = "*** CLIENT RECORD AMENDED *** \n" . $reason;
	$callclass = "ADMIN";
	$sql  = "INSERT INTO calls (clientid, chat, class) ";
	$sql .= "VALUES ('$clientid', '$logstr', '$callclass')";
	$result = mysql_query($sql);
	if (!$result) {
		die('Invalid query: ' . mysql_error());
	}

	echo "Record updated/edited!<p>";
	print  "<a href=\" $PHP_SELF \">ADD/Select a RECORD</a><p>";

} // if ($submit)
elseif ($delete) {

// delete a record - actually move to black list

	$sql = "UPDATE clients SET list='black' WHERE clientid=$clientid";
	$result = mysql_query($sql);
	if (!$result) {
		die('Invalid query: ' . mysql_error());
	}

	echo "Client inactivated!p>";
	print  "<a href=\" $PHP_SELF \">ADD/Select a RECORD</a><p>";

}
else {

	// this part happens if we don't press submit
	if (!$clientid) {

	// print the list if there is not editing
	$result = mysql_query("SELECT * FROM clients ORDER BY lastname",$dbConnect);
	if (!$result) {
		die('Invalid query: ' . mysql_error());
	}


	print("<table>");
	while ($myrow = mysql_fetch_array($result)) {
		$out =  "<tr><td><img src=\"images/";
		$out .= $myrow["list"];
		$out .= ".png\" width=\"16\" height=\"16\" border=\"0\"></td>";
		$out .= sprintf("<td><a href=\"%s?clientid=%s\">%s, %s</a></td>", $PHP_SELF, $myrow["clientid"], $myrow["lastname"],$myrow["firstname"]);
		$out .= sprintf("<td><a href=\"%s?clientid=%s&delete=yes\">(Remove)</a></td></tr>", $PHP_SELF, $myrow["clientid"]);

		print( $out);
	}
	print("</table>");
}

 ?>
<p>
<font face="Verdana, Arial, Helvetica, sans-serif" >
<a href="<?php echo $PHP_SELF?>">ADD/SELECT ANOTHER RECORD</a> <br>
</font>

<form method="post" action="<?php echo $PHP_SELF?>">
<font face="Verdana, Arial, Helvetica, sans-serif">

<?php

if ($clientid) {

    // editing so select a record

	$sql = "SELECT * FROM clients WHERE (clients.clientid=$clientid)";
	$result = mysql_query($sql, $dbConnect);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}
	$myrow = mysql_fetch_array($result);

	$clientid = $myrow["clientid"];
	$firstname=$myrow["firstname"];
	$lastname=stripslashes($myrow["lastname"]);
	$initials=$myrow["initials"];
	$title=$myrow["title"];
	$houseno=$myrow["houseno"];
	$postcode=$myrow["postcode"];
	$street=$myrow["street"];
	$phone1=$myrow["phone1"];
	$phone2=$myrow["phone2"];
	$housetype=$myrow["housetype"];
	$dob=$myrow["dob"];
	$startdate=$myrow["startdate"];
	$leavedate=$myrow["leavedate"];
	$alone=$myrow["alone"];
	$ailments=$myrow["ailments"];
	$contact1name=stripslashes($myrow["contact1name"]);
	$contact1relationship=$myrow["contact1relationship"];
	$contact1address=$myrow["contact1address"];
	$contact1phone1=$myrow["contact1phone1"];
	$contact1phone2=$myrow["contact1phone2"];
	$contact2name=stripslashes($myrow["contact2name"]);
	$contact2relationship=$myrow["contact2relationship"];
	$contact2address=$myrow["contact2address"];
	$contact2phone1=$myrow["contact2phone1"];
	$contact2phone2=$myrow["contact2phone2"];
	$gpname=stripslashes($myrow["gpname"]);
	$referrer=$myrow["referrer"];
	$housing=$myrow["housing"];
	$timeslot=$myrow["timeslot"];
	$list=$myrow["list"];
	$note=$myrow["note"];
	$description1=$myrow["description1"];
	$description2=$myrow["description2"];
    // print the clientid for editing

    ?>
  <input type=hidden name="clientid" value="<?php echo $clientid ?>">
  <?php

} // if ($clientid)

//<!------------------->
//<!----HTML LAYOUT---->
//<!------------------->


    ?> </font>

 <table width="100%" border="0" cellspacing="4" cellpadding="2">
<tr><td bgcolor="#EEEEEE" colspan="2"><font face="verdana, arial, helvetica" size="-1"><b>Edit Personal Details</b></font><br></td></tr>
<br>
<br>

<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="15%" VALIGN="top">
</td>
<td ALIGN="right" width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>First name:</b></font><input type="Text" name="firstname" value="<?php echo $firstname ?>" size="30" maxlength="30">
</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>


<td ALIGN="left" width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Date of Birth:</b></font>
            <input type="Text" name="dob" value="<?php
  	  if($dob) {
                if( ereg( "([1-2]{1}[0-9]{3})-([0-1]{1}[0-9]{1})-([0-9]{2})", $dob, $reg)) {
                  print("$reg[3]/$reg[2]/$reg[1]");
                }
                else {
  		print("bad dob");
  	      }
  	  }
  	  ?>" size="11">
</td>
<td ALIGN="right" width="15%"  VALIGN="top"></td>
</TABLE>



<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="15%" VALIGN="top">
</td>
<td ALIGN="right" width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Last name:</b></font><input type="Text" name="lastname" value="<?php echo $lastname ?>" size="30" maxlength="30">
</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="left" width="30%"  VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Alone: </b></font>
           <input type="checkbox" name="alone" size="5" maxlength="5"
		     	  <?php
		     	  if($alone) {
		     	    print(" checked");
  	  }
?>

</td>
<td ALIGN="left" width="15%"  VALIGN="top"></td>
</TABLE>


<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="15%" VALIGN="top">
</td>
<td ALIGN="right"  width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>House #:</b></font><input type="Text" name="houseno" value="<?php echo $houseno ?>" size="30" maxlength="30">
</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="left" width="30%"  VALIGN="top"><font face="verdana, arial, helvetica" size="-1"><b>Note:</b></font>
           <textarea name="note" cols="27" rows="4" ><?php echo $note ?> </textarea>
  </td>
<td ALIGN="left" width="15%"  VALIGN="top"></td>
</tr>
</TABLE>



<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="15%" VALIGN="top">
</td>
<td ALIGN="right"  width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Street:</b></font><input type="Text" name="street" value="<?php echo $street ?>" size="30">
</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="left" width="30%"  VALIGN="top"><font face="verdana, arial, helvetica" size="-1"><b>Landlord:</b></font>
           <select name="housing">
		                 <option value="FCC">FCC</option>
		                 <option value="Owner Occupier">Owner Occupier</option>
		                 <option value="HA">HA - add note</option>
		                 <option value="other">Other</option>
		                 <option value="<?php echo $housing ?>" selected><?php echo $housing ?></option>
            </select>
  </td>
<td ALIGN="left" width="15%"  VALIGN="top"></td>
</tr>
</TABLE>



<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="15%" VALIGN="top">
</td>
<td ALIGN="right"  width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Post Code:</b></font> <input type="Text" name="postcode" value="<?php echo $postcode ?>" size="20">
</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="left" width="30%"  VALIGN="top"><font face="verdana, arial, helvetica" size="-1"><b>House Type:</b></font>
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
              <option value="<?php echo $housetype ?>" selected><?php echo $housetype ?></option>
            </select>
  </td>
<td ALIGN="left" width="15%"  VALIGN="top"></td>
</tr>
</TABLE>



<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="15%" VALIGN="top">
</td>
<td ALIGN="right"  width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Phone Number 1:</b></font><input type="Text" name="phone1" value="<?php echo $phone1 ?>" size="15">
</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="left" width="30%"  VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Phone Number 2:</b></font><input type="text" name="phone2" size="14" value="<?php echo $phone2 ?>">
  </td>
<td ALIGN="left" width="15%"  VALIGN="top"></td>
</tr>
</TABLE>

<table width="100%" border="0" cellspacing="4" cellpadding="2">
<tr><td bgcolor="#EEEEEE" colspan="2"><font face="verdana, arial, helvetica" size="-1"><b>Edit Medical Details</b></font><br></td></tr>



<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="15%" VALIGN="top">
</td>
<td ALIGN="right"  width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Referrer:</b></font>
<select name="referrer">
                <option value="Health Board">Health Board</option>
                <option value="Social Work">Social Work</option>
                <option value="Private">Private</option>
                <option value="Voluntary Group">Voluntary Group</option>
                <option value="Self-Referred">Self Referred</option>
                <option selected value="<?php echo $referrer ?>"><?php echo $referrer ?></option>
              </select>
</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="left" width="30%"  VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>GP name:</b></font><input type="text" name="gpname" maxlength="30" size="30" value="<?php echo $gpname ?>">
  </td>
<td ALIGN="left" width="15%"  VALIGN="top"></td>
</tr>
</TABLE>



<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="15%" VALIGN="top">
</td>
<td ALIGN="right"  width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Other:</b></font><input type="text" name="description2" size="25" value="<?php echo $description2  ?>" maxlength="40">

</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="left" width="30%"  VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Ailments:</b></font><input type="text" name="ailments" size="30" value="<?php echo $ailments?>" maxlength="150">
</td>
  </td>
<td ALIGN="left" width="15%"  VALIGN="top"></td>
</tr>
</TABLE>



<table width="100%" border="0" cellspacing="4" cellpadding="2">
<tr><td bgcolor="#EEEEEE" colspan="2"><font face="verdana, arial, helvetica" size="-1"><b>Edit Home Help Details</b></font><br></td></tr>




<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="15%" VALIGN="top">
</td>
<td ALIGN="right"  width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Contact name 1:</b></font><input type="text" name="contact1name" value="<?php echo $contact1name  ?>" size="22" maxlength="30">

</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="left" width="30%"  VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Phone:</b></font><input type="text" name="contact1phone1" size="15" maxlength="15" value="<?php echo $contact1phone1 ?>">
</td>
  </td>
<td ALIGN="left" width="15%"  VALIGN="top"></td>
</tr>
</TABLE>


<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="15%" VALIGN="top">
</td>
<td ALIGN="right"  width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Address:</b></font><input type="text" name="contact1address" size="32" maxlength="50" value="<?php echo $contact1address ?>">

</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="left" width="30%"  VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Relationship:</b></font><input type="text" name="contact1relationship" size="20" value="<?php echo $contact1relationship ?>">
</td>
  </td>
<td ALIGN="left" width="15%"  VALIGN="top"></td>
</tr>
</TABLE>




<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="15%" VALIGN="top">
</td>
<td ALIGN="right"  width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Contact name 2:</b></font><input type="text" name="contact2name" value="<?php echo $contact2name  ?>" size="22" maxlength="30">

</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="left" width="30%"  VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Phone:</b></font><input type="text" name="contact2phone1" size="15" maxlength="15" value="<?php echo $contact2phone1?>">
</td>
  </td>
<td ALIGN="left" width="15%"  VALIGN="top"></td>
</tr>
</TABLE>


<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="15%" VALIGN="top">
</td>
<td ALIGN="right"  width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Address:</b></font><input type="text" name="contact2address" size="32" maxlength="50" value="<?php echo $contact2address ?>">

</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="left" width="30%"  VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Relationship:</b></font><input type="text" name="contact2relationship" size="20" value="<?php echo $contact2relationship ?>">
</td>
  </td>
<td ALIGN="left" width="15%"  VALIGN="top"></td>
</tr>
</TABLE>



<table width="100%" border="0" cellspacing="4" cellpadding="2">
<tr><td bgcolor="#EEEEEE" colspan="2"><font face="verdana, arial, helvetica" size="-1"><b>Enter Change Details</b></font><br></td></tr>


<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="15%" VALIGN="top">
</td>
<td ALIGN="right"  width="30%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Timeslot:</b></font>
<input type="text" name="timeslot" value="<?php
  	  if($timeslot) {
  	  	if(ereg("([0-9){2}):([0-9]{2}):([0-9]{2})", $timeslot, $reg)) {
  			print("$reg[1]:$reg[2]");
  		}
  	  }?>" size="6" maxlength="5">
  	  <b><font size="1" color="#FF0000">24 hour format (HH:MM)</font></b>

</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="left" width="30%"  VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Colour:</b></font>
<select name="list">
              <option value="<?php echo $list ?>" selected><?php echo $list  ?></option>
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
  </td>
<td ALIGN="left" width="15%"  VALIGN="top"></td>
</tr>
</TABLE>


<TABLE BORDER=0 WIDTH=100%>
<tr>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="center"  width="90%" VALIGN="top">
<font face="verdana, arial, helvetica" size="-1"><b>Give Reason for Change and your initials: (e.g. &quot;New Contact - zz&quot;)</b></font><input type="text" name="reason" size="50" maxlength="50">

</td>
<td ALIGN="right" width="10%" VALIGN="top">
</td>
<td ALIGN="left" width="30%"  VALIGN="top">
<font face="verdana, arial, helvetica" size="-1">
</td>
  </td>
<td ALIGN="left" width="15%"  VALIGN="top"></td>
</tr>
</TABLE>


<br>


  <div align="center"><font face="Verdana, Arial, Helvetica, sans-serif">
  <input type="Submit" name="submit" value="Enter information">

  </font></div>
  </form>
  <?php
  }
  ?>
  </body>
  </html>
