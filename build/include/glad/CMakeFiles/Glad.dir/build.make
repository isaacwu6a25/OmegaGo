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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.19.6/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.19.6/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/isaacwu/Documents/GitHub/OmegaGo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/isaacwu/Documents/GitHub/OmegaGo/build

# Include any dependencies generated for this target.
include include/glad/CMakeFiles/Glad.dir/depend.make

# Include the progress variables for this target.
include include/glad/CMakeFiles/Glad.dir/progress.make

# Include the compile flags for this target's objects.
include include/glad/CMakeFiles/Glad.dir/flags.make

include/glad/CMakeFiles/Glad.dir/glad.c.o: include/glad/CMakeFiles/Glad.dir/flags.make
include/glad/CMakeFiles/Glad.dir/glad.c.o: ../include/glad/glad.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/isaacwu/Documents/GitHub/OmegaGo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object include/glad/CMakeFiles/Glad.dir/glad.c.o"
	cd /Users/isaacwu/Documents/GitHub/OmegaGo/build/include/glad && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Glad.dir/glad.c.o -c /Users/isaacwu/Documents/GitHub/OmegaGo/include/glad/glad.c

include/glad/CMakeFiles/Glad.dir/glad.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Glad.dir/glad.c.i"
	cd /Users/isaacwu/Documents/GitHub/OmegaGo/build/include/glad && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/isaacwu/Documents/GitHub/OmegaGo/include/glad/glad.c > CMakeFiles/Glad.dir/glad.c.i

include/glad/CMakeFiles/Glad.dir/glad.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Glad.dir/glad.c.s"
	cd /Users/isaacwu/Documents/GitHub/OmegaGo/build/include/glad && /Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/isaacwu/Documents/GitHub/OmegaGo/include/glad/glad.c -o CMakeFiles/Glad.dir/glad.c.s

# Object files for target Glad
Glad_OBJECTS = \
"CMakeFiles/Glad.dir/glad.c.o"

# External object files for target Glad
Glad_EXTERNAL_OBJECTS =

include/glad/libGlad.a: include/glad/CMakeFiles/Glad.dir/glad.c.o
include/glad/libGlad.a: include/glad/CMakeFiles/Glad.dir/build.make
include/glad/libGlad.a: include/glad/CMakeFiles/Glad.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/isaacwu/Documents/GitHub/OmegaGo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library libGlad.a"
	cd /Users/isaacwu/Documents/GitHub/OmegaGo/build/include/glad && $(CMAKE_COMMAND) -P CMakeFiles/Glad.dir/cmake_clean_target.cmake
	cd /Users/isaacwu/Documents/GitHub/OmegaGo/build/include/glad && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Glad.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
include/glad/CMakeFiles/Glad.dir/build: include/glad/libGlad.a

.PHONY : include/glad/CMakeFiles/Glad.dir/build

include/glad/CMakeFiles/Glad.dir/clean:
	cd /Users/isaacwu/Documents/GitHub/OmegaGo/build/include/glad && $(CMAKE_COMMAND) -P CMakeFiles/Glad.dir/cmake_clean.cmake
.PHONY : include/glad/CMakeFiles/Glad.dir/clean

include/glad/CMakeFiles/Glad.dir/depend:
	cd /Users/isaacwu/Documents/GitHub/OmegaGo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/isaacwu/Documents/GitHub/OmegaGo /Users/isaacwu/Documents/GitHub/OmegaGo/include/glad /Users/isaacwu/Documents/GitHub/OmegaGo/build /Users/isaacwu/Documents/GitHub/OmegaGo/build/include/glad /Users/isaacwu/Documents/GitHub/OmegaGo/build/include/glad/CMakeFiles/Glad.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : include/glad/CMakeFiles/Glad.dir/depend

