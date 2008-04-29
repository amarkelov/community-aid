<?php
session_start();
require_once("login.inc");
require_once("classifications.inc");

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Add/Edit Classifications", 0);

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
if(isset($_GET['sub'])) {
	if(ctype_digit($_GET['sub'])) {
		$clean['sub'] = $_GET['sub'];
	}
}
if(isset($_POST['change_main'])) {
	$clean['change_main'] = 1;
}
if(isset($_POST['add_main'])) {
	$clean['add_main'] = 1;
}
if(isset($_POST['mclass_id'])) {
	if(ctype_digit($_POST['mclass_id'])) {
		$clean['mclass_id'] = $_POST['mclass_id'];
	}
}
if(isset($_POST['mclass_name'])) {
	if( ctype_print($_POST['mclass_name'])) {
		$clean['mclass_name'] = $_POST['mclass_name'];
	}
}
if(isset($_POST['edit_mclass_name'])) {
	$clean['edit_mclass_name'] = $_POST['edit_mclass_name'];
}
if(isset($_POST['new_mclass_name'])) {
	$clean['new_mclass_name'] = $_POST['new_mclass_name'];
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

if ( $clean['change_main']) {
	if( updateMainClassificationName( $clean['mclass_id'], $clean['edit_mclass_name'])) {
		printMessage( 'Main classification name updated!');
	}
	else {
		printErrorMessage( 'Error occured while updating main classification name!');
	}
	printMessage('<a href="' . $_SERVER['PHP_SELF'] . '">Edit main classification name</a>');
}
else if( $clean['add_main']) {
	$arNewMains = explode( "\n", $clean['new_mclass_name']);
	if( addNewMainClassificationNames( $arNewMains)) {
		printMessage( 'New main classification name(s) added!');
	}
	else {
		printErrorMessage( 'Error occured while adding new main classification name(s)!');
	}
	printMessage('<a href="' . $_SERVER['PHP_SELF'] . '">Add more main classification names</a>');
}
else {
	if( isset($clean['sub']) && $clean['sub'] == 1) {
		print '<font face="verdana, arial, helvetica" size="3">
				<b>Edit existing sub-classification names:</b>
				|
				<a href="' . $_SERVER['PHP_SELF'] . '?sub=0">
				<b>Edit existing main classification names</b>
				</a>
				</font>';
		printAddEditSubClassificationLayout();
	}
	else {
		print '<font face="verdana, arial, helvetica" size="3">
				<b>Edit existing main classification names:</b>
				|
				<a href="' . $_SERVER['PHP_SELF'] . '?sub=1">
				<b>Edit existing sub-classification names</b>
				</a>
				</font>';
		printAddEditMainClassificationLayout();
	}
}

print '</form></body></html>';

?>