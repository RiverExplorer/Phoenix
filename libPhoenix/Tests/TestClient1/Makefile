#
# Project: Phoenix
# Time-stamp: <2025-03-02 22:57:44 doug>
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
VERSION			= 0.0.1
VENDER			= RiverExplorer

# TOP of the build tree where BuildTools/Make... files exist
#
TOP					= ../../..
LIBTOP			= ../..

# What to call this program.
# And what the include/RiverExplorer/$(TARGET) header directory will be.
#
TARGET			= TestClient1

CPPFLAGS		+= 

NOW = $(shell date +%Y%m%dT%H%M%S)

LINUX_CPPFLAGS	+= \
	-I/usr/include/tirpc \
	-DTARG_BINARY_s='"$(TARGET)"' \
	-DFULLVERSION_s='"$(NOW)"' \
	-DOS_s='"Linux"' \
	-DOSVER_s='"$(shell uname -r)"'

include $(TOP)/BuildTools/Makefile.top

SOURCES.cpp = TestClient.cpp
SOURCES.hpp = TestClient.hpp

W64_CPPFLAGS	+=

ANDROID_CPPFLAGS += 

ANDROID_LIBS	+= -L/usr/local/Phoenix/Linux/Debug/lib

LIBS		+= -lPhoenix -lxslt -lxml2 -ltirpc -lcrypto -lssl -lz -lcppunit -Wl,--as-needed

# Build as much as possible, with static libs.
#
SLIBS		+= -Wl,-Bstatic -lPhoenix -lcrypto -Wl,-Bdynamic -lxslt -lxml2 -ltirpc -lssl -lcppunit

include $(TOP)/BuildTools/Makefile.bottom

clean ::
	$(RM) XMLResults.xml

LinuxDebugRun : LinuxDebug
	BuildTemp/Linux/Debug/bin/TestClient1

RunTests::
	BuildTemp/Linux/Tcov/sbin/TestClient1

