# BMS 2023 - Projekt 
# LDPC kódování a dekódování
# author: Bc. Jakub Komárek (xkomar33) 

CFLAGS = -Wall -Werror -O3 -std=c++17
BIN = bms
ZIP = 222161.zip
CC = g++
RM = rm -f
SRC = main.cpp coder.cpp decoder.cpp matrixMaker.cpp matOp.cpp
OBJ = $(SRC:.cpp=.o)

.PHONY: all build run pack clean

all: build

build: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -I/usr/include/opencv4 -lopencv_core -lopencv_highgui -lopencv_imgproc

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ -I/usr/include/opencv4

pack: clean
	zip $(ZIP) *.cpp *.hpp README.md makefile

run: $(BIN)
	./$(BIN)

clean:
	$(RM) $(BIN) $(ZIP) $(OBJ)
