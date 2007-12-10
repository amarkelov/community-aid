<?php
session_start();
require 'functions.inc';

$clean = array();
$mysql = array();

$settings = get_ca_settings();

// START LOG IN CODE
$doWeExit = displayLogin(basename($_SERVER['PHP_SELF']), false, true);
if($doWeExit == true){
	exit;
}
// END LOG IN CODE

// if debug flag is set, print the following info
if($settings['debug'] == 1){
	print "<b>\$_POST:</b><br>";
	print_r( $_POST);
	print "<p>";

	print "<b>\$settings:</b><br>";
	print_r( $settings);
	print "<p>";

}

/*
 * Cleaning the input data
 */
 
if(isset($_POST['report'])) {
	$clean['report'] = $_POST['report'];
}

if (isset($clean['report'])) {
    // flag for multiple selection
    $multi = 0;
    $rowclr = 0;
    $fs = 2; // to change font size in the output
    $sql = ""; // string for sql query
    $big_report = 0; // flag for output bigger than 100 lines
    
    $sql_norm = "SELECT callid,clientid,TO_CHAR(time,'DD/MM/YYYY HH24:MI') as time,chat FROM calls where ";
    $sql_count = "SELECT count(*) FROM calls where ";
    
    // check what fields are selected
    // (1) classification
    if(isset($_POST['class_cb'])) {
		$multi = 1;
		$sql .= "class= " . $_POST['mclass'];
    }
    // (2) date
    if(isset($_POST['date_cb'])) {
		if($multi) {
		    $sql .= " AND ";
		}
		else {
		    $multi = 1;
		}
	
		switch($_POST['when']) {
		    case "year":
				$sql .= "YEAR(time) = " . $_POST['year'];
				break;
		    case "dates":
				if(ereg("([0-9]{2})/([0-9]{2})/([0-9]{4})", $_POST['date_from'], $regs)) {
				    $df = "$regs[3]-$regs[2]-$regs[1] 00:00:00";
				}
				if(ereg("([0-9]{2})/([0-9]{2})/([0-9]{4})", $_POST['date_to'], $regs)) {
				    $dt = "$regs[3]-$regs[2]-$regs[1] 23:59:59";
				}
		
				$sql .= "(time <= '$dt' AND time >= '$df') ";
				break;
		}
    }
    // (3) client details
    if(isset( $_POST['client_cb'])) {
		if($multi) {
		    $sql .= " AND ";
		}
		else {
		    $multi = 1;
		}

		switch($_POST['client']) {
		    case "name":
				$sql .= " clientid=" . $_POST['client'] . " ";
				break;
		    case "gender":
		
				$sql .= " (time <= '$dt' AND time >= '$df') ";
				break;
		}
		
		
    }
    // (4) district
    if(isset( $_POST['district_cb'])) {
		if($multi) {
		    $sql .= " AND ";
		}
		else {
		    $multi = 1;
		}
	
		$sql .= " clientid IN (SELECT clientid FROM clients WHERE districtid=" . $_POST['districtid'] . ") ";
    }
    
	// if debug_sql_limit is set, append it to the query
	if($settings['debug_sql_limit'] > 0) {
		$sql .= " LIMIT " . $settings['debug_sql_limit'];
	}
	
	// check if the report is bigger than we allow per page
	$sql_count .= $sql;
	$big_report = setpagesenv( $sql_count); 
	
	$sql_norm .= $sql;

	$dbConnect = dbconnect();
	
	$result = pg_query( $dbConnect, $sql_norm);
	if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
	}

    $out  = '<hr noshade>';
    $out .= '<font face="Verdana, Arial, Helvetica, sans-serif" size="2">';
    $out .= '<table width="100%" border="1" cellpadding="0" cellspacing="0">';
    $out .= '<tr bgcolor="#00FF00">';
    $out .= '<td width="2%"><font size="' . $fs . '"><b>Call ID</td>';
    $out .= '<td width="2%"><font size="' . $fs . '"><b>Client ID</td>';
    $out .= '<td width="15%"><font size="' . $fs . '"><b>Date & time</td>';
    $out .= '<td><font size="' . $fs . '"><b>Call details</td>';    
    $out .= '</tr>';

	if (!$big_report) {

		// Page Header ...
		printHeader( "Report", 0);

		print "<h1>Report</h1><p>";

		print $out;

		// report table itself    
	    while( $row = pg_fetch_array($result)) {
			if ($rowclr % 2) {
			    $out = '<tr bgcolor="#FFFFFF">';
			}
			else {
			    $out = '<tr bgcolor="#DDDDDD">';	
			}
			
			$out .= '<td><font size="' . $fs . '">';
			$out .= $row['callid'];
			$out .= '</td><td width="2%"><font size="' . $fs . '">';
			$out .= $row['clientid'];
			$out .= '</td><td width="15%"><font size="' . $fs . '">';	
			$out .= $row['time'];
			$out .= '</td><td><font size="' . $fs . '">';	
			if(0 == strlen($row['chat'])) {
				$out .= '&nbsp';
			}
			else {
				$out .= $row['chat'];
			}
			$out .= "</td></tr>";
	
			print $out;
		
			$rowclr++; // change row's background colour
	    }
	}
	else {
		$pdf=new PDF();
		$pdf->AliasNbPages();
		$pdf->sql_query = $sql_norm;
		$pdf->border = $settings['pdf_draw_cell_border'];
		
		//Column titles
		$pdf->header = array('Call ID','Client ID','Date & time','Call details');
		$pdf->SetFont('Arial','',8);
		$pdf->AddPage();
	    
			//Data loading
	//		$data=$pdf->LoadData('countries.txt');
		$pdf->ColoredTable($header,$result);
	
		// print the actual SQL query at the end of the report
		if($settings['debug_pdf'] == 1) {
			$pdf->Write(5, $pdf->sql_query);
		}
		
		$pdf->Output();
	}
	
	dbclose($dbConnect);
}

print "</body></html>";

// return 1 if the report is bigger than $numofrecs
function setpagesenv( $sql) {
	$numofrecs = 25;
	$big_report = 0;
	$pages = 0;
	
// if debug flag is set, print the following info
$settings = get_ca_settings();
if($settings['debug'] == 1){
	print "<b>Query:</b><br>";
	print $sql . "<p>";
}
	
	$dbConnect = dbconnect();
	
	$result = pg_query($dbConnect, $sql);
	if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
	}
	
	$row = pg_fetch_array($result);

	if($row[0] > $settings['force_pdf_when_more_than']) {
		$big_report = 1;
//		print "Number of records: $row[0]<br>";
	}

	dbclose($dbConnect);
	
	return $big_report;
}


?>