<?php
require_once("district.inc");
require_once("groups.inc");

function verifyClientData( &$arClientData, &$clean) {
	$magic = get_magic_quotes_gpc();

	if(isset($arClientData['firstname'])) {
		$firstname = htmlentities(strtoupper($arClientData['firstname']), ENT_QUOTES );
		if( $magic) {
			$clean['firstname'] = stripslashes( $firstname);
		}
		else {
			$clean['firstname'] = $firstname;
		}
	}
	
	if(isset($arClientData['lastname'])) {
		$lastname = htmlentities(strtoupper($arClientData['lastname']), ENT_NOQUOTES );
		if( $magic) {
			$clean['lastname'] = stripslashes( $lastname);
		}
		else {
			$clean['lastname'] = $lastname;
		}
	}
	
	if(isset($arClientData['title'])) {
		$title =  htmlentities($arClientData['title'], ENT_QUOTES );
		if( $magic) {
			$clean['title'] = stripslashes( $title);
		}
		else {
			$clean['title'] = $title;
		}
	}
	
	if(isset($arClientData['gender'])) {
		if(ctype_print($arClientData['gender'])) {
			$clean['gender'] = htmlentities($arClientData['gender'], ENT_QUOTES );
		}
	}
	
	if(isset($arClientData['address'])) {
		if(ctype_print($arClientData['address'])) {
			$address = htmlentities($arClientData['address'], ENT_NOQUOTES );
			if( $magic) {
				$clean['address'] = stripslashes( $address);
			}
			else {
				$clean['address'] = $address;
			}
		}
	}
	
	if(isset($arClientData['area'])) {
		if(ctype_print($arClientData['area'])) {
			$area = htmlentities($arClientData['area'], ENT_NOQUOTES );
			if( $magic) {
				$clean['area'] = stripslashes( $area);
			}
			else {
				$clean['area'] = $area;
			}
		}
	}
	
	if(isset($arClientData['districtid'])) {
		if(ctype_digit($arClientData['districtid'])) {
			$clean['districtid'] = $arClientData['districtid'];
		}
	}
	
	if(isset($arClientData['phone1'])) {
		$clean['phone1'] = filter_phone_number($arClientData['phone1']);
	}
	
	if(isset($arClientData['phone2'])) {
		$clean['phone2'] = filter_phone_number($arClientData['phone2']);
	}
	
	if(isset($arClientData['groupid'])) {
		if(ctype_digit($arClientData['groupid'])) {
			$clean['groupid'] = $arClientData['groupid'];
		}
	}
	
	if(isset($arClientData['dob'])) {
		$reg = 0;
		if(ereg( "([0-3]{1}[0-9]{1})/([0-1]{1}[0-9]{1})/([1-2]{1}[0-9]{3})", $arClientData['dob'], $reg)) {
			$clean['dob'] = $reg[3] . "-" . $reg[2] . "-" . $reg[1];
		}
	}
	
	if(isset($arClientData['gpname'])) {
		if(ctype_print($arClientData['gpname'])) {
			$gpname = htmlentities($arClientData['gpname'], ENT_NOQUOTES );
			if( $magic) {
				$clean['gpname'] = stripslashes( $gpname);
			}
			else {
				$clean['gpname'] = $gpname;
			}
		}
	}
	
	if(isset($arClientData['housetype'])) {
		if(ctype_print($arClientData['housetype'])) {
			$housetype = htmlentities($arClientData['housetype'], ENT_QUOTES );
			if( $magic) {
				$clean['housetype'] = stripslashes( $housetype);
			}
			else {
				$clean['housetype'] = $housetype;
			}
		}
	}
	
	if(isset($arClientData['alone'])) {
		if(strtoupper($arClientData['alone']) == "ON") {
			$clean['alone'] = 't';
		}
		else {
			$clean['alone'] = 'f';
		}
	}
	else {
		$clean['alone'] = 'f';
	}
	
	if(isset($arClientData['medical_notes'])) {
		if(ctype_print($arClientData['medical_notes'])) {
			$medical_notes = htmlentities($arClientData['medical_notes'], ENT_NOQUOTES );
			if( $magic) {
				$clean['medical_notes'] = stripslashes( $medical_notes);
			}
			else {
				$clean['medical_notes'] = $medical_notes;
			}
		}
	}
	
	if(isset($arClientData['alerts'])) {
		if(ctype_print($arClientData['alerts'])) {
			$alerts = htmlentities($arClientData['alerts'], ENT_NOQUOTES );
			if( $magic) {
				$clean['alerts'] = stripslashes( $alerts);
			}
			else {
				$clean['alerts'] = $alerts;
			}
		}
	}
	
	if(isset($arClientData['referrer'])) {
		if( ctype_print($arClientData['referrer'])) {
			$referrer = htmlentities($arClientData['referrer'], ENT_NOQUOTES );
			if( $magic) {
				$clean['referrer'] = stripslashes($referrer);
			}
			else {
				$clean['referrer'] = $referrer;
			}
		}
	}
	
	if(isset($arClientData['referrer_other'])) {
		if( ctype_print($arClientData['referrer_other'])) {
			$referrer = htmlentities($arClientData['referrer_other'], ENT_NOQUOTES );
			if( $magic) {
				$clean['referrer'] = stripslashes($referrer);
			}
			else {
				$clean['referrer'] = $referrer;
			}
		}
	}
	
	if(isset($arClientData['contact1name'])) {
		if(ctype_print($arClientData['contact1name'])) {
			$contact1name = htmlentities($arClientData['contact1name'], ENT_NOQUOTES );
			if( $magic) {
				$clean['contact1name'] = stripslashes($contact1name);
			}
			else {
				$clean['contact1name'] = $contact1name;
			}
		}
	}
	
	if(isset($arClientData['contact1relationship'])) {
		if(ctype_print($arClientData['contact1relationship'])) {
			$contact1relationship = htmlentities($arClientData['contact1relationship'], ENT_QUOTES );
			if( $magic) {
				$clean['contact1relationship'] = stripslashes( $contact1relationship);
			}
			else {
				$clean['contact1relationship'] = $contact1relationship;
			}
		}
	}
	
	if(isset($arClientData['contact1address'])) {
		if(ctype_print($arClientData['contact1address'])) {
			$contact1address = htmlentities($arClientData['contact1address'], ENT_NOQUOTES );
			if( $magic) {
				$clean['contact1address'] = stripslashes( $contact1address);
			}
			else {
				$clean['contact1address'] = $contact1address;
			}
		}
	}
	
	if(isset($arClientData['contact1phone'])) {
		$clean['contact1phone'] = filter_phone_number($arClientData['contact1phone']);
	}
	
	
	if(isset($arClientData['contact2name'])) {
		if(ctype_print($arClientData['contact2name'])) {
			$contact2name = htmlentities($arClientData['contact2name'], ENT_NOQUOTES );
			if( $magic) {
				$clean['contact2name'] = stripslashes($contact2name);
			}
			else {
				$clean['contact2name'] = $contact2name;
			}
		}
	}
	
	if(isset($arClientData['contact2relationship'])) {
		if(ctype_print($arClientData['contact2relationship'])) {
			$contact2relationship = htmlentities($arClientData['contact2relationship'], ENT_QUOTES );
			if( $magic) {
				$clean['contact2relationship'] = stripslashes( $contact2relationship);
			}
			else {
				$clean['contact2relationship'] = $contact2relationship;
			}
		}
	}
	
	if(isset($arClientData['contact2address'])) {
		if(ctype_print($arClientData['contact2address'])) {
			$contact2address = htmlentities($arClientData['contact2address'], ENT_NOQUOTES );
			if( $magic) {
				$clean['contact2address'] = stripslashes( $contact2address);
			}
			else {
				$clean['contact2address'] = $contact2address;
			}
		}
	}
		
	if(isset($arClientData['contact2phone'])) {
		$clean['contact2phone'] = filter_phone_number($arClientData['contact2phone']);
	}
	
	if(isset($arClientData['timeslot'])){
		$clean['timeslot'] = verify_timeslot($arClientData['timeslot']);
	}
}

function printAddEditClientJavaScript() 
{
	print '
		<script language="JavaScript1.1">
		<!-- hide it from old browsers or from those with JavaScript disabled
		function vtslot(s)
		{
		   var ts = Array();
		   var ts = s.value.split(":");
		   var hour = Number(ts[0]);
		   var min = Number(ts[1]);

		   if(!isNaN(hour) && !isNaN(min)) {
		      if((hour>=0 && hour<=23) && (min>=0 && min<=59)) {
		         return true;
		      }
		   }
		   alert("Time has to be of 24 hours format (HH:MM)!");
		   return false;
		}
		
		function vphone(phone)
		{
			var result = phone.value.match(/\d+/g);

			if( result != null) {
				return true;
			}
			else {
				if(phone.optional) {
					return true;
				}
				else {
					alert("Phone number should consist of digits only!");
					return false;
				}
			}
		}

		function vdob(dob)
		{
			var msg = "";
			var today = new Date();
			var checked_dob = Array();
			checked_dob = dob.value.split("/");
			
			var day = Number(checked_dob[0]);
			var month = Number(checked_dob[1]);
			var year = Number(checked_dob[2]);

			if(!isNaN(day) && !isNaN(month) && !isNaN(year)) {
				if((day >= 1 && day <= 31) && 
					(month >= 1 && month <= 12) && 
					(year >= 1900 && year <= (today.getYear()+1900))) {
				   return true;
				}
				else {
					msg  = "Day range: 1-31\n";
					msg += "Month range: 1-12\n";
					msg += "Year range: 1900 - ";
					msg += (today.getYear()+1900);
				}
			}
			alert("Date of birth must be of format DD/MM/YYYY!\n\n" + msg);
			return false;
		}
		
		function isEmpty(text, field_name)
		{
			if( text.value.length == 0) {
				alert( field_name + " can not be empty!");
				return false;
			}
			return true;
		}
		
		// A utility function that returns true if a string contains only 
		// whitespace characters
		function isblank(s) {
		    for(var i = 0; i < s.length; i++) {
		        var c = s.charAt(i);
		        if ((c != " ") && (c != "\n") && (c != "\t")) return false;
		    }
		    return true;
		}
		
		function verify(f)
		{
			var msg = "Please, fill all fields marked with red asterisk.\nThe fields are mandatory!\n";
			var empty_fields = "";
			var errors = "";
			var ret;
		
		    // Loop through the elements of the form, looking for all Text and 
		    // Textarea elements that do not have an "optional" property defined. 
		    // Then check for fields that are empty and make a list of them.
		    // Put together error messages for fields that are wrong.
		    for(var i = 0; i < f.length; i++) {
		        var e = f.elements[i];
		        if (((e.type == "text") || (e.type == "textarea")) && !e.optional) {
		            // First check if the field is empty
		            if ((e.value == null) || (e.value == "") || isblank(e.value)) {
		                empty_fields += "\n          " + e.name;
		                continue;
		            }
		        }
		    }

			if( !vdob(f.dob)) {
				errors += "Date of birth format: DD/MM/YYYY\n";
			}

			if( !vphone(f.phone1) || !vphone(f.contact1phone)) {
				errors += "Phone numbers must consist of digits only!\n";
			}

			if(!vphone(f.phone2) && !f.phone2.optional) {
				errors += "Phone numbers must consist of digits only!\n";
			}

			// Now, if there were any errors, display the messages, and
		    // return false to prevent the form from being submitted. 
		    // Otherwise, return true.
		    if (!empty_fields && !errors) return true;
		
		    msg += "______________________________________________________\n\n"
		
		    if (empty_fields) {
		        msg += "- The following required field(s) are empty:" 
		                + empty_fields + "\n";
		        if (errors) msg += "\n";
		    }
		    msg += errors;
		    alert(msg);
		    return false;
		}
		
		-->
		</script>';

}

function printAddEditClientTable( $clean, $edit = 0)
{
	$arDistricts = array(); // array to populate district list
	
	print'<table width="100%" border="0" cellspacing="4" cellpadding="2">
	<tr>
		<td bgcolor="#EEEEEE" colspan="2">
			<font face="verdana, arial, helvetica" size="-1">
			<b>Personal Details</b><br>
			</font>
		</td>
	</tr>
	
	<br>
	<br>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right" width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>First name: </b>
		</font>
		<input type="Text" name="firstname" value="' . $clean['firstname'] . '" size="30" maxlength="30">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Date of Birth: </b>
		</font>
		<input type="Text" name="dob" onchange="return vdob(this);" value="';
		    
	if($clean['dob']) {
		if( ereg( "([1-2]{1}[0-9]{3})-([0-1]{1}[0-9]{1})-([0-9]{2})", $clean['dob'], $reg)) {
			print("$reg[3]/$reg[2]/$reg[1]");
		}
		else {
			print("bad dob");
		}
	}
	
	print '" size="11">
		<font size="1" color="#FF0000">
		<b>* Format DD/MM/YYYY</b>
		</font>
		</td>
	
		<td ALIGN="right" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right" width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Last name: </b>
		</font>
		<input type="Text" name="lastname" value="' . $clean['lastname'] . '" size="30" maxlength="30">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Alone: </b>
		</font>
		<input type="checkbox" name="alone" size="5" maxlength="5"';
				
	if($clean['alone']) {
		print(" checked");
	}
	
	print '>
		</td>
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Address: </b>
		</font>
		<input type="Text" name="address" value="' . $clean['address'] . '" size="30" maxlength="30">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>

		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>';
		
		print '<input type="radio" name="gender" value="female" ';
		if($clean['gender'] == 'female') {
			print 'checked';
		}
		print '>Female</input>';
		
		print '<input type="radio" name="gender" value="male" ';
		if($clean['gender'] == 'male') {
			print 'checked ';
		}
		print '>Male</input>';
		
		print '</b>
		</font>
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Area: </b>
		</font>
		<input type="Text" name="area" value="' . $clean['area'] . '" size="25">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Alerts: </b><br>
		</font>
		<textarea name="alerts" cols="27" rows="4" >' . $clean['alerts'] . '</textarea>
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>District: </b>
		</font>';
			
		if( getDistrictList( $arDistricts)) {
			print '<select name="districtid">';
			
			foreach( $arDistricts as $did => $district_name) {
				if($clean['districtid'] == $did) {
					print '<option value="' . $did . '" selected>' . $district_name . '</option>';
				}
				else {
					print '<option value="' . $did . '">' . $district_name . '</option>';
				}
			}
			
			print '</select>';
		}
		print '<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>House Type: </b>
		</font>
		<select name="housetype">
			<option value="apartment">Apartment</option>
			<option value="bungalow">Bungalow</option>
			<option value="cottage flat">Country House / Cottage</option>
			<option value="detached">Detached House</option>
			<option value="farm">Farm</option>
			<option value="holiday">Holiday Home</option>
			<option value="other">Other</option>
			<option value="semi-detached">Semi-Detached House</option>
			<option value="terraced">Terraced House</option>
			<option value="' . $clean['housetype'] . '" selected>' . $clean['housetype'] . '</option>
		</select>
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Phone Number 1 <font size="1" color="#FF0000">(digits only!)</font>: </b>
		</font>
		<input type="Text" name="phone1" onchange="return vphone(this);" value="' . $clean['phone1'] . '" size="15">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Phone Number 2 <font size="1" color="#FF0000">(digits only!)</font>: </b>
		</font>
		<input type="text" name="phone2" onchange="return vphone(this);" size="14" value="' . $clean['phone2'] . '">
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Group: </b>
		</font>';

		if( getGroupNames( $arGroupNames)) {
			print '<select name="groupid">';
			
			foreach( $arGroupNames as $gid => $group_name) {
				if($clean['groupid'] == $gid) {
					print '<option value="' . $gid . '" selected>' . $group_name . '</option>';
				}
				else {
					print '<option value="' . $gid . '">' . $group_name . '</option>';
				}
			}
			
			print '</select>';
		}

		print '
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%" VALIGN="top">
		</td>
		
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	<table width="100%" border="0" cellspacing="4" cellpadding="2">
	<tr>
		<td bgcolor="#EEEEEE" colspan="2">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Medical Details</b><br>
		</font>
		</td>
	</tr>
	</table>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Referrer: </b>
		</font>
		<select name="referrer">
			<option value="Health Board">Health Board</option>
			<option value="Social Work">Social Work</option>
			<option value="Private">Private</option>
			<option value="Voluntary Group">Voluntary Group</option>
			<option value="Self-Referred">Self Referred</option>
			<option selected value="' . $clean['referrer'] . '">' . $clean['referrer'] . '</option>
		</select>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>GP name: </b>
		</font>
		<input type="text" name="gpname" maxlength="30" size="30" value="' . $clean['gpname'] . '">
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Other: </b>
		</font>
		<input type="text" name="referrer_other" size="25" value="' . $clean['referrer_other'] . '" maxlength="40">
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Medical notes: </b><br>
		</font>
		<textarea name="medical_notes" cols="27" rows="4" >' . $clean['medical_notes'] . '</textarea>
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	<table width="100%" border="0" cellspacing="4" cellpadding="2">
	<tr>
		<td bgcolor="#EEEEEE" colspan="2">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Contact Details</b><br>
		</font>
		</td>
	</tr>
	</table>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Contact name 1: </b>
		</font>
		<input type="text" name="contact1name" value="' . $clean['contact1name'] . '" size="22" maxlength="30">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Phone <font size="1" color="#FF0000">(digits only!)</font>: </b>
		</font>
		<input type="text" name="contact1phone" onchange="return vphone(this);" size="15" maxlength="15" value="' . $clean['contact1phone'] . '">
		<font size="1" color="#FF0000">
		<b> *</b>
		</font>
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Address: </b>
		</font>
		<input type="text" name="contact1address" size="32" maxlength="50" value="' . $clean['contact1address'] . '">
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Relationship: </b>
		</font>
		<input type="text" name="contact1relationship" size="20" value="' . $clean['contact1relationship'] . '">
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top"></td>
	</tr>
	</TABLE>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Contact name 2: </b>
		</font>
		<input type="text" name="contact2name" value="' . $clean['contact2name'] . '" size="22" maxlength="30">
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Phone <font size="1" color="#FF0000">(digits only!)</font>: </b>
		</font>
		<input type="text" name="contact2phone" onchange="return vphone(this);" size="15" maxlength="15" value="' . $clean['contact2phone'] . '">
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="right"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Address: </b>
		</font>
		<input type="text" name="contact2address" size="32" maxlength="50" value="' . $clean['contact2address'] . '">
		</td>
	
		<td ALIGN="right" width="10%" VALIGN="top">
		</td>
	
		<td ALIGN="left" width="30%"  VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Relationship: </b>
		</font>
		<input type="text" name="contact2relationship" size="20" value="' . $clean['contact2relationship'] . '">
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>
	
	<table width="100%" border="0" cellspacing="4" cellpadding="2">
	<tr>
		<td bgcolor="#EEEEEE" colspan="2">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Time of the first call on a day</b><br>
		</font>
		</td>
	</tr>
	</table>
	
	<TABLE BORDER=0 WIDTH=100%>
	<tr>
		<td ALIGN="right" width="15%" VALIGN="top">
		</td>
	
		<td ALIGN="center"  width="30%" VALIGN="top">
		<font face="verdana, arial, helvetica" size="-1">
		<b>Time slot: </b>
		</font>
		<input type="text" name="timeslot" onchange="return vtslot(this);" 
			value="' . $clean['timeslot'] . '" size="6" maxlength="5">
		<font size="1" color="#FF0000">
		<b>* 24 hour format (HH:MM)</b>
		</font>
		</input>
		</td>
	
		<td ALIGN="left" width="15%"  VALIGN="top">
		</td>
	</tr>
	</TABLE>';

	if( $edit) {
		print '<table width="100%" border="0" cellspacing="4" cellpadding="2">
		<tr>
			<td bgcolor="#EEEEEE" colspan="2">
			<font face="verdana, arial, helvetica" size="-1">
			<b>Why did you edit the client record?</b><br>
			</font>
			</td>
		</tr>
		</table>
		
		<TABLE BORDER=0 WIDTH=100%>
		<tr>
			<td ALIGN="right" width="15%" VALIGN="top">
			</td>
		
			<td ALIGN="left"  width="30%" VALIGN="top">
			<font face="verdana, arial, helvetica" size="-1">
			<b>Change note: </b>
			</font>
			<font size="1" color="#FF0000">
			<b>*</b>
			</font>
			<textarea name="changenote" cols="100" rows="2" ></textarea>
			</td>
		
			<td ALIGN="right" width="10%" VALIGN="top">
			</td>
		</tr>
		</TABLE>';
	}		
}

?>