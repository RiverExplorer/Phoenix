
TARGET			= rpcgen++

SOURCES.hpp = \
	rpcgen.hpp

GENERATED.cpp = \
	xdrBaseListener.cpp \
	xdrLexer.cpp \
	xdrListener.cpp \
	xdrParser.cpp

GENERATED.h = \
	xdrBaseListener.h \
	xdrLexer.h \
	xdrListener.h \
	xdrParser.h

GENERATED.other = \
	xdr.interp \
	xdrLexer.interp \
	xdrLexer.tokens \
	xdr.tokens

SOURCES.cpp = \
	$(GENERATED.cpp) \
	rpcgen.cpp \
	GenerateConstant.cpp \
	GenerateEnumValue.cpp \
	GenerateTypeDef.cpp \
	GenerateStruct.cpp \
	GenerateUnionCase.cpp \
	GenerateUnion.cpp \
	GenerateProcedure.cpp \
	GenerateVersion.cpp \
	GenerateProgram.cpp

OBJECTS		= $(SOURCES.cpp:.cpp=.o)

CXXFLAGS	= -ggdb -std=c++20 -I/usr/include/antlr4-runtime -I/usr/include/tirpc
LDFLAGS		= -ggdb -std=c++20
LIBS			=	-lantlr4-runtime

$(TARGET) : $(OBJECTS)
	g++ -o $@ $(LDFLAGS) $(OBJECTS) $(LIBS)

$(OBJECTS) : $(SOURCES.hpp) $(GENERATED.h)

$(GENERATED.cpp) $(GENERATED.hpp) : xdr.g4
	antlr4 -Dlanguage=Cpp -o . xdr.g4

clean:
	$(RM) *.o $(GENERATED.cpp) $(GENERATED.h) $(GENERATED.other)
