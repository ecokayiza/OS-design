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
CMAKE_SOURCE_DIR = /home/eco/桌面/design

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eco/桌面/design/build

# Include any dependencies generated for this target.
include CMakeFiles/time.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/time.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/time.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/time.dir/flags.make

CMakeFiles/time.dir/util/time_util.c.o: CMakeFiles/time.dir/flags.make
CMakeFiles/time.dir/util/time_util.c.o: ../util/time_util.c
CMakeFiles/time.dir/util/time_util.c.o: CMakeFiles/time.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eco/桌面/design/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/time.dir/util/time_util.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/time.dir/util/time_util.c.o -MF CMakeFiles/time.dir/util/time_util.c.o.d -o CMakeFiles/time.dir/util/time_util.c.o -c /home/eco/桌面/design/util/time_util.c

CMakeFiles/time.dir/util/time_util.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/time.dir/util/time_util.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/eco/桌面/design/util/time_util.c > CMakeFiles/time.dir/util/time_util.c.i

CMakeFiles/time.dir/util/time_util.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/time.dir/util/time_util.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/eco/桌面/design/util/time_util.c -o CMakeFiles/time.dir/util/time_util.c.s

# Object files for target time
time_OBJECTS = \
"CMakeFiles/time.dir/util/time_util.c.o"

# External object files for target time
time_EXTERNAL_OBJECTS =

libtime.so: CMakeFiles/time.dir/util/time_util.c.o
libtime.so: CMakeFiles/time.dir/build.make
libtime.so: CMakeFiles/time.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eco/桌面/design/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library libtime.so"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/time.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/time.dir/build: libtime.so
.PHONY : CMakeFiles/time.dir/build

CMakeFiles/time.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/time.dir/cmake_clean.cmake
.PHONY : CMakeFiles/time.dir/clean

CMakeFiles/time.dir/depend:
	cd /home/eco/桌面/design/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eco/桌面/design /home/eco/桌面/design /home/eco/桌面/design/build /home/eco/桌面/design/build /home/eco/桌面/design/build/CMakeFiles/time.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/time.dir/depend

