<?php
session_start();
require 'functions.inc';

$settings = get_gmp_settings();
?>


<html>
<head>
<title> Report -- Good Morning <?php echo $settings['location'] ?></TITLE>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>


<body bgcolor="#BEC8FD" link="#0000FF" vlink="#0000FF" alink="#0000FF">
<font face="Verdana" size="2">
<?php 
	if ($settings['debug'] > 0) {
		print_debug( $clean, $settings);
	}

	// START LOG IN CODE
		$doWeExit = displayLogin(basename($_SERVER['PHP_SELF']), false);
		if($doWeExit == true){
			exit;
		}
	// END LOG IN CODE
?>

<table>
<tr>
	<td><a href='calls.php'>Calls</a></td>
	<td>Calls is the main operator's toolbox to log calls to/from client(s)</td>
</tr>
<tr>
	<?php if(checkIsAdmin($_SESSION['s_username'])){?>
	<td><a href='add_client.php'>Add Client</a></td>
	<td>Add new client to the system</td>
</tr>
<tr>
	<td><a href='edit_client.php'>Edit Client</a></td>
	<td>Edit existing client</td>
</tr>
<tr>
	<td><a href='delete_client.php'>De-activate Client</a></td>
	<td>Marks client as 'inactive'. Clients are never deleted from database. 
		Once marked 'inactive' clients do not appearing on operators lists.</td>
</tr>
<tr>
	<td><a href='restore_client.php'>Re-activate Client</a></td>
	<td>Marks client 'active'. The client now can be assigned to operator(s) again.</td>
</tr>
<tr>
	<td><a href='operator2clients.php'>Assign Client(s) to Operator</a></td>
	<td>Before a client can be called she/he must be assigned to an operator</td>
</tr>
<tr>
	<td><a href='add_operator.php'>Add Operators</a></td>
	<td>Add new operator to the system</td>
</tr>
<tr>
	<td><a href='edit_operator.php'>Edit Operators</a></td>
	<td>Edit operator's name, login name, change password, add administrator privileges</td>
</tr>
<tr>
	<td><a href='delete_operator.php'>Delete Operators</a></td>
	<td>Delete operator from the system</td>
</tr>
<tr>
	<td><a href='report.php'>Report</a></td>
	<td>Create call reports by Classification, date, client name (no names are displayed in the resulted report!) and district</td>
	<?php }?>
</tr>
</table>

</body>
</html>
