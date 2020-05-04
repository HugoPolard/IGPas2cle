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


if (isset($_POST['locked'],$_POST['code']){
	$locked = $_POST['locked'];
	switch ($locked){
		case "true" : update_locked(dbConnection,1,$_POST['code']);
		case "false": update_locked(dbConnection,0,$_POST['code']);
		default : echo "j'en veux pas";
	}
} else {
	$arr["locked"] =  get_door_locked($dbConnection, $_POST['code']) 
	echo json_encode($arr);
}
