<?php
session_start();
require_once(dirname(dirname(__FILE__)) . "/php/login.inc");
require_once(dirname(dirname(__FILE__)) . "/php/add_edit_client.inc");
require_once(dirname(dirname(__FILE__)) . "/php/client.inc");
require_once(dirname(dirname(__FILE__)) . "/php/functions.inc");

$clean = array();
$settings = get_ca_settings();

// Page Header ...
printHeader( "Edit Client", 0, "printAddEditClientJavaScript");

// if debug flag is set, print the following info
if ($settings['debug'] > 0) {
	require_once(dirname(dirname(__FILE__)) . "/php/functions.inc");
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
}

if(isset($_POST['edit'])) {
	$clean['edit'] = $_POST['edit'];
}

if(isset($_SESSION['operatorid'])) {
	$clean['operatorid'] = $_SESSION['operatorid'];
}

verifyClientData( $_POST, $clean);

if($settings['debug'] == 1){
	print "<b>\$clean on entry:</b><br>";
	print_r( $clean);
	print "<p>";
}

/*
 * End of filtering input
 */
 
if ($clean['submit']) {
	if( updateClient( $clean)) {	
		printMessage("Record updated/edited!");
	}
	else {
		printErrorMessage( 'Error occured while updating client!');
	}
	printMessage( '<a href="' . $_SERVER['PHP_SELF'] . '">Choose another client to edit</a>');
} // if ($submit)
else if( $clean['edit']) {
	print '<form name="edit_client"
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
				action="' . $_SERVER['PHP_SELF'] . '">
			<font face="Verdana, Arial, Helvetica, sans-serif">';
	
	if ($clean['clientid']) {
	    // editing so select a record
		getClientData( $clean);
		
		// if debug flag is set, print the following info
		if ($settings['debug'] > 0) {
			print_debug( $clean, $settings);
		}
				
		print '<input type=hidden name="clientid" value="' . $clean['clientid']. '">';
	
	} // if ($clean['clientid'])
	
	print '</font>';

	printAddEditClientTable( $clean, 1);	
	
	print '<br>
	
	<div align="center">
	<font face="Verdana, Arial, Helvetica, sans-serif">
	<input type="Submit" name="submit" value="Submit">
	</font>
	</div>
	
	</form>
	</body>
	</html>';
}
else {

	// this part happens if we don't press submit
	if (!$clean['clientid']) {
		// pull the list of active clients
		$clients = array();
		
		if( getClients( $clients, 0, ACTIVE_CLIENTS)) {
			print '<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
					<font face="Verdana, Arial, Helvetica, sans-serif">
					<div align="left">
					<table>';
	
			print '<tr><td><select name="clientid">';
			
			foreach ( $clients as $cid => $value) {
				print '<option value="' . $cid . '">'
					  . $value . ' (' . $cid . ')' . 
					  '</option>';
			
			}
	
			print '</select></td></tr>';
			print '<tr><td><font face="Verdana, Arial, Helvetica, sans-serif">
					<input type="Submit" name="edit" value="Edit client">
					</font>
					</div></td></tr>';
			
			print '</table></form>';
		}
	}
}
	
?>
