CC = gcc
CXX = g++
CFLAGS  = -g -Wall -O3
LINKING = -lglut -lGL -lGLU -lm
TARGET = *

all:
	$(CXX) $(CFLAGS) -o fps $(TARGET).cpp $(LINKING)
	./3dtest

clean:
	$(RM) 3dtest
