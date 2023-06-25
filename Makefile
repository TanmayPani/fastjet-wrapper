# compiler
CXX = g++

# compiler flags
CXXFLAGS = -std=c++20 $(shell fastjet-config --cxxflags)

# path to additional libraries
#LIB_PATH = /path/to/your/libs

# linker flags
LDFLAGS = $(shell fastjet-config --libs) -lConstituentSubtractor

# source files
SRC = *.cpp

# output library name
LIB = libFastJetWrapper.dylib

all: $(LIB)

$(LIB): $(SRC)
	$(CXX) $(CXXFLAGS) -shared -o $(LIB) $(SRC) $(LDFLAGS)

clean:
	rm -f $(LIB)
