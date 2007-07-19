<?php
require 'functions.inc';

// pdf stuff begin
require('fpdf.php');



class PDF extends FPDF
{
var $sql_query = 0;
var $header = 0;
	
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
        $this->Cell($w[0],4,$row[0],0,0,'L',$fill);
        // caller id
        $this->Cell($w[1],4,$row[1],0,0,'L',$fill);
        // date & time
        $this->Cell($w[2],4,$row[2],0,0,'C',$fill);
        // chat
        $this->MultiCell($w[3],4,$row[3],0,'L',$fill);
        $fill=!$fill;
    }
    $this->Cell(array_sum($w),0,'','T');
}
}



// pdf stuff end

// global $db, $clientid, $list, $done  ;
// if (!$list )  {$list="Grey" ; }

$db = "gmpDb";
//$db = $HTTP_HOST . "Db";
$dbConnect = mysql_connect("localhost", "gmadmin", "old290174");
if (!$dbConnect) {
	die('Could not connect: ' . mysql_error());
}

mysql_select_db($db, $dbConnect);

if ($report) {
    // flag for multiple selection
    $multi = 0;
    $rowclr = 0;
    $fs = 2; // to change font size in the output
    $sql = ""; // string for sql query
    $big_report = 0; // flag for output bigger than 100 lines
    $filename = 'report.html'; // filename for big reports
    $repfh = NULL; 
    
    $sql_norm = "SELECT callid,clientid,DATE_FORMAT(time,'%d/%m/%Y %H:%i'),chat FROM calls where ";
    $sql_count = "SELECT count(*) FROM calls where ";
    
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

// first check if result will exceed 100 lines
	$sql_count .= $sql;
//	print $sql_count;
	$result = mysql_query($sql_count);
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . '<br>';
		$message .= 'Whole query: ' . $sql_count;
		die($message);
	}
	$row = mysql_fetch_array($result);

	if($row[0] > 100) {
		$big_report = 1;

		// create report file		
	    if (!$repfh = fopen($filename, 'w')) {
	         echo "Cannot open file ($filename)";
	    }
	}
	
	$sql_norm .= $sql;
	$result = mysql_query($sql_norm);
	
	if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $sql_norm;
		die($message);
	}

	$pdf=new PDF();
	$pdf->AliasNbPages();
	$pdf->sql_query = $sql_norm;
	
	//Column titles
	$pdf->header = array('Call ID','Client ID','Date & time','Call details');
	$pdf->SetFont('Arial','',8);
	$pdf->AddPage();
    
    while( $row = mysql_fetch_array($result)) {
		//Data loading
//		$data=$pdf->LoadData('countries.txt');
		$pdf->ColoredTable($header,$result);

    }

	$pdf->Write(5, $pdf->sql_query);
	$pdf->Output();
	   
}

?>

</form>
</body>
</html>
