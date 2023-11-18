#BMS 2023 - Projekt 1
#author: Jakub Komárek (xkomar33)

CFLAGS= -Wall -g

BIN=bms
ZIP=222161.zip
CC=g++ 
RM=rm -f
SRC=main.cpp coder.cpp decoder.cpp

.PHONY: all build run pack clean

all: build

build: 
	$(CC) $(CFLAGS) -o $(BIN) $(SRC) 

pack: clean
	zip  $(ZIP) * 

run:
	test -f $(BIN) && ./$(BIN)

clean:
	rm -rf $(BIN) $(ZIP) *.o 