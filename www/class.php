<?php
require 'functions.inc';

// Page Header ...
printHeader( "Classifications dictionary", 0);

if( !getClassificationDictionary()) {
	printErrorMessage( 'Can not get classifications dictionary!');
}
?>
