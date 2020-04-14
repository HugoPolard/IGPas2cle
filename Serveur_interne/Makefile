CC=gcc
TARGET=Serveur

CFLAGS=-Wall
LDFLAGS=-lpthread -lcurl

LIBSRC=$(wildcard src/libs/*.c)
LIBOBJ=$(patsubst src/libs/%.c, out/%.o, $(LIBSRC))

OBJ=$(wildcard out/*.o)


all : $(TARGET)

$(TARGET): $(LIBOBJ) out/main.o
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS) $(CFLAGS)

out/%.o : src/libs/%.c
	$(CC) -c $< -o $@ $(CFLAGS)


out/log.o : src/libs/log.c
	$(CC) -c $< -DLOG_USE_COLOR -o out/log.o $(CFLAGS)

out/main.o : src/main.c 
	$(CC) -o out/main.o -c src/main.c $(CFLAGS)

clean :	
	rm out/*.o Serveur