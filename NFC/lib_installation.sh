#!/bin/bash

EXCHANGE_DIR=/var/data_exchange/
EXCHANGE_FILE=/var/data_exchange/to_server.txt

#chemin du compilateur croisé
CC=/root/Bureau/RPI/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc

echo "Compilation de la librarie bcm2835 de façon indépendante"
cd lib/bcm2835-1.64
pwd
./configure --host=x86_64-build_unknown-linux-gnu --target=arm-linux-gnueabihf
make
make install

echo "Compilation de la librarie rc522 et du programme getUID (futur nfcd)"
make

echo "Creation des fichiers de communication avec le serveur local"
mkdir $EXCHANGE_DIR
touch $EXCHANGE_FILE

