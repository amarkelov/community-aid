<?php
function printMessage( $message) {
	print '<p><b><font face="Verdana, Arial, Helvetica, sans-serif" size="2">' .
		$message . '</font></b></p>';
}

function printErrorMessage( $message) {
    openlog("community-aid", LOG_PID | LOG_PERROR, LOG_LOCAL0);
    $settings = get_ca_settings();

    if($settings['debug'] > 0) {
        print '<p><b><font face="Verdana, Arial, Helvetica, sans-serif" size="2" color="#FF0000">' .
            $message . '</font></b></p>';
    }

    syslog(LOG_ERR, $message);
    closelog();
}
?>
