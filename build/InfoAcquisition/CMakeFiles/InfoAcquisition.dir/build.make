# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/build

# Include any dependencies generated for this target.
include InfoAcquisition/CMakeFiles/InfoAcquisition.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include InfoAcquisition/CMakeFiles/InfoAcquisition.dir/compiler_depend.make

# Include the progress variables for this target.
include InfoAcquisition/CMakeFiles/InfoAcquisition.dir/progress.make

# Include the compile flags for this target's objects.
include InfoAcquisition/CMakeFiles/InfoAcquisition.dir/flags.make

InfoAcquisition/CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.o: InfoAcquisition/CMakeFiles/InfoAcquisition.dir/flags.make
InfoAcquisition/CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.o: ../InfoAcquisition/infoAcquisition.cpp
InfoAcquisition/CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.o: InfoAcquisition/CMakeFiles/InfoAcquisition.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object InfoAcquisition/CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.o"
	cd /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/build/InfoAcquisition && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT InfoAcquisition/CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.o -MF CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.o.d -o CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.o -c /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/InfoAcquisition/infoAcquisition.cpp

InfoAcquisition/CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.i"
	cd /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/build/InfoAcquisition && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/InfoAcquisition/infoAcquisition.cpp > CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.i

InfoAcquisition/CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.s"
	cd /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/build/InfoAcquisition && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/InfoAcquisition/infoAcquisition.cpp -o CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.s

# Object files for target InfoAcquisition
InfoAcquisition_OBJECTS = \
"CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.o"

# External object files for target InfoAcquisition
InfoAcquisition_EXTERNAL_OBJECTS =

InfoAcquisition/libInfoAcquisition.a: InfoAcquisition/CMakeFiles/InfoAcquisition.dir/infoAcquisition.cpp.o
InfoAcquisition/libInfoAcquisition.a: InfoAcquisition/CMakeFiles/InfoAcquisition.dir/build.make
InfoAcquisition/libInfoAcquisition.a: InfoAcquisition/CMakeFiles/InfoAcquisition.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libInfoAcquisition.a"
	cd /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/build/InfoAcquisition && $(CMAKE_COMMAND) -P CMakeFiles/InfoAcquisition.dir/cmake_clean_target.cmake
	cd /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/build/InfoAcquisition && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/InfoAcquisition.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
InfoAcquisition/CMakeFiles/InfoAcquisition.dir/build: InfoAcquisition/libInfoAcquisition.a
.PHONY : InfoAcquisition/CMakeFiles/InfoAcquisition.dir/build

InfoAcquisition/CMakeFiles/InfoAcquisition.dir/clean:
	cd /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/build/InfoAcquisition && $(CMAKE_COMMAND) -P CMakeFiles/InfoAcquisition.dir/cmake_clean.cmake
.PHONY : InfoAcquisition/CMakeFiles/InfoAcquisition.dir/clean

InfoAcquisition/CMakeFiles/InfoAcquisition.dir/depend:
	cd /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/InfoAcquisition /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/build /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/build/InfoAcquisition /home/joaquin/Documents/Asignaturas/CESI/practica/BottomFlow/build/InfoAcquisition/CMakeFiles/InfoAcquisition.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : InfoAcquisition/CMakeFiles/InfoAcquisition.dir/depend

