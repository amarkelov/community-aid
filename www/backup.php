<?php 
session_start();

require_once(dirname(dirname(__FILE__)) . "/php/login.inc");

$clean = array();
$settings = get_ca_settings();

// clean the input
if(isset( $_POST['backup'])) {
	$clean['backup'] = true;
}
else {
	$clean['backup'] = false;
}

if(isset( $_POST['backup_ready'])) {
	$clean['backup_ready'] = true;
}
else {
	$clean['backup_ready'] = false;
}
// clean (end)

if( $clean['backup']) {
	printHeader( "One-off database backup", 0);
}
else {
	printHeader( "One-off database backup", 0);
}

if ($settings['debug'] > 0) {
	require_once(dirname(dirname(__FILE__)) . "/php/functions.inc");
	print_debug( $clean, $settings);
}

// START LOG IN CODE
$doWeExit = displayLogin(basename($_SERVER['PHP_SELF']), false);
if($doWeExit == true){
	exit;
}
else {
	$clean['operator'] = $_SESSION['s_username'];
}
// END LOG IN CODE

if( $clean['backup']) {
	printMessage( 'Taking the database backup. Please wait.....');

	$command  = $settings['backup_command'] . ' ' . $settings['database'] . ' | /bin/gzip -9 >';
	$command .= $settings['backup_dir'] . '/' . $settings['database'] . '-dataonly.';
	$command .= date('dmYHi') . '.sql.gz';
	
	exec( $command);
	
	if ($handle = opendir($settings['backup_dir'])) {
		$gotit = false;
	    while (false !== ($file = readdir($handle)) && !$gotit) {
	        if ($file != "." && $file != "..") {
				$gotit = true;	            
	        }
	    }
		closedir($handle);
	}
	
	printMessage( 'Backup is ready. To retrieve the file, please, press the button below.');
	$out  = '<form method="post" action="' . $_SERVER['PHP_SELF'] . ' " >';
	$out .= '<input type="submit" name="backup_ready" value="Get the backup file">';
	$out .= '</form>';
	print $out;
}
elseif( $clean['backup_ready']) {
	printMessage( 'You can now click on the file name below to download the backup to your local drive.');

	if ($handle = opendir($settings['backup_dir'])) {
		while (false !== ($file = readdir($handle))) {
	        if ($file != "." && $file != "..") {
				$bdir_elements = explode("/", $settings['backup_dir']);
				$subdir = $bdir_elements[count($bdir_elements) - 1];
				print '<a href="/' . $subdir . '/' . $file . '">' . $file . '</a><br>';
			}
	    }
	    closedir($handle);
	}	
}
else {
	/* clean the directory */
	$command = '/bin/rm -rf ' . $settings['backup_dir'] . '/*';
	exec( $command);
	
	printMessage( ' This page allows you to make a one-off backup of the database (data only!).');
	printMessage( ' To proceed, press "Backup" button. ');

	$out  = '<form method="post" action="' . $_SERVER['PHP_SELF'] . ' " >';
	$out .= '<input type="submit" name="backup" value="Backup">';
	$out .= '</form>';
	print $out;
}
?>