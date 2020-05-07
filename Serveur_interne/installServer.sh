#!/bin/bash

touch .env
cat << EOF > .env
EXCHANGE_FILE=to_server.txt
EXCHANGE_DIR=/tmp/data_exchange/
#Valeur par défaut, à changer
SERVER_URL=http://localhost/projet/index.php
EOF

echo "vous pouvez modifier l'adresse du serveur distant dans le fichier .env"