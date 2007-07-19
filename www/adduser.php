<?php   session_start(); 
	$requirePassConfirm = ture; //no checking done yet
	$checkDelete = true;
	$failMessage ="<font color=red><strong>Failed!</ strong></font> "	;

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />
<meta http-equiv="pragma" content="no-cache" />
<title>GMP  --  Add Users</title>
<style type="text/css">
<!--
.style7 {color: #FF0000; font-weight: bold; font-size: small; }
.stylePicLink { border: 0; }
body {
	background-color: #BEC8FD;
}
-->
</style>

</head>

<body>
  <?php
  require("functions.inc");

	// START LOG IN CODE
	$doWeExit = displayLogin(basename($_SERVER['PHP_SELF']), true);
	if($doWeExit == true){
		exit;
	}
	// END LOG IN CODE
	
	function messagebox($success, $title, $description){
		$daBox = '<table width="369" border="0" cellpadding="2">
      			<tr><td align="center"align="middle" bgcolor="#AAAAAA">
				<table width="100%" border="0" cellpadding="1" bgcolor="#FFFFFF"> <tr>';
		if($success){
			$daBox .= '<td width="15%" rowspan="2"></td>
						<td width="92%"><font color=green ><b>Success!</b></font> ';
		}else{
			$daBox .= '<td width="8%" rowspan="2">&nbsp;</td>
						<td width="92%"><font color=red ><b>Failed!</b></font> ';
		}
			$daBox .= $title.'</td></tr>
					  <tr>
						<td>'.$description.'</td>
					  </tr>
					</table></td>
					  </tr>
					</table><br/>';
		echo $daBox;
	}

	$link = dbconnect();
	
	if(isset($_GET['rm'])){
		$tempU=$_GET['rm'];

		if($_SESSION['s_username'] == $tempU){ // Don't allow deleteing of self 
				messagebox(false, "Deleting User: <b>".$tempU ."</b>",
						"You cannot delete yourself!");
		}else{
			if(deleteUser($tempU)){
				messagebox(true, "Deleting User: <b>".$tempU ."</b>",
					 $tempU." user was deleted successfully"); 
			}else{
				messagebox(false, "Deleting User: <b>".$tempU ."</b>",
					"Unkown error.");
			}
		}
	}elseif(isset($_GET['add'])){
		$drawAddTable=true;
		$aoe= "?add";
		if (isset($_POST['Submit'])){
			
			$fname=$_POST['fname'];
			$uname=$_POST['uname'];
			$pass=($_POST['pass']);	
			$pass2=($_POST['pass2']);
				
			if(isset($_POST['isadmin'])){
				$typeofuser = "As an administrator.";
				$isadmin='true';
			}else{
				$isadmin='false';
			}
			if(checkValues($fname, $uname, $pass, $pass2)){
				dbConnect();
				$sql = "SELECT count(*) username FROM gmpdb.users where username = '$uname'";
				$query = mysql_query($sql);
				$data=mysql_fetch_array($query);
				if ($data[0] >= 1){
					//Print error that username is already taken
					messagebox(false, "Adding User",
							"The username <b>$uname</b> already exists.<br />".
							"Please select a new user name.");
				}elseif (addUser($uname, $fname, $pass, $isadmin)){
					//Print Success
					messagebox(true, "Adding User",
							"<b>". $_POST['fname'].
							"</b> has been added to the list of users. ". 
							$typeofuser);
					
					$drawAddTable=false;
					$success = true;
				}else{
					messagebox(true, "Adding User",
						"Something may have gone wrong connecting to the datebase.");
				}
			}
		}
	if($drawAddTable){ ?>	
	<form action="adduser.php<?php echo $aoe; ?>" method="post" name="form1" id="form1" >
	<table width="369" border="0" cellpadding="2">
	  <tr>
		<td align="center" align="middle" bgcolor="#AAAAAA">
		  <table width="100%" border="0" cellpadding="3" cellspacing="0" bgcolor="#EEEEEE">
			<tr>
			  <td colspan="2">
				<div align="center"><b>Add a new user.</b></div>	
			  </td>
			</tr>
			<tr>
			  <td width="45%" ><strong>Full Name:</strong></td>
			  <td width="55%" ><input name="fname" id="fname" type="text" value="<?php echo $fname;?>" />
			  <span class="style7">*</span></td>
			</tr>
			<tr>
			  <td ><strong>User Name: </strong></td>
			  <td ><input type="text" name="uname" value="<?php echo $uname;?>"/>
			  <span class="style7">*</span></td>
			</tr>
			<tr>
			  <td ><strong>Password:</strong></td>
			  <td ><input type="password" name="pass" value=""/>
			  <span class="style7">*</span></td>
			</tr>
			<?php if($requirePassConfirm == true){ ?>
			 <tr>
			  <td ><strong>Confirm Password :</strong></td>
			  <td ><input type="password" name="pass2" value=""/>
				  <span class="style7">*</span></td>
			</tr>
			 <?php }?>
			<tr>
			  <td ><strong>Is admin?</strong></td>
			  <td >
			  <input name="isadmin" type="checkbox" value="true" 
				<?php  if(isset($_POST['isadmin'])){echo 'checked="checked"';} ?>/>	  </td>
			</tr>
			<tr>
			  <td >&nbsp;</td>
			  <td  class="style7">* = Required </td>
			</tr>
			<tr>
			  <td>&nbsp;</td>
			  <td><input type="submit" name="Submit" value="Submit" />
				<input type="reset" name="Reset" value="Cancel" /></td>
			</tr>
		  </table>
	    </td>
	  </tr>
	</table>	
	</form>
	<?php } //ENDOF if($drawAddTable){ 
	}elseif(isset($_GET['edit'])){
		$drawEditTable=true;
		$uname = $_GET['edit'];
		$sql = "SELECT  fullname, isAdmin FROM gmpdb.users where username = '$uname'";
		$query = mysql_query($sql);
		$data=mysql_fetch_array($query);
		
		$fname=trim($data['fullname']); // trim so ' ' not allowed as complete name
		$isadmin=trim($data['isAdmin']);// trim so ' ' not allowed as complete name
		
		if (isset($_POST['Submit'])){
			
			$newfname=$_POST['fname'];
			$newuname=$_POST['uname'];
			
			if(isset($_POST['isadmin'])){
				$newisadmin='true';
			}else{
				$newisadmin='false';
			}

			if($newfname ^ $newuname == ""){
				messagebox(false, "Updating User: <b>".$uname ."</b>",
			 		"You have left a required field blank!<br /> Please fill it in an submit again.");
			}else{
				$sql = "SELECT count(*) username FROM gmpdb.users where username = '$newfname'";
				$query = mysql_query($sql);
				$data=mysql_fetch_array($query);
				if ($data[0] >= 1){
					//Print error that username is already taken
					messagebox(false, "Adding User",
						"The username <b>$uname</b> already exists.<br />".
						"Please select a new user name.");
				}elseif(updateUser($uname, $newuname, $newfname, $newisadmin)){
					messagebox(true, "Updating User: <b>".$uname ."</b>",
					 	$tempU." user was updated successfully"); 
						$drawEditTable=false;
				}else{
					messagebox(false, "Updating User: <b>".$uname ."</b>",
						"Unkown error.");
				}
			}
		}
	if($drawEditTable){
	?>
	<form action="adduser.php?edit=<?php echo $uname;  ?>" method="post" name="form1" id="form1" >
	<table width="369" border="0" cellpadding="2">
	  <tr>
	    <td align="center" align="middle" bgcolor="#AAAAAA">
	      <table width="100%" border="0" cellpadding="3" cellspacing="0" bgcolor="#EEEEEE">
	        <tr>
	          <td colspan="2">
	            <div align="center"><b>Edit user: <?php echo $uname;?></b></div>	
	          </td>
	        </tr>
	        <tr>
	          <td width="45%" ><strong>Full Name:</strong></td>
	          <td width="55%" ><input name="fname" id="fname" type="text" value="<?php echo $fname;?>" />
	            <span class="style7">*</span>
			  </td>
	        </tr>
	        <tr>
	          <td ><strong>User Name: </strong></td>
	          <td ><input type="text" name="uname" value="<?php echo $uname;?>"/>
	            <span class="style7">*</span>
			  </td>
	        </tr>
	          <td ><strong>Is admin?</strong></td>
	          <td >
	            <input name="isadmin" type="checkbox" value="true" 
	            <?php  if($isadmin == "true"){echo 'checked="checked"';} ?>/>	  
			  </td>
	        </tr>
	        <tr>
	          <td >&nbsp;</td>
	          <td  class="style7">* = Cannot be blank. </td>
	        </tr>
	        <tr>
	          <td>&nbsp;</td>
	          <td><input type="submit" name="Submit" value="Submit" />
	            <input type="reset" name="Reset" value="Cancel" />
	          </td>
	        </tr>
	      </table>
	    </td>
	   </tr>
	</table>
	</form>
	<?php } //ENDOF if($drawEditTable)
	} //ENDOF elseif(isset($_GET['edit']))
	?>
    <table width="369" border="0" cellpadding="2">
      <tr>
        <td align="center"align="middle" bgcolor="#AAAAAA">
		<table width="364" border="0" align="left" cellpadding="2" cellspacing="0">
          <tr bgcolor="#999999">
            <td width="100" bgcolor="#999999"><div align="left"><strong>Name</strong></div></td>
            <td width="75" bgcolor="#999999"><div align="left"><strong>Username </strong></div></td>
            <td width="55" bgcolor="#999999"><div align="left"><strong>Admin?</strong></div></td>
            <td width="55" bgcolor="#999999"><div align="left"><strong>Edit</strong></div></td>
            <td width="55" bgcolor="#999999"><div align="left"><strong>Delete</strong></div></td>
          </tr>
          <?php 

 			$link =  dbConnect();
  			$sql = "SELECT fullname, username, isAdmin 
					FROM gmpdb.users 
					WHERE 1
					ORDER BY fullname";
			$query = mysql_query($sql);
			$count=0;
			
			$result = "";
			$thisPage=basename($_SERVER['PHP_SELF']);
			
			while ($row = mysql_fetch_array($query)) {
			extract($row);
			
				if($count%2=='0'){
					$result .='<tr bgcolor="#DDDDDd">';
				}else{
					$result .='<tr bgcolor="#CCCCCC">';
				}
				$count++;
				$result .= '<td >' . $fullname .'</td>';
				$result .= '<td >' . $username .'</td>';
				$temp ="";
				if($isAdmin == "true"){ 
					$temp ='<img src="images/ok.gif" alt="True" width="19" height="17" class="stylePicLink"/>';
				}
				$result .= '<td >' .$temp. '</td>';
				$result .= '<td ><a href='.$thisPage.'?edit='.$username .'>';
				$result .= '<img src="images/b_edit.png" alt="edit" class="stylePicLink"/></a></td>';
				$result .= '<td ><a href='.$thisPage.'?rm='.$username .'>';
				$result .= '<img src="images/b_drop.png" alt="Delete"class="stylePicLink" /></a></td>';
				$result .= '</tr>';
			}
			if($count%2=='0'){
					$result .='<tr bgcolor="#DDDDDd">';
				}else{
					$result .='<tr bgcolor="#CCCCCC">';
				}
            $result .= '<td  colspan="4"><div align="right">Add a new user </div></td>';
            $result .= '<td width="55" ><div align="left"><a href='.$thisPage.'?add >';
			$result .= '<img src="images/b_add.gif" alt="Delete"class="stylePicLink" /></a></div></td></tr>';
			echo $result;
			
			
?>			
        </table></td>
      </tr>
	  
    </table>
    
</body>
</html>
