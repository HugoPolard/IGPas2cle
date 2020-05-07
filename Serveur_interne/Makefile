DEBUG=yes
CFLAGS=-Wall
ifeq ($(DEBUG),yes)
	CC=gcc
	TARGET=Serveur_interne
else
	CC=/home/gregoire/Documents/RPI/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc
	TARGET=Serveur_RPI
	CURL_LIB=-L/home/gregoire/Documents/RPI/Projet/IGPas2cle/Serveur_Interne/build/lib
	CURL_INC=-I/home/gregoire/Documents/RPI/Projet/IGPas2cle/Serveur_Interne/build/include
	CFLAGS=-Wall -std=gnu99
endif

LDFLAGS=-lpthread -lcurl

LIBSRC=$(wildcard src/libs/*.c)
LIBOBJ=$(patsubst src/libs/%.c, out/%.o, $(LIBSRC))

OBJ=$(wildcard out/*.o)


all : $(TARGET)

$(TARGET): $(LIBOBJ) out/main.o
	$(CC) -o $(TARGET) $(OBJ) $(CURL_LIB) $(CURL_INC) $(LDFLAGS) $(CFLAGS)

out/%.o : src/libs/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

out/curlHelper.o : src/libs/curlHelper.c 
	$(CC) -c $< -o out/curlHelper.o $(CURL_LIB) $(CURL_INC)

out/log.o : src/libs/log.c
	$(CC) -c $< -DLOG_USE_COLOR -o out/log.o $(CFLAGS)

out/main.o : src/main.c 
	$(CC) -o out/main.o -c src/main.c $(CFLAGS) $(CURL_LIB) $(CURL_INC)

clean :	
	rm out/*.o Serveur_interne
