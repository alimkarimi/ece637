# For Linux or any machines with gcc compiler
CC = gcc 
CFLAGS = -std=c99 -Wall -pedantic
BIN = ../bin

all: ImageReadWriteExample SurrogateFunctionExample SolveExample 2dfirfilter_sandbox sharpening_filter 2d_iir_filter

clean:
	/bin/rm *.o $(BIN)/*

OBJ = tiff.o allocate.o randlib.o qGGMRF.o solve.o 

ImageReadWriteExample: ImageReadWriteExample.o $(OBJ) 
	$(CC) $(CFLAGS) -o ImageReadWriteExample ImageReadWriteExample.o $(OBJ) -lm
	mv ImageReadWriteExample $(BIN)

SurrogateFunctionExample: SurrogateFunctionExample.o $(OBJ) 
	$(CC) $(CFLAGS) -o SurrogateFunctionExample SurrogateFunctionExample.o $(OBJ) -lm
	mv SurrogateFunctionExample $(BIN)

SolveExample: SolveExample.o $(OBJ) 
	$(CC) $(CFLAGS) -o SolveExample SolveExample.o $(OBJ) -lm
	mv SolveExample $(BIN)

2dfirfilter_sandbox: 2dfirfilter_sandbox.o $(OBJ)
	$(CC) $(CFLAGS) -o 2dfirfilter_sandbox 2dfirfilter_sandbox.o $(OBJ) -lm
	mv 2dfirfilter_sandbox $(BIN)

sharpening_filter: sharpening_filter.o $(OBJ)
	$(CC) $(CFLAGS) -o sharpening_filter sharpening_filter.o $(OBJ) -lm
	mv sharpening_filter $(BIN)

2d_iir_filter: 2d_iir_filter.o $(OBJ)
	$(CC) $(CFLAGS) -o 2d_iir_filter 2d_iir_filter.o $(OBJ) -lm
	mv 2d_iir_filter $(BIN)

