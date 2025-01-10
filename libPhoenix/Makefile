#
# Version of this program/library
#
# These two must be set.
# VENDER sets the location name of the include files.
#
VERSION			= 0.0.1
VENDER			= RiverExplorer

# TOP of the build tree where BuildTools/Make... files exist
#
TOP						= ..

TESTSUBDIRS		= Tests
SUBDIRS				=
DOCDIRS				= Documentation

# What to call this program.
# And what the include/$(VENDER)/$(TARGET) header directory will be.
#
TARGET			= Phoenix

# Used to determin if we should include files with "" or <>
# When not defined, it uses <>
#
include $(TOP)/BuildTools/Makefile.top.lib

CPPFLAGS		+= -DBUILDING_LIBPHOENIX

# Generate the C++ files.
#
RPCGEN = rpcgen++ -+RiverExplorer::Phoenix

# Version of C++
#
CXX_VERSION = -std=c++17

#
SOURCES.c = 

# Naming convention:
# The .x files create a .cpp and a .hpp .
# Their supporting files are prefixed with Cpp.
#
# Example:
#
# foo.x creates foo.cpp and foo.hpp
#
# So the supporting code goes into CppFoo.cpp
#
# .cpp files that do not have a corrasponding .x file,
# are not prefixed with Cpp.
#
SOURCES.cpp = \
	CppAcl.cpp \
	CppAddMessage.cpp \
	CppAuthenticate.cpp \
	CppCapability.cpp \
	CppCloseFolder.cpp \
	CppCopyMessage.cpp \
	CppCreateFolder.cpp \
	CppDeleteFolder.cpp \
	CppExpunge.cpp \
	CppGetMessage.cpp \
	CppListFolders.cpp \
	CppNotSupported.cpp \
	CppPing.cpp \
	CppRenameFolder.cpp \
	CppSearch.cpp \
	CppSelectFolder.cpp \
	CppSubscribe.cpp \
	CppTimeout.cpp \
	CppUpdateMessage.cpp \
	IO.cpp \
	Misc.cpp \
	PhoenixEvent.cpp \
	Register.cpp \
	Server.cpp

SOURCES.x = \
	CmdAcl.x \
	CmdAddMessage.x \
	CmdAuthenticate.x \
	CmdCapability.x \
	CmdCloseFolder.x \
	CmdCopyMessage.x \
	CmdCreateFolder.x \
	CmdDeleteFolder.x \
	CmdExpunge.x \
	CmdGetMessage.x \
	CmdListFolders.x \
	CmdNotSupported.x \
	CmdPing.x \
	CmdRenameFolder.x \
	CmdSearch.x \
	CmdSelectFolder.x \
	CmdSubscribe.x \
	CmdTimeout.x \
	CmdUpdateMessage.x \
	Commands.x \
	KeyString.x \
	Mime.x \
	Email.x

SOURCES.hpp += \
	CppCommands.hpp \
	CppMime.hpp \
	IO.hpp \
	PhoenixClient.hpp \
	PhoenixEvent.hpp \
	PhoenixServer.hpp \
	Register.hpp \
	Server.hpp \

SOURCES.h		+=

# $(GENERATED_FROM_XDR) comes from ../BuildDir/Makefile.top.lib
#
ALL_GENERATED     = \
									$(GENERATED.hpp) \
									$(GENERATED_XDR.cpp) \
									$(CLIENT_XDR.cpp) \
									$(SERVER_XDR.cpp)

GENERATED_OBJECTS = \
									$(GENERATED_XDR.cpp:.cpp=.o) \
									$(CLIENT_XDR.cpp:.cpp=.o) \
									$(SERVER_XDR.cpp:.cpp=.o)


C_OBJECTS         = $(SOURCES.c:.c=.o)
CPP_OBJECTS       = $(SOURCES.cpp:.cpp=.o)

CPPFLAGS					+=	-I/usr/include/tirpc

CPPFLAGS.debug		=

CPPFLAGS.release	=

CPPFLAGS.tcov			=

CFLAGS						+=
CXXFLAGS					+=

W64_CPPFLAGS			+= 

ANDROID_CPPFLAGS	+=

LINUX_LIBS				+= -z defs

W64_LIBS					+= -lws2_32

ANDROID_LIBS			+=-z defs

LIBS							+= -ltirpc -lssl -lcrypto -lz

include $(TOP)/BuildTools/Makefile.bottom.lib

docs:
	(cd Documentation ; make)

SubDirs ::
	for D in $(SUBDIRS); do \
		cd $$D ; $(MAKE) $(MAKECMDGOALS); \
	done

clean::
	$(RM) LOG* \#*#
	$(RM) -r GeneratedFromXDR
	for D in $(DOCDIRS); do \
		(cd $$D ; $(MAKE) clean); \
	done
