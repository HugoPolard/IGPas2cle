#!/bin/bash

# Script d'installation automatique de la partie NFC du projet IGpas2cle

echo "[+] Modification de la configuration des ports du raspberry pour utiliser le bus SPI"
tee /boot/config.txt << EOM
device_tree_param=spi=on
dtoverlay=spi-bcm2708
EOM



echo "[+] Copie des librairies rc522 et bcm2835 et de l'exécutable nfcd"
cd ./bin
cp librc522.so /usr/lib
cp libbcm2835.a /usr/lib
cp nfcd /usr/bin/nfcd

cd ../etc

echo "[+] Ajout de nfcd aux services du systeme"
cp nfcd.service /etc/systemd/system/nfcd.service
systemctl enable nfcd
systemctl start nfcd

cp RC522.conf /etc/
echo "[+] Vous allez désormais configurerla librairie RC522 (press enter)"
read nothing
nano /etc/RC522.conf

cp nfcd.conf /etc/
echo "[+] Vous allez désormais configurer nfcd (press enter)"
read nothing
nano /etc/nfcd.conf

echo "[+] Installation de la partie NFC terminée, nécessite un redémarrage"
