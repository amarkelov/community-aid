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



  ?> </font> 

<table width="100%" border="0" cellpadding="2" height="621">
    <tr> 
      <td> 
	First name:<br>
          <input type="Text" name="firstname" value="<?php echo $firstname ?>" size="20" maxlength="30">
      </td>
      <td> 
	Last name:<br>
          <input type="Text" name="lastname" value="<?php echo $lastname ?>" size="20" maxlength="30">
      </td>
      <td> 
      </td>
    </tr>
    <tr> 
      <td> 
	House #:<br> 
        <input type="Text" name="houseno" value="<?php echo $houseno ?>" size="11">
      </td>
      <td> 
	Street:<br>
          <input type="Text" name="street" value="<?php echo $street ?>" size="30">
      </td>
      <td> 
      Post Code:<br>
          <input type="Text" name="postcode" value="<?php echo $postcode ?>" size="6">
      </td>
    </tr>
    <tr> 
      <td> 
      </td>
      <td> 
      Phone 1:<br>
          <input type="Text" name="phone1" value="<?php echo $phone1 ?>" size="14">
      </td>
      <td> 
	Phone 2:<br>
          <input type="text" name="phone2" size="14" value="<?php echo $phone2 ?>">
      </td>
    </tr>
    <tr> 
      <td height="7"> 
	D.o.B.:<br>
          <input type="Text" name="dob" value="<?php echo $dob ?>" size="11">
          <br><br>
        Alone: 
          <input type="checkbox" name="alone" size="5" maxlength="5" 
	  <?php 
	  if($alone) {
	    print(" checked");
	  }
	  ?>
      </td>
      <td height="7"> 
          Note:<br>
          <textarea name="note" cols="40" rows="5" >  <?php echo $note ?> </textarea>
      </td>
      <td height="7"> 
          Landlord:<br>
          <select name="housing">
            <option value="FCC">FCC</option>
            <option value="Owner Occupier">Owner Occupier</option>
            <option value="HA">HA - add note</option>
            <option value="other">Other</option>
            <option value="<?php echo $housing ?>" selected><?php echo $housing ?></option>
          </select>
          <br>
          House Type:<br>
          <select name="housetype">
            <option value="terraced">terraced</option>
            <option value="semi-detached">semi-detached</option>
            <option value="detached">detached</option>
            <option value="high rise">high rise</option>
            <option value="deck access flat">deck access flat</option>
            <option value="cottage flat">cottage flat</option>
            <option value="tenement">tenement</option>
            <option value="sheltered">sheltered</option>
            <option value="other">other</option>

            <option value="<?php echo $housetype ?>" selected><?php echo $housetype ?></option>
          </select>
      </td>
    </tr>
    <tr> 
      <td colspan="3" height="20"> 
          <hr noshade>
      </td>
    </tr>
    <tr> 
      <td height="97"> 
            Referrer:<br>
            <select name="referrer">
              <option value="Health Board">Health Board</option>
              <option value="Social Work">Social Work</option>
              <option value="Private">Private</option>
              <option value="Voluntary Group">Voluntary Group</option>
              <option value="Self-Referred">Self Referred</option>
              <option selected value="<?php echo $referrer ?>"><?php echo $referrer ?></option>
            </select>
	    <br>
            Other:<br>
            <input type="text" name="description2" size="25" value="<?php echo $description2  ?>" maxlength="40">
      </td>
      <td colspan="2" height="97"> 
	  GP name:<br>
            <input type="text" name="gpname" maxlength="30" size="30" value="<?php echo $gpname ?>">
	  <br>
	  Ailments:<br>
            <input type="text" name="ailments" size="72" value="<?php echo $ailments?>" maxlength="250">
      </td>
    </tr>
    <tr> 
      <td colspan="3" height="14"> 
          <hr noshade>
      </td>
    </tr>
    <tr> 
      <td colspan="2"> 
	Contact name 1:<br>
          <input type="text" name="contact1name" value="<?php echo $contact1name  ?>" size="30" maxlength="30">
          <br>
        Address:<br>
          <input type="text" name="contact1address" size="50" maxlength="50" value="<?php echo $contact1address ?>">
          </font></div>
      </td>
      <td> 
          Phone:<br>
          <input type="text" name="contact1phone1" size="15" maxlength="15" value="<?php echo $contact1phone1 ?>">
          <br>
          Relationship:<br>
          <input type="text" name="contact1relationship" size="20" value="<?php echo $contact1relationship ?>">
      </td>
    </tr>
    <tr> 
      <td colspan="2" height="13">&nbsp;</td>
      <td height="13">&nbsp;</td>
    </tr>
    <tr> 
      <td colspan="2"> 
	Contact name 2:<br>
          <input type="text" name="contact2name" value="<?php echo $contact2name  ?>" size="30" maxlength="30">
          <br>
        Address:<br>
          <input type="text" name="contact2address" size="50" maxlength="50" value="<?php echo $contact2address ?>">
      </td>
      <td> 
          Phone:<br>
          <input type="text" name="contact2phone1" size="15" maxlength="15" value="<?php echo $contact2phone1?>">
          Relationship:<br>
          <input type="text" name="contact2relationship" size="20" value="<?php echo $contact2relationship ?>">
      </td>
    </tr>
    <tr> 
      <td colspan="3"> 
        <div align="right"> 
          <hr noshade>
      </td>
    </tr>
    <tr> 
      <td> 
      </td>
      <td> 
          Timeslot:<br>
          <input type="text" name="timeslot" value="<?php echo $timeslot  ?>" size="6" maxlenght="5">
	  <b><font size="1" color="#FF0000">24 hour format (HH:MM)</font></b>
      </td>
      <td> 
          List:<br>
          <select name="list">
            <option value="<?php echo $list ?>" selected><?php echo $list  ?></option>
            <option value="magenta">magenta</option>
            <option value="red">red</option>
            <option value="yellow">yellow</option>
            <option value="green">green</option>
            <option value="olive">olive</option>
            <option value="cyan">cyan</option>
            <option value="blue">blue</option>
            <option value="grey">grey</option>
            <option value="black">black</option>

          </select>
      </td>
    </tr>
    <tr> 
      <td height="12">
      Give Reason for Change and your initials;<br>
      e.g. &quot;New Contact - zz&quot;.
      <br>
      </td>
      <td height="12"> 
        <input type="text" name="reason" size="50" maxlength="50">
      </td>
      <td height="12"> </td>
    </tr>
</table>

<div align="center"><font face="Verdana, Arial, Helvetica, sans-serif">
<input type="Submit" name="submit" value="Enter information">

</font></div>
</form> 
<?php
}
?>
</body>
</html>
