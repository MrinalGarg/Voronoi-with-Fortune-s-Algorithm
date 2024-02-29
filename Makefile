CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: voronoi generate_output plot

voronoi: voronoi.cpp
	$(CXX) $(CXXFLAGS) -o voronoi voronoi.cpp

generate_output: voronoi input.txt
	./voronoi < input.txt > log.txt

plot: output.txt
	python3 plot.py input.txt output.txt

clean:
	rm -f voronoi output.txt

.PHONY: all clean plot
