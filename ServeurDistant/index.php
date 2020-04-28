<?php

include("config.php");

function get_user_code_by_UID($uid) {
    $stmt = $dbConnection->prepare('SELECT code FROM users WHERE uid = ?');
    $stmt->bind_param('s', $uid); // 's' specifies the variable type => 'string'
    $stmt->execute();
    $result = $stmt->get_result();
    $row = $result->fetch_assoc();
    $stmt->close();
    if (isset($row[0])) {
        return $row[0];
    }
    return -1;
}

function get_user_infos($uid) {
    $stmt = $dbConnection->prepare('SELECT last_name, first_name FROM users WHERE code = ?');
    $stmt->bind_param('s', $uid); // 's' specifies the variable type => 'string'
    $stmt->execute();
    $result = $stmt->get_result();
    $row = $result->fetch_assoc();
    $stmt->close();
    if (isset($row[0])) {
        return $row[0];
    }
    return null;
}

// Connexion à la base de données et ouverture des fichiers de logs
$dbConnection = mysqli_connect($BDD_host, $BDD_user, $BDD_password, $BDD_base);
$logfile = fopen($logfilepath, "a");
$entriesfile = fopen($logfilepath, "a");

// Check if image file is a actual image or fake image
if(isset($_POST["uid"]) && isset($_FILES["image"])) {

    // Upload the reveived image to the server
    $uploadfile = $uploaddir . basename($_FILES['image']['name']);
    $uploadOk = 1;
    $imageFileType = strtolower(pathinfo($target_file,PATHINFO_EXTENSION));
    $check = getimagesize($_FILES["image"]["tmp_name"]);
    if($check !== false) {
        $uploadOk = 1;
    } else {
        $error =  "File is not an image";
        $uploadOk = 0;
    }
    // Only allow pictures to upload
    if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg") {
        $error =  "File is not JPG, JPEG or PNG";
        $uploadOk = 0;
    }
    // Check if $uploadOk is set to 0 by an error
    if ($uploadOk == 0) {
       printf($logfile, "[%s] UPLOAD ERROR : %s", date("Y,M,d-h:i:s-A"), $error); 
    // if everything is ok, try to upload file
    } else {
        if (move_uploaded_file($_FILES['image']['tmp_name'], $uploadfile)) {
            printf($logfile, "[%s] IMAGE UPLOADED : %s",  date("Y,M,d-h:i:s-A"), $uploadfile);
        } else {
            printf($logfile, "[%s] UPLOAD ERROR : possible file attack", date("Y,M,d-h:i:s-A")); 
        }
    }

    // Get the user code from the received uid (or nothing if uid not in db)
    $user_code = get_user_code_by_UID($_POST["uid"]);
    if ($user_code != -1) {
        // Get user first and last name form database via user code
        if ($user_infos = get_user_infos($user_code)) {
            //Request door opening via rpi server
            $response = http_get("http://".$rpiServerAddress."/".$rpiFile."?open=true");
            if ($responses == 200)  // Réponse http valide pour l'ouverture de la porte
               printf($entriesfile, "[%s] nom=%s ; prenom=%s ; picture=%s;", date("Y,M,d-h:i:s-A"), user_infos["last_name"], user_infos["first_name"], $uploadfile);
            else
                printf($logfile, "[%s] ACCES ERROR : request to door failed", date("Y,M,d-h:i:s-A"));
        } else {    // problème interne au serveur pour avoir les infos utilisateurs
            printf($logfile, "[%s] DB REQUEST ERROR while getting user infos", date("Y,M,d-h:i:s-A"));
        }
    } else {    // L'uid n'est pas en base, accès refusé
        printf($logfile, "[%s] DB REQUEST ERROR while getting user code", date("Y,M,d-h:i:s-A"));
    }
} else {
    printf($logfile, "[%s] REQUEST ERROR : request field missing", date("Y,M,d-h:i:s-A"));
}

mysqli_close($dbConnection);

?>