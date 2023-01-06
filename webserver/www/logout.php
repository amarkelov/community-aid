<?php
session_start();

require_once(dirname(dirname(__FILE__)) . "/php/config.inc");

$_SESSION = array();
session_destroy();
$settings = get_ca_settings();
header("Location: " . $settings['start_page']);
?>