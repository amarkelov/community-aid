<?php
session_start();
require_once("login.inc");
require_once("groups.inc");

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Add Group", 0);

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

if(isset($clean['add'])) {
/*	if(ctype_print($_POST['groupname'])) {*/
		$clean['groupname'] = $_POST['groupname'];
/*	}*/
}

if(isset($clean['edit'])) {
	if(ctype_digit($_POST['groupid'])) {
		$clean['groupid'] = $_POST['groupid'];
	}
	if(ctype_print($_POST['newgroupname'])) {
		$clean['newgroupname'] = $_POST['newgroupname'];
	}
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

if ($clean['add']) {
	if ($clean['operatorid']) {
		if(isset($clean['groupname'])) {
			$arNewGroups = array();
			$arNewGroups = explode( "\n", $clean['groupname']);
			if( addGroup( $arNewGroups)) {
				printMessage( 'New group(s) added!');
			}
			else {
				printErrorMessage( 'Error occured while adding new group(s)!');
			}
			printMessage('<a href="' . $_SERVER['PHP_SELF'] . '">Add more groups</a>');
		}
	}
}
else if($clean['edit']) {
	if ($clean['operatorid']) {
		if( updateGroupName( $clean['groupid'], $clean['newgroupname'])) {
			printMessage( 'Group name updated!');
		}
		else {
			printErrorMessage( 'Error occured while updating group name!');
		}
		printMessage('<a href="' . $_SERVER['PHP_SELF'] . '">Edit another group name</a>');
	}
}
else {	// this part happens if we don't press submit
	if ( $clean['operatorid']) {
		print '<font face="verdana, arial, helvetica" size="3"><b>Edit existing Group name:</b></font>
			<div align="left"><font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
			<table>
			<tr>
				<td>Old Group name:</td><td>';
		getGroupNamesAsDropDownList();

		print '</td></tr>';
			
		print '
			<tr>
				<td>New name for the selected district:</td>
				<td><input type="text" name="newgroupname" maxlength="30" size="30"></td>
			</tr>
			</table>
			<input name="edit" type="submit" value="Save New Group name">
			</form>
			</font>
			</div>';
	
		print '<hr noshade><br>';
		
		print '<font face="verdana, arial, helvetica" size="3"><b>Add new Group name(s):</b></font>';
		print '<div align="left"><font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
			<font face="verdana, arial, helvetica" size="2">
			Add new names of groups (one per line) and press "Add new group(s)" button below
			<br>
			<textarea name="groupname" cols="27" rows="4" ></textarea>
			<br><br>
			<input name="add" type="submit" value="Add new group(s)">
			</form></font></div>';
	}
}

print '</body></html>';

?>
