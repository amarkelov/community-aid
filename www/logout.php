<?php
session_start();

require_once("config.inc");

$_SESSION = array();
session_destroy();
$settings = get_ca_settings();
header("Location: " . $settings['start_page']);
?>