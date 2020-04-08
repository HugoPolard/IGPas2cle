#!/bin/bash

#chemin du compilateur croisé
CC=/root/Bureau/RPI/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc

if [ ! -z $1 ]; then
	PATH=$1
else
	echo "./configure <destination_path>"
	exit
fi

echo "Compilation de la librarie bcm2835"
cd lib/bcm2835-1.56
pwd
./configure --program-prefix=$PATH CC=$CC

#./configure -program-prefix=../rpi_libs CC=/root/Bureau/RPI/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc --host=x86_64-build_unknown-linux-gnu --target=arm-linux-gnueabihf