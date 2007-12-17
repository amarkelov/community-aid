<?php
session_start();
require 'functions.inc';

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Add Client", 0, "printAddEditClientJavaScript");

// if debug flag is set, print the following info
if ($settings['debug'] > 0) {
	print_debug();
}

// START LOG IN CODE
$doWeExit = displayLogin(basename($_SERVER['PHP_SELF']), true);
if($doWeExit == true){
	exit;
}
// END LOG IN CODE

/*
 * Start filtering input
 */
 
if(isset($_POST['clientid'])) {
	if(ctype_digit($_POST['clientid'])) { 
		$clean['clientid'] = $_POST['clientid'];
	}
}

if(isset($_POST['submit'])) {
	$clean['submit'] = $_POST['submit'];
	verifyClientData( $_POST, $clean);
}

if($settings['debug'] == 1){
	print "<b>\$clean:</b><br>";
	print_r( $clean);
	print "<p>";
}

/*
 * End of filtering input
 */
 
if ($clean['submit']) {
	if( addClient($clean)) {
		print "Record Added!<p>";
	}
	print  '<a href="' . $_SERVER['PHP_SELF'] . '">Add another client</a><p>';
} // if ($submit)
else {
	print '<form name="add_client" 
				onSubmit="
				this.title.optional = true;
				this.phone2.optional = true;
				this.gpname.optional = true;
				this.contact1relationship.optional = true;
				this.contact1address.optional = true;
				this.contact2name.optional = true;
				this.contact2relationship.optional = true;
				this.contact2address.optional = true;
				this.contact2phone.optional = true;
				this.alerts.optional = true;
				this.referrer_other.optional = true;
				return verify(this);"
				method="post" 
				action="' . $_SERVER['PHP_SELF'] . '">';
	
	printAddEditClientTable( $clean);
	
	print '<br>
	
	<div align="center">
	<font face="Verdana, Arial, Helvetica, sans-serif">
	<input type="Submit" name="submit" value="Add client">
	</font>
	</div>
	
	</form>
	</body>
	</html>';
}
?>
