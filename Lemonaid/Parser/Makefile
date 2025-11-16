#
# RiverExplorer is a trademark of RiverExplorer LLC.
# This program is free to use as long as this notice is
# kept intact and visible.
#
# Copyright (C) 2025, RiverExplorer LLC

# Version of this program/library
#
#.SUFFIXES: .g4 .cpp

VERSION			= 0.0.1
VENDER			= RiverExplorer

# TOP of the build tree where BuildTools/Make... config files exist
#
TOP					= ..
LIBTOP			= ..

# What to call this program.
# And what the include/RiverExplorer/$(TARGET) header directory will be.
#
TARGET			= Protocol

NOW = $(shell date +%Y%m%dT%H%M%S)

LINUX_CPPFLAGS	+= \
	-I/usr/include/tirpc \
	-DTARG_BINARY_s='"$(TARGET)"' \
	-DFULLVERSION_s='"$(NOW)"' \
	-DOS_s='"Linux"' \
	-DOSVER_s='"$(shell uname -r)"'

include $(TOP)/BuildTools/Makefile.top

CXX_VERSION = -std=c++20

SOURCES.yy = Parser.yy
SOURCES.yy.cpp = Parser.tab.cpp
SOURCES.yy.hpp = Parser.tab.hpp

SOURCES.l = Lexer.l
SOURCES.l.cpp = Lexer.yy.cpp
SOURCES.l.hpp =

SOURCES.cpp = \
	$(SOURCES.yy.cpp) \
	$(SOURCES.l.cpp) \
	Content.cpp \
	Driver.cpp \
	Generate.cpp \
	GenerateCBOR.cpp \
	GenerateJSON.cpp \
	GenerateNATIVE.cpp \
	GenerateREST.cpp \
	GenerateXDR.cpp \
	GenerateXML.cpp \
	Globals.cpp \
	Main.cpp \
	Pack.cpp \
	Protocol.cpp \
	RmDirRecursive.cpp \
	Scope.cpp \
	Symbol.cpp \
	SymbolTable.cpp \
	Version.cpp

SOURCES.hpp = \
	$(SOURCES.yy.hpp) \
	$(SOURCES.l.hpp) \
	Content.hpp \
	Driver.hpp \
	Generate.hpp \
	GenerateCBOR.hpp \
	GenerateJSON.hpp \
	GenerateNATIVE.hpp \
	GenerateREST.hpp \
	GenerateXDR.hpp \
	GenerateXML.hpp \
	Globals.hpp \
	Pack.hpp \
	Range.hpp \
	Protocol.hpp \
	RmDirRecursive.hpp \
	Scanner.hpp \
	Scope.hpp \
	Symbol.hpp \
	SymbolTable.hpp \
	Version.hpp

W64_CPPFLAGS	+=

ANDROID_CPPFLAGS += 

ANDROID_LIBS	+=

CPPFLAGS	+= \
	-I/usr/include/tirpc

CXXFLAGS	+= -ggdb -std=c++20
LDFLAGS		+= -ggdb -std=c++20

LIBS		+= 

# Build as much as possible, with static libs.
#
SLIBS		+= $(LIBS)

Default:
	@echo "Use one of:"
	@echo "  make LinuxDebug"
	@echo "  make LinuxRelease"
	@echo "  make LinuxTcov"
	@echo "  make Linux - To make LinuxDebug, LinuxRelease, and LinuxTcov"
	@echo ""
	@echo "  make W64Debug"
	@echo "  make W64Tcov"
	@echo "  make W64Releaes"
	@echo "  make W64 - To make W64Debug, W64Release, and W64Tcov"
	@echo ""
	@echo "  make AndroidDebug"
	@echo "  make AndroidTcov"
	@echo "  make AndroidReleaes"
	@echo "  make Android - To make AndroidDebug, AndroidRelease, and AndroidTcov"
	@echo ""
	@echo "  make all - To make Linux, W64, and Android"

include $(TOP)/BuildTools/Makefile.bottom


Parser.tab.o : Parser.tab.cpp
Parser.tab.hpp Parser.tab.cpp : Parser.yy
	bison -d -v -o Parser.tab.cpp $< 

Lexer.yy.o : Lexer.yy.cpp
Lexer.yy.cpp : Lexer.l
	flex -o $@ $<

clean ::
	$(RM) XMLResults.xml
	$(RM) $(GENERATED.cpp) $(GENERATED.h) $(GENERATED.other)
	$(RM) -rf GeneratedFromXDR/* 
	$(RM) Parser.tab.cc Parser.tab.o Parser.output Parser.tab.hpp
	$(RM) Lexer.yy.cc Lexer.yy.o position.hh location.hh
	$(RM) *.d stack.hh
	$(RM) LOG

LinuxDebugRun : LinuxDebug
	BuildTemp/Linux/Debug/bin/TestClient1

RunTests::
	BuildTemp/Linux/Tcov/sbin/TestClient1

LocalNeeds :: Dirs

