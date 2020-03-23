#!/bin/bash

# Script d'installation automatique de la partie NFC du projet IGpas2cle

tee /boot/config.txt << EOM
device_tree_param=spi=on
dtoverlay=spi-bcm2708
EOM

echo "Veuillez activer le bus SPI (5.Interfacing options => SPI enabled)"
read nothing
raspi-config

echo "N'oubliez pas de descendre tous les interrupteurs de la malette en position OFF"
read nothing

echo "Installation de la librairie c rfid-rc522..."
echo "Installation de la dépendance bc2835"
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.56.tar.gz
tar -zxf bcm2835-1.56.tar.gz
cd bcm2835-1.56
./configure
sudo make check
sudo make install
cd  ..
echo "Installation de la librairie"
git clone https://github.com/paulvha/rfid-rc522.git
cd rfid-rc522
tar -zxvf rc522-1-6-5.tar.gz
cd rc522
./mc
sudo cp RC522.conf   /etc
sudo chmod 666 /etc/RC522.conf
echo "Now put your own options in the RC522 library conf file"
read nothing
nano /etc/RC522.conf

echo "NFC installation DONE, need reboot"
