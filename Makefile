.PHONY: all run grade clean

CXX      = g++
CXXFLAGS = -Wall --std=c++17

# Default: build and run YOUR program (the example main in MainProgram.cpp).
all: run

main: MainProgram.cpp
	$(CXX) $(CXXFLAGS) MainProgram.cpp -o main

run: main
	./main

# The official tests are hidden: they run on GitHub every time you push.
grade:
	@echo "The tests run on GitHub when you push: see 'Releases' in your repository for your score."

clean:
	rm -f main grader main.exe grader.exe *.o
