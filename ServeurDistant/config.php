<?php

// Database connection
$dbConnection = null;
$BDD_host="localhost";
$BDD_user="root";
$BDD_password="";
$BDD_base="igpas2cles";

// Server variables
$logfilepath = "logs/server.log";	// log file for server actions/errors
$entriesfile = "logs/entries.log";	// log file keeping infos on who passed the door
$uploaddir = realpath('./') . '/pictures/';		// local directory to put aploaded pictures in
$rpiServerAddress = "127.0.0.1";	// rpi ip address
$rpiFile = "test";	//speceifc file to send requests to for the rpi server

?>
