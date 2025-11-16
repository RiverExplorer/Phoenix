#
# Project: Phoenix
# Time-stamp: <2025-03-25 00:18:13 doug>
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
	Client.cpp \
	CppAcl.cpp \
	CppAuthMD5.cpp \
	CppAddMessage.cpp \
	CppBye.cpp \
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
	Configuration.cpp \
	ConfigureXslt.cpp \
	HostName.cpp \
	IO.cpp \
	Iov.cpp \
	IPPeer.cpp \
	Log.cpp \
	MD5.cpp \
	Misc.cpp \
	PhoenixEvent.cpp \
	Register.cpp \
	Server.cpp \
	ThreadName.cpp \
	xdr_SSL.cpp

SOURCES.x = \
	AuthAnonymous.x \
	AuthCert.x \
	AuthCertTls.x \
	AuthCertUser.x \
	AuthMD5.x \
	Bye.x \
	Capability.x \
	CmdAcl.x \
	CmdAddMessage.x \
	CmdAdministration.x \
	CmdCopyMessage.x \
	CmdExpunge.x \
	CmdFolder.x \
	CmdGetMessage.x \
	CmdKeepAlive.x \
	CmdNotSupported.x \
	CmdSearch.x \
	CmdSubscribe.x \
	CmdTimeout.x \
	CmdUpdateMessage.x \
	Commands.x \
	CommonDefinitions.x \
	ConfigSet.x \
	Email.x \
	FileCopy.x \
	FileCreate.x \
	FileDelete.x \
	FileGet.x \
	FileMetaData.x \
	FileModify.x \
	FileMove.x \
	FileRename.x \
	FileShare.x \
	FolderCapability.x \
	FolderCopy.x \
	FolderCreate.x \
	FolderDelete.x \
	FolderList.x \
	FolderMetaData.x \
	FolderMove.x \
	FolderOpen.x \
	FolderRename.x \
	FolderShare.x \
	Index.x \
	MDNEntry.x \
	MDNRecord.x \
	MetaData.x \
	Mime.x \
	PacketBody.x \
	PacketHeader.x \
	PhoenixString.x \
	Ping.x \
	SEQ_t.x \
	ServerConfigure.x \
	ServerConfigureReply.x \
	ServerKick.x \
	ServerLogs.x \
	ServerManageBans.x \
	ServerStats.x \
	Shutdown.x \
	StringRef.x \
	Types.x \
	UserCreate.x \
	UserDelete.x \
	UserList.x \
	UserPermissions.x \
	UserRename.x \
	Vendor.x \
	VendorID.x

SOURCES.hpp += \
	Certs.hpp \
	Client.hpp \
	CppAuthMD5.hpp \
	CppCapability.hpp \
	CppCommands.hpp \
	CppMime.hpp \
	CppTypes.hpp \
	CppBye.hpp \
	Configuration.hpp \
	HostName.hpp \
	IO.hpp \
	Iov.hpp \
	Log.hpp \
	IPPeer.hpp \
	MD5.hpp \
	PhoenixClient.hpp \
	PhoenixEvent.hpp \
	PhoenixServer.hpp \
	Register.hpp \
	Server.hpp \
	ThreadName.hpp \
	xdr_SSL.hpp \
	XSLT/Configure.xslt \
	XSLT/Configure.css

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

CPPFLAGS					+=	\
											-I../libPhoenixServer \
											-I../libPhoenixClient \
											-I/usr/include/tirpc \
											-I/usr/include/libxml2

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

LIBS							+= -ltirpc -lxslt -lxml2 -lssl -lcrypto -lz

include $(TOP)/BuildTools/Makefile.bottom.lib

SubDirs ::
	for D in $(SUBDIRS); do \
		cd $$D ; $(MAKE) $(MAKECMDGOALS); \
	done

clean::
	$(RM) LOG* \#*#
	$(RM) -r GeneratedFromXDR
	$(RM) -rf TestCoverage

Test:
	@echo "-----------------------------------"
	@echo "To run the test, type: make RunTests
	@echo "-----------------------------------"

RunTests:: LinuxTcov
	-@for D in $(TESTSUBDIRS); do \
		cd $$D ; $(MAKE) $(MAKECMDGOALS); \
	done
	-@$(RM) -rf TestCoverage
	-@mkdir TestCoverage
	(cd TestCoverage ; gcovr -r .. --html-details TestCoverage.html --decisions --calls)
	@echo "-----------------------------------------------"
	@echo "Test Coverage in TestCoverage/TestCoverage.html"
	@echo "-----------------------------------------------"

gcov : 
	-@$(RM) -rf TestCoverage
	-@mkdir TestCoverage
	(cd TestCoverage ; gcovr -r .. --html-details TestCoverage.html --decisions --calls)

