<?php
	require 'functions.inc';
	session_start();
	$_SESSION = array();
	session_destroy();
	$settings = get_ca_settings();
	header("Location: " . $settings['start_page']);
?>