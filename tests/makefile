#BMS 2023 - Projekt 1
#author: Jakub Komárek (xkomar33)

CFLAGS= -Wall -g 


BIN=test
CC=g++ 
SRC=matrixGeneration.cpp ../coder.cpp ../decoder.cpp ../matrixMaker.cpp ../matOp.cpp

.PHONY: all build  clean

all: build

build: 
	$(CC) $(CFLAGS) -o $(BIN) $(SRC) -I/usr/include/opencv4  -lopencv_core -lopencv_highgui -lopencv_imgproc

clean:
	rm -rf $(BIN) $(ZIP) *.o 