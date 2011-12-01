CC = g++
CFLAGS = -Wall -O3
CONFIG_DIR = /etc
BINARY_DIR = /sbin

.PHONY: install clean
mplex: Utils.o Logger.o VNCServer.o Container.o ConfigFile.o main.o
	$(CC) $(CFLAGS) -o mplex Utils.o Logger.o VNCServer.o Container.o ConfigFile.o main.o
install:
	cp -n ./default.conf $(CONFIG_DIR)/mplex.conf
	cp ./mplex $(BINARY_DIR)/mplex
Utils.o: Utils.cpp Utils.h
	$(CC) $(CFLAGS) -c Utils.cpp
Logger.o: Logger.cpp Logger.h
	$(CC) $(CFLAGS) -c Logger.cpp
VNCServer.o: VNCServer.cpp VNCServer.h
	$(CC) $(CFLAGS) -c VNCServer.cpp
Container.o: Container.cpp Container.h
	$(CC) $(CFLAGS) -c Container.cpp
ConfigFile.o: ConfigFile.cpp ConfigFile.h
	$(CC) $(CFLAGS) -c ConfigFile.cpp
main.o: main.cpp main.h
	$(CC) $(CFLAGS) -c main.cpp
clean:
	rm -rf *.o mplex
