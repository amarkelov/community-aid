<?php
session_start();

require_once("calls.inc");
require_once("db.inc");
require_once("config.inc");
require_once("login.inc");

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