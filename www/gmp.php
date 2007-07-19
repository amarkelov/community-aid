<?php   session_start(); ?><html>
<head>
<title>Good Morning Project</title>
<script language="JavaScript">

var sParams = "toolbar=no,menubar=no,resizable=yes,height=600,width=800";
var sURL ;
var sWindowName = "GMP";

function openWin()
{
var newWindow = window.open(sURL, sWindowName, sParams);
newWindow.focus();
}
</script>
<style>
body {
	background-color: #BEC8FD;
}
</style>
</head>
 <?php  
 	require("functions.inc");
	
	$settings = get_gmp_settings();
	
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
<body onLoad="javascript:window.name = 'GMP';">
<button name="swap" onClick="javascript: sURL = 'calls.php'; openWin();" > Calls </button>
<?php if(checkIsAdmin($_SESSION['s_username'])){?>
<button name="swap" onClick="javascript: sURL = 'add_client.php'; openWin();" > Add Client </button>
<button name="swap" onClick="javascript: sURL = 'edit_client.php'; openWin();" > Edit Client </button>
<button name="swap" onClick="javascript: sURL = 'delete_client.php'; openWin();" > De-activate Client </button>
<button name="swap" onClick="javascript: sURL = 'restore_client.php'; openWin();" > Re-activate Client </button>
<button name="swap" onClick="javascript: sURL = 'operator2clients.php'; openWin();" > Assign Client(s) to Operator </button>
<button name="swap" onClick="javascript: sURL = 'add_operator.php'; openWin();" > Add Operators </button>
<button name="swap" onClick="javascript: sURL = 'edit_operator.php'; openWin();" > Edit Operators </button>
<button name="swap" onClick="javascript: sURL = 'delete_operator.php'; openWin();" > Delete Operators </button>
<button name="swap" onClick="javascript: sURL = 'report.php'; openWin();" > Report </button>
<?php }?>
</body>
</html>
