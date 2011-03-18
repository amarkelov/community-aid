<?php
session_start();
require_once("login.inc");
require_once("report.inc");
require_once("classifications.inc");
require_once("district.inc");

$clean = array();
$settings = get_ca_settings();

// START LOG IN CODE
$doWeExit = displayLogin(basename($_SERVER['PHP_SELF']), false, true);
if($doWeExit == true){
	exit;
}
// END LOG IN CODE

// if debug flag is set, print the following info
if($settings['debug'] == 1){
	require_once("functions.inc");
	print_debug();
}

/*
 * Cleaning the input data
 */
validateL1ClassificationList ( $_POST, $clean);
validateL2ClassificationList ( $_POST, $clean);

if(isset($_POST['report'])) {
	$clean['report'] = $_POST['report'];
}

if(isset($_POST['sclass_id']) && ctype_digit($_POST['sclass_id'])) {
	$clean['sclass_id'] = $_POST['sclass_id'];
}

if (isset($clean['report'])) {
    // flag for multiple selection
    $multi = 0;
    $no_where_clause = true;
    $rowclr = 0;
    $fs = 2; // to change font size in the output
    $sql = ""; // string for sql query
    $criteria = array();
    $big_report = false; // flag for output bigger than force_pdf_when_more_than lines
    
    $sql_norm = "SELECT callid,clientid,TO_CHAR(time,'DD/MM/YYYY HH24:MI') as time,chat FROM calls where ";
    $sql_count = "SELECT count(*) FROM calls where ";
    
    // check what fields are selected
    // (1) classification
    if(isset($_POST['class_cb'])) {
		$multi = 1;
		$L1_class_id_list = "";
    	
		$sql .= "callid in (select callid from call_l1_class where l1id in (";
				
    	/* handle call classification now */
		if( isset($clean['L1']) && sizeof($clean['L1']) > 0) {
			$arL1 = array();
			$arL1 = $clean['L1'];
	
			for( $i=0; $i < sizeof($arL1); $i++) {
				if($i == sizeof($arL1) - 1) {
					$L1_class_id_list .= sprintf( "%d", $arL1[$i]);
				}
				else {
					$L1_class_id_list .= sprintf( "%d,", $arL1[$i]);
				} 
			}
		}

		$sql .= $L1_class_id_list . ") ";

		/* if L2 chosen, add it to the mix */
		if( isset($clean['L2']) && sizeof($clean['L2']) > 0) {
			$arL2 = array();
			$arL2 = $clean['L2'];
			$L2_class_id_list = "";
			$sql .= " UNION select callid from call_l2_class where l2id in ("; 			
	
			for( $i=0; $i < sizeof($arL2); $i++) {
				if($i == sizeof($arL2) - 1) {
					$L2_class_id_list .= sprintf( "%d", $arL2[$i]);
				}
				else {
					$L2_class_id_list .= sprintf( "%d,", $arL2[$i]);
				} 
			}
			
			$sql .= $L2_class_id_list . ")) ";
		}
		
//		$sql .= "class= " . $clean['sclass_id'];
		$no_where_clause = false;
		$criteria['Classification'] = getL1andL2ClassificationNamesList( $arL1, $arL2);
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
				$sql .= "to_char(time,'YYYY') = '" . $_POST['year'] . "'";
				$no_where_clause = false;
				$criteria['Year'] = $_POST['year'];
				break;
		    case "dates":
				if(ereg("([0-9]{2})/([0-9]{2})/([0-9]{4})", $_POST['date_from'], $regs)) {
				    $df = "$regs[3]-$regs[2]-$regs[1] 00:00:00";
				}
				if(ereg("([0-9]{2})/([0-9]{2})/([0-9]{4})", $_POST['date_to'], $regs)) {
				    $dt = "$regs[3]-$regs[2]-$regs[1] 23:59:59";
				}
		
				$sql .= "(time <= '$dt' AND time >= '$df') ";
				$no_where_clause = false;
				
				$criteria['Dates'] =  $_POST['date_from'] . " - " . $_POST['date_to'];
				
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

		switch($_POST['clientby']) {
		    case "byid":
				$sql .= " clientid=" . $_POST['clientid'] . " ";
				$no_where_clause = false;
				$criteria['Client ID'] = $_POST['clientid'];
				break;
		    case "bygender":
				$sql .= " clientid IN ( SELECT clientid FROM clients WHERE gender='" . $_POST['gender'] . "') ";
				$no_where_clause = false;
				$criteria['Gender'] = $_POST['gender'];
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
		$no_where_clause = false;
		$criteria['District'] = getDistrictName( $_POST['districtid']);
    }
    
	// if debug_sql_limit is set, append it to the query
	if($settings['debug_sql_limit'] > 0) {
		$sql .= " LIMIT " . $settings['debug_sql_limit'];
	}

	if( !$no_where_clause) {
		// check if the report is bigger than we allow per page
		$sql_count .= $sql;
		$records_found = setpagesenv( $sql_count, $big_report); 
		
		$sql_norm .= $sql . " ORDER BY callid";

		/* if we asked for count only */
		if(isset($_POST['countonly_cb'])) {
			// Page Header ...
			printHeader( "Report", 0);
	
			print '<div align="right"><a href="report.php">Create another report</a></div><br>';
			printMessage( 'Report created on ' . date( 'l M d, o \a\t H:i'));
			
			foreach( $criteria as $cr_key => $cr_value) {
				print '<br><font size="1">' . $cr_key . ': ' . $cr_value . '</font>';
			}
		
			printMessage("For the chosen filter " . $records_found . " records found.");
		}
		else {
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
		
				print '<div align="right"><a href="report.php">Create another report</a></div><br>';
				printMessage( 'Report created on ' . date( 'l M d, o \a\t H:i'));
				
				foreach( $criteria as $cr_key => $cr_value) {
					print '<br><font size="1">' . $cr_key . ': ' . $cr_value . '</font>';
				}
				
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
		
				$pdf->top_message = "Report created on " . date( 'l M d, o \a\t H:i');
				$pdf->criteria = $criteria;
				
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
	}
	else {
		// Page Header ...
		printHeader( "Report", 0);
		printMessage( "You forgot to choose a filter for the report. Please, <a href='report.php'>try again</a>.");
	}
}
print "</body></html>";

/*
 * returns count of records found
 * $big_report is set to true, if the number of found records is greater 
 * than force_pdf_when_more_than parameter of the system
 */
function setpagesenv( $sql, &$big_report) {
	$settings = get_ca_settings();
	
	$dbConnect = dbconnect();
	
	$result = pg_query($dbConnect, $sql);
	if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
	}
	
	$row = pg_fetch_array($result);

	if($row[0] > $settings['force_pdf_when_more_than']) {
		$big_report = true;
	}

	dbclose($dbConnect);
	
	return $row[0];
}


?>