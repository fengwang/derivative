CXX           = clang++
CXXFLAGS      = -std=c++11 -stdlib=libc++ -O4 -Wall -ferror-limit=2 -DNDEBUG
LFLAGS 		  = -lc++ -lc++abi -O4
INCPATH       = -Iinclude
LINK          = $(CXX)
OBJECTS_DIR   = obj
BIN_DIR 	  = bin

all: rural_derivative second_derivative derivative_test 

rural_derivative: test/derivative/rural_derivative.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/rural_derivative.o test/derivative/rural_derivative.cc
	$(LINK) $(LFLAGS) -o $(BIN_DIR)/rural_derivative $(OBJECTS_DIR)/rural_derivative.o

second_derivative: test/derivative/second_derivative.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/second_derivative.o test/derivative/second_derivative.cc
	$(LINK) $(LFLAGS) -o $(BIN_DIR)/second_derivative $(OBJECTS_DIR)/second_derivative.o

derivative_test: test/derivative/derivative.cc
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o $(OBJECTS_DIR)/derivative_test.o test/derivative/derivative.cc
	$(LINK) $(LFLAGS) -o $(BIN_DIR)/derivative_test $(OBJECTS_DIR)/derivative_test.o

clean: 
	rm -rf $(OBJECTS_DIR)/*
	rm -rf $(BIN_DIR)/*

