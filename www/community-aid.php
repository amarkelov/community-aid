<?php
session_start();
require_once("login.inc");

$settings = get_ca_settings();

// Page Header ...
printHeader( "Main menu", 0);

if ($settings['debug'] > 0) {
	require_once("functions.inc");
	print_debug( $clean, $settings);
}

// START LOG IN CODE
$doWeExit = displayLogin(basename($_SERVER['PHP_SELF']), false);
if($doWeExit == true){
	exit;
}
// END LOG IN CODE

?>
<font face="verdana, arial, helvetica" size="2">
<table frame="border">
<tr>
	<td valign="top"><a href='calls.php'>Calls</a></td>
	<td valign="top">Calls is the main operator's toolbox to log calls to/from client(s)</td>
</tr>
<tr>
	<?php if(checkIsAdmin($_SESSION['s_username'])){?>
	<td valign="top"><a href='add_client.php'>Add Client</a></td>
	<td valign="top">Add new client to the system</td>
</tr>
<tr>
	<td valign="top"><a href='edit_client.php'>Edit Client</a></td>
	<td valign="top">Edit existing client</td>
</tr>
<tr>
	<td valign="top"><a href='delete_client.php'>De-activate Client</a></td>
	<td valign="top">Marks client as 'inactive'. Clients are never deleted from database. 
		Once marked 'inactive' clients do not appearing on operators lists.</td>
</tr>
<tr>
	<td valign="top"><a href='restore_client.php'>Re-activate Client</a></td>
	<td valign="top">Marks client 'active'. The client now can be assigned to operator(s) again.</td>
</tr>
<tr>
	<td valign="top"><a href='add_edit_groups.php'>Add/Edit Group names</a></td>
	<td valign="top">Clients can be grouped together. One client can only belong to one group.</td>
</tr>
<tr>
	<td valign="top"><a href='move_clients.php'>Move clients</a></td>
	<td valign="top">Move clients from one group list to another.</td>
</tr>
<tr>
	<td valign="top"><a href='print_group.php'>Print Group list</a></td>
	<td valign="top">Print out the list of clients in the group.</td>
</tr>
<tr>
	<td valign="top"><a href='operator2clients.php'>Assign Client(s) to Operator</a></td>
	<td valign="top">Before a client can be called she/he must be assigned to an operator</td>
</tr>
<tr>
	<td valign="top"><a href='add_operator.php'>Add Operators</a></td>
	<td valign="top">Add new operator to the system</td>
</tr>
<tr>
	<td valign="top"><a href='edit_operator.php'>Edit Operators</a></td>
	<td valign="top">Edit operator's name, login name, change password, add administrator privileges</td>
</tr>
<tr>
	<td valign="top"><a href='delete_operator.php'>Delete Operators</a></td>
	<td valign="top">Delete operator from the system</td>
</tr>
<tr>
	<td valign="top"><a href='add_edit_districts.php'>Add/Edit Districts</a></td>
	<td valign="top">District name is a mandatory field in every client record. 
	Make sure you have added all the districts you have in your area.</td>
</tr>
<tr>
	<td valign="top"><a href='class.php'>Classify calls</a></td>
	<td valign="top">You can give classification to every call on the system. For example "anti-social behaviour", or
	"health problem". The classification is 2 layered system. Operators see only Layer 1 classification
	and Administrator can see Level 1 and Level 2 classifications.</td>
</tr>
<tr>
	<td valign="top"><a href='report.php'>Report</a></td>
	<td valign="top">Create call reports by Classification, date, client name (no names are displayed in the resulted report!) and district</td>
</tr>
<tr>
	<td valign="top"><a href='config.php'>System configuration</a></td>
	<td valign="top">Change system configuration parameters (i.e. page titles, database name, etc.). Please, make sure you know what you are doing
	before you change any parameters on this screen! It's easy to render the system unusable, if parameters set wrong.</td>
</tr>
<tr>
	<td valign="top"><a href='backup.php'>Quick Database backup</a></td>
	<td valign="top">You can make a quick one-off backup of the database.</td>
	<?php }?>
</tr>
</table>
</font>

</body>
</html>
