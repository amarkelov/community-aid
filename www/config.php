<?php
session_start();
require_once("login.inc");
require_once("district.inc");
require_once("classifications.inc");

$clean = array();
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

if(isset( $_POST['org']) && ctype_print( $_POST['org'])) {
	$clean['org'] = $_POST['org'];
}
if(isset( $_POST['location']) && ctype_print( $_POST['location'])) {
	$clean['location'] = $_POST['location'];
}
if(isset( $_POST['start_page']) && ctype_print( $_POST['start_page'])) {
	$clean['start_page'] = $_POST['start_page'];
}
if(isset( $_POST['database']) && ctype_print( $_POST['database'])) {
	$clean['database'] = $_POST['database'];
}
if(isset( $_POST['pghost']) && ctype_print( $_POST['pghost'])) {
	$clean['pghost'] = $_POST['pghost'];
}
if(isset( $_POST['pgport']) && ctype_digit( $_POST['pgport'])) {
	$clean['pgport'] = $_POST['pgport'];
}
if(isset( $_POST['pdf_draw_cell_border']) && ctype_digit( $_POST['pdf_draw_cell_border'])) {
	if($_POST['pdf_draw_cell_border'] > 0) {
		$clean['pdf_draw_cell_border'] = 1;
	}
	else {
		$clean['pdf_draw_cell_border'] = 0;
	}
}
if(isset( $_POST['force_pdf_when_more_than']) && ctype_digit( $_POST['force_pdf_when_more_than'])) {
	$clean['force_pdf_when_more_than'] = $_POST['force_pdf_when_more_than'];
}
if(isset( $_POST['debug']) && ctype_digit( $_POST['debug'])) {
	if($_POST['debug'] > 0) {
		$clean['debug'] = 1;
	}
	else {
		$clean['debug'] = 0;
	}
}
if(isset( $_POST['debug_pdf']) && ctype_digit( $_POST['debug_pdf'])) {
	if( $_POST['debug_pdf'] > 0) {
		$clean['debug_pdf'] = 1;
	}
	else {
		$clean['debug_pdf'] = 0;
	}
}
if(isset( $_POST['debug_sql_limit']) && ctype_digit( $_POST['debug_sql_limit'])) {
	$clean['debug_sql_limit'] = $_POST['debug_sql_limit'];
}
if(isset( $_POST['defaults']) && ctype_alpha( $_POST['defaults'])) {
	if(strtoupper( $_POST['defaults']) == "ON") {
		$clean['defaults'] = true;
	}
	else {
		$clean['defaults'] = false;
	}
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
	if( saveIniFile( $clean, $clean['defaults'])) {
		printMessage('New configuration saved.');
	}
	else {
		printErrorMessage('Error occured while saving configuration file!');
	}
}
else {	// this part happens if we don't press submit
	$arDistricts = array();
	
	print '<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
			<font face="Verdana, Arial, Helvetica, sans-serif">
			<div align="left">';

	print '<table border="0" width="100%">
	<tr>
		<td colspan="2"><font face="verdana, arial, helvetica" size="2">
		Organisation name attribute will change the name in	each page caption.
		</font></td>
	</tr>
	<tr>
		<td ALIGN="left" width="20%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="2">
		<b>Organisation name: </b>
		</font>
		</td>
		<td>
		<input type="Text" name="org" value="' . $settings['org'] . '" size="30" maxlength="30">
		</td>
	</tr>
	<tr>
		<td colspan="2"><font face="verdana, arial, helvetica" size="2">
		Location attribute will change the name of your location in
		each page caption.
		</font></td>
	</tr>
	<tr>
		<td ALIGN="left" width="20%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="2">
		<b>Location: </b>
		</font>
		</td>
		<td>
		<input type="Text" name="location" value="' . $settings['location'] . '" size="30" maxlength="30">
		</td>
	</tr>
	<tr>
		<td colspan="2"><font face="verdana, arial, helvetica" size="2">
		Available Districts. You can add/remove districts.
		</font></td>
	</tr>';
	
	print '<tr>
		<td ALIGN="left"  width="20%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="2">
		<b>Districts: </b>
		</font>
		</td>
		<td><font face="verdana, arial, helvetica" size="2">';
			
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
		print ' <a href="/add_edit_districts.php">Add/Edit districts</a></font></td>
	</tr>
	<tr>
		<td ALIGN="left"  width="20%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="2">
		<b>Call Classification:</b>
		</td>
		
		<td>
		<font face="verdana, arial, helvetica" size="2">';
             
	// draw classification  
	getCombinedClassificationList();
		
	print ' <a href="/add_edit_classifications.php">Add/Edit classifications</a></font></td>
	</tr>
	<tr>
		<td ALIGN="left" width="20%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="2">
		<b>Force PDF creation if the report consists of more than: </b>
		</font>
		</td>
		<td>
		<font face="verdana, arial, helvetica" size="2">
		<input type="Text" name="force_pdf_when_more_than" value="' . $settings['force_pdf_when_more_than'] . '" size="10" maxlength="30">
		lines
		</font>
		</td>
	</tr>

	<tr><td colspan="2">&nbsp</td></tr>

	<tr>
		<td colspan="2"><font face="verdana, arial, helvetica" size="2">
		Database server parameters.<br>
		<font color="#FF0000"><b>
		Do not change any of the database parameters unless you know exactly what you are doing!<br>
		If you made mistake, read the Administration manual on how to edit configuration file manualy. 
		</b></font></td>
	</tr>
	<tr>
		<td ALIGN="left" width="20%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="2">
		<b>Database name: </b>
		</font>
		</td>
		<td>
		<input type="Text" name="database" value="' . $settings['database'] . '" size="30" maxlength="30">
		</td>
	</tr>
	<tr> 
		<td ALIGN="left" width="20%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="2">
		<b>Database server name: </b>
		</font>
		</td>
		<td>
		<input type="Text" name="pghost" value="' . $settings['pghost'] . '" size="30" maxlength="30">
		</td>
	</tr>
	<tr>
		<td ALIGN="left" width="20%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="2">
		<b>Database server port number: </b>
		</font>
		</td>
		<td>
		<input type="Text" name="pgport" value="' . $settings['pgport'] . '" size="10" maxlength="30">
		</td>
	</tr>';
		
	print '</div></table>';
	
	print '<font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			<br><input type="checkbox" name="defaults">Set everything back to default values</input><br>
			<br><input type="Submit" name="submit" value="Submit">
			</font></form>';
}


print '</body></html>';
?>