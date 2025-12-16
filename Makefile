CXX = g++
CXXFLAGS = -Wall -std=c++17

SRC = src/main.cpp src/server.cpp src/request.cpp src/utils.cpp

TARGET = tcp

all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
