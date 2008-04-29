<?php
session_start();
require_once("login.inc");
require_once("district.inc");

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Add/Edit Districts", 0);

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

/*
 * Start filtering input
 */
if(isset($_POST['edit'])) {
	$clean['edit'] = 1;
}
if(isset($_POST['add'])) {
	$clean['add'] = 1;
}
if(isset($_POST['districtid'])) {
	if(ctype_digit($_POST['districtid'])) {
		$clean['districtid'] = $_POST['districtid'];
	}
}
if(isset($_POST['edit_district_name'])) {
	if( ctype_print($_POST['edit_district_name'])) {
		$clean['edit_district_name'] = $_POST['edit_district_name'];
	}
}
if(isset($_POST['new_district_name'])) {
	$clean['new_district_name'] = $_POST['new_district_name'];
}
if(isset($_SESSION['operatorid'])){
	if(ctype_digit($_SESSION['operatorid'])) {
		$clean['operatorid'] = $_SESSION['operatorid'];
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

if ( $clean['edit']) {
	if( updateDistrictName( $clean['districtid'], $clean['edit_district_name'])) {
		printMessage( 'District name updated!');
	}
	else {
		printErrorMessage( 'Error occured while updating district name!');
	}
	printMessage('<a href="' . $_SERVER['PHP_SELF'] . '">Edit another district name</a>');
}
else if( $clean['add']) {
	$arNewDistricts = explode( "\n", $clean['new_district_name']);
	if( addDistrict( $arNewDistricts)) {
		printMessage( 'New district(s) added!');
	}
	else {
		printErrorMessage( 'Error occured while adding new district(s)!');
	}
	printMessage('<a href="' . $_SERVER['PHP_SELF'] . '">Add more districts</a>');
}
else {
	$arDistricts = array();
	
	print '<font face="verdana, arial, helvetica" size="3"><b>Edit existing district name:</b></font>';
	print '<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
		<font face="verdana, arial, helvetica" size="2">
		<TABLE BORDER=0 WIDTH=100%>
			<tr>
				<td ALIGN="left"  width="10%" VALIGN="top">
				<font face="verdana, arial, helvetica" size="2">
				<b>District: </b>
				</font>
				</td>
				<td ALIGN="left"  width="30%" VALIGN="top">';
					
				if( getDistrictList( $arDistricts)) {
					print '<select name="districtid">';
					
					foreach( $arDistricts as $did => $district_name) {
						print '<option value="' . $did . '">' . $district_name . '</option>';
					}
					
					print '</select>';
				}

	$out .= '</td>
			</tr>
			<tr>
			<td ALIGN="left" width="10%" VALIGN="top">
			<b>New name for the selected district:</b>
			</td>
			<td ALIGN="left" width="30%" VALIGN="top">
			<input type="text" name="edit_district_name" maxlength="30" size="30">
			</td>
			</tr>
			<tr>
			<td ALIGN="left" width="10%" VALIGN="top">
			<input name="edit" type="submit" value="Save the new name">
			</td>
			</tr>
			</TABLE>
			</font>';
				
	print $out . '<p><font face="verdana, arial, helvetica" size="2">';
	
	print '<hr noshade><br>
			<font face="verdana, arial, helvetica" size="3"><b>Add new district:</b></font>
			<br>
			Add new names of districts (one per line) and press "Add new district(s)" button below
			<br>
			<textarea name="new_district_name" cols="27" rows="4" ></textarea>
			<br><br>
			<input name="add" type="submit" value="Add new district(s)">';
	
}

print '</form></body></html>';

?>