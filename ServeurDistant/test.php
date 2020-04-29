<?php
include("config.php");

$code=1;
$dbConnection = mysqli_connect($BDD_host, $BDD_user, $BDD_password, $BDD_base);
$stmt = $dbConnection->prepare('SELECT locked FROM doors WHERE code = ?');
    $stmt->bind_param('i', $code); 
    $stmt->execute();
    $result = $stmt->get_result();
    $row = $result->fetch_assoc();
    $stmt->close();
    if ($row != null) {
        var_dump($row) ;
    }
    echo null;
?>