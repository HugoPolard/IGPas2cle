<?php

include('config.php');

function get_door_locked($dbConnection, $code) {
    $stmt = $dbConnection->prepare('SELECT locked FROM doors WHERE code = ?');
    $stmt->bind_param('i', $code); 
    $stmt->execute();
    $result = $stmt->get_result();
    $row = $result->fetch_assoc();
    $stmt->close();
    if ($row != null && isset($row["locked"])) {
        return $row["locked"];
    }
    return null;
}


function update_locked($dbConnection,$locked,$code){
	$statm = $dbConnection->prepare("UPDATE doors SET locked = ? WHERE code = ?");
	$statm->bind_param('ii',$locked,$code);
	$statm->execute();
	if ($statm->affected_rows > 0){
		return true;
	}else {
		return false;
	}
	
}


if (isset($_POST['locked'],$_POST['code'])){
	// Connexion à la base de données
	$dbConnection = mysqli_connect($BDD_host, $BDD_user, $BDD_password, $BDD_base);
	if (!$dbConnection) 
	{  // Si la connexion à la base a échoué
	    echo"Database connection failed\n";
	}
	$locked = $_POST['locked'];
	var_dump($locked);
	switch ($locked){
		case "true" : update_locked($dbConnection,1,$_POST['code']);
		break;
		case "false": update_locked($dbConnection,0,$_POST['code']);
		break;
		default : echo "param locked non reconnu";
		break;
	}
} else if (isset($_GET['code'])){
	// Connexion à la base de données
	$dbConnection = mysqli_connect($BDD_host, $BDD_user, $BDD_password, $BDD_base);
	if (!$dbConnection) 
	{  // Si la connexion à la base a échoué
	    echo"Database connection failed\n";
	}
	$arr["locked"] =  get_door_locked($dbConnection, $_GET['code']);
	echo json_encode($arr);
}
