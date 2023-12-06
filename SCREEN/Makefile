CXX = g++
CXXFLAGS = -Wall -std=c++17

all: main

main: main.o Screen.o
	$(CXX) $(CXXFLAGS) -o main main.o Screen.o

main.o: main.cpp Utility.h TimePoint.h Screen.h
	$(CXX) $(CXXFLAGS) -c main.cpp

Screen.o: Screen.cpp Screen.h TimePoint.h
	$(CXX) $(CXXFLAGS) -c Screen.cpp

clean:
	rm -f *.o main
