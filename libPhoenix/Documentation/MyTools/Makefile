#
#

SOURCES.cpp = SimpleCpp.cpp
CXXFLAGS		= -g

OBJECTS			= $(SOURCES.cpp:.cpp=.o)

SimpleCpp : $(OBJECTS)
	$(CXX) -g $(OBJECTS) -o $@

clean:
	$(RM) $(OBJECTS)

