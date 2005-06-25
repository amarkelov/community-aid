<html>
<head>
<?php

require 'functions.inc';

// global $db, $clientid, $list, $done  ;

// if (!$list )  {$list="Grey" ; }

$db = "gmpDb";
//$db = $HTTP_HOST . "Db";
$dbConnect = mysql_connect("localhost", "gmadmin", "old290174");
if (!$dbConnect) {
	die('Could not connect: ' . mysql_error());
}

mysql_select_db($db, $dbConnect);

if ($submit and $clientid) {

	$sql  = "UPDATE clients SET timeslot='$timeslot',";
	$sql .= " done='$done' WHERE clientid=$clientid";
	$result = mysql_query($sql);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}

	$callclass = strtoupper(trim($callclass));
	$sql  = "INSERT INTO calls (clientid,chat,class)";
	$sql .= " VALUES ('$clientid', '$chat', '$mclass') ";
	$result = mysql_query($sql);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}
	unset($clientid);
}

if ($clientid and !$submit) {

	$sql = "UPDATE clients SET done='true' WHERE clientid=$clientid";
	$result = mysql_query($sql);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}

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
	$done=$myrow["done"];
} // if ($clientid and !$submit)

if (!$clientid and $list != 'white' and $list != 'black')   {
	echo (' <meta http-equiv="refresh" content="12" url="http://'
			. $HTTP_ENV_VARS["HOSTNAME"]
			. $PHP_SELF . '?list=' . $list . '"  /> ');
}

?>

<title> Call <?php echo $list ?>  List -- Good Morning Blanchardstown</TITLE>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">


<script language="JavaScript">
<!--
function displayStatusMsg(msgStr) { //v1.0
  status=msgStr;
  document.returnValue = true;
}
//-->
</script>
</head>


<body bgcolor="#BEC8FD" link="#0000FF" vlink="#0000FF" alink="#0000FF">
<font face="Verdana, Arial, Helvetica, sans-serif" size="2">

<table width="100%" border="0" cellpadding="5">
  <tr> 
    <td   bgcolor="<?php if (($list != 'Grey') and ($list!='grey')) {echo $list; } else {echo 'gray'; } ?>" height="33">
      <div align="right">
<?php   if (!$clientid)  {
  echo("
<a href=\"$PHP_SELF?list=magenta\"><img src=\"images/magenta.png\" width=\"16\" height=\"20\" border=\"0\"></a> \n
<a href=\"$PHP_SELF?list=red\"><img src=\"images/red.png\" width=\"16\" height=\"20\" border=\"0\"></a> \n
<a href=\"$PHP_SELF?list=yellow\"><img src=\"images/yellow.png\" width=\"16\" height=\"20\"  border=\"0\"></a> \n
<a href=\"$PHP_SELF?list=green\"><img src=\"images/green.png\" width=\"16\" height=\"20\"  border=\"0\"></a> \n
<a href=\"$PHP_SELF?list=olive\"><img src=\"images/olive.png\" width=\"16\" height=\"20\"  border =\"0\"></a> \n
<a href=\"$PHP_SELF?list=cyan\"><img src=\"images/cyan.png\" width=\"16\" height=\"20\"  border =\"0\"></a> \n
<a href=\"$PHP_SELF?list=blue\"><img src=\"images/blue.png\" width=\"16\" height=\"20\"  border=\"0\"></a> \n
<a href=\"$PHP_SELF?list=grey\"><img src=\"images/grey.png\" width=\"16\" height=\"20\" border=\"0\"></a> \n
<a href=\"$PHP_SELF?list=white\"><img src=\"images/white.png\" width=\"16\" height=\"20\"  border=\"0\"></a> \n
  ");
}
?>

</div>
</td>
<td width="75%" height="33">
<form method="post" action="<?php echo ($PHP_SELF . "?list=" . $list) ?>" >

<?php
// Client name (bold and big) when one selected
	print('<font size="4">');
	print("<b>");
	printf ($firstname);
	print("  ");
	printf ($lastname);
	print(" </b>");
	print("</font>");
?>

</font>
</td>
</tr>
<tr> 
<td rowspan="3" width="25%" valign="top"> 
<font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
<br>
<?php
      draw_clients_list($dbConnect, $list, $clientid);
?>
</font> 
</td>

<td width="75%" valign="top" align="center" height="120" > 
<font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
</font>
<div align="right">
<font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
<img src="images/new/home.png" alt="<?php echo "$houseno  $street  $postcode  -- $housing" ?>"onClick="displayStatusMsg('<?php echo  "$houseno  $street  $postcode  -- $housing" ?>');return document.returnValue"> 
<img src="images/new/doctor.png" alt="<?php echo $gpname  ?>" onClick="displayStatusMsg('<?php echo "$gpname" ?>');return document.returnValue"> 
<img src="images/new/phone1.png" alt="<?php echo "$contact1name, $contact1relationship  -- $contact1phone1" ?>"onClick="displayStatusMsg('<?php echo "$contact1name, $contact1relationship  -- $contact1phone1" ?>');return document.returnValue"> 
<img src="images/new/phone2.png" alt="<?php echo "$contact2name, $contact2relationship  -- $contact2phone1" ?>" onClick="displayStatusMsg('<?php echo "$contact2name, $contact2relationship  -- $contact2phone1" ?>');return document.returnValue"> 
<br>
<br>
<br>
</font> </div>
<div align="left">
<font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 

<b>Client details:</b>
	<?php
	    draw_client_details($clientid, $dbConnect);
	?>
<br>
<b>Report:</b>
<br> 
<textarea name="chat" cols="65" rows="4" wrap="PHYSICAL"></textarea>
<br><br>
<table width="100%">
<tr>
<td width="30%">
<b>Call Classification:</b>
</td>
<td>
<select name="mclass" size="1">

	<?php	
// classification	
	    draw_classification($dbConnect);
	?>
	
</select>
</td>
</tr>
<tr>
<td width="30%">
<b>Next Call at:</b>
</td>
<td>
<input type="text" name="timeslot" size="6" maxlength="5" value="<?php 
//	echo $timeslot ===> set maxlength="5"
	if(!empty($timeslot)) {  
	    if ( ereg( "([0-9]{2}):([0-9]{2})", $timeslot, $regs ) ) {
		print "$regs[1]:$regs[2]";
	    }
	    else {
		print "Invalid time format: $timeslot";
	    }
	}
	?>"><b><font size="1" color="#FF0000"> (24 hour format HH:MM)</font></b>
</div>
</td>
</tr>
</table>
<br><br>

<table width="100%">
<tr>
<td width="30%"><center>
<input type="submit" name="submit" value="Not Finished"
  onClick='javascript: done.value="false";'  />
</td>
<td width="30%"><center>
<input type="submit" name="submit" value="Finished"
  onClick='javascript: done.value="true";'  />
<td></td>
</tr>
</table>

<input type="hidden" name="clientid" value="<?php echo $clientid ?>">
<input type="hidden" name="list" value="<?php echo $list ?>">
<input type="hidden" name="done" value="<?php echo $done ?>" />

</form>
</td>
</tr>
<tr></tr>
<tr>
<td width="75%" valign="top" align="left" height="100"><font size ="1"> 

<?php

$last_dow = 6;

if($clientid) {
    draw_calls( $dbConnect, $clientid);
} // if($clientid)
?> 
      <p>&nbsp;</p>
      </font> </td>
  </tr>
</table>

</font>

</body>
</html>
