<html>
<head>
<?php

// global $db, $clientid, $list

// if (!$list )  {$list="Grey" ; }

//$db = $HTTP_HOST . "Db";
$db = "gmpDb";
$dbConnect = mysql_connect("localhost", "gmadmin", "old290174");
if (!$dbConnect) {
	die('Could not connect: ' . mysql_error());
}

mysql_select_db($db,$dbConnect);

if ($submit and $clientid) {
	$sql  = "UPDATE clients SET timeslot='$timeslot', ";
	$sql .= "done='$done' WHERE clientid=$clientid";
	$result = mysql_query($sql);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}

	$sql = "INSERT INTO calls (clientid,chat) VALUES ('$clientid', '$chat') ";
	$result = mysql_query($sql);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}
}

if ($clientid and !$submit) {

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
	$done=$myrow["done"];
}

?>

<title><?php echo $list ?>  List -- GoodMorningNorthGlasgow/Eng-IntLtd</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="expires" content="-1">
<meta http-equiv="Cache-Control" content="no-cache">
</head>

<body bgcolor="#FFFFFF" link="#0000FF" vlink="#0000FF" alink="#0000FF">
<font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
<table width="100%" border="0" cellpadding="5" >
  <tr> 
    <td width="10">  </td>	
    <td width="30%"  bgcolor="<?php if (($list != 'grey') and ($list!='grey')) {echo $list; } else {echo 'gray'; } ?>" height="33"> 
      <div align="right"> 
	<a href="<?php echo "$PHP_SELF","?list=magenta" ?>"><img src="images/magenta.png" width="16" height="24" border="0"></a>
	<a href="<?php echo "$PHP_SELF","?list=red" ?>"><img src="images/red.png" width="16" height="24" border="0"></a> 
        <a href="<?php echo "$PHP_SELF","?list=yellow" ?>"><img src="images/yellow.png" width="16" height="24" border="0"></a> 
        <a href="<?php echo "$PHP_SELF","?list=green" ?>"><img src="images/green.png" width="16" height="24" border="0"></a> 
        <a href="<?php echo "$PHP_SELF","?list=olive" ?>"><img src="images/olive.png" width="16" height="24" border="0"></a>
        <a href="<?php echo "$PHP_SELF","?list=cyan" ?>"><img src="images/cyan.png" width="16" height="24" border="0"></a>
        <a href="<?php echo "$PHP_SELF","?list=blue" ?>"><img src="images/blue.png" width="16" height="24" border="0"></a> 
        <a href="<?php echo "$PHP_SELF","?list=grey" ?>"><img src="images/grey.png" width="16" height="24" border="0"></a> 
        <a href="<?php echo "$PHP_SELF","?list=white" ?>"><img src="images/white.png" width="16" height="24" border="0"></a>
      </div>
    </td>
    <td width="30%" height="33">&nbsp;</td>
    <td width="30%" height="33"> 
      <?php echo   "<b>$list List</b>" ?>
    </td>
  </tr>
  <tr> 
    <td> </td>
    <td  valign="top"> 
      <b>name</b>
    </td>
    <td valign="top" > 
      <b>time</b>
    </td>
    <td valign="top"  > 
      <b>number</b>
    </td>
  </tr>
  
<?php

// print the list
if  ($list != "white")  {
	$sql  = "SELECT * FROM clients where (clients.list = '$list' )";
	$sql .= " order by timeslot, firstname";
	$result = mysql_query( $sql, $dbConnect);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}
}
else {
	$sql  = "SELECT * FROM clients WHERE (list != 'black')";
	$sql .= " order by timeslot,lastname, firstname";
	$result = mysql_query( $sql, $dbConnect);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}
} 

while ($myrow = mysql_fetch_array($result)) {
	$out  = "<tr><td>";
	$out .= "<img src=\"images/";
	$out .= $myrow["list"];
	$out .= ".png\" width=\"16\" height=\"16\" border=\"0\">";
	$out .= sprintf("%4s", $myrow["clientid"]); 
	$out .= "</td> <td>";
	$out .= sprintf ("%s %s", $myrow["firstname"], $myrow["lastname"]);
	$out .= "</td> <td>";
	$out .= $myrow["timeslot"];
	$out .= "</td> <td>";
	$out .= $myrow["phone1"];
	$out .= "</td></tr>";
	print($out);
}

?>
  <tr> 
    <td>&nbsp;</td>
    <td></td>
    <td></td>
  </tr>
</table>
</font>
</body>

</html>

