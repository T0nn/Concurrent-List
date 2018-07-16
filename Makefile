CC = g++
CXXFLAGS  = -std=c++11 -Wall -pthread

SRC = $(wildcard *.cpp)
TARGET = test

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CXXFLAGS) -o $@ $^

.PHONY: clean run
run:
	./$(TARGET) < input.txt
clean:
	rm -f $(TARGET)