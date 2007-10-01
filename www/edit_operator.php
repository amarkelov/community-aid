<?php
session_start();
require 'functions.inc';

$clean = array();
$mysql = array();

$settings = get_gmp_settings();

print '<html><head>
		<title>Edit Operator --  Friendly Call Service -- ' . $settings['location'] . '</title>
		<meta http-equiv="expires" content="-1">
		<meta http-equiv="Cache-Control" content="no-cache">
		<script type="text/javascript">
		<!-- hide it from old browsers or from those with JavaScript disabled

/** 
* 
*  Secure Hash Algorithm (SHA1) 
*  http://www.webtoolkit.info/ 
* 
**/  
  
function SHA1 (msg) {  
   
     function rotate_left(n,s) {  
         var t4 = ( n<<s ) | (n>>>(32-s));  
         return t4;  
     };  
   
     function lsb_hex(val) {  
         var str="";  
         var i;  
         var vh;  
         var vl;  
   
         for( i=0; i<=6; i+=2 ) {  
             vh = (val>>>(i*4+4))&0x0f;  
             vl = (val>>>(i*4))&0x0f;  
             str += vh.toString(16) + vl.toString(16);  
         }  
         return str;  
     };  
   
     function cvt_hex(val) {  
         var str="";  
         var i;  
         var v;  
   
         for( i=7; i>=0; i-- ) {  
             v = (val>>>(i*4))&0x0f;  
             str += v.toString(16);  
         }  
         return str;  
     };  
   
   
     function Utf8Encode(string) {  
         string = string.replace(/\r\n/g,"\n");  
         var utftext = "";  
   
         for (var n = 0; n < string.length; n++) {  
   
             var c = string.charCodeAt(n);  
   
             if (c < 128) {  
                 utftext += String.fromCharCode(c);  
             }  
             else if((c > 127) && (c < 2048)) {  
                 utftext += String.fromCharCode((c >> 6) | 192);  
                 utftext += String.fromCharCode((c & 63) | 128);  
             }  
             else {  
                 utftext += String.fromCharCode((c >> 12) | 224);  
                 utftext += String.fromCharCode(((c >> 6) & 63) | 128);  
                 utftext += String.fromCharCode((c & 63) | 128);  
             }  
   
         }  
   
         return utftext;  
     };  
   
     var blockstart;  
     var i, j;  
     var W = new Array(80);  
     var H0 = 0x67452301;  
     var H1 = 0xEFCDAB89;  
     var H2 = 0x98BADCFE;  
     var H3 = 0x10325476;  
     var H4 = 0xC3D2E1F0;  
     var A, B, C, D, E;  
     var temp;  
   
     msg = Utf8Encode(msg);  
   
     var msg_len = msg.length;  
   
     var word_array = new Array();  
     for( i=0; i<msg_len-3; i+=4 ) {  
         j = msg.charCodeAt(i)<<24 | msg.charCodeAt(i+1)<<16 |  
         msg.charCodeAt(i+2)<<8 | msg.charCodeAt(i+3);  
         word_array.push( j );  
     }  
   
     switch( msg_len % 4 ) {  
         case 0:  
             i = 0x080000000;  
         break;  
         case 1:  
             i = msg.charCodeAt(msg_len-1)<<24 | 0x0800000;  
         break;  
   
         case 2:  
             i = msg.charCodeAt(msg_len-2)<<24 | msg.charCodeAt(msg_len-1)<<16 | 0x08000;  
         break;  
   
         case 3:  
             i = msg.charCodeAt(msg_len-3)<<24 | msg.charCodeAt(msg_len-2)<<16 | msg.charCodeAt(msg_len-1)<<8  | 0x80;  
         break;  
     }  
   
     word_array.push( i );  
   
     while( (word_array.length % 16) != 14 ) word_array.push( 0 );  
   
     word_array.push( msg_len>>>29 );  
     word_array.push( (msg_len<<3)&0x0ffffffff );  
   
   
     for ( blockstart=0; blockstart<word_array.length; blockstart+=16 ) {  
   
         for( i=0; i<16; i++ ) W[i] = word_array[blockstart+i];  
         for( i=16; i<=79; i++ ) W[i] = rotate_left(W[i-3] ^ W[i-8] ^ W[i-14] ^ W[i-16], 1);  
   
         A = H0;  
         B = H1;  
         C = H2;  
         D = H3;  
         E = H4;  
   
         for( i= 0; i<=19; i++ ) {  
             temp = (rotate_left(A,5) + ((B&C) | (~B&D)) + E + W[i] + 0x5A827999) & 0x0ffffffff;  
             E = D;  
             D = C;  
             C = rotate_left(B,30);  
             B = A;  
             A = temp;  
         }  
   
         for( i=20; i<=39; i++ ) {  
             temp = (rotate_left(A,5) + (B ^ C ^ D) + E + W[i] + 0x6ED9EBA1) & 0x0ffffffff;  
             E = D;  
             D = C;  
             C = rotate_left(B,30);  
             B = A;  
             A = temp;  
         }  
   
         for( i=40; i<=59; i++ ) {  
             temp = (rotate_left(A,5) + ((B&C) | (B&D) | (C&D)) + E + W[i] + 0x8F1BBCDC) & 0x0ffffffff;  
             E = D;  
             D = C;  
             C = rotate_left(B,30);  
             B = A;  
             A = temp;  
         }  
   
         for( i=60; i<=79; i++ ) {  
             temp = (rotate_left(A,5) + (B ^ C ^ D) + E + W[i] + 0xCA62C1D6) & 0x0ffffffff;  
             E = D;  
             D = C;  
             C = rotate_left(B,30);  
             B = A;  
             A = temp;  
         }  
   
         H0 = (H0 + A) & 0x0ffffffff;  
         H1 = (H1 + B) & 0x0ffffffff;  
         H2 = (H2 + C) & 0x0ffffffff;  
         H3 = (H3 + D) & 0x0ffffffff;  
         H4 = (H4 + E) & 0x0ffffffff;  
   
     }  
   
     var temp = cvt_hex(H0) + cvt_hex(H1) + cvt_hex(H2) + cvt_hex(H3) + cvt_hex(H4);  
   
     return temp.toLowerCase();  
  
}  
		-->
		</script>
		</head>
		<body bgcolor="#BEC8FD"><p>';
 
// if debug flag is set, print the following info
if ($settings['debug'] > 0) {
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

if(isset($_POST['submit'])) {
	$clean['submit'] = $_POST['submit'];

	if(isset($_POST['loginname'])) {
		if( ctype_alnum( $_POST['loginname'])) {
			$clean['loginname'] = $_POST['loginname'];
		}
	}
	if(isset($_POST['fullname'])) {
		if( ctype_print($_POST['fullname'])) {
			$clean['fullname'] = $_POST['fullname'];
		}
	}
	if(isset($_POST['password'])){
		$clean['password'] = $_POST['password']; 
	}
	if(isset($_POST['isAdmin'])) {
		if(strtoupper($_POST['isAdmin']) == "ON") {
			$clean['isAdmin'] = 1;
		}
		else {
			$clean['isAdmin'] = 0;
		}
	}
}
else if(isset($_POST['edit'])) {
	$clean['edit'] = $_POST['edit'];

	if(isset($_POST['password'])){
		if( strlen($_POST['password']) > 0) {
			$clean['password'] = $_POST['password'];
		} 
	}
}

if(isset($_POST['operatorid_edit'])){
	if( ctype_digit($_POST['operatorid_edit'])) {
		$clean['operatorid_edit'] = $_POST['operatorid_edit'];
	}
}

if(isset($_SESSION['operatorid'])){
	if(ctype_digit($_SESSION['operatorid'])) {
		$clean['operatorid'] = $_SESSION['operatorid'];
	}
}
if($settings['debug'] == 1){
	print "<b>\$clean:</b><br>";
	print_r( $clean);
	print "<p>";
}
/*
 * End of filtering input
 */
 
if ($clean['submit']) {
	if ($clean['operatorid_edit']) {

		$dbConnect = dbconnect();
		
		$sql = 'UPDATE operators SET loginname="' . $clean['loginname'] . '",
				fullname="' . $clean['fullname'] . '",';
		
		if( strlen($clean['password']) > 0) {
			$sql .= 'saltypwd=SHA1("' . $clean['password'] . getTheSalt() . '"),';
		}
		
		$sql .= 'isAdmin="' . $clean['isAdmin'] . '",
				modified=NOW(),
				modifiedby="' . $clean['operatorid'] . '" 
				WHERE operatorid="' . $clean['operatorid_edit'] . '"';

		$result = mysql_query( $sql,$dbConnect);
		if ( !$result) {
			$message  = 'Invalid query: ' . mysql_error() . '<br>' . 'Query: ' . $sql;
			die($message);
		}
		else {
			print '<b>Operator ' . $clean['loginname'] . ' (' . $clean['fullname'] . ') changed!</b><p>
					<a href="' . $_SERVER['PHP_SELF'] . '">Edit another operator</a><p>';
		}
		
		dbclose( $dbConnect);

	}
}
else  if( $clean['edit']){
	if ( $clean['operatorid_edit']) {
		print '<div align="left">
			<table>
			<form method="post" action="' . $_SERVER['PHP_SELF'] . '" onsubmit="return(this.password = SHA1(this.password + \"' . getTheSalt() . '\"));">
			<tr><td>Operator\'s login name: </td>
				<td><input name="loginname" type="text" size="20" maxlength="64" value="' . getOperatorLoginName($clean['operatorid_edit']) .'" /></td></tr>
			<tr><td>Operator\'s full name: </td>
				<td><input name="fullname" type="text" size="20" maxlength="64" value="' . getOperatorName($clean['operatorid_edit']) .'"/></td></tr>
			<tr><td>Password: </td>
				<td><input name="password" type="password" size="20" maxlength="64" /></td></tr>
			<tr><td>Administrator: </td>';
		
		if(isOperatorAdmin($clean['operatorid_edit'])) {
			print '<td><input type="checkbox" name="isAdmin" size="5" maxlength="5" checked></td>';
		}
		else {
			print '<td><input type="checkbox" name="isAdmin" size="5" maxlength="5"></td>';
		}
		
		print '<td><<<< Check the checkbox if you want to give the operator administrator privileges</td></tr>
			<tr><td><input name="submit" type="submit" value="Submit" /></td></tr>
			<input type="hidden" name="operatorid_edit" value="' . $clean['operatorid_edit'] . '" />
			</form></div></tr></td></table>';
	}
}
else {
	if ( $clean['operatorid']) {
		// pull the list of operators
		$operators = array();
		
		if( getOperators( $operators)) {
			print '<form method="post" action="' . $_SERVER['PHP_SELF'] . '">
					<font face="Verdana, Arial, Helvetica, sans-serif">
					<div align="left">
					<table>';
	
			print '<tr><td><select name="operatorid_edit">';
			
			foreach ( $operators as $oid => $value) {
				print '<option value="' . $oid . '">'
					  . $value . ' (' . $oid . ')' . 
					  '</option>';
			
			}
	
			print '</select></td></tr>';
			print '<tr><td><font face="Verdana, Arial, Helvetica, sans-serif">
					<input type="Submit" name="edit" value="Edit Operator">
					</font>
					</div></td></tr>';
			
			print '</table></form>';
		}
	}

}

print '</body></html>';

?>