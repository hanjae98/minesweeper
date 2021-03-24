### Makefile for minesweeper
### Writer: Han Jaesung
### 2021. 03. 24. (수) 00:28:14 KST

.PHONY:	clean

RM = rm -f
CC = g++
CFLAGS = -Wall
TARGET = minesweeper

$(TARGET) : minesweeper.o minesweeper_definition.o
	$(CC) $(CFLAGS) -o $(TARGET) minesweeper.o minesweeper_definition.o

minesweeper.o: minesweeper.cpp minesweeper_header.h
	$(CC) $(CFLAGS) -c minesweeper.cpp

minesweeper_definition.o: minesweeper_definition.cpp minesweeper_header.h 
	$(CC) $(CFLAGS) -c minesweeper_definition.cpp 

clean:
	$(RM) *.o $(TARGET)
