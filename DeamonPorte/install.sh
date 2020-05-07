#! /bin/bash


if [ $(whoami) !="root"]
then
	echo "Must be root"
	exit 1
fi

cp doorD /usr/bin/dord
cp doorD.service /etc/systemd/service
systemctl enable doorD.service
systemctl start doorD.service
