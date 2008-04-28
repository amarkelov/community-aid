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
	print_debug( $clean, $settings);
}
	
if (isset($_SESSION['s_username'])) {
	header("Location: ".$_GET['f'] ."?a"); //already logged in
}else{
	login( $clean['loginname'], $clean['password']);
}
if (isset($_GET['f']) ){
	header("Location: ".$_GET['f']);  // return to page we came from
	
}else{
	header("Location: " . $settings['start_page']); // dont know where to go!
} 
?>   
