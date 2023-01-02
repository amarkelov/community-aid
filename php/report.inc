<?php
use Fpdf\Fpdf as FPDF;

class PDF extends FPDF
{
	var $sql_query = 0;
	var $top_message = 0;
	var $criteria = array();
	var $header = 0;
	var $border = 0;

	//Page header
	function Header()
	{
		$this->SetTextColor(0);
		$this->Cell(0,5, $this->top_message,0,1,'L');
		foreach( $this->criteria as $cr_key => $cr_value) {
			$this->Cell(0,5, $cr_key . ": " . $cr_value,0,1,'L');
		}
	    $this->Ln();

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
	    // //Colors, line width and bold font
	    // $this->SetFillColor(17,40,200);
	    // $this->SetTextColor(255);
	    // $this->SetDrawColor(128,0,0);
	    // $this->SetLineWidth(.3);
	    // $this->SetFont('','B');
	    //Header
	    $w=array(20,20,40,105);
	    // for($i=0;$i<count($header);$i++)
	    //     $this->Cell($w[$i],7,$header[$i],1,0,'C',1);
	    // $this->Ln();
	    //Color and font restoration
	    $this->SetFillColor(224,235,255);
	    $this->SetTextColor(0);
	    $this->SetFont('');
	    //Data
	    $fill=0;


	    while( $row = pg_fetch_array($query_result))
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
?>
