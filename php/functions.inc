<?php

define(CONFIG_FILE, "gmp.ini");

$last_dow = 6;

function get_gmp_settings()
{
	$settings = array();
	
	$settings = parse_ini_file(CONFIG_FILE);
	
	if(!isset($settings['force_pdf_when_more_than'])) {
		$settings['force_pdf_when_more_than'] = 300;	
	}

	if(isset($_SERVER['Operator'])) {
		$settings['operator'] = $_SERVER['Operator'];
	}
	else {
		$settings['operator'] = 'gmoperator';
	}
	
	if(isset($_SERVER['Operator_Password'])) {
		$settings['optrpswd'] = $_SERVER['Operator_Password'];
	}
	else {
		$settings['operator'] = 'gmoperatorpwd';
	}
	
	if(isset($_SERVER['DbHost'])) {
		$settings['dbhost'] = $_SERVER['DbHost'];
	}
	else {
		$settings['dbhost'] = 'localhost';
	}
	
	return $settings;
}

function dbconnect()
{
	$settings = array();
	
	$settings = get_gmp_settings();
	
//	$db = "gmpDb";
//	$dbConnect = mysql_connect("localhost", "gmadmin", "old290174");
	
	$dbConnect = mysql_connect($settings['dbhost'], $settings['operator'], $settings['optrpswd']);
	if (!$dbConnect) {
		die('Could not connect: ' . mysql_error());
	}
	
//	mysql_select_db($db, $dbConnect);
	mysql_select_db($settings['database'], $dbConnect);
	
	return $dbConnect;
}

function dbclose( $dbConnect)
{
	mysql_close($dbConnect);
}

function draw_calls( $clientid) 
{
	$dbConnect = dbconnect();
	
    $sql  = "SELECT time,chat,class FROM calls WHERE";
    $sql .= " (clientid='$clientid') ORDER BY callid DESC ";
    $result = mysql_query( $sql, $dbConnect);
    if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $sql;
		die($message);
    }

    print ("<hr noshade>");
    print('<table border="0" width="100%" cellspacing="0" cellpadding="0">');
    $i=0; // need this to change cell bgcolor

    while ($myrow = mysql_fetch_array($result)) {
// select sclass short name
        $class_ = $myrow["class"];
	$sql  = "SELECT sclass_sname FROM call_sclass WHERE";
	$sql .= " sclass_id='$class_'";
	$res = mysql_query( $sql, $dbConnect);
	if (!$res) {
	    $message  = 'Invalid query: ' . mysql_error() . "\n";
	    $message .= 'Whole query: ' . $sql;
	    die($message);
	}
	$class_row = mysql_fetch_row($res);
// select sclass short name (end)

	$i=$i + 1; // need this to change cell bgcolor
	$time=$myrow["time"];

	$regs = 0;
	if ( ereg( "([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})", $time, $regs ) ) {
	    $out="$regs[3]/$regs[2]/$regs[1] $regs[4]:$regs[5]";
	}
	else {
	    $out="Invalid date format: $time";
	}

	if($i % 2) {
	    print('<tr bgcolor="#FFFFFF">');
	}
	else {
	    print('<tr bgcolor="#DDDDDD">');
	}
// time column
	print('<td width="20%"> <font size="1" color="#FF0000">');
	print($out);
	print('</font>');
// class column
	print('</td><td width="10%"><font size="1" color="#00FF00">');
	
	if(!empty($class_row[0])) { 
		print("<a href=\"class.php\" onClick=\"window.open('class.php','','status=yes,toolbar=no,location=no,width=400,height=500');return false;\">$class_row[0]</a>");
	}
	else {
	    print("&nbsp");
	}	
	print('</font>');
// chat column
	print('</td><td><font size="1" color="#0000FF">');
	
	if(!empty($myrow["chat"])) { 
	    print($myrow["chat"] );
	}
	else {
	    print("&nbsp");
	}
	    
	print("</td></tr>");
    } // while
    
    print("</table>");

	dbclose($dbConnect);
}

function draw_classification()
{
	$dbConnect = dbconnect();

//      $sql = "SELECT mclass_id,mclass_name FROM call_mclass";
    $sql  = "SELECT m.mclass_id,s.sclass_id,m.mclass_name,s.sclass_name";
    $sql .= " FROM call_mclass AS m,call_sclass AS s";
    $sql .= " WHERE m.mclass_id=s.mclass_id ORDER BY m.mclass_id";
    
    $result = mysql_query($sql, $dbConnect);
    if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $sql;
		die($message);
    }
    while( $class = mysql_fetch_array($result)) {
		printf("<option value=\"%d\">%s -- %s</option>",$class[sclass_id],$class[mclass_name],$class[sclass_name]);
    }
    
    dbclose($dbConnect);
}

function draw_area($selected = 0)
{
	$dbConnect = dbconnect();
	
    $sql = "SELECT id,codename FROM postcode"; // ORDER BY codename";
    
    $result = mysql_query($sql, $dbConnect);
    if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $sql;
		die($message);
    }
    while( $area = mysql_fetch_array($result)) {
    	
		printf("<option value=\"%d\">%s</option>",$area[id],$area[codename]);
    }
    dbclose($dbConnect);
}

// this is for drop-down list of clients
function draw_clients()
{
	$dbConnect = dbconnect();
	
    $sql = "SELECT clientid,lastname,firstname FROM clients ORDER BY lastname";
    
    $result = mysql_query($sql, $dbConnect);
    if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $sql;
		die($message);
    }
    while( $client = mysql_fetch_array($result)) {
		printf("<option value=\"%d\">%s, %s (%d)</option>",
	    $client[clientid],strtoupper($client[lastname]),
	    strtoupper($client[firstname]),$client[clientid]);
    }
    
    dbclose($dbConnect);
}

function draw_client_details($clientid)
{
    $fs = 1; // font size
    $regs = 0;

	$dbConnect = dbconnect();

    if( $clientid and $dbConnect) {
		$sql  = "SELECT phone1,phone2,dob,ailments,note from clients";
		$sql .= " WHERE clientid='$clientid'";
		
		$result = mysql_query( $sql, $dbConnect);
		if (!$result) {
		    $message  = 'Invalid query: ' . mysql_error() . "\n";
		    $message .= 'Whole query: ' . $sql;
		    die($message);
		}
		else {	
	   	    while ($myrow = mysql_fetch_array($result)) {
				if($myrow["phone1"]) {
		        	    $phone1 = $myrow["phone1"]; 
				}
				else {
				    $phone1 = "N/A";
				}
				if($myrow["phone2"]) {
		    	    	    $phone2 = $myrow["phone2"];
				}
				else {
				    $phone2 = "N/A";
				}
				if ( ereg( "([0-9]{4})-([0-9]{2})-([0-9]{2})", $myrow["dob"], $regs ) ) {
				    $dob = "$regs[3]/$regs[2]/$regs[1]";
				}
		       	else {
				    $dob = "Invalid DOB";
				} 
				$ailments = $myrow["ailments"];
		       	$note = $myrow["note"];
		    }
		}
    }    

    $out  = '<table border="0" width="100%">';
    $out .= '<tr>';
    $out .= "<td width=\"30%\"><font size=$fs>Phone 1:</font></td>";
    $out .= "<td><font size=$fs>$phone1</font></td>";
    $out .= '</tr><tr>';        
    $out .= "<td width=\"30%\"><font size=$fs>Phone 2:</font></td>";
    $out .= "<td><font size=$fs>$phone2</font></td>";
    $out .= '</tr><tr>';    
    $out .= "<td width=\"30%\"><font size=$fs>D.O.B. (DD/MM/YYYY):</font></td>";
    $out .= "<td><font size=$fs>$dob</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=$fs>Ailments:</font></td>";
    $out .= "<td><font size=$fs>$ailments</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=$fs>Notes:</font></td>";
    $out .= "<td><font size=$fs>$note</font></td>";
    $out .= '</tr></table>';
    
    print "$out";

	dbclose($dbConnect);
}

// Draw clients list
// left part of the screen
function draw_clients_list($list, $clientid)
{
	$dbConnect = dbconnect();
	
	$fs = 1; // font size
	
	if ($list == "black")  {
	    $sql  = "SELECT * FROM clients where (clients.list = '$list' )";
	    $sql .= " order by lastname, firstname";
	    $result = mysql_query( $sql, $dbConnect);
	    if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $sql;
		die($message);
	    }
	} 
	elseif ($list == "white") {
	    $sql  = "SELECT * FROM clients where (clients.list != 'black' )";
	    $sql .= " order by done, lastname, firstname";
	    $result = mysql_query( $sql, $dbConnect);
	    if (!$result) {
	    	$message  = 'Invalid query: ' . mysql_error() . "\n";
			$message .= 'Whole query: ' . $sql;
	    	die($message);
	    }
	}
	else {
	    $sql  = "SELECT * FROM clients where (clients.list = '$list' )";
	    $sql .= " order by done, timeslot, firstname";
	    $result = mysql_query( $sql, $dbConnect);
	    if (!$result) {
			$message  = 'Invalid query: ' . mysql_error() . "\n";
			$message .= 'Whole query: ' . $sql;
			die($message);
	    }
	}

	print("<table>");
	
	while ($myrow = mysql_fetch_array($result)) {
	    $tmp_done = $myrow["done"]; 
	    $tmp_clientid = $myrow["clientid"];
	    $tmp_firstname = $myrow["firstname"]; 
	    $tmp_lastname = stripslashes($myrow["lastname"]);
	
	    print("<tr>");
	
	    if ( $clientid)  {
			$out  = "<td><font size=\"$fs\">";
			$out .= "$tmp_firstname  $tmp_lastname";
			$out .= "</font></td>";
			print($out);
	    }
	    else {
			$out  = "<td><a href=\"$PHP_SELF?clientid=$tmp_clientid\">";
			$out .= "<font size=\"$fs\">";
			$out .= "$tmp_firstname  $tmp_lastname</a>";
			$out .= "</font></td>";
			print ($out);
	    }
	
	    if ( !$tmp_done) {
	    	$regs = 0;
			ereg( "([0-9]{2}):([0-9]{2}):([0-9]{2})", $myrow["timeslot"], $regs );
			$out  = "<td><font size=\"$fs\">";
			$out .= "$regs[1]:$regs[2]";
			$out .= "</font></td>";
			print ($out);
	    }
	    
	    print("</tr>");
	}
	
	print("</table>");
	
	dbclose($dbConnect);

}

function verify_timeslot($timeslot)
{
	$ts = "00:00";	
	$vt = explode(":", $timeslot, 2);
	
	if(ctype_digit($vt[0]) && ctype_digit($vt[1])) {
		if(($vt[0]>=0 && $vt[0]<=23) && ($vt[1]>=0 && $vt[1]<=59)){
			$ts = "$vt[0]:$vt[1]";
		}
	}
	
	return $ts; 
}

//function draw_calendar() {
//<table>
//<td>
//<input type="text" name="day" size="2"></input><br>
//DD
//</td>
//<td>
//<input type="text" name="month" size="2"></input><br>
//MM
//</td>
//<td>
//<input type="text" name="year" size="4"></input><br>
//YYYY
//</td>
//</table>
//}

// pdf stuff begin
require('fpdf.php');

class PDF extends FPDF
{
	var $sql_query = 0;
	var $header = 0;
	var $border = 0;
		
	//Page header
	function Header()
	{
	    //Colors, line width and bold font
	    $this->SetFillColor(17,40,200);
	    $this->SetTextColor(255);
	    $this->SetDrawColor(128,0,0);
	    $this->SetLineWidth(.3);
	    $this->SetFont('','B');
	    //Header
	    $w=array(20,20,40,105);
	    for($i=0;$i<count($this->header);$i++)
	        $this->Cell($w[$i],7,$this->header[$i],1,0,'C',1);
	    $this->Ln();
	}
	
	//Page footer
	function Footer()
	{
	    //Position at 1.5 cm from bottom
	    $this->SetY(-15);
	    //Arial italic 8
	    $this->SetFont('Arial','I',8);
	    //Page number
	    $this->Cell(0,10,'Page '.$this->PageNo().'/{nb}',0,0,'C');
	}
	
	//Colored table
	function ColoredTable($header,$query_result)
	{
	 /*   //Colors, line width and bold font
	    $this->SetFillColor(17,40,200);
	    $this->SetTextColor(255);
	    $this->SetDrawColor(128,0,0);
	    $this->SetLineWidth(.3);
	    $this->SetFont('','B');*/
	    //Header
	    $w=array(20,20,40,105);
	/*    for($i=0;$i<count($header);$i++)
	        $this->Cell($w[$i],7,$header[$i],1,0,'C',1);
	    $this->Ln();*/
	    //Color and font restoration
	    $this->SetFillColor(224,235,255);
	    $this->SetTextColor(0);
	    $this->SetFont('');
	    //Data
	    $fill=0;
	
	
	    while( $row = mysql_fetch_array($query_result))
	    {
	    	// call id
	        $this->Cell($w[0],4,$row[0],$this->border,0,'L',$fill);
	        // caller id
	        $this->Cell($w[1],4,$row[1],$this->border,0,'L',$fill);
	        // date & time
	        $this->Cell($w[2],4,$row[2],$this->border,0,'C',$fill);
	        // chat
	        $this->MultiCell($w[3],4,$row[3],$this->border,'L',$fill);
	        $fill=!$fill;
	    }
	    $this->Cell(array_sum($w),0,'','T');
	}
}

// pdf ends

?>
