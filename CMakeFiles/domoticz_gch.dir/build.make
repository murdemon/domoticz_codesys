# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/dev-domoticz

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/dev-domoticz

# Utility rule file for domoticz_gch.

# Include the progress variables for this target.
include CMakeFiles/domoticz_gch.dir/progress.make

CMakeFiles/domoticz_gch: stdafx.h.gch/.c++

stdafx.h.gch/.c++: main/stdafx.h
	$(CMAKE_COMMAND) -E cmake_progress_report /home/pi/dev-domoticz/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating stdafx.h.gch/.c++"
	/usr/bin/c++ -O3 -DNDEBUG -I/home/pi/dev-domoticz/main -I/usr/local/include -I/usr/include -I/usr/include -I/usr/include -I/home/pi/dev-domoticz/hardware/openzwave -DBUILTIN_MQTT -DWITH_LIBUSB -DWWW_ENABLE_SSL -DWITH_OPENZWAVE -DWITH_GPIO -x c++-header -o /home/pi/dev-domoticz/stdafx.h.gch/.c++ /home/pi/dev-domoticz/main/stdafx.h

domoticz_gch: CMakeFiles/domoticz_gch
domoticz_gch: stdafx.h.gch/.c++
domoticz_gch: CMakeFiles/domoticz_gch.dir/build.make
.PHONY : domoticz_gch

# Rule to build all files generated by this target.
CMakeFiles/domoticz_gch.dir/build: domoticz_gch
.PHONY : CMakeFiles/domoticz_gch.dir/build

CMakeFiles/domoticz_gch.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/domoticz_gch.dir/cmake_clean.cmake
.PHONY : CMakeFiles/domoticz_gch.dir/clean

CMakeFiles/domoticz_gch.dir/depend:
	cd /home/pi/dev-domoticz && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/dev-domoticz /home/pi/dev-domoticz /home/pi/dev-domoticz /home/pi/dev-domoticz /home/pi/dev-domoticz/CMakeFiles/domoticz_gch.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/domoticz_gch.dir/depend

