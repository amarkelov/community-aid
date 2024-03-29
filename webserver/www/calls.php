<?php 
session_start();

require_once(dirname(dirname(__FILE__)) . "/php/login.inc");
require_once(dirname(dirname(__FILE__)) . "/php/calls.inc");
require_once(dirname(dirname(__FILE__)) . "/php/client.inc");
require_once(dirname(dirname(__FILE__)) . "/php/operator.inc");
require_once(dirname(dirname(__FILE__)) . "/php/classifications.inc");
require_once(dirname(dirname(__FILE__)) . "/php/functions.inc");

$clean = array();
$settings = get_ca_settings();

/*
 * Cleaning the input data
 */

validateL1ClassificationList ( $_POST, $clean);

if(isset($_POST['submit'])) {
	$clean['submit'] = $_POST['submit'];

	if(isset($_POST['nextcalltime'])){
		$clean['nextcalltime'] = verify_timeslot($_POST['nextcalltime']);
	}
	
	if(isset($_POST['call_finished'])){
		switch(strtoupper($_POST['call_finished'])) {
			case "ON":
				$clean['call_finished'] = 't';
				break;
			default:
				$clean['call_finished'] = 'f';
				break;
		}
	}
	else {
		$clean['call_finished'] = 'f';
	}

	if(isset($_POST['sclass_id'])) {
		if(ctype_digit($_POST['sclass_id'])) {
			$clean['sclass_id'] = $_POST['sclass_id'];
		} 	
	}
	
	if(isset($_POST['chat'])) {
		$clean['chat'] = htmlentities($_POST['chat'], ENT_QUOTES );
	}

	if(isset($_POST['callid'])) {
		if(ctype_digit($_POST['callid'])) { 
			$clean['callid'] = $_POST['callid'];
		}
	}
}

if(isset($_POST['clientid'])) {
	if(ctype_digit($_POST['clientid'])) { 
		$clean['clientid'] = $_POST['clientid'];
	}
}
elseif(isset($_GET['clientid']) && !isset($_POST['submit'])) {
	if(ctype_digit($_GET['clientid'])) {
		$clean['clientid'] = $_GET['clientid'];
	}
}

if(isset($_GET['floating'])) {
	if(ctype_digit($_GET['floating'])) {
		if($_GET['floating'] == 1) {
			$clean['floating_list'] = true;
		}
		else {
			$clean['floating_list'] = false;
		}
	}
}
else {
	$clean['floating_list'] = false;
}

$clean['operator'] = $_SESSION['s_username'];
$clean['operatorid'] = $_SESSION['operatorid'];

/*
 * Cleaning the input data (end)
 */

if ($clean['clientid'] and !$clean['submit']) {
	printCallsHeader( "Calls", 0, "printCallsJavaScript");	
}
else {
	// no refresh for this page! AJAX is going to refresh the clients list
	printCallsHeader( "Calls", 1);
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

if (isset($clean['submit']) and $clean['clientid']) {
	// the first call to the client is made
	setFirstCallDone( $clean['clientid']);
	
	// record the call
	recordTheCall( $clean);
}

// we only need client's data if we clicked on a client in the list
if ($clean['clientid'] and !isset($clean['submit'])) {
	getClientData( &$clean);	

	$out = '<form onsubmit="return(verifyCall(this.nextcalltime, this));" 
			method="post" action="' . $_SERVER['PHP_SELF'] . ' " >
			<table width="100%" border="0" cellpadding="5">
			<tr><td>';

	// Client name (bold and big) when one selected
	$out .= '<font size="4"><b>' . $clean['firstname'] . ' ' . $clean['lastname'] . '</b></font>
			<p><font size="2"><a href="' . $_SERVER['PHP_SELF'];

	if( $clean['floating_list']) {
    	$out .= '?floating=1';
    }
    
    $out .= '">Back to list of clients</a></font></p>';
	
	$out .= '</td></tr>
			<tr> 
			<td rowspan="3" width="25%" valign="top"> 
			<font face="Verdana, Arial, Helvetica, sans-serif" size="2"><br/>';
    $out .= '</font>
		    </td>
		    <td width="75%" valign="top" align="center" height="120" >
			<font face="Verdana, Arial, Helvetica, sans-serif" size="2"> 
		    <b><div align="left">Client details:</div></b>';
	print $out;
    
	drawClientDetails($clean['clientid']);

	print '<br>
		    <b><div align="left" valign="top">
			<font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			Report:</div></b>
		    <div align="left">
			<textarea tabindex="1" name="chat" id="chat" cols="65" rows="4" wrap="physical"></textarea>
			</div>
		    <br>
		    <b><div align="left" valign="top">Call Classification:</div></b>';
             
	// classification  
	drawLevel1ClassificationListPanel();

	print '<div align="left"><br><br>
			<b>Next Call at:</b> ';
	
	/*
	 * displaing nextcalltime make sense only if we operate
	 * with a clientid
	 */
	if(!empty($clean['nextcalltime'])) {  
	  if ( preg_match( "/([0-9]{2}):([0-9]{2})/", $clean['nextcalltime'], $regs ) ) {
	    $nextcalltime  = "$regs[1]:$regs[2]";
	  }else {
	    $nextcalltime  = "Invalid time format: " . $clean['nextcalltime'];
	  }
	}

	print '<input type="text" tabindex="2" name="nextcalltime" size="6" maxlength="5" value="' . $nextcalltime . '" />
			<font color="#FF0000" size="1" face="Arial, Helvetica, sans-serif">(24 hour format HH:MM)</font>
			<br><br>
			<input type="checkbox" tabindex="3" name="call_finished">Call finished</input>
			</font>
			</div>
			<br><br>
			
			<div align="center">
			<input type="submit" tabindex="4" name="submit" value="Submit" />  
			</div>
			
			<input type="hidden" name="clientid" value="' . $clean['clientid'] . '" />
			<input type="hidden" name="callid" value="' . $clean['callid'] . '" />
			    
			</td>
			</tr>
			<tr>
			<td width="75%" valign="top" align="left" height="100">
			<font size ="1"> ';

	if($clean['clientid']) {
		drawCalls( $clean['clientid']);
	} 
	
	print '</font></td></tr></table></form>
			<script type="text/javascript">
				document.getElementById(\'chat\').focus();
			</script>';
} // if ($clientid and !$submit)
else {
	$out = '<table width="100%" border="0" cellpadding="5">
			<tr><td>';
	$out .= '<font face="verdana, arial, helvetica" size="4"><b>Welcome back, ';
	$out .= $_SESSION['fullname'] . '!</b></font>';
	$out .= '<p><font face="verdana, arial, helvetica" size="2">Below is the list of your clients for today.
				<br>On the right from the name(s) is the time of next call for the client.
				Click on the name to enter details of your next call.</font></p>';
	
	if( !$clean['floating_list']) {
		$out .= '<p><font size="2">
				<a href="' . $_SERVER['PHP_SELF'] . '?floating=1">
				Include clients from the floating list
				</a>
				</font></p>
				<input type="hidden" name="floating" id="floating" value="0" />';
	}
	else {
		$out .= '<p><font size="2">
				<a href="' . $_SERVER['PHP_SELF'] . '?floating=0">
				Exclude clients from the floating list
				</a>
				</font></p>
				<input type="hidden" name="floating" id="floating" value="1" />';
	}
	
	$out .= '</td></tr>
			<tr><td rowspan="3" width="25%" valign="top"> 
			<font face="Verdana, Arial, Helvetica, sans-serif" size="2"><br/>';
	print $out; 
	
//	Time for AJAX to care of list refresh
	print '<div id="ClientList"></div>';
	
	print '</font></td></tr></table>';
}

print '</font></body></html>';
?>