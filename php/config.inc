<?php
require_once("db.inc");
require_once("messages.inc");

define(CONFIG_FILE, "../conf/community-aid.ini");
define(BACKUP_DIR, $_SERVER[DOCUMENT_ROOT] . "/one-off-backups");

function get_ca_settings(){
	$settings = array();
	
	$settings = parse_ini_file(CONFIG_FILE);
	
	$settings['backup_dir'] = BACKUP_DIR;
	
	if(!isset($settings['force_pdf_when_more_than'])) {
		$settings['force_pdf_when_more_than'] = 300;	
	}

	if(!isset($settings['start_page'])) {
		$settings['start_page'] = 'community-aid.php';	
	}
	
	if(isset($_SERVER['Operator'])) {
		$settings['operator'] = $_SERVER['Operator'];
	}
	else {
		$settings['operator'] = 'caoperator';
	}
	
	if(isset($_SERVER['Operator_Password'])) {
		$settings['operator_password'] = $_SERVER['Operator_Password'];
	}
	else {
		$settings['operator_password'] = 'caoperatorpwd';
	}
	
	if(isset($_SERVER['Admin'])) {
		$settings['admin'] = $_SERVER['Admin'];
	}
	else {
		$settings['admin'] = 'caadmin';
	}
	
	if(isset($_SERVER['Admin_Password'])) {
		$settings['admin_password'] = $_SERVER['Admin_Password'];
	}
	else {
		$settings['admin_password'] = 'caadminpwd';
	}
	
	if(!isset($settings['pghost'])) {
		$settings['pghost'] = 'localhost';
	}

	if($settings['calendar_enabled'] == 1) {
		$settings['calendar_enabled'] = true;	
	}
	else {
		$settings['calendar_enabled'] = false;	
	}	

	return $settings;
}

function saveIniFile( $clean, $setDefaults = false) {
	$retval = true;
	$inifile = CONFIG_FILE;
	$newConfig = '';

	// we need to reset everything back to default values
	if($setDefaults) {
		$newConfig = "
org = Friendly Call Service\t\t; header option for organisation name
location = Blanchardstown\t\t; header option for project location
start_page=community-aid.php\t\t; identifies system's main page

;****************************
; Database related parameters
;****************************
database = community-aid\t\t\t; name of the database
pghost = localhost\t\t\t; host name of your database server
pgport = 5432\t\t\t; port number the database server listens to

;**************************
; PDF parameters
;**************************
pdf_draw_cell_border = 1\t\t; set to 1, if you need cell borders
\t\t\t\t\t; in the PDF report table

force_pdf_when_more_than = 100\t\t; force PDF output, if report results
\t\t\t\t\t; in more than n number of records
;***************************
; debug parameters
;***************************
debug = 0\t\t; debug set to 1 gives debug output at the top of each page.
debug_pdf = 0\t; debug_pdf set to 1 gives debug output at the bottom of
\t\t\t; PDF report.
\t\t\t; when PDF is chosen for report output, make sure that
\t\t\t; debug = 0. otherwise, you will have error message
\t\t\t; 'FPDF error: Some data has already been
\t\t\t; output to browser, can't send PDF file' and won't see
\t\t\t; the PDF report

;debug_sql_limit = 150\t; if you need a limited output, to save yourself
\t\t\t; some time, set this to the number of records
\t\t\t; you want to limit SELECT statement to.\n";	
	}
	else {
		if(isset( $clean['org']) && ctype_print( $clean['org'])) {
			$newConfig .= "org = " . $clean['org'] . "\n";
		}
		if(isset( $clean['location']) && ctype_print( $clean['location'])) {
			$newConfig .= "location = " . $clean['location'] . "\n";
		}
		if(isset( $clean['start_page']) && ctype_print( $clean['start_page'])) {
			$newConfig .= "start_page = " . $clean['start_page'] . "\n";
		}
		if(isset( $clean['database']) && ctype_print( $clean['database'])) {
			$newConfig .= "database = " . $clean['database'] . "\n";
		}
		if(isset( $clean['pghost']) && ctype_print( $clean['pghost'])) {
			$newConfig .= "pghost = " . $clean['pghost'] . "\n";
		}
		if(isset( $clean['pgport']) && ctype_digit( $clean['pgport'])) {
			$newConfig .= "pgport = " . $clean['pgport'] . "\n";
		}
		if(isset( $clean['pdf_draw_cell_border']) && ctype_digit( $clean['pdf_draw_cell_border'])) {
			if($clean['pdf_draw_cell_border'] > 0) {
				$newConfig .= "pdf_draw_cell_border = 1\n";
			}
			else {
				$newConfig .= "pdf_draw_cell_border = 0\n";
			}
		}
		if(isset( $clean['force_pdf_when_more_than']) && ctype_digit( $clean['force_pdf_when_more_than'])) {
			$newConfig .= "force_pdf_when_more_than = " . $clean['force_pdf_when_more_than'] . "\n";
		}
		if(isset( $clean['debug']) && ctype_digit( $clean['debug'])) {
			if($clean['debug'] > 0) {
				$newConfig .= "debug = 1\n";
			}
			else {
				$newConfig .= "debug = 0\n";
			}
		}
		if(isset( $clean['debug_pdf']) && ctype_digit( $clean['debug_pdf'])) {
			if( $clean['debug_pdf'] > 0) {
				$newConfig .= "debug_pdf = 1\n";
			}
			else {
				$newConfig .= "debug_pdf = 0\n";
			}
		}
		if(isset( $clean['debug_sql_limit']) && ctype_digit( $clean['debug_sql_limit'])) {
			$newConfig .= "debug_sql_limit = " . $clean['debug_sql_limit'] . "\n";
		}
	}
	
	// Let's make sure the file exists and is writable first.
	if (is_writable( $inifile)) {
	    if ( !$fh = fopen( $inifile, 'w')) {
	         printErrorMessage('Cannot open file ' . $inifile . '!');
	         $retval = false;
	    }
	
	    // Write $somecontent to our opened file.
	    if ( fwrite( $fh, $newConfig) === FALSE) {
	        printErrorMessage('Cannot write to file ' . $inifile . '!');
	        $retval = false;
	    }

	    fclose( $fh);
	
	} else {
	    printErrorMessage('File ' . $inifile . ' is not writable!');
	    $retval = false;
	}
	
	return $retval;
}

?>