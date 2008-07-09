<?php
session_start();
require_once("login.inc");
require_once("report.inc");
require_once("district.inc");
require_once("classifications.inc");
require_once("client.inc");

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Report", 0);

// if debug flag is set, print the following info
if ($settings['debug'] > 0) {
	require_once("functions.inc");
	print_debug();
}

// START LOG IN CODE
$doWeExit = displayLogin(basename($_SERVER['PHP_SELF']), true);
if($doWeExit == true){
	exit;
}
// END LOG IN CODE
?>
<h1>Report</h1>
<font face="Verdana, Arial, Helvetica, sans-serif" size="2">
Please, choose criteria for the report and press 'Submit' button.
</font>
<p>

<form method="post" action="report_res.php" >

<font face="Verdana, Arial, Helvetica, sans-serif" size="2">
<table border="0" cellpadding="5">
<tr> 
    <td>
    <input type="checkbox" name="class_cb"></input>
    </td>

    <td>
    by Classification: 
    <?php getCombinedClassificationList() ?>
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
</tr>
<tr>
    <td valign="top">
    <input type="checkbox" name="client_cb"></input>
    </td>
    <td valign="top">
    by Client details:
    <br>
    <table border="1" cellpadding="0" cellspacing="0">
    <tr>
	    <td>
		    <input type="radio" name="clientby" value="byid" checked></input>
	    </td>
    	<td>
		    Name:
		    <select name="clientid" size="1">
		    <?php getClientsAsDropDownList() ?>
		    </select>
		</td>
	</tr>
	<tr>
	    <td>
		    <input type="radio" name="clientby" value="bygender"></input>
	    </td>
		<td>
			Gender:
			<input type="radio" name="gender" value="female" checked>Female</input>
			<input type="radio" name="gender" value="male">Male</input>
		</td>
	</tr>
	</table>
    </td>
</tr>
<tr>
    <td>
    <input type="checkbox" name="district_cb"></input>
    </td>
    <td valign="top">
    by District:
		<?php
		$arDistricts = array(); 
		if( getDistrictList( $arDistricts)) {
			print '<select name="districtid">';
			
			foreach( $arDistricts as $did => $district_name) {
				if($clean['districtid'] == $did) {
					print '<option value="' . $did . '" selected>' . $district_name . '</option>';
				}
				else {
					print '<option value="' . $did . '">' . $district_name . '</option>';
				}
			}
			
			print '</select>';
		}
		?>
     </td>
</tr>

</table>
<br>

<left><input type="submit" name="report" value="Submit"></input></left>
<input type="hidden" name="debug" value="<?php echo $settings['debug'] ?>">

</font>
</form>
</body>
</html>
