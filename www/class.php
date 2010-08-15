<?php 
session_start();

require_once("login.inc");
require_once("client.inc");
require_once("groups.inc");
require_once("calls.inc");
require_once("classifications.inc");
require_once("functions.inc");

$clean = array();
$settings = get_ca_settings();

/*
 * Cleaning the input data
 */

validateL1ClassificationList ( $_POST, $clean);
validateL2ClassificationList ( $_POST, $clean);

if(isset($_POST['submit']) or isset($_POST['submit_class'])) {
	if(isset($_POST['submit'])) {
		$clean['submit'] = $_POST['submit'];
	}

	if(isset($_POST['submit_class'])) {
		$clean['submit_class'] = $_POST['submit_class'];
	}
	
	if(isset($_POST['callid'])) {
		if(ctype_digit($_POST['callid'])) { 
			$clean['callid'] = $_POST['callid'];
		}
	}

	if(isset($_POST['clientid']) and strtoupper($_POST['client_cb']) == 'ON') {
		if(ctype_digit($_POST['clientid'])) { 
			$clean['clientid'] = $_POST['clientid'];
		}
	}
	
	if(isset($_POST['groupid']) and strtoupper($_POST['groupid_cb']) == 'ON') {
		if(ctype_digit($_POST['groupid'])) { 
			$clean['groupid'] = $_POST['groupid'];
		}
	}
	
	if(isset($_POST['noclass_cb']) and strtoupper($_POST['noclass_cb']) == 'ON') {
		$clean['noclass'] = true;
	}

	if(isset($_POST['exclnoissues_cb']) and strtoupper($_POST['exclnoissues_cb']) == 'ON') {
		$clean['exclnoissues'] = true;
	}
	
	if(isset($_POST['timeperiod_cb']) and strtoupper($_POST['timeperiod_cb']) == 'ON') {
		switch ($_POST['tperiod']) {
			case 'today':
				$clean['tperiod'] = 'today';
				break;
			case 'week':
				$clean['tperiod'] = 'week';
				break;
			case 'month':
				$clean['tperiod'] = 'month';
				break;
		}
	}
}

/*
 * Cleaning the input data (end)
 */

printHeader( "Calls calssification", 0, "printClassJavaScript");

if ($settings['debug'] > 0) {
	require_once("functions.inc");
	print_debug( $clean, $settings);
}

// START LOG IN CODE
$doWeExit = displayLogin(basename($_SERVER['PHP_SELF']), true);
if($doWeExit == true){
	exit;
}
else {
	$clean['operator'] = $_SESSION['s_username'];
}
// END LOG IN CODE

if(isset($clean['submit_class']) and $clean['callid'] > 0) {
	// record the call's classifications
	if( classifyTheCall( $clean['callid'], $clean['L1'], $clean['L2'])) {
		printMessage('Classification recorded successfuly');
	}
	else {
		printErrorMessage('Error occured while recording the calssification!');
	}
	printMessage( '<a href="' . $_SERVER['PHP_SELF'] . '">Classify another call</a>');
}
else if (isset($clean['submit']) and isset($clean['callid'])) {
	$call = array();
	
	getCallDetails( $clean['callid'], $call);
	
	$out  = '<table width="100%">';
	$out .= '<tr><td valign="top" width="30%"><b>Call id:<b></td><td>' . $call['callid'] . '</td></tr>';
	$out .= '<tr><td valign="top" width="30%"><b>Client name:<b></td><td>' . $call['lastname'] . ', ' . $call['firstname'] . '</td></tr>';
	$out .= '<tr><td valign="top" width="30%"><b>Operator:<b></td><td>' . $call['opname'] . '</td></tr>';
	$out .= '<tr><td valign="top" width="30%"><b>Chat:<b></td><td>' . $call['chat'] . '</td></tr>';
	$out .= '</table>';
	$out .= '<br><b>Classifications:</b><br>';
	print $out;
	
	print '<form method="post" action="class.php" >';
	
	drawLevel1AndLevel2ClassificationListPanel( $clean['callid']);
	
	$out  = '<br><left>';
	$out .= '<input type="submit" name="submit_class" value="Save the classification"></input>';
	$out .= '</left>';
	$out .= '<input type="hidden" name="callid" value="' . $clean['callid'] . '">';
	$out .= '</form>';
	print $out;
}
elseif (isset($clean['submit']) and !isset($clean['callid'])) {
	print '<form method="post" action="class.php" >';
	
	// we have criteria chosen, now draw the calls
	drawCallsForClassification( $clean);
	
	print '<br><left><input type="submit" name="submit" value="Classify the call"></input></left></form>';
}
else {
	$out = '<h1>Classify the calls</h1>';
	$out .= '<p><font face="verdana, arial, helvetica" size="3">';
	$out .= 'Please, choose critirea below and press <b>Submit</b> button
			 to see the list of calls.</font></p>';
	$out .= '<form method="post" action="class.php" >';
	
	$out .= '<table width="100%" border="0" cellpadding="5">';
	$out .= '<tr> 
			    <td valign="top" width="2%">
			    <input type="checkbox" name="noclass_cb id="noclass_cb"></input>
			    </td>
			    <td valign="top" width="20%">
			    <font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			    All not yet classified calls
			    </td>
			    <td valign="top">
			    <font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			    (Limited to the latest 100 not classified calls!)
			    </td>
			</tr>
			<tr> 
			    <td valign="top" width="2%">
			    <input type="checkbox" name="exclnoissues_cb" id="exclnoissues_cb"></input>
			    </td>
			    <td valign="top" width="20%">
			    <font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			    Exclude calls with no issues
			    </td>
			    <td valign="top">
				&nbsp;
			    </td>
			</tr>';
	print $out;
	
	print '<tr>
			    <td valign="top" width="2%">
			    <input type="checkbox" name="groupid_cb" id="groupid_cb"></input>
			    </td>
			    <td valign="top" width="20%">
			    <font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			    by Group name:
			    <td>';
				    	getGroupNamesAsDropDownList();
	print		'</td>
			<tr>
			    <td valign="top" width="2%">
			    <input type="checkbox" name="client_cb" id="client_cb"></input>
			    </td>
			    <td valign="top" width="20%">
			    <font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			    by Client name:
			    <td>
				    <select name="clientid" onchange="document.forms[0].client_cb.checked=true;" size="1">';
				    	getClientsAsDropDownList();
	print		    '</select>
				</td>
			</tr>';
		
	$out = '<tr>
			    <td valign="top" width="2%">
			    <input type="checkbox" name="timeperiod_cb" id="timeperiod_cb"></input>
			    </td>
			    <td valign="top" width="20%">
			    <font face="Verdana, Arial, Helvetica, sans-serif" size="2">
			    by time period:
			    <td>
				    <select onchange="document.forms[0].timeperiod_cb.checked=true;" name="tperiod" size="1">
				    	<option value="today">Today</option>
				    	<option value="week">Last 7 days</option>
				    	<option value="month">Last 30 days</option>
				    </select>
				</td>
			</tr>';
	$out .= '</table>';
	
	$out .= '<left><input type="submit" name="submit" value="Submit"></input></left></form>';
	
	print $out;
}

print '</font></body></html>';
?>