#
# Project: Phoenix
# Time-stamp: <2025-03-12 23:41:07 doug>
#
# Copyright (C) 2025 by Douglas Mark Royer (A.K.A. RiverExplorer)
# Author Douglas Mark Royer
# @date 3-JAN-20205
#
# licensed under CC BY 4.0.
#
# RiverExplorer is a trademark of Douglas Mark Royer
#

# Version of this program/library
#
.SUFFIXES: .g4 .cpp

VERSION			= 0.0.1
VENDER			= RiverExplorer

# TOP of the build tree where BuildTools/Make... files exist
#
TOP					= ..
LIBTOP			= ..

# What to call this program.
# And what the include/RiverExplorer/$(TARGET) header directory will be.
#
TARGET			= rpcgen++

NOW = $(shell date +%Y%m%dT%H%M%S)

LINUX_CPPFLAGS	+= \
	-I/usr/include/tirpc \
	-DTARG_BINARY_s='"$(TARGET)"' \
	-DFULLVERSION_s='"$(NOW)"' \
	-DOS_s='"Linux"' \
	-DOSVER_s='"$(shell uname -r)"'

include $(TOP)/BuildTools/Makefile.top

CXX_VERSION = -std=c++20

ANTLR4.g4				= xdr.g4

ANTLR4_DIRECTORY	= .

ANTLR4_GENERATED.cpp = \
	xdrBaseListener.cpp \
	xdrLexer.cpp \
	xdrListener.cpp \
	xdrParser.cpp

ANTLR4_GENERATED.h = \
	xdrBaseListener.h \
	xdrLexer.h \
	xdrListener.h \
	xdrParser.h

ANTLR4.other = \
	xdr.interp \
	xdrLexer.interp \
	xdrLexer.tokens \
	xdr.tokens

SOURCES.cpp = \
	GenerateComment.cpp \
	GenerateConstant.cpp \
	Generate.cpp \
	GenerateCpp.cpp \
	GenerateEnum.cpp \
	GeneratePassThrough.cpp \
	GenerateProcedure.cpp \
	GenerateProgram.cpp \
	GenerateStruct.cpp \
	GenerateTypeDef.cpp \
	GenerateUnion.cpp \
	GenerateVersion.cpp \
	Item.cpp \
	rpcgen.cpp \
	$(ANTLR4_GENERATED.cpp)

SOURCES.hpp = \
	GenerateCpp.hpp \
	Generate.hpp \
	rpcgen.hpp \
	$(ANTLR4_GENERATED.h)

W64_CPPFLAGS	+=

ANDROID_CPPFLAGS += 

ANDROID_LIBS	+= -L/usr/local/Phoenix/Linux/Debug/lib

CPPFLAGS	+= \
	-I/usr/include/antlr4-runtime \
	-I/usr/include/tirpc \
	-I$(ANTLR4_DIRECTORY)

CXXFLAGS	+= -ggdb -std=c++20
LDFLAGS		+= -ggdb -std=c++20

LIBS		+= -lantlr4-runtime

# Build as much as possible, with static libs.
#
SLIBS		+= $(LIBS)

include $(TOP)/BuildTools/Makefile.bottom

clean ::
	$(RM) XMLResults.xml
	$(RM) $(GENERATED.cpp) $(GENERATED.h) $(GENERATED.other)
	$(RM) -rf Generated/* $(ANTLR4_GENERATED.cpp) $(ANTLR4_GENERATED.h)

$(ANTLR4_GENERATED.cpp) $(ANTLR4_GENERATED.h) : xdr.g4
	antlr4 -Dlanguage=Cpp -o $(ANTLR4_DIRECTORY) xdr.g4

$(ANTLR4_DIRECTORY)/%.cpp : %.g4
	antlr4 -Dlanguage=Cpp -o $(ANTLR4_DIRECTORY) xdr.g4

$(ANTLR4_DIRECTORY)/%.h : %.g4
	antlr4 -Dlanguage=Cpp -o $(ANTLR4_DIRECTORY) xdr.g4

$(ANTLR4_DIRECTORY)/%.cpp : %.cpp

$(ANTLR_DIRECTORY) :
	-@mkdir $@

LinuxDebugRun : LinuxDebug
	BuildTemp/Linux/Debug/bin/TestClient1

RunTests::
	BuildTemp/Linux/Tcov/sbin/TestClient1
