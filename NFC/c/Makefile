# NOTES
# La compilation de la librairie bcm2835 doit être faite ultérieurement

all: getUID 

############## Variables

LIB_RC522=/root/Bureau/IGPas2cle/NFC/c/lib/rc522
LIB_BCM2835=/root/Bureau/IGPas2cle/NFC/c/lib/bcm2835-1.64/src
TARGET_LIBS=/root/Bureau/IGPas2cle/NFC/c/lib/bcm2835-1.56/src/
# options pour la précompilation 
CPPFLAGS=-I$(LIB_RC522) -I$(LIB_BCM2835)
# options pour l'édition de liens 
LDFLAGS=-L$(LIB_RC522) -L$(LIB_BCM2835)
#chemin du compilateur croisé 
CC=/root/Bureau/RPI/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc

############## Compilation basique

librc522: $(LIB_RC522)/config.c $(LIB_RC522)/rc522.c $(LIB_RC522)/rfid.c $(LIB_RC522)/value.c
	gcc -c -fPIC $(LIB_RC522)/rc522.c -o $(LIB_RC522)/rc522.o
	gcc -c -fPIC $(LIB_RC522)/value.c -o $(LIB_RC522)/value.o
	gcc -c -fPIC $(LIB_RC522)/rfid.c -o $(LIB_RC522)/rfid.o
	gcc -c -fPIC $(LIB_RC522)/config.c -o $(LIB_RC522)/config.o
	gcc -c -fPIC $(LIB_RC522)/main.c -o $(LIB_RC522)/main.o
	gcc -shared $(LIB_RC522)/main.o $(LIB_RC522)/rfid.o $(LIB_RC522)/config.o $(LIB_RC522)/value.o $(LIB_RC522)/rc522.o -o $(LIB_RC522)/librc522.so

librc522_install:
	@echo "copie de librc522.so dans /usr/lib"
	cp $(LIB_RC522)/librc522.so /usr/lib/librc522.so
	@echo "copie de rc522.conf depuis la librairie dans /etc"
	cp $(LIB_RC522)/RC522.conf /etc/RC522.conf

getUID: src/getUID.c librc522 librc522_install
	gcc $(CPPFLAGS) $(LDFLAGS) src/getUID.c -o bin/getUID -lrc522 -lbcm2835



############## Compilation croisée

librc522_cross: $(LIB_RC522)/config.c $(LIB_RC522)/rc522.c $(LIB_RC522)/rfid.c $(LIB_RC522)/value.c
	$(CC) $(CPPFLAGS) -c -fPIC $(LIB_RC522)/rc522.c -o $(LIB_RC522)/rc522.o
	$(CC) $(CPPFLAGS) -c -fPIC $(LIB_RC522)/value.c -o $(LIB_RC522)/value.o
	$(CC) $(CPPFLAGS) -c -fPIC $(LIB_RC522)/rfid.c -o $(LIB_RC522)/rfid.o
	$(CC) $(CPPFLAGS) -c -fPIC $(LIB_RC522)/config.c -o $(LIB_RC522)/config.o
	$(CC) $(CPPFLAGS) -c -fPIC $(LIB_RC522)/main.c -o $(LIB_RC522)/main.o
	$(CC) -shared $(LIB_RC522)/main.o $(LIB_RC522)/rfid.o $(LIB_RC522)/config.o $(LIB_RC522)/value.o $(LIB_RC522)/rc522.o -o $(LIB_RC522)/librc522.so

getUID_cross: src/getUID.c librc522_cross
	$(CC) $(CPPFLAGS) $(LDFLAGS) src/getUID.c -o bin/getUID -lrc522 -lbcm2835

getUID_cross_static: src/getUID.c 
	$(CC) -static $(CPPFLAGS) $(LDFLAGS) src/read.c -o read

clean: 
	rm -rf *.o
