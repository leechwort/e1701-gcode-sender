PROGRAM = gcode-sender

INCLUDEDIRS = -I.. 


LIBDIRS = -L/usr/lib

LIBS = -le1701

CXXSOURCES = main.cpp

DBGFLAGS = -O2 -g0 -DNDEBUG
ifeq ($(DEBUG),1)
DBGFLAGS = -O0 -g3 -D_DEBUG
endif
ifeq ($(PROFILE),1)
DBGFLAGS = -O2 -g3
endif

CXXOBJECTS = $(CXXSOURCES:.cpp=.o)
CXXFLAGS = -Wall -DESRI_UNIX $(INCLUDEDIRS) -DENV_LINUX $(DBGFLAGS)
CXX = g++

LDFLAGS = $(LIBDIRS) $(LIBS)

all: $(PROGRAM)

$(PROGRAM): $(CXXOBJECTS)
	$(CXX) -o $@ $(CXXOBJECTS) $(LDFLAGS)

clean:
	$(RM) -f $(CXXOBJECTS) $(PROGRAM)

run:
	./$(PROGRAM)


