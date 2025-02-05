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
#DOCDIRS				= Documentation

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
	Certs.cpp \
	CppAcl.cpp \
	CppAddMessage.cpp \
	CppAuthenticate.cpp \
	CppCapability.cpp \
	CppCopyMessage.cpp \
	CppExpunge.cpp \
	CppGetMessage.cpp \
	CppMime.cpp \
	CppMimeHeaders.cpp \
	CppMimeBodyPart.cpp \
	CppNotSupported.cpp \
	CppPing.cpp \
	CppSearch.cpp \
	CppSubscribe.cpp \
	CppTimeout.cpp \
	CppUpdateMessage.cpp \
	IO.cpp \
	Iov.cpp \
	IPPeer.cpp \
	Misc.cpp \
	PhoenixEvent.cpp \
	Register.cpp \
	Server.cpp

SOURCES.x = \
	CmdAcl.x \
	CmdAddMessage.x \
	CmdAuthenticate.x \
	CmdAdministration.x \
	CmdCapability.x \
	CmdCopyMessage.x \
	CmdExpunge.x \
	CmdFolder.x \
	CmdGetMessage.x \
	CmdKeepAlive.x \
	CmdNotSupported.x \
	CmdPing.x \
	CmdSearch.x \
	CmdSubscribe.x \
	CmdTimeout.x \
	CmdUpdateMessage.x \
	Commands.x \
	Email.x \
	MetaData.x \
	Mime.x \
	Types.x

SOURCES.hpp += \
	Certs.hpp \
	CppCommands.hpp \
	CppMime.hpp \
	CppTypes.hpp \
	IO.hpp \
	Iov.hpp \
	IPPeer.hpp \
	PhoenixClient.hpp \
	PhoenixEvent.hpp \
	PhoenixServer.hpp \
	Register.hpp \
	Server.hpp

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

SubDirs ::
	for D in $(SUBDIRS); do \
		cd $$D ; $(MAKE) $(MAKECMDGOALS); \
	done

clean::
	$(RM) LOG* \#*#
	$(RM) -r GeneratedFromXDR
