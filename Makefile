CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: voronoi generate_output plot

vornoi1: vornoi1.cpp
	$(CXX) $(CXXFLAGS) -o voronoi voronoi.cpp

generate_output: voronoi input.txt
	./voronoi < input.txt > output.txt

plot: output.txt
	python3 plot.py input.txt output.txt

random: generate_random generate_output plot

generate_random:
	@echo "Enter the number of random points to generate: "; \
	read num_points; \
	python3 generate_random_points.py $$num_points > input.txt

clean:
	rm -f voronoi output.txt

.PHONY: all clean plot random
