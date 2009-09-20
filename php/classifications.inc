<?php
require_once("db.inc");
require_once("functions.inc");
require_once("messages.inc");

function drawLevel1AndLevel2ClassificationListPanel( $callid = -1) {
	$numofcolumns = 2;
	$colindx = $numofcolumns;
	$i1 = $i2 = 0;
	$level1 = array();
	$level2 = array();
	$level12 = array();
	$exlevel1 = array();	// if call was already classified, it will store the current L1
	$exlevel2 = array();	// if call was already classified, it will store the current L2
	
	$dbConnect = dbconnect();
	
	if( ctype_digit($callid) and $callid > 0) {
		$sql = "SELECT l1id FROM call_l1_class WHERE callid={$callid}";
	    $result = pg_query( $dbConnect, $sql);
	    if (!$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
			$message .= 'Whole query: ' . $sql;
			printErrorMessage($message);
	    }
	    while( $calls_l1_class = pg_fetch_array($result)) {
	    	$exlevel1[] = $calls_l1_class['l1id'];
		}

		$sql = "SELECT l2id FROM call_l2_class WHERE callid={$callid}";
	    $result = pg_query( $dbConnect, $sql);
	    if (!$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
			$message .= 'Whole query: ' . $sql;
			printErrorMessage($message);
	    }
	    while( $calls_l2_class = pg_fetch_array($result)) {
	    	$exlevel2[] = $calls_l2_class['l2id'];
		}
	}
	
	print '<table frame="border" width="100%"><tr>';
	    
    $sql  = "SELECT l1_class.l1id,l1name,l2id,l2name ";
    $sql .= "FROM l1_class LEFT OUTER JOIN l2_class ON (l1_class.l1id=l2_class.l1id) ORDER BY l1_class.l1id";
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
    }
    while( $class = pg_fetch_array($result)) {
    	$level1[$class['l1id']] = $class['l1name'];
    	$level2[$class['l2id']] = $class['l2name'];
    	
    	/* since not all L1 may have L2 classes, we need to check for empty lines */
    	if( $class['l2id'] != "") {
    		$level21[$class['l2id']] = $class['l1id'];
    	}
    }

    dbclose($dbConnect);
    
    // now is the time to draw it all
    foreach( $level1 as $l1id => $l1name) {
    	if( $colindx == 0) {
    		print "</tr><tr>";
    		$colindx = $numofcolumns;
    	}
    	
    	print "<td valign=\"top\"><font face=\"Verdana, Arial, Helvetica, sans-serif\" size=\"3\">";
    	
    	if($l1id == 0) {
			printf("<input onchange=\"uncheck_all_when_noissues(); document.forms[0].class_cb.checked=true;\"
					type=\"checkbox\" id=\"L1_%d\" name=\"L1_%d\"", $l1id, $l1id);
    	}
    	else {
    		printf("<input onchange=\"uncheck_noissues(); document.forms[0].class_cb.checked=true;\" 
    				type=\"checkbox\" id=\"L1_%d\" name=\"L1_%d\"", $l1id, $l1id);
    	}
    	
		if(in_array($l1id, $exlevel1)) {
			print " checked";
		}
		printf(">%s", $l1name);
		
		// draw L2 subtable inside the cell
		// first cell in the table is empty to create an offset
		print '<table>';
		foreach( $level21 as $l2id => $id) {
			if( $id == $l1id) {
				print '<tr><td valign="top" width="10%">&nbsp</td>
						<td>
						<font face="Verdana, Arial, Helvetica, sans-serif" size="2">';
				printf("<input onchange=\"checkL1(this); document.forms[0].class_cb.checked=true;\" 
						type=\"checkbox\" name=\"L1_%d_L2_%d\" id=\"L1_%d_L2_%d\"", $l1id, $l2id, $l1id, $l2id);
				if(in_array($l2id, $exlevel2)) {
					print " checked";
				}
				printf("> %s</td>", $level2[$l2id]);
				
				print '</tr>';
			}
		}
		print '</table></td>';
				
		$colindx--;
		
    }

	print '</tr></table>';
	
}

function printClassJavaScript() {
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
				for(i=1; i<document.forms[0].elements.length; ++i) {
					document.forms[0].elements[i].checked = false;
				}
				return true;
			}
		}		
		
		function checkL1(cb)
		{
			var l1_name_pattern = /L[1-9]+_[1-9]+/;
			var l1_name;
			
			if( (l1_name = l1_name_pattern.exec(cb.name)) != null) {
				document.getElementById(l1_name[0]).checked = true;
			}
			uncheck_noissues();
			
			return true;
		}
		-->
		</script>';
}


function drawLevel1ClassificationListPanel() {
	$numofcolumns = 2;
	$colindx = $numofcolumns;
	
	print '<table frame="border" width="100%"><tr>';
	
	$dbConnect = dbconnect();
	
    $sql  = "SELECT l1id,l1name";
    $sql .= " FROM l1_class ORDER BY l1id";
    
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
    }
    while( $class = pg_fetch_array($result)) {
    	if( $colindx == 0) {
    		print "</tr><tr>";
    		$colindx = $numofcolumns;
    	}
    	
    	if($class['l1id'] > 0) {
			printf("<td><input onchange=\"uncheck_noissues()\" type=\"checkbox\" name=\"L1_%s\" id=\"L1_%s\" size=\"5\" maxlength=\"5\"> %s</td>",
				$class['l1id'], $class['l1id'], $class['l1name']);
    	}
    	else {
			printf("<td><input onchange=\"uncheck_all_when_noissues()\" type=\"checkbox\" name=\"L1_%s\" id=\"L1_%s\" size=\"5\" maxlength=\"5\"> %s</td>",
				$class['l1id'], $class['l1id'], $class['l1name']);
    	}
		$colindx--;
		
    }
    print '</tr></table>';
	
    dbclose($dbConnect);
}

function validateL1ClassificationList( &$arL1Classifications, &$clean) {
	if( is_array( $arL1Classifications)) {
		$arL1 = array();
		
		$dbConnect = dbconnect();
		
	    $sql  = "SELECT l1id FROM l1_class";
	    
	    $result = pg_query( $dbConnect, $sql);
	    if (!$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
			$message .= 'Whole query: ' . $sql;
			printErrorMessage($message);
	    }
	    while( $class = pg_fetch_array($result)) {
	    	if( strtoupper( $arL1Classifications['L1_' . $class['l1id']]) == 'ON') {
	    		$arL1[] = $class['l1id'];
	    	}			
	    }
	    
	    if( sizeof( $arL1) > 0) {
	    	$clean['L1'] = $arL1;
	    }
		
	    dbclose($dbConnect);
	}	
}

function validateL2ClassificationList( &$post, &$clean) {
	if( is_array( $post)) {
		$arL2Classifications = array();
		$arL2 = array();
		
		foreach( $post as $key => $value) {
			if(ereg("L1_[1-9]_L2_([0-9]+)", $key, $reg)) {
				$arL2Classifications[$reg[1]] = $value; 
			}
		}
		
		$dbConnect = dbconnect();
		
	    $sql  = "SELECT l2id FROM l2_class";
	    
	    $result = pg_query( $dbConnect, $sql);
	    if (!$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
			$message .= 'Whole query: ' . $sql;
			printErrorMessage($message);
	    }
	    while( $class = pg_fetch_array($result)) {
	    	if( strtoupper( $arL2Classifications[$class['l2id']]) == 'ON') {
	    		$arL2[] = $class['l2id'];
	    	}			
	    }
	    
	    if( sizeof( $arL2) > 0) {
	    	$clean['L2'] = $arL2;
	    }
		
	    dbclose($dbConnect);
	}	
}

function getL1andL2ClassificationNamesList( &$arL1, &$arL2)
{
	$retval = "";
	$i = 0;
	$nrows = 0;
	
	$dbConnect = dbconnect();

//    $sql  = "SELECT l1name FROM l1_class WHERE l1id in (" . $L1_class_id_list . ")";

// --> fix the query
  	$sql = "SELECT l1_class.l1id,l1name,l2id,l2name FROM l1_class LEFT OUTER JOIN l2_class ON (l1_class.l1id=l2_class.l1id) 
  			WHERE l1_class.l1id in " . array2sqllist($arL1) . " and l2id in " . array2sqllist($arL2) . " ORDER BY l1_class.l1id";
  	  
    $result = pg_query( $dbConnect, $sql);
    if (!$result) {
		$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
		$message .= 'Whole query: ' . $sql;
		printErrorMessage($message);
    }
    else {
    	$nrows = pg_num_rows($result);

// ---> here is the place to fix.    	
	    while( $class = pg_fetch_array($result)) {
	    	$i++;
	    	if( $i == $nrows) {
	    		$retval .= sprintf("%s", $class['l1name']);
	    	}
	    	else {
				$retval .= sprintf("%s,", $class['l1name']);
	    	}
	    }
    }
    
    dbclose($dbConnect);
	
	return $retval;
}

/*
 * SELECT l1_class.l1id,l1name,l2id,l2name FROM l1_class LEFT OUTER JOIN l2_class ON (l1_class.l1id=l2_class.l1id) WHERE l1_class.l1id in (4,5) and l2id in (15,30) ORDER BY l1_class.l1id;
 */

function classifyTheCall( $callid, &$arL1, &$arL2)
{
	$retval = true;
	
	if(ctype_digit($callid) and $callid > 0) {
		$dbConnect = dbconnect();
	
		$sql  = "BEGIN;";

		/* get rid of all that's there already */
		$sql .= "DELETE FROM call_l1_class WHERE callid={$callid};";
		$sql .= "DELETE FROM call_l2_class WHERE callid={$callid};";
		
		if(isset($arL1) and is_array($arL1) and sizeof($arL1) > 0) {
			foreach( $arL1 as $l1id) {
			    $sql .= "INSERT INTO call_l1_class (callid,l1id)
						 VALUES ({$callid},  {$l1id});";
			}
		}
		
		if(isset($arL2) and is_array($arL2) and sizeof($arL2) > 0) {
			foreach( $arL2 as $l2id) {
			    $sql .= "INSERT INTO call_l2_class (callid,l2id)
						 VALUES ({$callid},  {$l2id});";
			}
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
	else {
		$retval = false;
	}
		
    return $retval;
}

function getCombinedClassificationItem( $sclass_id)
{
	$retval = false;
	
	if (ctype_digit( $sclass_id) && $sclass_id > 0) {
		$dbConnect = dbconnect();
	
	    $sql  = "SELECT mclass_name,sclass_name FROM call_sclass";
	    $sql .= " NATURAL JOIN call_mclass WHERE sclass_id=" . pg_escape_string( $sclass_id);
	    
	    $result = pg_query( $dbConnect, $sql);
	    if (!$result) {
			$message  = 'Invalid query: ' . pg_result_error( $result) . "\n";
			$message .= 'Whole query: ' . $sql;
			printErrorMessage($message);
	    }
	    if( $class = pg_fetch_array($result)) {
			$retval = sprintf("%s -- %s", $class['mclass_name'], $class['sclass_name']);
	    }
	    
	    dbclose($dbConnect);
	}
	
	return $retval;
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