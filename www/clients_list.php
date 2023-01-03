<?php
session_start();

require_once(dirname(dirname(__FILE__)) . "/php/calls.inc");
require_once(dirname(dirname(__FILE__)) . "/php/db.inc");
require_once(dirname(dirname(__FILE__)) . "/php/config.inc");
require_once(dirname(dirname(__FILE__)) . "/php/login.inc");

if( isset($_SESSION['s_username'])) {
	/*
	 *  $_GET['floating'] will come from /js/calls.js
	 */
	drawClientsList($_SESSION['s_username'], $_GET['floating']);
}
else {    /* we haven't logged in yet */
	printErrorMessage("Not enough privileges to do the request!");
}
?>