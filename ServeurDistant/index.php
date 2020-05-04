<?php

include("config.php");

function get_user_code_by_UID($dbConnection, $uid) {
    $stmt = $dbConnection->prepare('SELECT code FROM users WHERE uid_carte = ?');
    if ($stmt ==false)
        return -2;
    $stmt->bind_param('s', $uid); // 's' specifies the variable type => 'string'
    $stmt->execute();
    $result = $stmt->get_result();
    $row = $result->fetch_assoc();
    $stmt->close();
    if ($row != null && isset($row["code"])) {
        return $row["code"];
    }
    return -1;
}

function get_user_infos($dbConnection, $code) {
    $stmt = $dbConnection->prepare('SELECT nom, prenom FROM users WHERE code = ?');
    $stmt->bind_param('i', $code); 
    $stmt->execute();
    $result = $stmt->get_result();
    $row = $result->fetch_assoc();
    $stmt->close();
    if ($row != null) {
        return $row;
    }
    return null;
}

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

$door_code = 1;
// Ouveture des fichiers et indication de la requête dans les logs
$logfile = fopen($logfilepath, "a");
$entriesfile = fopen($entriesfilepath, "a");
fprintf($logfile, "[%s] requete reçue\n", date("Y,M,d-h:i:s-A"));

// Connexion à la base de données
$dbConnection = mysqli_connect($BDD_host, $BDD_user, $BDD_password, $BDD_base);
if (!$dbConnection) 
{  // Si la connexion à la base a échoué
    echo"Database connection failed\n";
}


// Check if image file is a actual image or fake image
if(isset($_POST["uid"]) && isset($_FILES["image"])) {

    // Upload the reveived image to the server
    $uploadfile = $uploaddir . basename($_FILES['image']['name']);
    $uploadOk = 1;
    $check = getimagesize($_FILES["image"]["tmp_name"]);
    if($check !== false) {
        $uploadOk = 1;
    } else {
        $error =  "File is not an image";
        $uploadOk = 0;
    }
    // Check if $uploadOk is set to 0 by an error
    if ($uploadOk == 0) {
       fprintf($logfile, "[%s] UPLOAD ERROR : %s\n", date("Y,M,d-h:i:s-A"), $error);
    // if everything is ok, try to upload file
    } else {
        if (move_uploaded_file($_FILES['image']['tmp_name'], $uploadfile)) {
            fprintf($logfile, "[%s] IMAGE UPLOADED : %s\n",  date("Y,M,d-h:i:s-A"), $uploadfile);
        } else {
            fprintf($logfile, "[%s] UPLOAD ERROR : possible file attack\n", date("Y,M,d-h:i:s-A"));
            echo "UPLOAD ERROR";
        }
    }

// Get the user code from the received uid (or nothing if uid not in db)
    $user_code = get_user_code_by_UID($dbConnection, $_POST["uid"]);
    $door_locked = get_door_locked($dbConnection, $door_code);
    if ($user_code !== -1 && $door_locked === 0) {
        // Get user first and last name form database via user code
        if ($user_infos = get_user_infos($dbConnection, $user_code)) {
            //Request door opening via rpi server
            $response = file_get_contents("http://".$rpiServerAddress.":".$rpiPort."/".$rpiFile."?open=true");
	        if ($response === "") { // Réponse http valide pour l'ouverture de la porte
               fprintf($entriesfile, "[%s];acces=true;nom=%s;prenom=%s; picture=%s\n", date("Y,M,d-h:i:s-A"), $user_infos["nom"], $user_infos["prenom"], $uploadfile);
            }
            else {
                fprintf($logfile, "[%s] ACCES ERROR : request to door failed\n", date("Y,M,d-h:i:s-A"));
            }
        } 
        else {    // problème interne au serveur pour avoir les infos utilisateurs
            fprintf($logfile, "[%s] DB REQUEST ERROR while getting user infos\n", date("Y,M,d-h:i:s-A"));
        }
    } 
    else {    // L'uid n'est pas en base, accès refusé
        $response = file_get_contents("http://".$rpiServerAddress.":".$rpiPort."/".$rpiFile."?open=false");
            if ($response === "") { // Réponse http valide pour l'ouverture de la porte
            fprintf($entriesfile, "[%s];acces=false;uid=%s;picture=%s\n", date("Y,M,d-h:i:s-A"), $_POST["uid"], $uploadfile);
        }
        else {
            fprintf($logfile, "[%s] ACCES ERROR : request to door failed\n", date("Y,M,d-h:i:s-A"));
        }
    }
} else {    
    fprintf($logfile, "[%s] REQUEST ERROR : field missing\n", date("Y,M,d-h:i:s-A"));
    echo "REQUEST ERROR : field missing";
}

mysqli_close($dbConnection);

?>
