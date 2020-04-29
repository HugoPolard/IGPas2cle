<?php

// Database connection
$dbConnection = null;
$BDD_host="localhost";
$BDD_user="root";
$BDD_password="mysql";
$BDD_base="IGpas2cles";

// Server variables
$logfilepath = "logs/server.log";	// log file for server actions/errors
$entriesfilepath = "logs/entries.log";	// log file keeping infos on who passed the door
$uploaddir = realpath('./') . '/pictures/';		// local directory to put aploaded pictures in
$rpiServerAddress = "192.168.0.15";	// rpi ip address
$rpiPort = "10000";	// rpi server port
$rpiFile = "door";	//speceifc file to send requests to for the rpi server

?>
