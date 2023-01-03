<?php
session_start();

require_once(dirname(dirname(__FILE__)) . "/php/functions.inc");
require_once(dirname(dirname(__FILE__)) . "/php/header.inc");
require_once(dirname(dirname(__FILE__)) . "/php/login.inc");
require_once(dirname(dirname(__FILE__)) . "/php/add_edit_client.inc");
require_once(dirname(dirname(__FILE__)) . "/php/client.inc");

$clean = array();
$settings = get_ca_settings();
$php_self = htmlspecialchars($_SERVER['PHP_SELF']);

// Page Header ...
printHeader( "Add Client", 0, "printAddEditClientJavaScript");

// if debug flag is set, print the following info
if ($settings['debug'] > 0) {
	require_once(dirname(dirname(__FILE__)) . "/php/functions.inc");
	print_debug();
}

// START LOG IN CODE
$doWeExit = displayLogin(basename($php_self), true);
if($doWeExit == true){
	exit;
}
// END LOG IN CODE

/*
 * Start filtering input
 */

if(isset($_SESSION['operatorid'])) {
	$clean['operatorid'] = $_SESSION['operatorid'];
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
		printMessage("Record Added!");
	}
	else {
		printErrorMessage( 'Error occured while adding client!');
	}
	printMessage( '<a href="' . $php_self . '">Add another client</a>');
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
				action="' . $php_self . '">';

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
