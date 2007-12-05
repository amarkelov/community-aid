<?php
session_start();
require 'functions.inc';

$clean = array();
$mysql = array();

$settings = get_ca_settings();

// Page Header ...
printHeader( "System Configuration", 0);

// if debug flag is set, print the following info
if ($settings['debug'] > 0) {
	print_debug();
}

// START LOG IN CODE
$doWeExit = displayLogin(basename($_SERVER['PHP_SELF']), true);
if($doWeExit == true){
	exit;
}
// END LOG IN CODE

/*
 * Start filtering input
 */
 
if(isset($_POST['operatorid'])) {
	if(ctype_digit($_POST['operatorid'])) { 
		$clean['operatorid'] = $_POST['operatorid'];
	}
}

if(isset($_POST['submit'])) {
	$clean['submit'] = $_POST['submit'];
}

if(isset($_POST['org'])) {
	$clean['org'] = $_POST['org'];
}

if($settings['debug'] == 1){
	print "<b>\$clean:</b><br>";
	print_r( $clean);
	print "<p>";
}
/*
 * End of filtering input
 */

if ($clean['submit']) {
}
else {	// this part happens if we don't press submit
	$arDistricts = array();
	
	print '<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
			<font face="Verdana, Arial, Helvetica, sans-serif">
			<div align="left">';

	print '<table border="0" width="100%">
	<tr>
		<td colspan="2"><font face="verdana, arial, helvetica" size="-1">
		Organisation name attribute will change the name in	each page caption.
		</font></td>
	</tr>
	<tr>
		<td ALIGN="left" width="10%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Organisation name: </b>
		</font>
		</td>
		<td>
		<input type="Text" name="location" value="' . $settings['org'] . '" size="30" maxlength="30">
		</td>
	</tr>
	<tr>
		<td colspan="2"><font face="verdana, arial, helvetica" size="-1">
		Location attribute will change the name of your location in
		each page caption.
		</font></td>
	</tr>
	<tr>
		<td ALIGN="left" width="10%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Location: </b>
		</font>
		</td>
		<td>
		<input type="Text" name="location" value="' . $settings['location'] . '" size="30" maxlength="30">
		</td>
	</tr>
	<tr>
		<td colspan="2"><font face="verdana, arial, helvetica" size="-1">
		Available Districts. You can add/remove districts.
		</font></td>
	</tr>';
	
	print '<tr>
		<td ALIGN="left"  width="10%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Districts: </b>
		</font>
		</td>
		<td>';
			
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
		print '</td>
	</tr>';
	
	print '<tr><td><font face="Verdana, Arial, Helvetica, sans-serif">
			<input type="Submit" name="submit" value="Submit">
			</font>
			</div></td></tr>';
	
	print '</table></form>';
}


print '</body></html>';
?>