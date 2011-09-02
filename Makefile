CC            = gcc
CXX           = g++
DEFINES       = 
CFLAGS        = -O2
CXXFLAGS      = -O2 
INCPATH       = -Iinclude
LINK          = g++
LFLAGS        = 

####### Files

SOURCES       = src/fdtd.cpp 
OBJECTS       = fdtd.o
TARGET        = fdtd

####### Build rules

all: Makefile $(TARGET)

$(TARGET):  $(OBJECTS)  
	$(LINK) $(LFLAGS) -o $(TARGET) $(OBJECTS) $(OBJCOMP) $(LIBS)

####### Compile

fdtd.o: src/fdtd.cpp include/fdtd.hpp \
		include/matrix.hpp 
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o fdtd.o src/fdtd.cpp

