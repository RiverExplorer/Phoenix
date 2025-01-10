
To build you will need rpcgen++ in your path.
rpcgen++ which is the Sun (now Oracle) public rpcgen altered for C++.

rpcgen++ sources are included.

	 rpcgen++

The Makefile is designed to run on Linux.

Build files will be placed in subdirectores in /usr/local/Phoenix .

The user building this project needs write permission to those directories.

To change these locations, edit Makefile and add these:

		 ANDROID_INSTALL_DIR.Release = ...YourPath...
		 ANDROID_INSTALL_DIR.Debug   = ...YourPath...
		 ANDROID_INSTALL_DIR.Tcov    = ...YourPath...

		 LINUX_INSTALL_DIR.Release   = ...YourPath...
		 LINUX_INSTALL_DIR.Debug     = ...YourPath...
		 LINUX_INSTALL_DIR.Tcov      = ...YourPath...

		 W64_INSTALL_DIR.Release     = ...YourPath...
		 W64_INSTALL_DIR.Debug       = ...YourPath...
		 W64_INSTALL_DIR.Tcov        = ...YourPath...

The defaults are:

		 ANDROID_INSTALL_DIR.Release = /usr/local/Phoenix/Android/Release
		 ANDROID_INSTALL_DIR.Debug   = /usr/local/Phoenix/Android/Debug
		 ANDROID_INSTALL_DIR.Tcov    = /usr/local/Phoenix/Android/Tcov

		 LINUX_INSTALL_DIR.Release   = /usr/local/Phoenix/Linux/Release
		 LINUX_INSTALL_DIR.Debug     = /usr/local/Phoenix/Linux/Debug
		 LINUX_INSTALL_DIR.Tcov      = /usr/local/Phoenix/Linux/Tcov

		 W64_INSTALL_DIR.Release     = /usr/local/Phoenix/W64/Release
		 W64_INSTALL_DIR.Debug       = /usr/local/Phoenix/W64/Debug
		 W64_INSTALL_DIR.Tcov        = /usr/local/Phoenix/W64/Tcov

--- LINUX ----

For android builds you will need gcc and g++

	 		LINUX_CC		= ...path to your linux compiler.
	 		LINUX_CXX		= ...path to your linux compiler.
	 		LINUX_STRIP = ...path to your strip

The defaults are:

	 		LINUX_CC		= gcc
	 		LINUX_CXX		= g++
	 		LINUX_STRIP = strip

Ways to run make for Linux:

Build debug (for gdb):

	 make LinuxDebug

Build Release (optimized and stripped of excess symbols):
	 make LinuxRelease

Build Tcov (Built with test coverage -fprofile-arcs -ftest-coverage):
	 make LinuxTcov

To build Debug, Release, and Tcov for Linux:

	 make Linux

--- ANDROID ----

To build for Android, you need the Android cross-compliers.
In your makefile add:

	 		ANDROID_CC = ...path to your android C cross compiler.
	 		ANDROID_CXX = ...path to your android C++ cross compiler.
	 		ANDROID_STRIP = ...path to your android llvm-stip

The defaults (Which will NOT WORK FOR YOU) are:

		ANDROID_CC = ~doug/Android/android-ndk-r26/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android34-clang

		ANDROID_CXX	=~doug/Android/android-ndk-r26/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android34-clang++

		ANDROID_STRIP	= ~/Android/android-ndk-r26/toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-strip
		
Ways to run make for Android:

Build debug (for gdb):

	 make AndroidDebug

Build Release (optimized and stripped of excess symbols):
	 make AndroidRelease

Build Tcov (Built with test coverage -fprofile-arcs -ftest-coverage):
	 make AndroidTcov

To build Debug, Release, and Tcov for Android:

	 make Android

--- WINDOWS ----

To build for Android, you need the Android mingw cross-compliers.
In your makefile add:

	 		ANDROID_CC = ...path to your android C cross compiler.
	 		ANDROID_CXX = ...path to your android C++ cross compiler.
	 		ANDROID_STRIP = ...path to your android llvm-stip

The defaults are (which might work for you):

	 		ANDROID_CC		= x86_64-w64-mingw32-gcc
	 		ANDROID_CXX		= x86_64-w64-mingw32-g++
	 		ANDROID_STRIP = x86_64-w64-mingw32-strip

Ways to run make for Windows:

Build debug (for gdb):

	 make W64Debug

Build Release (optimized and stripped of excess symbols):
	 make W64Release

Build Tcov (Built with test coverage -fprofile-arcs -ftest-coverage):
	 make W64Tcov

To build Debug, Release, and Tcov for Android:

	 make W64

--- OTHER BUILD OPTIONS ----

To build debug for Android, Linux, and Windows:

		make Debug

To build release for Android, Linux, and Windows:

		make Releaes

To build tcov for Android, Linux, and Windows:

		make Tcov
