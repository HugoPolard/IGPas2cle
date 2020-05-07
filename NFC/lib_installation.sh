#!/bin/bash

# Script de compilation de la partie NFC du projet IGpas2cles

echo "Veuillez activer le bus SPI (5.Interfacing options => SPI enabled)"
read nothing
raspi-config

echo "N'oubliez pas de descendre tous les interrupteurs de la malette en position OFF"
read nothing

echo "Installation de la librairie c rfid-rc522..."
echo "Installation de la dépendance bc2835"
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.56.tar.gz
tar -zxf bcm2835-1.56.tar.gz
cd lib/bcm2835-1.64
#Si on avait une compilation croisée : ./configure --host=x86_64-build_unknown-linux-gnu --target=arm-linux-gnueabihf
./configure
sudo make check
sudo make install
cd  ../..

echo "Compilation et installation de la librairie rc522"
make librc522
make librc522_install
sudo cp etc/RC522.conf   /etc
sudo chmod 666 /etc/RC522.conf
echo "Modifiez le fichier de configuration comme vous en avez besoin"
read nothing
nano /etc/RC522.conf

echo "Compilation et installation du programme nfcd"
make nfcd
make nfcd_install
