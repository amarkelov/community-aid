<?php
function draw_calls( $clientid) 
{
	$current_week_number = 0;
	$last_week_number = 0;
	
	$dbConnect = dbconnect();
	
    $sql  = "SELECT calls.time as time,
				calls.chat as chat,
				calls.class as class,
				operators.loginname as operator FROM calls, operators
				WHERE clientid={$clientid} AND 
				time > (NOW() - INTERVAL '3 WEEK') AND 
				calls.operatorid=operators.operatorid 
				ORDER BY callid DESC ";
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
    }

    print ("<hr noshade>");
    print('<table border="0" width="100%" cellspacing="0" cellpadding="0">');
    $i=0; // need this to change cell bgcolor
	$firstround = 1;
    while ($myrow = pg_fetch_array($result)) {
// select sclass short name
        $class_ = $myrow["class"];
		$sql  = "SELECT sclass_sname FROM call_sclass WHERE";
		$sql .= " sclass_id='$class_'";
		$res = pg_query( $dbConnect, $sql);
		if (!$res) {
		    $message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		    $message .= 'Whole query: ' . $sql;
		    printErrorMessage($message);
		}
		$class_row = pg_fetch_row($res);
// select sclass short name (end)
	
		$i=$i + 1; // need this to change cell bgcolor
		$time=$myrow["time"];
	
		$regs = 0;
		if ( ereg( "([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})", $time, $regs ) ) {
		    $out="$regs[3]/$regs[2]/$regs[1] $regs[4]:$regs[5]";
		    $current_week_number = date("W",mktime(0,0,0,$regs[2],$regs[3],$regs[1]));
		}
		else {
		    $out="Invalid date format: $time";
		}


		if( $firstround) {
			$last_week_number = $current_week_number;
			$firstround = 0;
		}
		
		if($current_week_number < $last_week_number) {
			print '<tr><td>&nbsp</td></tr>';
			print '<tr><td><font size="1"><b>Week ' . $current_week_number . '</font></td></tr>';
			$last_week_number = $current_week_number;
		}
		
		if($i % 2) {
		    print('<tr bgcolor="#FFFFFF">');
		}
		else {
		    print('<tr bgcolor="#DDDDDD">');
		}
	
	// time column
		print('<td width="20%"> <font size="1" color="#FF0000">');
		print $out;
		print('</font>');
	// class column
		print('</td><td width="10%"><font size="1" color="#00FF00">');
		
		if(!empty($class_row[0])) { 
			print("<a href=\"class.php\" onClick=\"window.open('class.php','','status=yes,toolbar=no,location=no,width=400,height=500');return false;\">$class_row[0]</a>");
		}
		else {
		    print("&nbsp");
		}	
		print('</font>');
	// chat column
		print('</td><td><font size="1" color="#0000FF">');
		
		if(!empty($myrow["chat"])) { 
		    print($myrow["chat"] );
		}
		else {
		    print("&nbsp");
		}
	// operator
		print('</td><td width="10%"><font size="1" color="#0000FF">');
		
		if(!empty($myrow["operator"])) { 
		    print($myrow["operator"] );
		}
		else {
		    print("&nbsp");
		}
		    
		print("</td></tr>");

    } // while
    
    print("</table>");

	dbclose($dbConnect);
}

function draw_client_details($clientid)
{
    $fs = 2; // font size
    $regs = 0;

	$dbConnect = dbconnect();

    if( $clientid and $dbConnect) {
		$sql  = "SELECT phone1,phone2,alone,dob,medical_notes,alerts,area,
					contact1name,contact1phone,
					contact2name,contact2phone,
					gpname,
					TO_CHAR(timeslot, 'HH24:MI') as timeslot from clients
					WHERE clientid='$clientid' AND active='1'";
		
		$result = pg_query( $dbConnect, $sql);
		if (!$result) {
		    $message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		    $message .= 'Whole query: ' . $sql;
		    printErrorMessage($message);
		}
		else {	
	   	    while ($myrow = pg_fetch_array($result)) {
				if($myrow["phone1"]) {
		        	    $phone1 = $myrow["phone1"]; 
				}
				else {
				    $phone1 = "N/A";
				}
				if($myrow["phone2"]) {
		    	    	    $phone2 = $myrow["phone2"];
				}
				else {
				    $phone2 = "N/A";
				}
				if ( ereg( "([0-9]{4})-([0-9]{2})-([0-9]{2})", $myrow["dob"], $regs ) ) {
				    $dob = "$regs[3]/$regs[2]/$regs[1]";
				}
		       	else {
				    $dob = "Invalid DOB";
				} 
				$area = $myrow['area'];

				$contact1name = $myrow['contact1name'];
				$contact1phone = $myrow['contact1phone'];

				$contact2name = $myrow['contact2name'];
				if(strlen(trim($contact2name)) == 0) $contact2name = 'N/A';

				$contact2phone = $myrow['contact2phone'];
				if(strlen(trim($contact2phone)) == 0) $contact2phone = 'N/A';

				$gpname = $myrow['gpname'];
				if(strlen(trim($gpname)) == 0) $gpname = 'N/A';
				
				$timeslot = $myrow['timeslot'];
				$medical_notes = $myrow["medical_notes"];
		       	$alerts = $myrow["alerts"];
		       	$alone = $myrow["alone"];
		    }
		}
    }    

    $out  = '<table frame="border" width="100%">';
    $out .= '<tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>Phone 1:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $phone1 . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>First Contact:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact1name . "</font></td>";
    $out .= '</tr><tr>';        
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Phone 2:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $phone2 . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>First Contact phone:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact1phone . "</font></td>";
    $out .= '</tr><tr>';    
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>D.O.B. (DD/MM/YYYY):</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $dob . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>Second Contact:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact2name . "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Area:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $area . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>Second Contact phone:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact2phone . "</font></td>";
    $out .= '</tr><tr>';    
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Time slot:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $timeslot . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>GP name:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $gpname . "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Alone:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">"; 
		if( $alone = 't') {
    		$out .= "Yes";
		}
		else {
			$out .= "No";
		}
	$out .= "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Medical notes:</b></font></td>";
    $out .= "<td><font size=" . $fs .">" . $medical_notes . "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Alerts:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $alerts . "</font></td>";
    $out .= '</tr></table>';
    
    print "$out";

	dbclose($dbConnect);
}

function printCallsJavaScript() {
	print '<script type="text/javascript">
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
		</script>';
}


// Draw clients list
// left part of the screen
function draw_clients_list($operator, $clientid, $floating_list=false)
{
	$clients = array();		// array to store clientids and the rest
	$calltime = array();	// array to store timeslots
	$nocalltime = array();	// array to store clientid's with no timeslot
	$redcalltime = array();	// array to store timeslots that were missed 
	$redcalltimemsg = array(); // array to hold red messages to append to timeslots
	$redpastcalltime = array();	// array to store timeslots that were missed from yesterday 
	$redpastcalltimemsg = array(); // array to hold redpast messages to append to timeslots
	$nextcall = array();	// array to store nextcalls
	$timeslot_done = array();	// to track if the timeslot call was made
	
	$dbConnect = dbconnect();
	
	$fs = 1; // font size
	
	// getting client list first ...
    $sql  = "SELECT clientid,firstname,lastname,
			TO_CHAR(timeslot, 'HH24:MI') AS timeslot
			FROM clients WHERE clientid IN (SELECT clientid FROM client2operator WHERE 
			operatorid=(SELECT operatorid FROM operators WHERE loginname='{$operator}'))";

    // add floating list to the mix
    if( $floating_list) {
    	$sql .= " OR groupid=1 ";
    }
    
    $sql .= " AND active='t' ORDER BY timeslot, lastname";
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
    }

	while ($myrow = pg_fetch_array($result)) {
		$clients[$myrow['clientid']] = $myrow['lastname'] . ', ' . $myrow['firstname'];
		$calltime[$myrow['clientid']] = $myrow['timeslot'];
	}

	// getting unfinished calls and nextcalltime for it
	$sql = "SELECT clientid,TO_CHAR(nextcalltime,'DD/MM/YYYY HH24:MI:SS') as nextcalltime
			FROM calls WHERE clientid in 
			(SELECT clientid FROM client2operator WHERE 
			operatorid=(SELECT operatorid FROM operators WHERE loginname='{$operator}')) 
			and call_finished='f'";
	
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '\n';
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
    }

	while ($myrow = pg_fetch_array($result)) {
		$nextcall[$myrow['clientid']] = $myrow['nextcalltime'];
	}
	
	// getting the timeslot_done flags for all the clients
	$sql = "SELECT clientid,timeslot_done
			FROM client_timeslot_call WHERE clientid in 
			(SELECT clientid FROM client2operator WHERE 
			operatorid=(SELECT operatorid FROM operators WHERE loginname='{$operator}'))";
	
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '\n';
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
    }

	while ($myrow = pg_fetch_array($result)) {
		$timeslot_done[$myrow['clientid']] = $myrow['timeslot_done'];
	}

	// processing now.....
	$now = time();
	$mnt = mktime(0, 0, 0, date("m"), date("d"), date("Y")); // tonight's midnight

	foreach( $calltime as $cid => $nctime) { // now go through it all and figure out the time....
		$nct = -1;
		if( isset($nextcall[$cid])) {
			if( ereg( "([0-9]{2})/([0-9]{2})/([0-9]{4}) ([0-9]{2}):([0-9]{2}):([0-9]{2})", $nextcall[$cid], $regs )) {
				$nct = mktime($regs[4],$regs[5],$regs[6],$regs[2],$regs[1],$regs[3]); // nextcalltime
			}
/*--worked			if( ereg( "([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})", $nextcall[$cid], $regs )) {
				$nct = mktime($regs[4],$regs[5],$regs[6],$regs[2],$regs[3],$regs[1]); // nextcalltime
			}---worked*/
		}

		$regs = 0;
		if( ereg( "([0-9]{2}):([0-9]{2})", $nctime, $regs )) {
			$tms = mktime($regs[1], $regs[2], 0, date("m"), date("d"), date("Y")); // time slot
		}
		else {
			$tms = 0;
		}

		if( ($mnt - $nct) > 0 && $nct > 0) { // we left unfinished call from yesterday. bad!
			$redpastcalltime[$cid] = $nextcall[$cid];
			$redpastcalltimemsg[$cid] = 'Unfinished call from the past. Call now!';
			unset( $calltime[$cid]);
		}
		else if( $tms < $now && $timeslot_done[$cid] == 'f' ) { // we missed client's usual time slot. bad!
			$redcalltime[$cid] = $nctime;
			$redcalltimemsg[$cid] = 'Missed time slot. Call now!';
			unset( $calltime[$cid]);
		}
		else if( $nct < $now && $nct > 0) {
			$redcalltime[$cid] = date( "H:i", $nct);
			$redcalltimemsg[$cid] = 'Missed call time. Call now!';
			unset( $calltime[$cid]);
		}
		else if( $nct > $now) {
			$calltime[$cid] = date( "H:i", $nct);
		}
		
/* --- worked		if( ($mnt - $nct) > 0 && $nct > 0) { // we left unfinished call from yesterday. bad!
			$calltime[$cid] = '<b><font color="#f11313">Unfinished call from the past. ' . $nextcall[$cid] . ' Call now!</font></b>';
		}
		else if( $tms < $now && $timeslot_done[$cid] == 'f' ) { // we missed client's usual time slot. bad!
			$calltime[$cid] = '<b><font color="#f11313">Missed time slot (' . $nctime . '). Call now!</font></b>';
		}
		else if( $nct < $now && $nct > 0) {
			$calltime[$cid] = '<b><font color="#f11313">Missed call time ('. date( "H:i", $nct) . '). Call now!</font></b>';
		}
		else if( $nct > $now) {
			$calltime[$cid] = date( "H:i", $nct);
		} --- worked */
		
		else if( $tms < $now) {
			$nocalltime[$cid] = '';
			unset( $calltime[$cid]);
		}
//		$calltime[$cid] .= '  --- nct: ' . date("Y-m-d H:i:s", $nct) . '; timeslot: ' . date("Y-m-d H:i:s", $tms) . '; ' . $nextcall[$cid] . '==' . $nct; 
	}
    
	// we need to sort the array now
	asort( $redcalltime);
	asort( $calltime);

	// print it all out ....
	print("<table>");

	// redpasts first
	foreach ( $redpastcalltime as $cid => $ts ) {
		$out  = '<tr><td><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="$fs">';
		$out .= $clients[$cid] . '</a></font></td>';
		$out .= '<td><b><font color="#f11313">' . $redpastcalltime[$cid] . '</font></b></td>';
		$out .= '<td><b><font color="#f11313">' . $redpastcalltimemsg[$cid] . '</font></b></td>';
		
	    print $out . '</tr>';
	}
	
	// reds then
	foreach ( $redcalltime as $cid => $ts ) {
		$out  = '<tr><td><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="$fs">';
		$out .= $clients[$cid] . '</a></font></td>';
		$out .= '<td><b><font color="#f11313">' . $redcalltime[$cid] . '</font></b></td>';
		$out .= '<td><b><font color="#f11313">' . $redcalltimemsg[$cid] . '</font></b></td>';
		
	    print $out . '</tr>';
	}
	
	// normals then
	foreach ( $calltime as $cid => $ts ) {
		$out  = '<tr><td><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="$fs">';
		$out .= $clients[$cid] . '</a></font></td>';
		$out .= '<td>' . $calltime[$cid] . '</td>';
		$out .= '<td>&nbsp</td>';
		
	    print $out . '</tr>';
	}
	
	// notimes first
	foreach ( $nocalltime as $cid => $ts ) {
		$out  = '<tr><td><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="$fs">';
		$out .= $clients[$cid] . '</a></font></td>';
		$out .= '<td>' . $nocalltime[$cid] . '</td>';
		$out .= '<td>&nbsp</td>';
		
	    print $out . '</tr>';
	}
	
/* --- worked	foreach ( $clients as $cid => $name ) {
		$out  = '<tr><td><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="$fs">';
		$out .= $name . '</a></font></td>';
		$out .= '<td>' . $calltime[$cid] . '</td>';
	    
	    print $out . '</tr>';
	}--- worked */

	print("</table>");

	dbclose($dbConnect);
}

function setFirstCallDone( $clientid) {
	$retval = true;
	
	$dbConnect = dbconnect();
	
	$sql = "UPDATE client_timeslot_call SET timeslot_done='t' WHERE clientid=" . $clientid;
	
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}

	dbclose( $dbConnect);
	
	return $retval;
}

function recordTheCall( &$clean) {
	$retval = true;
	
	$dbConnect = dbconnect();

	$sql  = sprintf("INSERT INTO calls (clientid, time, nextcalltime, chat, class, call_finished, operatorid) 
  		    VALUES ( %d, NOW(), CURRENT_DATE + TIME '%s',  '%s', %d, '%s', %d)",
			pg_escape_string( $clean['clientid']), 
			pg_escape_string( $clean['nextcalltime']),
			pg_escape_string( $clean['chat']), 
			pg_escape_string( $clean['mclass']),
			pg_escape_string( $clean['call_finished']), 
			pg_escape_string( $_SESSION['operatorid']));
	
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
	else if( $clean['call_finished'] == 't') { 
		// now we can set all unfinished calls to finished state, so the
		// calculation of the next call time for the client can be correct
		// NOTE: this is quick and dirty workaround for the moment!!!
		$sql  = "UPDATE calls SET call_finished='t' 
				WHERE clientid=" . $clean['clientid'];
		
		$result = pg_query( $dbConnect, $sql);
		if (!$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
			printErrorMessage( $message);
			$retval = false;
		}
	}  
	
	dbclose( $dbConnect);
	
	return $retval;
}

?>