<?php session_start(); ?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<?php
  
require("functions.inc");

$clean = array();
$mysql = array();

$settings = get_ca_settings();

/*
 * Cleaning the input data
 */
 
if(isset($_POST['submit'])) {
	$clean['submit'] = $_POST['submit'];

	if(isset($_POST['nextcalltime'])){
		$clean['nextcalltime'] = verify_timeslot($_POST['nextcalltime']);
	}
	
	if(isset($_POST['call_finished'])){
		switch(strtoupper($_POST['call_finished'])) {
			case "ON":
				$clean['call_finished'] = 1;
				break;
			default:
				$clean['call_finished'] = 0;
				break;
		}
	}

	if(isset($_POST['transfer'])){
		switch(strtoupper($_POST['transfer'])) {
			case "ON":
				$clean['transfer'] = 1;
				break;
			default:
				$clean['transfer'] = 0;
				break;
		}
	}
	
	if(isset($_POST['mclass'])) {
		if(ctype_digit($_POST['mclass'])) {
			$clean['mclass'] = $_POST['mclass'];
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


$clean['operator'] = $_SESSION['s_username'];
$clean['operatorid'] = $_SESSION['operatorid'];

/*
 * Cleaning the input data (end)
 */

if (isset($clean['submit']) and $clean['clientid']) {
	// the first call to the client is made
	setFirstCallDone( $clean['clientid']);
	
	// record the call
	recordTheCall( $clean);
	
	// check if need to transfer the client to Senior Operator
	if(isset( $clean['transfer'])) {
		transferClientToSnrOperator( $clean['clientid']);	
	}
	
	/* 
	 * we need to unset the clientid to get back to all clients
	 * rather than keep operate with the current one
	 */
	$clean['clientid'] = '';
}

if ($clean['clientid'] and !isset($clean['submit'])) {
	getClientData( &$clean);	
} // if ($clientid and !$submit)


$out = '<title>Calls -- Friendly Call Service --' . $settings['location'] . '</title>
		<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />


		<script type="text/javascript">
		<!-- hide it from old browsers or from those with JavaScript disabled
		function vtslot(time, form)
		{
			var ts = Array();
			var ts = time.value.split(":");
			var hour = Number(ts[0]);
			var min = Number(ts[1]);

			// if both hours and minutes are numeric and within the range => true		
			if(!isNaN(hour) && !isNaN(min)) {
			   if((hour>=0 && hour<=23) && (min>=0 && min<=59)) {
			      return true;
			   }
			}
			
			// if the call is finished => true
			if(form.call_finished.checked) {
				return true;
			}

			// otherwise throw error message
			msg = "Time has to be of 24 hours format (HH:MM)!\n";
			alert( msg);
			return false;
		}
		-->
		</script>
		</head>

		<body bgcolor="#BEC8FD" link="#0000FF" vlink="#0000FF" alink="#0000FF" >';

print $out;

if ($settings['debug'] > 0) {
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

// setup refresh rate for the page if we haven't clicked any name yet
if ( !$clean['clientid'])   {
	print ' <meta http-equiv="refresh" content="12" url="http://'
			. $HTTP_ENV_VARS["HOSTNAME"]
			. $_SERVER['PHP_SELF'] . '"  /> ';
} 

print '<form onsubmit="return(vtslot(this.nextcalltime, this));" 
		method="post" action="' . $_SERVER['PHP_SELF'] . ' " >
		<table width="100%" border="0" cellpadding="5">
		<tr><td>';

if ( $clean['clientid'])   {
	// Client name (bold and big) when one selected
	$out = '<font size="4"><b>' . $clean['firstname'] . ' ' . $clean['lastname'] . '</b></font>
			<p><font size="2"><a href="/calls.php">Back to list of clients</a></font></p>';

	// don't display transfer checkbox if the operator is the Senior
	if(!checkIsSnr( $clean['operatorid'])) {
		$out .= '</td><td align="right"><input type="checkbox" name="transfer">
				<b><font color="#FF0000">Transfer the client to Senior Operator</font></b>
				</input>';
	}
	
	print $out;
}
else {
	$out  = '<font size="4"><b>Welcome back, ' . $_SESSION['fullname'] . '!</b></font>';
	$out .= '<p><font size="2">Below is the list of your clients for today.
				<br>On the right from the name(s) is the time of next call for the client.
				Click on the name to enter details of your next call.</p></font>';
	print $out; 
}

print '</td>
 </tr>
 <tr> 
   <td rowspan="3" width="25%" valign="top"> 
     <font face="Verdana, Arial, Helvetica, sans-serif" size="2"><br/>';

if( !$clean['clientid'])   {
	draw_clients_list($clean['operator'], $clean['clientid']);
}
else {   
    print '</font>
		    </td>
		    <td width="75%" valign="top" align="center" height="120" > 
		    <b><div align="left">Client details:</div></b>';
    
	draw_client_details($clean['clientid']);

	print '<br />
		    <b><div align="left" valign="top">Report:</div></b>
		    <div align="left"><textarea name="chat" cols="65" rows="4" wrap="physical"></textarea></div>
		    <br /><br />
		    <!-- Embedded table No1-->
		      <table width="100%">
		        <tr>
		          <td width="30%">
		            <b>Call Classification:</b>
		          </td>
		          <td>
		          <select name="mclass" size="1">';
             
	// classification  
	draw_classification();

	print '</select>
          </td>
        </tr>
        <tr>
          <td width="30%">
            <b>Next Call at:</b>  
          </td>
          <td>';
	//  echo $nextcalltime ===> set maxlength="5"
	/*
	 * displaing nextcalltime make sense only if we operate
	 * with a clientid
	 */
	if(!empty($clean['nextcalltime'])) {  
	  if ( ereg( "([0-9]{2}):([0-9]{2})", $clean['nextcalltime'], $regs ) ) {
	    $nextcalltime  = "$regs[1]:$regs[2]";
	  }else {
	    $nextcalltime  = "Invalid time format: " . $clean['nextcalltime'];
	  }
	}

	print '<input type="text" name="nextcalltime" size="6" maxlength="5" value="' . $nextcalltime . '" />
				<font color="#FF0000" size="1" face="Arial, Helvetica, sans-serif">(24 hour format HH:MM)</font>
				</td>
			</tr>
			<tr>
				<td width="30%">
				<input type="checkbox" name="call_finished">Call finished</input>
				</td>
			</tr>
			</table>
			<br /><br />

    <!-- Embedded table No2-->
      <table width="100%">
        <tr>
          <td>
            <div align="center">
              <input type="submit" name="submit" value="Submit" />  
            </div>
          </td>
        </tr>
      </table>

    <input type="hidden" name="clientid" value="' . $clean['clientid'] . '" />
    <input type="hidden" name="callid" value="' . $clean['callid'] . '" />
    
    </td>
  </tr>
  <tr>
    <td width="75%" valign="top" align="left" height="100">
      <font size ="1"> ';

	if($clean['clientid']) {
		draw_calls( $clean['clientid']);
	} 
}

print '<!--<p>&nbsp;</p> -->
		</font> 
		</td>
		</tr>
		</table>
		</form>
		</font>
		</body>
		</html>';
