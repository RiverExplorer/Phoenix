#
#

SOURCES.cpp = XmlInclude.cpp
CXXFLAGS		= -g

OBJECTS			= $(SOURCES.cpp:.cpp=.o)

XmlInclude : $(OBJECTS)
	$(CXX) -g $(OBJECTS) -o $@

clean:
	$(RM) $(OBJECTS) XmlInclude
