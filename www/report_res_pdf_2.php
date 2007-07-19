<?php
require('functions.inc');
require('generate_pdf.inc');

$pdf = new PDF('P','pt','A4');
$pdf->SetFont('Arial','',9);
//$pdf->fill = 1;
$pdf->header_fill_color = array(17,40,200);
$pdf->cell_fill_color = array(224,235,255);
$pdf->AliasNbPages();
// The first Parameter is localhost again unless you are retrieving data from a different server.
// The second parameter is your MySQL User ID.
// The third parameter is your password for MySQL. In many cases these would be the same as your OS ID and Password.
// The fourth parameter is the Database you'd like to run the report on.
$pdf->connect('gmp','gmadmin','old290174','gmpDb');
// This is the title of the Report generated.
$attr=array('titleFontSize'=>12,'titleText'=>'GoodMorning Project report');
// This is your query. It should be a 'SELECT' query.
// Reports are run over 'SELECT' querires generally.
$pdf->mysql_report("SELECT callid as 'Call ID',clientid as 'Client ID',DATE_FORMAT(time,'%d/%m/%Y %H:%i') as 'Date & Time of the call',chat as 'Call Details' FROM calls where clientid=227",false,$attr);

?>

