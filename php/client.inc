<?php
require_once("db.inc");

define(ACTIVE_CLIENTS, 2);
define(INACTIVE_CLIENTS, 4);
define(ACTIVE_UNASSIGNED_CLIENTS, 6);
define(ALL_CLIENTS, 8);

function getClientData( &$clean) {
	$retval = true;
	
	$dbConnect = dbconnect();

	$sql = "SELECT firstname,lastname,
					initials,title,gender,
					address,area,districtid,phone1,phone2,housetype,dob,alone,medical_notes,
					contact1name,contact1relationship,contact1address,contact1phone,
					contact2name,contact2relationship,contact2address,contact2phone,
					gpname,referrer,alerts,groupid,
					TO_CHAR(timeslot, 'HH24:MI') as timeslot FROM clients WHERE (clients.clientid={$clean['clientid']})";
	
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = "Invalid query: " . pg_result_error( $result) . "\n";
		$message .= "Whole query: " . $sql . "\n";
		$retval = false;
	}
	$myrow = pg_fetch_array($result);

/*
 * Cleaning input from the database
 */
	$clean['firstname']				= htmlentities($myrow['firstname'], ENT_QUOTES);
	$clean['lastname']				= htmlentities($myrow['lastname'], ENT_QUOTES);
	$clean['initials']				= htmlentities($myrow['initials'], ENT_QUOTES);
	$clean['title']					= htmlentities($myrow['title'], ENT_QUOTES);
	$clean['gender']				= htmlentities($myrow['gender'], ENT_QUOTES);
	$clean['address']				= htmlentities($myrow['address'], ENT_QUOTES);
	$clean['area']					= htmlentities($myrow['area'], ENT_QUOTES);
	$clean['districtid']			= htmlentities($myrow['districtid'], ENT_QUOTES);
	$clean['phone1']				= htmlentities($myrow['phone1'], ENT_QUOTES);
	$clean['phone2']				= htmlentities($myrow['phone2'], ENT_QUOTES);
	$clean['housetype']				= htmlentities($myrow['housetype'], ENT_QUOTES);
	$clean['dob']					= htmlentities($myrow['dob'], ENT_QUOTES);
	$clean['alone']					= htmlentities($myrow['alone'], ENT_QUOTES);
	// translating the value from the database into true or false
	if($clean['alone'] == 't') {
		$clean['alone'] = true;
	}
	else {
		$clean['alone'] = false;
	}
	$clean['medical_notes']			= htmlentities($myrow['medical_notes'], ENT_QUOTES);
	$clean['contact1name']			= htmlentities($myrow['contact1name'], ENT_QUOTES);
	$clean['contact1relationship']	= htmlentities($myrow['contact1relationship'], ENT_QUOTES);
	$clean['contact1address']		= htmlentities($myrow['contact1address'], ENT_QUOTES);
	$clean['contact1phone']			= htmlentities($myrow['contact1phone'], ENT_QUOTES);
	$clean['contact2name']			= htmlentities($myrow['contact2name'], ENT_QUOTES);
	$clean['contact2relationship']	= htmlentities($myrow['contact2relationship'], ENT_QUOTES);
	$clean['contact2address']		= htmlentities($myrow['contact2address'], ENT_QUOTES);
	$clean['contact2phone']			= htmlentities($myrow['contact2phone'], ENT_QUOTES);
	$clean['gpname']				= htmlentities($myrow['gpname'], ENT_QUOTES);
	$clean['referrer']				= htmlentities($myrow['referrer'], ENT_QUOTES);
	$clean['timeslot']				= htmlentities($myrow['timeslot'], ENT_QUOTES);
	$clean['alerts']				= htmlentities($myrow['alerts'], ENT_QUOTES);
	$clean['groupid']				= htmlentities($myrow['groupid'], ENT_QUOTES);

	$sql = "SELECT dow FROM days WHERE clientid={$clean['clientid']}";
	
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = "Invalid query: " . pg_result_error( $result) . "\n";
		$message .= "Whole query: " . $sql . "\n";
		$retval = false;
	}
	while( $myrow = pg_fetch_array($result)) {
		$clean['dow'][$myrow['dow']] = true;
	}
	
	dbclose($dbConnect);
}

function getClientToDistrictArray( &$aClientToDistrict, $districtid, $unassigned_only = 0) {
	$retval = true;
	
	$dbConnect = dbconnect();

	$sql = "SELECT clients.clientid,districts.district_name FROM clients, districts 
				WHERE clients.districtid=districts.districtid 
				AND clients.active='t'";
	
	if( $unassigned_only == 1) {
		$sql .= " AND clients.clientid NOT IN (SELECT clientid FROM client2operator)";
	}

	if( ctype_digit($districtid) && $districtid > 0) {
		$sql .= " AND clients.districtid=" . $districtid;  
	}
	
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = "Invalid query: " . pg_result_error( $result) . "\n";
		$message .= "Whole query: " . $sql . "\n";
		$retval = false;
	}
	
	while( $myrow = pg_fetch_array($result)) {
		$aClientToDistrict[$myrow['clientid']] = htmlentities($myrow['district_name'], ENT_QUOTES);
	}
	
	dbclose($dbConnect);
	
	return $retval;
}

function addClient(&$clean) {
	$retval = true;
	$dbConnect = dbconnect();
    
	$sql  = "BEGIN;";
	$sql .= sprintf("SELECT add_client('%s'::varchar, '%s'::varchar, '%s'::varchar,
			'%s'::varchar, '%s'::varchar, '%s'::varchar, %d::bigint, '%s'::varchar, 
			'%s'::varchar, '%s'::varchar, '%s'::date, '%s'::boolean, '%s'::varchar,
			'%s'::varchar, '%s'::varchar, '%s'::varchar, '%s'::varchar, '%s'::varchar,
			'%s'::varchar, '%s'::varchar, '%s'::varchar, '%s'::varchar,'%s'::varchar,
			'%s'::varchar, '%s'::time, %d::bigint, %d::bigint) as new_clientid;",  
			pg_escape_string( $clean['firstname']),
			pg_escape_string( $clean['lastname']),
			pg_escape_string( $clean['title']),
			pg_escape_string( $clean['gender']),
			pg_escape_string( $clean['address']),
			pg_escape_string( $clean['area']),
			pg_escape_string( $clean['districtid']), 
			pg_escape_string( $clean['phone1']),
			pg_escape_string( $clean['phone2']),
			pg_escape_string( $clean['housetype']),
			pg_escape_string( $clean['dob']),
			pg_escape_string( $clean['alone']),
			pg_escape_string( $clean['medical_notes']),
			pg_escape_string( $clean['contact1name']),
			pg_escape_string( $clean['contact1relationship']),
			pg_escape_string( $clean['contact1address']),
			pg_escape_string( $clean['contact1phone']),
			pg_escape_string( $clean['contact2name']),
			pg_escape_string( $clean['contact2relationship']),
			pg_escape_string( $clean['contact2address']), 
			pg_escape_string( $clean['contact2phone']),
			pg_escape_string( $clean['gpname']),
			pg_escape_string( $clean['referrer']),
			pg_escape_string( $clean['alerts']),
			pg_escape_string( $clean['timeslot']),
			pg_escape_string( $clean['operatorid']),
			pg_escape_string( $clean['groupid']));
			
	// run SQL against the DB
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
/*		if( pg_errno() == 1062) {
			print "<font size=\"3\" color=\"#FF0000\"><b>Duplicate entry!</b></font>
					<p>Client with the same name and date of birth already exists on the system.<br>
					Please use Edit Client screen to change details of existing client or check that you entered
					name and date of birth of the new client correctly<p>";
			
			print  '<a href="' . $_SERVER['PHP_SELF'] . '">Add another client</a><p>';
		}
		else {*/
			$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
			printErrorMessage( $message);
			$retval = false;
//		}
	}
	else {
		/* 
		 * add_client() function returns clientid of the newly added client
		 * which we'll use below to get the 'call on' days added to the system
		 */
		$new_clientid = pg_fetch_result($result, 0, 'new_clientid');
		
		$sql = "";
		
		for($i=0; $i <7; $i++) {
			if(isset($clean['dow'][$i])) {
				$sql .= "INSERT INTO days (clientid, dow) VALUES ({$new_clientid}, {$i});";
			}
		}
		
		$sql .= "COMMIT;";
						
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

function deactivateClient( $clientid) {
	$retval = true;
	$dbConnect = dbconnect();

	$sql = "UPDATE clients SET active='0' WHERE clientid='" . $clientid . "'";
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
	
	dbclose( $dbConnect);
	
	return $retval;
}

function reactivateClient( $clientid) {
	$retval = true;
	$dbConnect = dbconnect();

	$sql = "UPDATE clients SET active='1' WHERE clientid='" . $clientid . "'";
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
		
	dbclose( $dbConnect);
	
	return $retval;
}

function getClientsForOperator( $operatorid, $districtid, &$clients) {
	$myrow = '';
	$dbConnect = dbconnect();

	$sql = 'SELECT clients.clientid,clients.firstname,clients.lastname,clients.initials 
			FROM clients LEFT JOIN client2operator ON clients.clientid=client2operator.clientid 
			WHERE client2operator.operatorid=' . $operatorid . ' ';


	if( ctype_digit($districtid) && $districtid > 0) {
		$sql .= ' AND clients.districtid=' . $districtid . ' ';  
	}
	
	$sql .= ' ORDER BY clients.lastname, clients.firstname';

	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			$clients[$myrow['clientid']] = $myrow['lastname'] . ', ' . $myrow['firstname'] . ' ' . $myrow['initials'];
		}
	}
	
	dbclose( $dbConnect);

	return $result;
}

function getClientTimeSlot( $clientid) {
	$myrow = '';
	$timeslot = '&nbsp;';

	if( !ctype_digit( trim($clientid))) {
		return 'Bad client ID';
	}
	
	$dbConnect = dbconnect();

	$sql = "SELECT TO_CHAR(timeslot, 'HH24:MI') as timeslot FROM clients WHERE clientid=" . $clientid;  

	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			$timeslot = $myrow['timeslot'];
		}
	}
	
	dbclose( $dbConnect);
	
	return $timeslot;
}

function getClients( &$clients, $districtid = 0, $what_clients = ALL_CLIENTS) {
	$myrow = '';
	$client_name = '';
	$dbConnect = dbconnect();
	
	$sql = "SELECT clientid,firstname,lastname,initials,active FROM clients ";

	// if none of the below, we get all clients
	switch( $what_clients) {
	case ACTIVE_CLIENTS:
			$sql .= " WHERE active='t'";
			break;
	case INACTIVE_CLIENTS:
			$sql .= " WHERE active='f'";
			break;
	case ACTIVE_UNASSIGNED_CLIENTS:
			$sql .= " WHERE clientid NOT IN (SELECT clientid FROM client2operator)";
			break;
	}

	// if $districtid=0, we want to see all active clients
	if( ctype_digit($districtid) && $districtid > 0) {
		$sql .= " AND districtid=" . $districtid . " ";  
	}
	
	$sql .= " ORDER BY lastname";
	
	$result = pg_query( $dbConnect, $sql);
	if ( !$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage($message);
	}
	else {
		while ( $myrow = pg_fetch_array($result)) {
			if(isset( $myrow['clientid'])) {
				if( ctype_digit($myrow['clientid'])) { 
					$client_name = htmlentities($myrow['lastname'], ENT_COMPAT) . ', ' .
										htmlentities($myrow['firstname'], ENT_QUOTES) . ' ' .
										htmlentities($myrow['initials'], ENT_QUOTES) . ' ';

					/* adding (-) to the name if the client is inactive */
					if( isset($what_clients) && $what_clients == ALL_CLIENTS 
								&& $myrow['active'] == 'f') {
						$client_name = '(-) ' . $client_name;
					}
					
					$clients[$myrow['clientid']]= $client_name; 
				}
			} 
		}
	}
	
	dbclose( $dbConnect);
	
	return $result;
}

function getActiveClients( &$clients, $districtid = 0, $unassigned_only = 0) {
	if( $unassigned_only == 1 ) {
		return getClients( $clients, $districtid, ACTIVE_UNASSIGNED_CLIENTS);
	}
	else {
		return getClients( $clients, $districtid, ACTIVE_CLIENTS);
	}
}

function getInactiveClients( &$clients, $districtid = 0, $unassigned_only = 0) {
	return getClients( $clients, $districtid, INACTIVE_CLIENTS);
}

function getAllClients( &$clients) {
	return getClients( $clients, 0, ALL_CLIENTS);
}


function updateClient( &$clean) {
	$retval = true;
	$dbConnect = dbconnect();

	// here if no ID then editing  else adding
	if ( $clean['clientid']) {
		$sql = "BEGIN;";
	    $sql  .= sprintf("UPDATE clients SET firstname='%s',
		    lastname='%s',title='%s',gender ='%s',address='%s',area='%s',
			phone1='%s',phone2='%s',dob='%s',gpname='%s',housetype='%s',
			referrer='%s',alone='%s',medical_notes='%s',alerts='%s',
			contact1name='%s',contact1relationship='%s',
		    contact1address='%s',contact1phone='%s',
			contact2name='%s',contact2relationship='%s',
		    contact2address='%s',contact2phone='%s', 
		    timeslot='%s',changenote='%s',districtid=%d,groupid=%d,modifiedby=%d WHERE clientid=%d;",
			pg_escape_string( $clean['firstname']),
			pg_escape_string( $clean['lastname']),
			pg_escape_string( $clean['title']),
			pg_escape_string( $clean['gender']),
			pg_escape_string( $clean['address']),
			pg_escape_string( $clean['area']),
			pg_escape_string( $clean['phone1']),
			pg_escape_string( $clean['phone2']),
			pg_escape_string( $clean['dob']),
			pg_escape_string( $clean['gpname']),
			pg_escape_string( $clean['housetype']),
			pg_escape_string( $clean['referrer']),
			pg_escape_string( $clean['alone']),
			pg_escape_string( $clean['medical_notes']),
			pg_escape_string( $clean['alerts']),
			pg_escape_string( $clean['contact1name']),
			pg_escape_string( $clean['contact1relationship']),
			pg_escape_string( $clean['contact1address']),
			pg_escape_string( $clean['contact1phone']),
			pg_escape_string( $clean['contact2name']),
			pg_escape_string( $clean['contact2relationship']),
			pg_escape_string( $clean['contact2address']),
			pg_escape_string( $clean['contact2phone']),
			pg_escape_string( $clean['timeslot']),
			pg_escape_string( $clean['changenote']),
			pg_escape_string( $clean['districtid']),
			pg_escape_string( $clean['groupid']),
			pg_escape_string( $clean['operatorid']),
			pg_escape_string( $clean['clientid']));
		
		$sql .= "DELETE FROM days WHERE clientid={$clean['clientid']};";
	
		for($i=0; $i <7; $i++) {
			if(isset($clean['dow'][$i])) {
				$sql .= "INSERT INTO days (clientid, dow) VALUES ({$clean['clientid']}, {$i});";
			}
		}
		
		$sql .= "COMMIT;";

		$result = pg_query( $dbConnect, $sql);
		if ( !$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
			printErrorMessage( $message);
			$retval = false;
		}
	}

	dbclose( $dbConnect);
	
	return $retval;
}

// this is for drop-down list of clients
function getClientsAsDropDownList()
{
	$dbConnect = dbconnect();
	
    $sql = "SELECT clientid,lastname,firstname FROM clients ORDER BY lastname";
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
    }
    while( $client = pg_fetch_array($result)) {
		printf("<option value=\"%d\">%s, %s (%d)</option>",
	    $client[clientid],strtoupper($client[lastname]),
	    strtoupper($client[firstname]),$client[clientid]);
    }
    
    dbclose($dbConnect);
}

?>