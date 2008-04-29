<?php
require_once("db.inc");
require_once("messages.inc");

function getMainClassificationList( &$arMainClassifications)
{
	$retval = true;
	$arMainClassifications = array();
	
	$dbConnect = dbconnect();

    $sql  = "SELECT mclass_id,mclass_name FROM call_mclass ORDER BY mclass_name";
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
		$retval=false;
    }
    while( $class = pg_fetch_array($result)) {
		$arMainClassifications[$class['mclass_id']] = $class['mclass_name'];
    }
    
    dbclose($dbConnect);
    return $retval;
}

function getSubClassificationList( &$arSubClassifications) {
	$retval = true;
	$arSubClassifications = array();
	
	$dbConnect = dbconnect();

    $sql  = "SELECT sclass_id,sclass_name FROM call_sclass ORDER BY sclass_name";
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
		$retval=false;
    }
    while( $class = pg_fetch_array($result)) {
		$arSubClassifications[$class['sclass_id']] = $class['sclass_name'];
    }
    
    dbclose($dbConnect);
    return $retval;
}

function getCombinedClassificationList()
{
	print '<select name="sclass_id" size="1">';
	
	$dbConnect = dbconnect();

    $sql  = "SELECT m.mclass_id,s.sclass_id,m.mclass_name,s.sclass_name";
    $sql .= " FROM call_mclass AS m,call_sclass AS s";
    $sql .= " WHERE m.mclass_id=s.mclass_id ORDER BY m.mclass_id";
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
    }
    while( $class = pg_fetch_array($result)) {
		printf("<option value=\"%d\">%s -- %s</option>",
				$class['sclass_id'],$class['mclass_name'],$class['sclass_name']);
    }
    print '</select>';
    
    dbclose($dbConnect);
}

function getClassificationDictionary() {
	$retval = true;
	$dbConnect = dbconnect();

	$sql  = "SELECT mc.mclass_name,sc.sclass_name";
	$sql .= " FROM call_mclass AS mc, ";
	$sql .= "call_sclass AS sc WHERE mc.mclass_id = sc.mclass_id";
	
	$result = pg_query( $dbConnect, $sql);
	if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . '<br>' . 'Query: ' . $sql;
		printErrorMessage( $message);
		$retval = false;
	}
		
	$out  = '<font face="Verdana, Arial, Helvetica, sans-serif" size="2">';
	$out .= '<table border="0" width="100%" cellspacing="0" cellpadding="0">';
	$out .= '<tr>';
	$out .= '<td width="40%"><b>Main Class name</b></font></td>';
	$out .= '<td width="40%"><b>Sub Class name</b></td>';
	$out .= '</tr>';
	
	print($out);
	
	$i=0; // need this to change cell bgcolor
	
	while ($myrow = pg_fetch_array($result)) {
	    // select sclass short name
	    $mclass_name = $myrow[0];
	    $sclass_name = $myrow[1];
	    
	    $i=$i + 1; // need this to change cell bgcolor
	
	    if($i % 2) {
		print('<tr bgcolor="#FFFFFF">');
	    }
	    else {
		print('<tr bgcolor="#DDDDDD">');
	    }
	
	    $out  = "<td>" . $mclass_name . "</td>";
	    $out .= "<td>" . $sclass_name . "</td>";
	    $out .= "</tr>";
	    
	    print($out);
	}
	
	return $retval;
}

function updateMainClassificationName( $mclass_id, $mclass_name) {
	$retval = true;
	
	$dbConnect = dbconnect();

    $sql  = "UPDATE call_mclass SET mclass_name='" . pg_escape_string($mclass_name) . "'
			 WHERE mclass_id=" . $mclass_id;
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
		$retval=false;
    }
    
    dbclose($dbConnect);
    return $retval;
}

function updateSubClassificationName( $sclass_id, $sclass_name) {
	$retval = true;
	
	$dbConnect = dbconnect();

    $sql  = "UPDATE call_sclass SET sclass_name='" . pg_escape_string($sclass_name) . "'
			 WHERE sclass_id=" . $sclass_id;
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
		$retval=false;
    }
    
    dbclose($dbConnect);
    return $retval;
}

function addNewMainClassificationNames( &$arNewMains) {
	$retval = true;
	
	if( count( $arNewMains) > 0) {
		$dbConnect = dbconnect();
	
		$sql = "BEGIN;";
	
		foreach( $arNewMains as $name) {
		    $sql .= "INSERT INTO call_mclass (mclass_name)
					 VALUES ('" . pg_escape_string(trim($name)) . "');";
		}
		
		$sql .= "COMMIT";
		
	    $result = pg_query( $dbConnect, $sql);
	    if (!$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
			$message .= 'Whole query: ' . $sql;
			printErrorMessage($message);
			$retval=false;
	    }
	    
	    dbclose($dbConnect);
	}
    return $retval;
}

function addNewSubClassificationNames( $mclass_id, &$arNewSubs) {
	$retval = true;
	
	if( count( $arNewSubs) > 0) {
		$dbConnect = dbconnect();
	
		$sql = "BEGIN;";
	
		foreach( $arNewSubs as $name) {
		    $sql .= "INSERT INTO call_sclass (mclass_id,sclass_name)
					 VALUES (" . $mclass_id . ", '" . pg_escape_string(trim($name)) . "');";
		}
		
		$sql .= "COMMIT";
		
	    $result = pg_query( $dbConnect, $sql);
	    if (!$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
			$message .= 'Whole query: ' . $sql;
			printErrorMessage($message);
			$retval=false;
	    }
	    
	    dbclose($dbConnect);
	}
    return $retval;
}

function printAddEditMainClassificationLayout() {
	$arMainClassifications = array();
	print '<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
		<font face="verdana, arial, helvetica" size="2">
		<TABLE BORDER=0 WIDTH=100%>
			<tr>
				<td ALIGN="left"  width="10%" VALIGN="top">
				<font face="verdana, arial, helvetica" size="2">
				<b>Main classifications: </b>
				</font>
				</td>
				<td ALIGN="left"  width="30%" VALIGN="top">';
					
				if( getMainClassificationList( $arMainClassifications)) {
					print '<select name="mclass_id">';
					
					foreach( $arMainClassifications as $cid => $mclass_name) {
						print '<option value="' . $cid . '">' . $mclass_name . '</option>';
					}
					
					print '</select>';
				}

	$out .= '</td>
			</tr>
			<tr>
			<td ALIGN="left" width="10%" VALIGN="top">
			<b>New name for the selected classification:</b>
			</td>
			<td ALIGN="left" width="30%" VALIGN="top">
			<input type="text" name="edit_mclass_name" maxlength="30" size="30">
			</td>
			</tr>
			<tr>
			<td ALIGN="left" width="10%" VALIGN="top">
			<input name="change_main" type="submit" value="Save the new name">
			</td>
			</tr>
			</TABLE>
			</font>';
				
	print $out . '<p><font face="verdana, arial, helvetica" size="2">';
	
	print '<hr noshade><br>
			<font face="verdana, arial, helvetica" size="3">
			<b>Add new main classification:</b></font>
			<br>
			Add new names of classifications (one per line) 
			and press "Add new classification(s)" button below
			<br>
			<textarea name="new_mclass_name" cols="27" rows="4" ></textarea>
			<br><br>
			<input name="add_main" type="submit" value="Add new classification(s)">';
}

function printAddEditSubClassificationLayout() {
	print '<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
		<font face="verdana, arial, helvetica" size="2">
		<TABLE BORDER=0 WIDTH=100%>
			<tr>
				<td ALIGN="left"  width="10%" VALIGN="top">
				<font face="verdana, arial, helvetica" size="2">
				<b>Sub-classifications: </b>
				</font>
				</td>
				<td ALIGN="left"  width="30%" VALIGN="top">';
					
	getCombinedClassificationList();

	$out .= '</td>
			</tr>
			<tr>
			<td ALIGN="left" width="10%" VALIGN="top">
			<b>New name for the selected classification:</b>
			</td>
			<td ALIGN="left" width="30%" VALIGN="top">
			<input type="text" name="edit_sclass_name" maxlength="30" size="30">
			<br><font color="#FF0000">NOTE! Classifications in the above drop-down list
			formatted as "Main classification name -- Sub-classification name". 
			Here you can only change the last part, which is the sub-classification name!
			</td>
			</tr>
			<tr>
			<td ALIGN="left" width="10%" VALIGN="top">
			<input name="change_sub" type="submit" value="Save the new name">
			</td>
			</tr>
			</TABLE>
			</font>';
				
	print $out . '<p><font face="verdana, arial, helvetica" size="2">';
	
	print '<hr noshade><br>
			<font face="verdana, arial, helvetica" size="3">
			<b>Add new sub-classification:</b></font>
			<br>
			Add new names of sub-classifications (one per line) 
			for the below main one and press "Add new classification(s)" button
			<br><br>
			Main Classification: ';
	
	if( getMainClassificationList( $arMainClassifications)) {
		print '<select name="mclass_id">';
		
		foreach( $arMainClassifications as $cid => $mclass_name) {
			print '<option value="' . $cid . '">' . $mclass_name . '</option>';
		}
		
		print '</select>';
	}

	print '<br>Select the main classification above and type in 
			new sub-classifications in the box below (one per line if you want to add
			more than one in one go):<br>
			<textarea name="new_sclass_name" cols="27" rows="4" ></textarea>
			<br><br>
			<input name="add_sub" type="submit" value="Add new classification(s)">';
}

?>