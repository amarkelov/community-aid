<?php
function printCallsHeader( $pageTitle, $ajax = 0, $javaScriptFuncName = NULL) {
	$settings = array();
	$settings = get_ca_settings();

	print '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"
			"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
			<html xmlns="http://www.w3.org/1999/xhtml">
			<head>
				<title>' . $pageTitle . ' -- '  . $settings['org'] . ' -- ' . $settings['location'] . '</title>
				<meta http-equiv="expires" content="-1">
				<meta http-equiv="Cache-Control" content="no-cache">';

	if($ajax > 0) {
		print '<script type="text/javascript" src="/js/calls.js"></script>';
	}

	if( isset( $javaScriptFuncName)) {
		$javaScriptFuncName();
	}

	print '</head>
			<body bgcolor="#BEC8FD" onload="process()">
			<font face="Verdana, Arial, Helvetica, sans-serif">
			<p>';
}

function drawCalls( $clientid)
{
	$current_week_number = 0;
	$last_week_number = 0;

	$dbConnect = dbconnect();

    $sql  = "SELECT calls.time as time,
				calls.chat as chat,
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
    print '<tr>
			<td width="20%"><font size="1"><b>Date/Time</td>
			<td><font face="Verdana, Arial, Helvetica, sans-serif" size="1"><b>Call details</td>
			<td width="10%"><font size="1"><b>Operator</td>
			</tr>';

    $i=0; // need this to change cell bgcolor
	$firstround = 1;
    while ($myrow = pg_fetch_array($result)) {
		$i=$i + 1; // need this to change cell bgcolor
		$time=$myrow["time"];

		$regs = 0;
		if ( preg_match( "/([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})/", $time, $regs ) ) {
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
			print '<tr><td><font face="Verdana, Arial, Helvetica, sans-serif" size="1"><b>Week ' . $current_week_number . '</font></td></tr>';
			$last_week_number = $current_week_number;
		}

		if($i % 2) {
		    print('<tr bgcolor="#FFFFFF">');
		}
		else {
		    print('<tr bgcolor="#DDDDDD">');
		}

	// time column
		print('<td width="20%"> <font face="Verdana, Arial, Helvetica, sans-serif" size="1" color="#FF0000">');
		print $out;
		print('</font>');
	// chat column
		print('</td><td><font face="Verdana, Arial, Helvetica, sans-serif" size="1" color="#0000FF">');

		if(!empty($myrow["chat"])) {
		    print($myrow["chat"] );
		}
		else {
		    print("&nbsp");
		}
	// operator
		print('</td><td width="10%"><font face="Verdana, Arial, Helvetica, sans-serif" size="1" color="#0000FF">');

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

function drawCallsForClassification( &$criteria, $limit=100)
{
	$current_week_number = 0;
	$last_week_number = 0;
	$sql = '';
	$need_and = false;


	if( is_array( $criteria)) {
	    $sql  = 'SELECT c.callid, time, chat, o.loginname
					FROM calls c
					LEFT OUTER JOIN operators o on c.operatorid=o.operatorid
					LEFT OUTER JOIN call_l1_class l1 on c.callid=l1.callid
					LEFT OUTER JOIN clients cl on c.clientid = cl.clientid';

		if( isset($criteria['noclass'])) {
			$sql .= ' LEFT OUTER JOIN call_l2_class cl2 on c.callid = cl2.callid';
		}

		$sql .= ' WHERE ';

		if( isset($criteria['noclass'])) {
			$sql .= ' l2id IS NULL ';
			$need_and = true;
		}

		if( isset($criteria['tperiod'])) {
			if( $need_and) {
				$sql .= ' AND ';
			}

			switch ($criteria['tperiod']) {
				case 'today':
					$sql .= " time > (CURRENT_DATE + TIME '00:00') ";
					$need_and = true;
					break;
				case 'week':
					$sql .= " time > (CURRENT_DATE + TIME '00:00' - INTERVAL '1 WEEK') ";
					$need_and = true;
					break;
				case 'month':
					$sql .= " time > (CURRENT_DATE + TIME '00:00' - INTERVAL '1 MONTH') ";
					$need_and = true;
					break;
			}
		}

		if( isset($criteria['clientid']) and ctype_digit($criteria['clientid'])) {
			if( $need_and) {
				$sql .= ' AND ';
			}
			$sql .= " c.clientid = {$criteria['clientid']} ";
			$need_and = true;
		}

		if( isset($criteria['exclnoissues'])) {
			if( $need_and) {
				$sql .= ' AND ';
			}
			$sql .= ' l1.l1id > 0 ';
			$need_and = true;
		}

		if( isset($criteria['groupid'])) {
			if( $need_and) {
				$sql .= ' AND ';
			}
			$sql .= " groupid = {$criteria['groupid']} ";
			$need_and = true;
		}

		$sql .= ' GROUP by c.callid, c.time, c.chat, o.loginname ORDER BY callid DESC ';

		if( $limit > 0) {
			$sql .= ' LIMIT ' . $limit;
		}

		$dbConnect = dbconnect();

	    $result = pg_query( $dbConnect, $sql);
	    if (!$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
			$message .= 'Whole query: ' . $sql;
			printErrorMessage($message);
	    }

	    print ("<hr noshade>");
	    print('<table border="0" width="100%" cellspacing="0" cellpadding="0">');
	    print '<tr>
				<td width="7%"><font face="verdana, arial, helvetica" size="1"><b>&nbsp</td>
	    		<td width="20%"><font face="verdana, arial, helvetica" size="1"><b>Date/Time</td>
				<td><font face="verdana, arial, helvetica" size="1"><b>Call details</td>
				<td width="10%"><font face="verdana, arial, helvetica" size="1"><b>Operator</td>
				</tr>';

	    $i=0; // need this to change cell bgcolor
		$firstround = 1;
	    while ($myrow = pg_fetch_array($result)) {
			$i=$i + 1; // need this to change cell bgcolor
			$time=$myrow["time"];

			$regs = 0;
			if ( preg_match( "/([0-9]{4})-([0-9]{2})-([0-9]{2}) ([0-9]{2}):([0-9]{2}):([0-9]{2})/", $time, $regs ) ) {
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
				print '<tr><td><font face="Verdana, Arial, Helvetica, sans-serif" size="1"><b>Week ' . $current_week_number . '</font></td></tr>';
				$last_week_number = $current_week_number;
			}

			if($i % 2) {
			    print('<tr bgcolor="#FFFFFF">');
			}
			else {
			    print('<tr bgcolor="#DDDDDD">');
			}

		// select
			print '<td width="1%" valign="top" align="center">';
			print '<input type="radio" name="callid" value="' . $myrow['callid'] . '"></input>';
			print '</td>';
		// time column
			print '<td width="20%"> <font face="verdana, arial, helvetica" size="1" color="#FF0000">';
			print $out;
			print '</font>';
		// chat column
			print('</td><td><font face="verdana, arial, helvetica" size="1" color="#0000FF">');

			if(!empty($myrow["chat"])) {
			    print($myrow["chat"] );
			}
			else {
			    print("&nbsp");
			}
		// operator
			print('</td><td width="10%"><font face="verdana, arial, helvetica" size="1" color="#0000FF">');

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
}


function drawClientDetails($clientid)
{
    $fs = 2; // font size
    $regs = 0;

	$dbConnect = dbconnect();

    if( $clientid and $dbConnect) {
		$sql  = "SELECT phone1,phone2,alone,dob,medical_notes,alerts,area,
					contact1name,contact1phone,contact1relationship,
					contact2name,contact2phone,contact2relationship,
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
				if ( preg_match( "/([0-9]{4})-([0-9]{2})-([0-9]{2})/", $myrow["dob"], $regs ) ) {
				    $dob = "$regs[3]/$regs[2]/$regs[1]";
				}
		       	else {
				    $dob = "Invalid DOB";
				}
				$area = $myrow['area'];

				$contact1name = $myrow['contact1name'];
				$contact1phone = $myrow['contact1phone'];
				$contact1relationship = $myrow['contact1relationship'];

				$contact2name = $myrow['contact2name'];
				if(strlen(trim($contact2name)) == 0) $contact2name = 'N/A';

				$contact2phone = $myrow['contact2phone'];
				if(strlen(trim($contact2phone)) == 0) $contact2phone = 'N/A';

				$contact2relationship = $myrow['contact2relationship'];

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
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>First Contact relationship:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact1relationship . "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>D.O.B. (DD/MM/YYYY):</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $dob . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>First Contact phone:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact1phone . "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Area:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $area . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>Second Contact:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact2name . "</font></td>";
    $out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Time slot:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $timeslot . "</font></td>";
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>Second Contact relationship:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact2relationship . "</font></td>";
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
    $out .= "<td width=\"30%\"><font size=" . $fs ."><b>Second Contact phone:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $contact2phone . "</font></td>";
	$out .= '</tr><tr>';
    $out .= "<td width=\"30%\"><font size=" . $fs . "><b>Medical notes:</b></font></td>";
    $out .= "<td><font size=" . $fs .">" . $medical_notes . "</font></td>";
	$out .= "<td width=\"30%\"><font size=" . $fs ."><b>GP name:</b></font></td>";
    $out .= "<td><font size=" . $fs . ">" . $gpname . "</font></td>";
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
		function uncheck_noissues() {
			if(document.getElementById("L1_0").checked) {
				document.getElementById("L1_0").checked = false;
				return true;
			}
		}

		function uncheck_all_when_noissues() {
			if(document.getElementById("L1_0").checked) {
				for( i=1; i<8; ++i) {
					document.getElementById("L1_" + i).checked = false;
				}
				return true;
			}
		}

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

		function verifyCall(time, form)
		{
			var classified = 0;
			var vtslot_ok = false;

			if(!vtslot(time, form)) {
				return false;
			}
			else {
				vtslot_ok = true;
			}

			// checking that at least one classification was chosen
			for( i=0; i<8; ++i) {
				if( document.getElementById("L1_" + i).checked) {
					classified++;
				}
			}

			if( classified == 0) {
				msg = "At least one classification category must be chosen!\n";
				alert( msg);
			}

			// if both checks are good, we are good
			if (vtslot_ok && classified > 0) {
				return true;
			}

			return false;

		}
		-->
		</script>';
}


// Draw clients list
// left part of the screen
function drawClientsList($operator, $floating_list=false)
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

	$today = getdate();
	$todays_dow = $today['wday'];

	$dbConnect = dbconnect();

	$fs = 2; // font size

	// getting client list first ...
    $sql  = "SELECT clientid,firstname,lastname,
			TO_CHAR(timeslot, 'HH24:MI') AS timeslot
			FROM clients WHERE ((clientid IN (SELECT clientid FROM client2operator WHERE
			operatorid=(SELECT operatorid FROM operators WHERE loginname='{$operator}')) AND active='t') ";

    // add floating list to the mix
    if( $floating_list) {
    	$sql .= " OR ( groupid=1 AND active='t')";
    }

    $sql .= ") AND clientid in (SELECT clientid FROM days WHERE dow={$todays_dow}) ";
    $sql .= " ORDER BY timeslot, lastname";

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
			FROM client_nextcalltime WHERE (clientid in
			(SELECT clientid FROM client2operator WHERE
			operatorid=(SELECT operatorid FROM operators WHERE loginname='{$operator}'))";

    // add floating list to the mix
    if( $floating_list) {
    	$sql .= " OR clientid IN (SELECT clientid FROM clients WHERE groupid=1 AND active='t') ";
    }

    $sql .= ")  AND clientid in (SELECT clientid FROM days WHERE dow={$todays_dow}) ";

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
			FROM client_timeslot_call WHERE ( clientid in
			(SELECT clientid FROM client2operator WHERE
			operatorid=(SELECT operatorid FROM operators WHERE loginname='{$operator}'))";

    // add floating list to the mix
    if( $floating_list) {
    	$sql .= " OR clientid IN (SELECT clientid FROM clients WHERE groupid=1)";
    }

    $sql .= ")  AND clientid in (SELECT clientid FROM days WHERE dow={$todays_dow}) ";

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
	    	/*
	    	 * I'm using # as the delimiter. The default one is / , but I would need to escape
	    	 * the rest of / in date spec if I was to use the default delimiter.
	    	 */
			if( preg_match( "#([0-9]{2})/([0-9]{2})/([0-9]{4}) ([0-9]{2}):([0-9]{2}):([0-9]{2})#", $nextcall[$cid], $regs )) {
				$nct = mktime($regs[4],$regs[5],$regs[6],$regs[2],$regs[1],$regs[3]); // nextcalltime
			}
		}

		$regs = 0;
		if( preg_match( "/([0-9]{2}):([0-9]{2})/", $nctime, $regs )) {
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

		/* now, those whose timeslot call was done by now */
		else if( ($tms < $now || $tms > $now) && $timeslot_done[$cid] == 't') {
			$nocalltime[$cid] = ''; // . 'timeslot: ' . date("Y-m-d H:i:s", $tms) . "; now: " . date("Y-m-d H:i:s", $now);
			unset( $calltime[$cid]);
		}

//		$calltime[$cid] .= '  --- nct: ' . date("Y-m-d H:i:s", $nct) . '; timeslot: ' . date("Y-m-d H:i:s", $tms) . '; ' . $nextcall[$cid] . '==' . $nct . "; now: " . date("Y-m-d H:i:s", $now);
	}

	// we need to sort the array now
	asort( $redcalltime);
	asort( $calltime);

	// print it all out ....
	$out = '<table>';

	// redpasts first
	foreach ( $redpastcalltime as $cid => $ts ) {
		$out .= '<tr><td ALIGN="left" width="15%"><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="' . $fs . '">';
		$out .= $clients[$cid] . '</a></font></td>';
		$out .= '<td ALIGN="right" width="5%"><b><font color="#f11313" size="' . $fs . '">' . $redpastcalltime[$cid] . '</font></b></td>';
		$out .= '<td><b><font color="#f11313" size="' . $fs . '">' . $redpastcalltimemsg[$cid] . '</font></b></td>';

	   	$out .= '</tr>';
	}

	// reds then
	foreach ( $redcalltime as $cid => $ts ) {
		$out .= '<tr><td ALIGN="left" width="15%"><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="' . $fs . '">';
		$out .= $clients[$cid] . '</a></font></td>';
		$out .= '<td ALIGN="right" width="5%"><b><font color="#f11313" size="' . $fs . '">' . $redcalltime[$cid] . '</font></b></td>';
		$out .= '<td><b><font color="#f11313" size="' . $fs . '">' . $redcalltimemsg[$cid] . '</font></b></td>';

		$out .= '</tr>';
	}

	// normals then
	foreach ( $calltime as $cid => $ts ) {
		$out .= '<tr><td ALIGN="left" width="15%"><a href="' . $PHP_SELF . '?clientid=' . $cid;
	    if( $floating_list) {
	    	$out .= '&floating=1';
	    }
		$out .= '">';
		$out .= '<font size="' . $fs . '">';
		$out .= $clients[$cid] . '</a></font></td>';
		$out .= '<td ALIGN="right" width="5%">' . $calltime[$cid] . '</td>';
		$out .= '<td>&nbsp</td>';

	    $out .= '</tr>';
	}

	// notimes the last
	foreach ( $nocalltime as $cid => $ts ) {
		$out .= '<tr><td ALIGN="left" width="15%"><a href="' . $PHP_SELF . '?clientid=' . $cid . '">';
		$out .= '<font size="' . $fs . '">';
		$out .= $clients[$cid] . '</a></font></td>';
		$out .= '<td ALIGN="right" width="5%">' . $nocalltime[$cid] . '</td>';
		$out .= '<td>&nbsp</td>';

	    $out .= '</tr>';
	}

	$out .= '</table>';

	print $out;

	dbclose($dbConnect);
}

function getCallDetails( $callid, &$call) {
	$retval = false;

	if( isset($callid) and ctype_digit($callid) and $callid > 0 and is_array($call)) {
		$dbConnect = dbconnect();

		$sql = "SELECT  calls.callid,
						clients.lastname,
						clients.firstname,
						calls.time,
						calls.chat,
						operators.loginname as opname
				FROM calls, clients, operators
				WHERE calls.clientid=clients.clientid
					AND operators.operatorid=calls.operatorid
					AND callid=" . $callid;

		$result = pg_query( $dbConnect, $sql);
		if (!$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
			printErrorMessage( $message);
			$retval = false;
		}
		else {
			$call = pg_fetch_array($result);
		}

		dbclose( $dbConnect);
		return $call;
	}

	return $retval;
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
	$callid = 0;

	$dbConnect = dbconnect();

    // begin transaction
	$sql = "BEGIN;";

	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
		$retval = false;
        dbclose($dbConnect);
        return $retval;
	}

	$result = pg_query_params("SELECT record_call($1::BIGINT,
										 $2::TEXT,
										 $3::TIMESTAMP,
										 $4::BOOLEAN,
										 $5::BIGINT) as callid;",
                               array(filter_var($clean['clientid'], FILTER_SANITIZE_NUMBER_INT),
                                     pg_escape_string($clean['chat']),
                                     pg_escape_string(strftime("%d/%m/%y %T", strtotime($clean['nextcalltime']))),
                                     pg_escape_string($clean['call_finished']),
                                     filter_var($_SESSION['operatorid'], FILTER_SANITIZE_NUMBER_INT)));


    // record_call() function returns callid of the newly recorded call
    // which we'll use below to get the call's classification(s) recorded
    $callid = pg_fetch_result($result, 0, 'callid');

    // handle call classification now
    if( isset($clean['L1']) && sizeof($clean['L1']) > 0 && $callid > 0) {
        $arL1 = array();
        $arL1 = $clean['L1'];

        $sql = "";

        foreach( $arL1 as $value) {
            if(!filter_var($value, FILTER_VALIDATE_INT)) {
                printErrorMessage('Level 1 category ID is not an integer! Skipped.');
                $retval = false;
            }

            $result = pg_query_params($dbConnect, "INSERT INTO call_l1_class (callid, l1id) VALUES ($1,$2);",
                                      array($callid, $value));
            if (!$result) {
                $message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
                printErrorMessage( $message);
                $retval = false;
            }
        }
    }

    // commit the transaction
    $sql = "COMMIT;";
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
        $message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
        printErrorMessage( $message);
        $retval = false;
    }

	dbclose( $dbConnect);

	return $retval;
}
?>
