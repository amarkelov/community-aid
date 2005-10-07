<html>
<head>
<title> Report -- Good Morning Blanchardstown</TITLE>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>


<body bgcolor="#BEC8FD" link="#0000FF" vlink="#0000FF" alink="#0000FF">
<font face="Verdana" size="2">

<h1>Report</h1>

Please, choose criteria for the report and press 'Submit' button.

<p>

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

?>


<form method="post" action="report_res.php" >

<table border="0" cellpadding="5">
<tr> 
    <td>
    <input type="checkbox" name="class_cb"></input>
    </td>

    <td>
    by Classification: 
    <select name="class" size="1">
    <?php draw_classification($dbConnect) ?>
    </select>
    </td>
</tr>
<tr>
    <td valign="top">
    <input type="checkbox" name="date_cb"></input>
    </td>
    
    <td valign="top">
    by Date when the call(s) was placed:
    <br>
    <table border="1" cellpadding="0" cellspacing="0">
    <tr>
    <td>
    <input type="radio" name="when" value="year" checked></input>
    </td>
    <td>
    Year (if you want report for the whole year of choice):
    <select name="year" size="1">
    <?php
    for ( $i = 2000; $i <= 2050; $i++) {
	printf("<option value=\"%d\">%d</option>", $i, $i);
    }
    ?>
    </select>
    </td>
    </tr>
    <tr>
    <td>
    <input type="radio" name="when" value="dates"></input>
    </td>
    <td>
    from (dd/mm/yyyy):
    <input type="text" name="date_from" size="10"></input>
    to (dd/mm/yyyy):
    <input type="text" name="date_to" size="10"></input>
    </td>
    </tr>
    </table>
    </td>
    
    <td>    
    </td>
</tr>
<tr>
    <td>
    <input type="checkbox" name="area_cb"></input>
    </td>
    <td valign="top">
    by Area: 
    <select name="area" size="1">
    <?php draw_area($dbConnect) ?>
    </select>
    </td>
</tr>
<tr>
    <td>
    <input type="checkbox" name="client_cb"></input>
    </td>
    <td valign="top">
    by Client name: 
    <select name="client" size="1">
    <?php draw_clients($dbConnect) ?>
    </select>
    </td>
</tr>

</table>
<br>

<left><input type="submit" name="submit" value="Submit"></input></left>

<?php
if ($submit) {
    // flag for multiple selection
    $multi = 0;
    $rowclr = 0;
    
    $sql = "SELECT callid,clientid,time,chat FROM calls where ";
    
    // check what fields are selected
    // (1) classification
    if($class_cb) {
	$multi = 1;
	$sql .= "class=$class ";
    }
    // (2) date
    if($date_cb) {
	if($multi) {
	    $sql .= "AND ";
	}
	else {
	    $multi = 1;
	}

	switch($when) {
	    case "year":
		$sql .= "YEAR(time) = $year ";
		break;
	    case "dates":
		if(ereg("([0-9]{2})/([0-9]{2})/([0-9]{4})", $date_from, $regs)) {
		    $df = "$regs[3]-$regs[2]-$regs[1] 00:00:00";
		}
		if(ereg("([0-9]{2})/([0-9]{2})/([0-9]{4})", $date_to, $regs)) {
		    $dt = "$regs[3]-$regs[2]-$regs[1] 23:59:59";
		}

		$sql .= "(time <= '$dt' AND time >= '$df') ";
		break;
	}
    }
    // (3) area
    if($area_cb) {
	if($multi) {
	    $sql .= "AND ";
	}
	else {
	    $multi = 1;
	}

	$sql .= "clientid in (select clientid FROM clients where postcode=";
	$sql .= "(select id FROM postcode where id='$area')) ";
    }
    // (4) client name
    if($client_cb) {
	if($multi) {
	    $sql .= "AND ";
	}
	else {
	    $multi = 1;
	}

	$sql .= "clientid=$client ";
    }

    print $sql;    
    
    $result = mysql_query($sql);
    if (!$result) {
	$message  = 'Invalid query: ' . mysql_error() . "\n";
	$message .= 'Whole query: ' . $query;
	die($message);
    }

    $out  = "<hr noshade>";
    $out .= "<table width='100%'>";
    $out .= "<tr bgcolor='#00FF00'>";
    $out .= "<td width='2%'><b>Call ID</td>";
    $out .= "<td width='2%'><b>Client ID</td>";
    $out .= "<td width='15%'><b>Date & time</td>";
    $out .= "<td><b>Call details</td>";    
    $out .= "</tr>";
    print $out;
    
    while( $row = mysql_fetch_array($result)) {
	if ($rowclr % 2) {
	    $out = '<tr bgcolor="#FFFFFF">';
	}
	else {
	    $out = '<tr bgcolor="#DDDDDD">';	
	}
	
	$out .= "<td>";
	$out .= "$row[callid]";
	$out .= "</td><td width='2%'>";
	$out .= "$row[clientid]";
	$out .= "</td><td width='15%'>";	
	$out .= "$row[time]";
	$out .= "</td><td>";	
	$out .= "$row[chat]";
	$out .= "</td></tr>";
    
        print $out;
	
	$rowclr++; // change row's background colour
    }
    

    print("<table>");
}

?>

</form>
</font>
</body>
</html>