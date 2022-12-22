<?php
session_start();
require_once("login.inc");

$settings = get_ca_settings();

$clean = array();

if(isset($_POST['password'])) {
	$clean['password'] = $_POST['password'];
}

if (isset($_POST['loginname'])) {
	$clean['loginname'] = $_POST['loginname'];
}

if ($settings['debug'] > 0) {
	require_once("functions.inc");
	print_debug( $clean, $settings);
}

// if set $_GET['f'] has the page URL the user came from
if(isset($_GET['f']) && filter_var(_GET['f'], FILTER_SANITIZE_URL)) {
    $return_to = filter_var(_GET['f'],  FILTER_VALIDATE_URL, FILTER_FLAG_PATH_REQUIRED|FILTER_FLAG_HOST_REQUIRED);
    if($return_to && isset($_SESSION['s_username'])) {
        header("Location: " . $return_to);  // return to page we came from
    }
    else {
        login($clean['loginname'], $clean['password']);
    }
}
else {
    header("Location: " . $settings['start_page']); // dont know where to go!
}
?>
