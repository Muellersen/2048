# 2048
This was the final project of the C++ lecture
# What did I learn?
- ncurses
- basics of C/C++

How to run on macOS:

- install cmake: brew install cmake
- install googletest: brew install googletest
- use this makefile:
  ```
  BREW_PREFIX := $(shell brew --prefix)

  CXX = g++
  CXXFLAGS = -std=c++17 -g -Wall -I$(BREW_PREFIX)/include
  LDFLAGS  = -L$(BREW_PREFIX)/lib
  MAIN_BINARIES = $(basename $(wildcard *Main.cpp))
  TEST_BINARIES = $(basename $(wildcard *Test.cpp))
  HEADERS = $(wildcard *.h)
  OBJECTS = $(addsuffix .o, $(basename $(filter-out %Main.cpp %Test.cpp, $(wildcard *.cpp))))
  LIBRARIES = -lncurses

  .PRECIOUS: %.o
  .SUFFIXES:
  .PHONY: clean compile test checkstyle

  all: compile test checkstyle

  compile: $(MAIN_BINARIES) $(TEST_BINARIES)

  test: $(TEST_BINARIES)
    for T in $(TEST_BINARIES); do ./$$T; done

  checkstyle:
    python3 ../cpplint.py --repository=. *.h *.cpp

  clean:
    rm -f *.o
    rm -f $(MAIN_BINARIES)
    rm -f $(TEST_BINARIES)

  %Main: %Main.o $(OBJECTS)
    $(CXX) $(LDFLAGS) -o $@ $^ $(LIBRARIES)

  %Test: %Test.o $(OBJECTS)
    $(CXX) $(LDFLAGS) -o $@ $^ $(LIBRARIES) -lgtest -lgtest_main -lpthread

  %.o: %.cpp $(HEADERS)
    $(CXX) $(CXXFLAGS) -c $<
  ´´´

- run: make compile
- run game with: ./2048Main
