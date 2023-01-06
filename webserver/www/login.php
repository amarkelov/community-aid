<?php
session_start();
require_once(dirname(dirname(__FILE__)) . '/php/login.inc');

$settings = get_ca_settings();

$clean = array();

if(isset($_POST['password'])) {
	$clean['password'] = $_POST['password'];
}

if (isset($_POST['loginname'])) {
	$clean['loginname'] = $_POST['loginname'];
}

if ($settings['debug'] > 0) {
	require_once(dirname(dirname(__FILE__)) . "/php/functions.inc");
	print_debug( $clean, $settings);
}

// if set $_GET['f'] has the page URL the user came from
if(isset($_GET['f']) && preg_match("/^[a-z-]+\.php$/", $_GET['f'])) {
    $got = $_GET['f'];
    $return_to = $got;

    printErrorMessage("INFO: return_to: $return_to ; GET: $got " );

    if($return_to && isset($_SESSION['s_username'])) {
        header("Location: " . $return_to);  // return to page we came from
    }
    else {
        login($clean['loginname'], $clean['password']);
    }
}
else {
    $got = $_GET['f'];
    printErrorMessage("CRITICAL: URL manipulation detected! GET: $got" );
    session_destroy();
    header("Location: " . $settings['start_page']); // re-direct to default start page
}
?>
