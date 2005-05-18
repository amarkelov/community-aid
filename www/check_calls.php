<html>
<HEAD>
<?php

// global $db, $clientid, $list, $done  ;

if (!$list )  {$list="white" ; }

$db = "gmpDb";
//$db = $HTTP_HOST . "Db";
$dbConnect = mysql_connect("localhost", "root", "old290174");
if(!$dbConnect) {
	die('Could not connect: ' . mysql_error());
}

mysql_select_db($db,$dbConnect);

if ($submit and $clientid) {

	$sql  = "UPDATE clients SET timeslot='$timeslot',";
	$sql .= " done='$done' WHERE clientid=$clientid";
	$result = mysql_query($sql);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}

	$sql  = "INSERT INTO calls (clientid,chat,class) ";
	$sql .= "VALUES ('$clientid', '$chat', '$callclass') ";
	$result = mysql_query($sql);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}
	unset($clientid);
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

//    $sql = "UPDATE clients SET done='true' WHERE clientid=$clientid";
//      $result = mysql_query($sql);

	$clientid = $myrow["clientid"];
	$firstname=stripslashes($myrow["firstname"]);
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
}

?>

<TITLE>Check <?php echo $list ?> List -- Good Morning Blanchardstown</TITLE>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv="expires" content="-1">
<meta http-equiv="Cache-Control" content="no-cache">

<script language="JavaScript">
<!--
function displayStatusMsg(msgStr) { //v1.0
  status=msgStr;
  document.returnValue = true;
}
//-->
</script>
</HEAD>


<BODY bgcolor="#DDAAAA" link="#0000FF" vlink="#0000FF" alink="#0000FF">
<FONT face="Verdana, Arial, Helvetica, sans-serif" size="2">

<TABLE width="100%" border="0" cellpadding="5" > 
  <TR> 
    <TD   bgcolor="<?php if (($list != 'Grey') and ($list!='grey')) {echo $list; } else {echo 'gray'; } ?>" height="33">
      <div align="right">
<?php   
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

?>

      </div>
    </td>
<TD width="75%" height="33">
<P align="center">
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

<font color="#CC9999">-</font>
    </TD>
  </TR>
  <tr> 
    <TD rowspan="3" width="25%" valign="top"> <FONT face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
      <BR />
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
	$sql  = "SELECT * FROM clients WHERE (list != 'black') ";
	$sql .= "order by lastname, firstname";
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

	$out  = "<a href=\"$PHP_SELF?clientid=$tmp_clientid\">";
	$out .= "$tmp_firstname  $tmp_lastname</a> \n";
	$out .= "<br><div align=right>";
	print( $out);
	// print($done);

	if ( $tmp_done != "true")
		print ($myrow["timeslot"]);

	print ("</div>");
}

?> </FONT> </TD>
    <TD width="75%" valign="top" align="center" height="120" > <FONT face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
      </font>
        <div align="right"><font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
          <font color="#CC9999">-</font><img src="images/house.png" width="30" height="30" alt="<?php echo "$houseno  $street  $postcode  -- $housing" ?>"onClick="displayStatusMsg('<?php echo  "$houseno  $street  $postcode  -- $housing" ?>');return document.returnValue"> 
          <font color="#CC9999">--</font><img src="images/medicine.png" width="30" height="30" alt="<?php echo $gpname  ?>" onClick="displayStatusMsg('<?php echo "$gpname" ?>');return document.returnValue"> 
          <font color="#CC9999">--</font><img src="images/telephon.png" width="30" height="30" alt="<?php echo "$contact1name, $contact1relationship  -- $contact1phone1" ?>"onClick="displayStatusMsg('<?php echo "$contact1name, $contact1relationship  -- $contact1phone1" ?>');return document.returnValue"> 
          <font color="#CC9999">--</font><img src="images/telephon.png" width="30" height="30" alt="<?php echo "$contact2name, $contact2relationship  -- $contact2phone1" ?>" onClick="displayStatusMsg('<?php echo "$contact2name, $contact2relationship  -- $contact2phone1" ?>');return document.returnValue"> 
          <br>
          <br>
          <br>
          </font> </div>
        
      <div align="left"><font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
        Phone 1: <b>
<!--       <?php echo $phone1 ?>
-->       </b> <FONT color="#CC9999">----</FONT>
        Phone 2: <b>
<!--        <?php echo $phone2 ?>
-->        </b> <FONT color="#CC9999">--------</FONT>
        D.o.b: <?php
	if ( ereg( "([0-9]{4})-([0-9]{1,2})-([0-9]{1,2})", $dob, $regs ) ) {
	echo "$regs[3]-$regs[2]-$regs[1]";
		}
		?> <br>
        Ailments: <?php echo $ailments ?><br>
        Notes: <b><?php echo $note ?></b><br>
        Report: 
        <br />
        Next Call: 
        <?php echo $timeslot  ?>
        <font color="#CC9999">-------</FONT>
        <font color="#CC9999">--------</font>
        </font></div>

       <input type="hidden" name="clientid" value="<?php echo $clientid ?>">
       <input type="hidden" name="list" value="<?php echo $list ?>">
       <input type="hidden" name="done" value="<?php echo $done ?>" />

      </FORM> </TD>
  </TR>
  <TR > </TR>
  <TR>
    <TD width="75%" bgcolor="#dddddd" valign="top" align="left" height="100"><font size ="1"> 

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
			if ($this_dow > $last_dow) {
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

		print ('<font color="#0000FF">');
		print ($myrow["class"]);
		print(" | ");
		print('</font>');
 
		print($myrow["chat"] );
		print ("<br>");

	} // while
} // if($clientid)

?> 
      <P>&nbsp;</P>
      </font> </TD>
  </TR>
  <TR> 
    <TD width="25%">&nbsp;</TD>
    <TD width="75%"><font face="Verdana, Arial, Helvetica, sans-serif"> </font></TD>
  </TR>
</TABLE>
</FONT>
</BODY>

</HTML>

