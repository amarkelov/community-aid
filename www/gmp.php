<html>
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
</head>
<body onload="javascript:window.name = 'GMP';">
<button name="swap" onclick="javascript: sURL = 'http:/calls.php'; openWin();" > Calls </button>
<button name="swap" onclick="javascript: sURL = 'http:/check_calls.php'; openWin();" > Check Calls </button>
<button name="swap" onclick="javascript: sURL = 'http:/lists.php'; openWin();" >Print Lists </button>
<button name="swap" onclick="javascript: sURL = 'http:/edit_client.php'; openWin();" > Edit/Add Client </button>
</body>
</html>
