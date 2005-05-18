<html>
<head>
<?php

// global $db, $clientid, $list, $done  ;

// if (!$list )  {$list="Grey" ; }

$db = "gmpDb";
//$db = $HTTP_HOST . "Db";
$dbConnect = mysql_connect("localhost", "root", "old290174");
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
	$sql .= " VALUES ('$clientid', '$chat', '$callclass') ";
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
	$result = mysql_query($sql);
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


<body bgcolor="#99CC99" link="#0000FF" vlink="#0000FF" alink="#0000FF">
<font face="Verdana, Arial, Helvetica, sans-serif" size="2">

<table width="100%" border="0" cellpadding="5" bgcolor="#99cc99">
  <tr> 
    <td   bgcolor="<?php if (($list != 'Grey') and ($list!='grey')) {echo $list; } else {echo 'gray'; } ?>" height="33">
      <div align="right">
<?php   if (!$clientid)  {
  echo("
<a href=\"$PHP_SELF?list=magenta\"><img src=\"images/magenta.png\" width=\"16\" height=\"20\" border=\"0\"></a> \n
<a href=\"$PHP_SELF?list=red\"><img src=\"images/red.png\" width=\"16\" height=\"20\" border=\"0\"></a> \n
  <a href=\"$PHP_SELF?list=yellow\"><img src=\"images/yellow.png\" width=\"16\" height=\"20\" border=\"0\"></a> \n
  <a href=\"$PHP_SELF?list=green\"><img src=\"images/green.png\" width=\"16\" height=\"20\" border=\"0\"></a> \n
  <a href=\"$PHP_SELF?list=olive\"><img src=\"images/olive.png\" width=\"16\" height=\"20\" border =\"0\"></a> \n
  <a href=\"$PHP_SELF?list=cyan\"><img src=\"images/cyan.png\" width=\"16\" height=\"20\" border =\"0\"></a> \n
  <a href=\"$PHP_SELF?list=blue\"><img src=\"images/blue.png\" width=\"16\" height=\"20\" border=\"0\"></a> \n
  <a href=\"$PHP_SELF?list=grey\"><img src=\"images/grey.png\" width=\"16\" height=\"20\" border=\"0\"></a> \n
  <a href=\"$PHP_SELF?list=white\"><img src=\"images/white.png\" width=\"16\" height=\"20\" border=\"0\"></a> \n
  ");
         }
?>

      </div>
	</td>
	<td width="75%" height="33">
	<form method="post" action="<?php echo ($PHP_SELF . "?list=" . $list) ?>" >


<?php
	print('<font size="4">');
	print("<b>");
	printf ($firstname);
	print("  ");
	printf ($lastname);
	print(" </b>");
	print("</font>");
?> 

<font color="#99CC99">-</font>
    </td>
  </tr>
  <tr> 
    <td rowspan="3" width="25%" valign="top"> <font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
      <br />
      <?php

    // print the list 
if ($list == "black")  {
	$sql  = "SELECT * FROM clients where (clients.list = '$list' )";
	$sql .= " order by lastname, firstname";
   $result = mysql_query( $sql, $dbConnect);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}
} 
elseif ($list == "white") {
	$sql  = "SELECT * FROM clients where (clients.list != 'black' )";
	$sql .= " order by done, lastname, firstname";
   $result = mysql_query( $sql, $dbConnect);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}
}
else {
	$sql  = "SELECT * FROM clients where (clients.list = '$list' )";
	$sql .= " order by done, timeslot, firstname";
	$result = mysql_query( $sql, $dbConnect);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}
}

while ($myrow = mysql_fetch_array($result)) {
	$tmp_done = $myrow["done"]; 
	$tmp_clientid = $myrow["clientid"];
	$tmp_firstname = $myrow["firstname"]; 
	$tmp_lastname = stripslashes($myrow["lastname"]);

	if ( $clientid)  {
		print ("$tmp_firstname  $tmp_lastname \n");
	}
	else {
		$out  = "<a href=\"$PHP_SELF?clientid=$tmp_clientid\">";
		$out .= "$tmp_firstname  $tmp_lastname</a> \n";
		print ($out);
	}

	print ("<br><div align=right>");
	//	 print($done);

	if ( $tmp_done != "true")
		print ($myrow["timeslot"]);

	print ("</div>");
}

?> </font> </td>
    <td width="75%" valign="top" align="center" height="120" > <font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
      </font>
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
          <font color="#99CC99">-</font><img src="images/house.png" width="30" height="30" alt="<?php echo "$houseno  $street  $postcode  -- $housing" ?>"onClick="displayStatusMsg('<?php echo  "$houseno  $street  $postcode  -- $housing" ?>');return document.returnValue"> 
          <font color="#99CC99">--</font><img src="images/medicine.png" width="30" height="30" alt="<?php echo $gpname  ?>" onClick="displayStatusMsg('<?php echo "$gpname" ?>');return document.returnValue"> 
          <font color="#99CC99">--</font><img src="images/telephon.png" width="30" height="30" alt="<?php echo "$contact1name, $contact1relationship  -- $contact1phone1" ?>"onClick="displayStatusMsg('<?php echo "$contact1name, $contact1relationship  -- $contact1phone1" ?>');return document.returnValue"> 
          <font color="#99CC99">--</font><img src="images/telephon.png" width="30" height="30" alt="<?php echo "$contact2name, $contact2relationship  -- $contact2phone1" ?>" onClick="displayStatusMsg('<?php echo "$contact2name, $contact2relationship  -- $contact2phone1" ?>');return document.returnValue"> 
          <br>
          <br>
          <br>
          </font> </div>
        
      <div align="left"><font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
        Phone 1: <b><?php echo $phone1 ?></b> <font color="#99CC99">----</font>
        Phone 2: <b><?php echo $phone2 ?></b> <font color="#99CC99">--------</font>
        D.o.b: <?php
	if ( ereg( "([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})", $dob, $regs ) ) {
	echo "$regs[3]-$regs[2]-$regs[1]";
		}
		?> <br>
        Ailments: <?php echo $ailments ?><br>
        Notes: <b><?php echo $note ?></b><br>
        Report: 
        <textarea name="chat" cols="65" rows="4" wrap="PHYSICAL"></textarea>
        <BR>
        Next Call: 
        <input type="text" name="timeslot" size="7" maxlength="7" value="<?php echo $timeslot  ?>">
        <font color="#99CC99">----</font>
	Class:
	<input type="text" name="callclass" size="7" maxlength="7" value="<?php echo $callclass  ?>">
        <font color="#99CC99">----</font>

         <input type="submit" name="submit" value="Not Finished"
           onClick='javascript: done.value="false";'  />
        <font color="#99CC99">----</font>
         <input type="submit" name="submit" value="Finished"
           onClick='javascript: done.value="true";'  />
        </font></div>

       <input type="hidden" name="clientid" value="<?php echo $clientid ?>">
       <input type="hidden" name="list" value="<?php echo $list ?>">
       <input type="hidden" name="done" value="<?php echo $done ?>" />

      </form> </td>
  </tr>
  <tr > </tr>
  <tr>
    <td width="75%" bgcolor="#dddddd" valign="top" align="left" height="100"><font size ="1"> 

<?php

$last_dow = 6;

if($clientid) {
	$sql  = "SELECT time,chat,class FROM calls WHERE";
	$sql .= " (clientid= '$clientid') ORDER BY callid DESC ";
	$result = mysql_query( $sql, $dbConnect);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}

	while ($myrow = mysql_fetch_array($result)) {
		$time=$myrow["time"];

		print ('<font color="#FF0000">');

		if ( ereg( "([0-9]{4})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})([0-9]{2})", $time, $regs ) ) {
			$this_dow = date("w",mktime(0,0,0,$regs[2],$regs[3],$regs[1]));

			if ($this_dow > $last_dow)   {
				print ('<hr />');
			}

			$last_dow = $this_dow;
			print "$regs[3]/$regs[2]/$regs[1] $regs[4]:$regs[5]";
		}
		else {
			print "Invalid date format: $time";
		}

		print(" | ");
		print('</font>');
		print('<font color="#0000FF">');
		print($myrow["class"]);
		print(" | ");
		print('</font>');
		print($myrow["chat"] );
		print("<br>");

	} // while
} // if($clientid)
?> 
      <p>&nbsp;</p>
      </font> </td>
  </tr>
  <tr> 
    <td width="25%">&nbsp;</td>
    <td width="75%"><font face="Verdana, Arial, Helvetica, sans-serif"> </font></td>
  </tr>
</table>
</font>
</body>

</html>

