# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/arpan/Documents/ComputerGraphics/TheOtherGuy

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/arpan/Documents/ComputerGraphics/TheOtherGuy/build

# Include any dependencies generated for this target.
include CMakeFiles/Dodecahedrons.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Dodecahedrons.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Dodecahedrons.dir/flags.make

CMakeFiles/Dodecahedrons.dir/source/main.cpp.o: CMakeFiles/Dodecahedrons.dir/flags.make
CMakeFiles/Dodecahedrons.dir/source/main.cpp.o: ../source/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/arpan/Documents/ComputerGraphics/TheOtherGuy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Dodecahedrons.dir/source/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Dodecahedrons.dir/source/main.cpp.o -c /home/arpan/Documents/ComputerGraphics/TheOtherGuy/source/main.cpp

CMakeFiles/Dodecahedrons.dir/source/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Dodecahedrons.dir/source/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/arpan/Documents/ComputerGraphics/TheOtherGuy/source/main.cpp > CMakeFiles/Dodecahedrons.dir/source/main.cpp.i

CMakeFiles/Dodecahedrons.dir/source/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Dodecahedrons.dir/source/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/arpan/Documents/ComputerGraphics/TheOtherGuy/source/main.cpp -o CMakeFiles/Dodecahedrons.dir/source/main.cpp.s

# Object files for target Dodecahedrons
Dodecahedrons_OBJECTS = \
"CMakeFiles/Dodecahedrons.dir/source/main.cpp.o"

# External object files for target Dodecahedrons
Dodecahedrons_EXTERNAL_OBJECTS =

Dodecahedrons: CMakeFiles/Dodecahedrons.dir/source/main.cpp.o
Dodecahedrons: CMakeFiles/Dodecahedrons.dir/build.make
Dodecahedrons: libraries/glfw/src/libglfw3.a
Dodecahedrons: libglad.a
Dodecahedrons: /usr/lib/librt.so
Dodecahedrons: /usr/lib/libm.so
Dodecahedrons: CMakeFiles/Dodecahedrons.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/arpan/Documents/ComputerGraphics/TheOtherGuy/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Dodecahedrons"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Dodecahedrons.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Dodecahedrons.dir/build: Dodecahedrons

.PHONY : CMakeFiles/Dodecahedrons.dir/build

CMakeFiles/Dodecahedrons.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Dodecahedrons.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Dodecahedrons.dir/clean

CMakeFiles/Dodecahedrons.dir/depend:
	cd /home/arpan/Documents/ComputerGraphics/TheOtherGuy/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/arpan/Documents/ComputerGraphics/TheOtherGuy /home/arpan/Documents/ComputerGraphics/TheOtherGuy /home/arpan/Documents/ComputerGraphics/TheOtherGuy/build /home/arpan/Documents/ComputerGraphics/TheOtherGuy/build /home/arpan/Documents/ComputerGraphics/TheOtherGuy/build/CMakeFiles/Dodecahedrons.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Dodecahedrons.dir/depend

