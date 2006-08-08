<html>
<head>
<?php

require 'functions.inc';

$clean = array();
$mysql = array();

/*
 * Cleaning the input data
 */
 
if(isset($_POST['submit'])) {
	$clean['submit'] = $_POST['submit'];
}

if(isset($_POST['timeslot'])){
	$clean['timeslot'] = verify_timeslot($_POST['timeslot']);
}

if(isset($_POST['done'])){
	switch(strtoupper($_POST['done'])) {
		case "ON":
			$clean['done'] = 1;
			break;
		default:
			$clean['done'] = 0;
			break;
	}
}
else {
	$clean['done'] = 0;
}

if(isset($_POST['mclass'])) {
	if(ctype_digit($_POST['mclass'])) {
		$clean['mclass'] = $_POST['mclass'];
	} 	
}

if(isset($_POST['chat'])) {
	$clean['chat'] = htmlentities($_POST['chat'], ENT_QUOTES );
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

if(isset($_POST['list'])) {
	if(ctype_alpha($_POST['list'])) {
		$clean['list'] = $_POST['list'];
	}
}
elseif(isset($_GET['list']) && !isset($_POST['submit'])) {
	if(ctype_alpha($_GET['list'])) {
		$clean['list'] = $_GET['list'];
	}
}

/*
 * Cleaning the input data (end)
 */

if (isset($clean['submit']) and $clean['clientid']) {
	$dbConnect = dbconnect();
	
	$mysql['timeslot'] = mysql_real_escape_string( $clean['timeslot']);
	
	$sql  = "UPDATE clients SET timeslot='{$mysql['timeslot']}',";
	$sql .= " done='{$clean['done']}' WHERE clientid='{$clean['clientid']}' ";
	
	$result = mysql_query($sql, $dbConnect);
	if (!$result) {
		$message  = "Invalid query: " . mysql_error() . "\n";
		$message .= "Whole query: " . $sql . "\n";
		die($message);
	}


	$mysql['chat'] = mysql_real_escape_string($clean['chat']);
	
	$sql  = "INSERT INTO calls (clientid,time,chat,class)";
	$sql .= " VALUES ('{$clean['clientid']}', NOW(), '{$mysql['chat']}', '{$clean['mclass']}') ";
	$result = mysql_query($sql, $dbConnect);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $query;
		die($message);
	}
	
	/* 
	 * we need to unset the clientid to get back to all clients
	 * rather than keep operate with the current one
	 */
	$clean['clientid'] = '';
	
	dbclose($dbConnect);
}

if ($clean['clientid'] and !isset($clean['submit'])) {
	$dbConnect = dbconnect();

	$sql = "UPDATE clients SET done='1' WHERE clientid={$clean['clientid']}";
	
	$result = mysql_query($sql);
	if (!$result) {
		$message  = "Invalid query: " . mysql_error() . "\n";
		$message .= "Whole query: " . $sql . "\n";
		die($message);
	}

	$sql = "SELECT * FROM clients WHERE (clients.clientid={$clean['clientid']})";
	
	$result = mysql_query($sql, $dbConnect);
	if (!$result) {
		$message  = "Invalid query: " . mysql_error() . "\n";
		$message .= "Whole query: " . $sql . "\n";
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

	dbclose($dbConnect);
	
} // if ($clientid and !$submit)

if (!$clean['clientid'] and $clean['list'] != 'white' and $clean['list'] != 'black')   {
	echo (' <meta http-equiv="refresh" content="12" url="http://'
			. $HTTP_ENV_VARS["HOSTNAME"]
			. $PHP_SELF . '?list=' . $clean['list'] . '"  /> ');
}

?>

<title> Call <?php print $clean['list'] ?>  List -- Good Morning Blanchardstown</TITLE>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">


<script language="JavaScript">
<!-- hide it from old browsers or from those with JavaScript disabled
function displayStatusMsg(msgStr) { //v1.0
  status=msgStr;
  document.returnValue = true;
}

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


<body bgcolor="#BEC8FD" link="#0000FF" vlink="#0000FF" alink="#0000FF">
<font face="Verdana, Arial, Helvetica, sans-serif" size="2">

<table width="100%" border="0" cellpadding="5">
  <tr> 
    <td   bgcolor="<?php if (($clean['list'] != 'Grey') and ($clean['list']!='grey')) {print $clean['list']; } else {echo 'gray'; } ?>" height="33">
      <div align="right">
<?php   if (!$clientid)  {
  echo("
<a href=\"$PHP_SELF?list=magenta\"><img src=\"/images/magenta.png\" width=\"16\" height=\"20\" border=\"0\"></a> \n
<a href=\"$PHP_SELF?list=red\"><img src=\"/images/red.png\" width=\"16\" height=\"20\" border=\"0\"></a> \n
<a href=\"$PHP_SELF?list=yellow\"><img src=\"/images/yellow.png\" width=\"16\" height=\"20\"  border=\"0\"></a> \n
<a href=\"$PHP_SELF?list=green\"><img src=\"/images/green.png\" width=\"16\" height=\"20\"  border=\"0\"></a> \n
<a href=\"$PHP_SELF?list=olive\"><img src=\"/images/olive.png\" width=\"16\" height=\"20\"  border =\"0\"></a> \n
<a href=\"$PHP_SELF?list=cyan\"><img src=\"/images/cyan.png\" width=\"16\" height=\"20\"  border =\"0\"></a> \n
<a href=\"$PHP_SELF?list=blue\"><img src=\"/images/blue.png\" width=\"16\" height=\"20\"  border=\"0\"></a> \n
<a href=\"$PHP_SELF?list=grey\"><img src=\"/images/grey.png\" width=\"16\" height=\"20\" border=\"0\"></a> \n
<a href=\"$PHP_SELF?list=white\"><img src=\"/images/white.png\" width=\"16\" height=\"20\"  border=\"0\"></a> \n
  ");
}
?>

</div>
</td>
<td width="75%" height="33">
<form onSubmit="return(vtslot(this.timeslot));" 
method="post" action="<?php echo ($PHP_SELF . "?list=" . $clean['list']) ?>" >

<?php
// Client name (bold and big) when one selected
	print('<font size="4">');
	print("<b>");
	printf ($clean['firstname']);
	print("  ");
	printf ($clean['lastname']);
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
      draw_clients_list($clean['list'], $clean['clientid']);
?>
</font> 
</td>

<td width="75%" valign="top" align="center" height="120" > 
<font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
</font>
<div align="right">
<font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 

<img src="images/new/home.png" alt="<?php echo $clean['houseno'] . ", " . $clean['street'] . " " . " -- " . $clean['housing'];?>"
	onClick="displayStatusMsg('<?php echo $clean['houseno'] . ", " . $clean['street'] . " " . " -- " . $clean['housing'];?>
	'); return document.returnValue"
> 
<img src="images/new/doctor.png" alt="<?php	echo $clean['gpname'];?>" 
	onClick="displayStatusMsg('<?php echo $clean['gpname'];?>');return document.returnValue"
> 
<img src="images/new/phone1.png" alt="
	<?php echo $clean['contact1name'] . ", " . $clean['contact1relationship'] . " -- " . $clean['contact1phone1'];?>"
	onClick="displayStatusMsg('<?php echo $clean['contact1name'] . ", " . $clean['contact1relationship'] . " -- " . $clean['contact1phone1'];?>');return document.returnValue"
> 
<img src="images/new/phone2.png" alt="<?php echo $clean['contact2name'] . ", " . $clean['contact2relationship'] . " -- " . $clean['contact2phone1'];?>"
	onClick="displayStatusMsg('<?php echo $clean['contact2name'] . ", " . $clean['contact2relationship'] . " -- " . $clean['contact2phone1'];?>');return document.returnValue"
> 

</font>
</div>
<br>
<br>
<br>
<div align="left">
<font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 

<b>Client details:</b>
	<?php
	    draw_client_details($clean['clientid']);
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
	    draw_classification();
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
		/*
		 * displaing timeslot make sense only if we operate
		 * with a clientid
		 */
		if($clean['clientid']) {
			if(!empty($clean['timeslot'])) {  
			    if ( ereg( "([0-9]{2}):([0-9]{2})", $clean['timeslot'], $regs ) ) {
					print "$regs[1]:$regs[2]";
			    }
			    else {
					print "Invalid time format: " . $clean['timeslot'];
			    }
			}
		}
			?>">
		<b>
		<font size="1" color="#FF0000"> 
		(24 hour format HH:MM)
		</font>
		</b>
	</td>
</tr>
<tr>
	<td width="30%">
		<input type="checkbox" name="done">Call finished</input>
	</td>
</tr>
</table>
<br>
<br>

<!--
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
-->

<table width="100%">
<tr>
	<td><center>
		<input type="submit" name="submit" value="Submit">
	</td>
</tr>
</table>

<input type="hidden" name="clientid" value="<?php print $clean['clientid'] ?>">
<input type="hidden" name="list" value="<?php print $clean['list'] ?>">

</form>
</td>
</tr>
<tr></tr>
<tr>
<td width="75%" valign="top" align="left" height="100"><font size ="1"> 

<?php

$last_dow = 6;

if($clean['clientid']) {
    draw_calls( $clean['clientid']);
} // if($clientid)
?> 
      <p>&nbsp;</p>
      </font> </td>
  </tr>
</table>

</font>

</body>
</html>
