<html>
<head>
<title>
Classification. Full and short names.
</title>
</head>

<body>

<?php
require 'functions.inc';

$dbConnect = dbconnect();

if (!$dbConnect) {
    die('Could not connect: ' . mysql_error());
}

$sql  = "select mc.mclass_name,sc.sclass_name,";
$sql .= "sc.sclass_sname from call_mclass as mc, ";
$sql .= "call_sclass as sc where mc.mclass_id = sc.mclass_id";

$result = mysql_query( $sql, $dbConnect);
if (!$result) {
    $message  = 'Invalid query: ' . mysql_error() . "\n";
    $message .= 'Whole query: ' . $query;
    die($message);
}

$out  = '<table border="0" width="100%" cellspacing="0" cellpadding="0">';
$out .= '<tr>';
$out .= '<td width="40%"><font size="1"><b>Main Class name</b></font></td>';
$out .= '<td width="40%"><font size="1"><b>Sub Class name</b></td>';
$out .= '<td><small><font size="1"><b>Short Sub Class name</b></td>';
$out .= '</tr>';

print($out);

$i=0; // need this to change cell bgcolor

while ($myrow = mysql_fetch_array($result)) {
    // select sclass short name
    $mclass_name = $myrow[0];
    $sclass_name = $myrow[1];
    $sclass_sname = $myrow[2];
    
    $i=$i + 1; // need this to change cell bgcolor

    if($i % 2) {
	print('<tr bgcolor="#FFFFFF">');
    }
    else {
	print('<tr bgcolor="#DDDDDD">');
    }

    $out  = "<td><font size=\"1\">$mclass_name</td>";
    $out .= "<td><font size=\"1\">$sclass_name</td>";
    $out .= "<td><font size=\"1\">$sclass_sname</td>";
    $out .= "</tr>";
    
    print($out);
}
?>

</body>
</html>
