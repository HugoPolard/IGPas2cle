all: lib getUID read_rpi

lib: src/main.c src/config.c src/rc522.c src/rfid.c
	gcc -c src/main.c -o src/main.o
	gcc -c src/rc522.c -o src/rc522.o
	gcc -c src/value.c -o src/value.o
	gcc -c src/config.c -o src/config.o
	gcc -c src/rfid.c -o src/rfid.o

getUID: src/getUID.c
	@echo "Compilation de getUID"
	gcc -c src/getUID.c -o src/getUID.o
	@echo "Edition des liens de getUID"
	gcc src/getUID.o src/main.o src/rfid.o src/rc522.o src/config.o src/value.o -o getUID -lbcm2835

TARGET_LIBS=/root/Bureau/IGPas2cle/NFC/c/lib/rpi_libs
# options pour la précompilation 
CFLAGS=-I$(TARGET_LIBS)
# options pour l'édition de liens 
LDFLAGS=-L$(TARGET_LIBS)
#chemin du compilateur croisé 
CC=/root/Bureau/RPI/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc

read_rpi: src/read.c 
	$(CC) $(CPPFLAGS) $(LDFLAGS) src/read.c -o bin/read

read_static: read.c 
	$(CC) -static $(CPPFLAGS) $(LDFLAGS) src/read.c -o read

clean: 
	rm -rf src/*.o
