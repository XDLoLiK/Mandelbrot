CXX = g++
CXXFLAGS = -g -c -Wall -Wextra -msse4.2 -mavx2 -O3

all: mandelbrot

mandelbrot: main.o mandelbrot.o
	$(CXX) main.o mandelbrot.o -lSDL2main -lSDL2 -o mandelbrot.out

main.o: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp

mandelbrot.o: mandelbrot.cpp
	$(CXX) $(CXXFLAGS) mandelbrot.cpp

.PHONY: clean
clean:
	rm -rf *.o mandelbrot
