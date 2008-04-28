<?php
require_once("login.inc");
require_once("classifications.inc");

// Page Header ...
printHeader( "Classifications dictionary", 0);

if( !getClassificationDictionary()) {
	printErrorMessage( 'Can not get classifications dictionary!');
}
?>
