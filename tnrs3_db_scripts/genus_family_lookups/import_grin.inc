<?php

///////////////////////////////////////////////////////////////////
// Purpose:
// Download GRIN taxonomy tables and import to new MySQL database
// These tables used by separate scripts to build family-genus 
// lookup tables
///////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////
// Get parameters
// Everything you need to set is in param.inc, and the function
// and utilities it calls.
///////////////////////////////////////////////////////////////////

include "params.inc";

///////////////////////////////////////////////////////////////////
// Confirm operation
///////////////////////////////////////////////////////////////////

// Confirm with user before starting
if (confirm($confmsg));
if (confirm($confmsg2)) {
	include $timer_on;	// start timer
	echo "\r\nBegin operation\r\n\r\n";
}

///////////////////////////////////////////////////////////////////
// Download and unzip files from ftp site
// Convert from dbase to plain text
///////////////////////////////////////////////////////////////////

include_once "download.inc";

///////////////////////////////////////////////////////////////////
// Connect to host
///////////////////////////////////////////////////////////////////

$dbh = mysql_connect($HOST,$USER,$PWD);
if (!$dbh) die("Could not connect to host $HOST: " . mysql_error());

///////////////////////////////////////////////////////////////////
// Create database, dropping previous database if exists
// This is why I used the over-specific variable name DB_GRIN
// To dangerous to use the generic 'DB'
///////////////////////////////////////////////////////////////////

include_once "create_database.inc";

///////////////////////////////////////////////////////////////////
// Reconnect to host, specify database
///////////////////////////////////////////////////////////////////

mysql_close($dbh);
if (!($dbh=db_connect($HOST,$USER,$PWD,$DB_NEW,$echo_on))) die();

///////////////////////////////////////////////////////////////////
// Create tables
///////////////////////////////////////////////////////////////////

// Create tables
echo "Creating tables...";
include_once "sql_create_grin_taxonomy_tables.inc";
if (sql_execute_multiple($sql_create_tables));
echo $msg_success;

///////////////////////////////////////////////////////////////////
// Load data from cvs files to raw data tables in new 
// MySQL database
///////////////////////////////////////////////////////////////////

// Import data from dump files into the raw tables
include_once "import.inc";

///////////////////////////////////////////////////////////////////
// Close connection and report execution time
///////////////////////////////////////////////////////////////////

mysql_close($dbh);
include_once $timer_off;
$msg = "\r\nTime elapsed: " . $tsecs . " seconds.\r\n"; 
$msg = $msg . "********* Operation completed " . $curr_time . " *********";
if  ($echo_on) echo $msg . "\r\n\r\n"; 

?>
