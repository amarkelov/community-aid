<?php
/*
 * Created on 05-Nov-2006
 *
 * To change the template for this generated file go to
 * Window - Preferences - PHPeclipse - PHP - Code Templates
 */
require 'functions.inc';

$clean = array();
$mysql = array();

$settings = get_gmp_settings();

?>

<html>
<head>
<title>Clients List -- Good Morning <?php echo $settings['location'] ?></title>
<meta http-equiv="expires" content="-1">
<meta http-equiv="Cache-Control" content="no-cache">
<script language="JavaScript">

<!-- hide it from old browsers or from those with JavaScript disabled
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
<body bgcolor="#BEC8FD">

<p>

<?php
// if debug flag is set, print the following info
if($settings['debug'] == 1){
	print "<b>\$_POST:</b><br>";
	print_r( $_POST);
	print "<p>";

	print "<b>\$_GET:</b><br>";
	print_r( $_GET);
	print "<p>";

	print "<b>\$settings:</b><br>";
	print_r( $settings);
	print "<p>";

}

	$dbConnect = dbconnect();
	
    $sql  = "SELECT clientid,firstname,lastname,houseno,street,list ";
    $sql .= " FROM clients ORDER BY lastname,firstname DESC";
    $result = mysql_query( $sql, $dbConnect);
    if (!$result) {
		$message  = 'Invalid query: ' . mysql_error() . "\n";
		$message .= 'Whole query: ' . $sql;
		die($message);
    }

    print ("<hr noshade>");
    print('<table border="1" width="100%" cellspacing="0" cellpadding="0">');
    $i=0; // need this to change cell bgcolor

    while ($myrow = mysql_fetch_array($result)) {
		$i=$i + 1; // need this to change cell bgcolor
	
		if($i % 2) {
		    print('<tr bgcolor="#FFFFFF">');
		}
		else {
		    print('<tr bgcolor="#DDDDDD">');
		}
	// check-box column
		print '<td width="1%"><font color="#FF0000">';
		print '<input type="checkbox" name="' . $myrow["clientid"] . '"></input>';
		print '</font>';
	// client column
		if( $myrow["list"] == "black") {
			print '</td><td width="0%"><font color="#000000"><b>';
		}
		else {
    		print '</td><td width="0%"><font color="#01cf01"><b>';
		}
		print strtoupper($myrow["lastname"]) . ", " . strtoupper($myrow["firstname"]) . "( " . $myrow["clientid"] . " )";
		print '</b></font>';
	// address column
		print '</td><td><font color="#0000FF">';
		print $myrow["houseno"] . ", " . strtoupper($myrow["street"]);
		print '</font>';
	// edit column
		print '</td><td><font color="#0000FF">';
		print '<a href="/edit_client.php?clientid=' . $myrow["clientid"] . '"><img src="images/new/edit.png" alt="Edit client record" border="0"></a>';
		print '</font>';
	// remove/black-list column
		print '</td><td>';
		if( $myrow["list"] == "black") {
			print '<a href="' . $PHP_SELF . '"><img src="images/new/unlock.png" alt="Restore client record" border="0"></a>';
		}
		else {
			print '<a href="' . $PHP_SELF . '"><img src="images/new/encrypted.png" alt="Remove client record" border="0"></a>';
		}
	// no more raws
		print "</td></tr>";
    } // while
    
    // end of table
    print("</table>");

	dbclose($dbConnect);

?>

</body>
</html>