<html>
<head>
<title>Edit Client List -- Good Morning Blanchardstown</title> 
<meta http-equiv="expires" content="-1">
<meta http-equiv="Cache-Control" content="no-cache">
</head>
<body bgcolor="#99CCCC">
<font face="Verdana, Arial, Helvetica, sans-serif" > 

<p align="right">
<i><b><font size="5" color="#FF9999">
Good Morning Blanchardstown Project
</font></b></i> 
</p>

<p align="left">&nbsp; </p>
<?php

$db = "gmpDb";
//$db = $HTTP_HOST . "Db";
$dbConnect = mysql_connect("localhost", "root", "old290174");
if (!$dbConnect) {
	die('Could not connect: ' . mysql_error());
}

mysql_select_db($db,$dbConnect);

if ($submit) {

	// here if no ID then editing  else adding 
	if ($clientid) {

		$sql = "UPDATE clients SET firstname='$firstname',lastname='$lastname',title='$title',houseno='$houseno',postcode='$postcode',street='$street',phone1='$phone1',phone2='$phone2',dob='$dob',gpname='$gpname',housing='$housing',housetype='$housetype',referrer='$referrer',alone='$alone',ailments='$ailments',note='$note',description1='$description1',description2='$description2',contact1name='$contact1name',contact1relationship='$contact1relationship',contact1address='$contact1address',contact1phone1='$contact1phone1',contact2name='$contact2name',contact2relationship='$contact2relationship',contact2address='$contact2address',contact2phone1='$contact2phone1',list='$list',timeslot='$timeslot'  
WHERE clientid=$clientid";

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
<div align="right"><i><b></b></i></div>
<P> <a href="<?php echo $PHP_SELF?>">ADD/SELECT ANOTHER RECORD</a> <br>
</font> 
<form method="post" action="<?php echo $PHP_SELF?>">
  <font face="Verdana, Arial, Helvetica, sans-serif">
<?php



if ($clientid) {

	// editing so select a record
	$sql = "SELECT * FROM clients WHERE clientid=$clientid";

	$result = mysql_query($sql);
	if (!$result) {
		die('Invalid query: ' . mysql_error());
	}

	$myrow = mysql_fetch_array($result);

	$clientid = $myrow["clientid"];
	$firstname=$myrow["firstname"];
	$lastname=$myrow["lastname"];
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
	$contact1name=$myrow["contact1name"];
	$contact1relationship=$myrow["contact1relationship"];
	$contact1address=$myrow["contact1address"];
	$contact1phone1=$myrow["contact1phone1"];
	$contact1phone2=$myrow["contact1phone2"];
	$contact2name=$myrow["contact2name"];
	$contact2relationship=$myrow["contact2relationship"];
	$contact2address=$myrow["contact2address"];
	$contact2phone1=$myrow["contact2phone1"];
	$contact2phone2=$myrow["contact2phone2"];
	$gpname=$myrow["gpname"];
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
  <div align="right"></div>
  <table width="100%" border="0" cellpadding="2" height="621">
    <tr> 
      <td> 
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif"> 
          </font></div>
      </td>
      <td> 
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif">First 
          name: 
          <input type="Text" name="firstname" value="<?php echo $firstname ?>" size="20" maxlength="30">
          </font></div>
      </td>
      <td> 
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif">Last 
          name: 
          <input type="Text" name="lastname" value="<?php echo $lastname ?>" size="20" maxlength="30">
          </font></div>
      </td>
    </tr>
    <tr> 
      <td> 
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif">HouseNo 
          : 
          <input type="Text" name="houseno" value="<?php echo $houseno ?>" size="11">
          </font></div>
      </td>
      <td> 
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif">Street: 
          <input type="Text" name="street" value="<?php echo $street ?>" size="30">
          </font></div>
      </td>
      <td> 
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif">PostCode: 
          <input type="Text" name="postcode" value="<?php echo $postcode ?>" size="6">
          </font></div>
      </td>
    </tr>
    <tr> 
      <td> 
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"></font></font></font></div>
      </td>
      <td> 
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif">Phone 
          1 
          <input type="Text" name="phone1" value="<?php echo $phone1 ?>" size="14">
          </font></div>
      </td>
      <td> 
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif">Phone 
          2 
          <input type="text" name="phone2" size="14" value="<?php echo $phone2 ?>">
          </font></div>
      </td>
    </tr>
    <tr> 
      <td height="7"> 
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif">D.o.B. 
          <input type="Text" name="dob" value="<?php echo $dob ?>" size="11">
          <br>
          Alone: 
          <input type="text" name="alone" size="5" maxlength="5" value="<?php echo $alone ?>">
          </font></div>
      </td>
      <td height="7"> 
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif"> 
          Note: 
          <textarea name="note" cols="40" rows="5" >  <?php echo $note ?> </textarea>
          </font> </div>
      </td>
      <td height="7"> 
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif"> 
          Landlord: 
          <select name="housing">
            <option value="FCC">FCC</option>
            <option value="Owner Occupier">Owner Occupier</option>
            <option value="HA">HA - add note</option>
            <option value="other">Other</option>
            <option value="<?php echo $housing ?>" selected><?php echo $housing ?></option>
          </select>
          <br>
          House Type: 
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
          </font></div>
      </td>
    </tr>
    <tr> 
      <td colspan="3" height="20"> 
        <div align="right"> 
          <hr noshade>
          <font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"></font></font></font></div>
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"></font></font></font></div>
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif"> 
          </font></div>
      </td>
    </tr>
    <tr> 
      <td height="97"> 
        <div align="right"> 
          <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif"> 
            Referrer: 
            <select name="referrer">
              <option value="Health Board">Health Board</option>
              <option value="Social Work">Social Work</option>
              <option value="Private">Private</option>
              <option value="Voluntary Group">Voluntary Group</option>
              <option value="Self-Referred">Self Referred</option>
              <option selected value="<?php echo $referrer ?>"><?php echo $referrer ?></option>
            </select>
            </font></div>
          <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif"> 
            other: 
            <input type="text" name="description2" size="25" value="<?php echo $description2  ?>" maxlength="40">
            </font></div>
          <font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"></font></font></font></div>
      </td>
      <td colspan="2" height="97"> 
        <div align="right">
          <p><font size="2" face="Verdana, Arial, Helvetica, sans-serif">GP name 
            <input type="text" name="gpname" maxlength="30" size="30" value="<?php echo $gpname ?>">
            </font></p>
          <p><font size="2" face="Verdana, Arial, Helvetica, sans-serif" color="#99CCCC">___</font><font size="2" face="Verdana, Arial, Helvetica, sans-serif">Ailments:</font><font size="2" face="Verdana, Arial, Helvetica, sans-serif"> 
            <input type="text" name="ailments" size="72" value="<?php echo $ailments?>" maxlength="250">
            </font></p>
        </div>
        </td>
    </tr>
    <tr> 
      <td colspan="3" height="14"> 
        <div align="right"> 
          <hr noshade>
          <font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"></font></font></font></div>
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"></font></font></font></div>
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"></font></font></font></div>
      </td>
    </tr>
    <tr> 
      <td colspan="2"> 
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"></font></font></font></div>
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif">Contact1 
          name 
          <input type="text" name="contact1name" value="<?php echo $contact1name  ?>" size="30" maxlength="30">
          <br>
          address: 
          <input type="text" name="contact1address" size="50" maxlength="50" value="<?php echo $contact1address ?>">
          </font></div>
      </td>
      <td> 
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"> 
          phone no: </font><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"> 
          <input type="text" name="contact1phone1" size="15" maxlength="15" value="<?php echo $contact1phone1 ?>">
          </font></font></font><font size="2"><br>
          relationship: 
          <input type="text" name="contact1relationship" size="20" value="<?php echo $contact1relationship ?>">
          </font></font></font></div>
      </td>
    </tr>
    <tr> 
      <td colspan="2" height="13">&nbsp;</td>
      <td height="13">&nbsp;</td>
    </tr>
    <tr> 
      <td colspan="2"> 
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"></font></font></font></div>
        <div align="right"><font size="2" face="Verdana, Arial, Helvetica, sans-serif">Contact2 
          name 
          <input type="text" name="contact2name" value="<?php echo $contact2name  ?>" size="30" maxlength="30">
          <br>
          address: 
          <input type="text" name="contact2address" size="50" maxlength="50" value="<?php echo $contact2address ?>">
          </font></div>
      </td>
      <td> 
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"> 
          phone no: </font><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"> 
          <input type="text" name="contact2phone1" size="15" maxlength="15" value="<?php echo $contact2phone1?>">
          </font></font></font><font size="2"><br>
          relationship: 
          <input type="text" name="contact2relationship" size="20" value="<?php echo $contact2relationship ?>">
          </font></font></font></div>
      </td>
    </tr>
    <tr> 
      <td colspan="3"> 
        <div align="right"> 
          <hr>
          <font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"></font></font></font></div>
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"></font></font></font></div>
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"></font></font></font></div>
      </td>
    </tr>
    <tr> 
      <td> 
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"></font></font></font></div>
      </td>
      <td> 
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"> 
          timeslot: 
          <input type="text" name="timeslot" value="<?php echo $timeslot  ?>" size="10" />
          </font></font></font></div>
      </td>
      <td> 
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif"><font face="Verdana, Arial, Helvetica, sans-serif"><font size="2"> 
          list: 
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
          </font></font></font></div>
      </td>
    </tr>
    <tr> 
      <td height="12"><font face="Verdana, Arial, Helvetica, sans-serif" size="2">Give 
        Reason for Change and your initials ; e.g. &quot;New Contact - zz&quot;.<br>
        </font> </td>
      <td height="12"> 
        <input type="text" name="reason" size="50" maxlength="50">
      </td>
      <td height="12"> </td>
    </tr>
  </table>
  <div align="center"><font face="Verdana, Arial, Helvetica, sans-serif">
<input type="Submit" name="submit" value="Enter information">
    </font></div>
</form> <?php

}

?> 
</body>
</html>

