# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.21.4/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.21.4/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/build

# Include any dependencies generated for this target.
include bin/CMakeFiles/paex_record.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include bin/CMakeFiles/paex_record.dir/compiler_depend.make

# Include the progress variables for this target.
include bin/CMakeFiles/paex_record.dir/progress.make

# Include the compile flags for this target's objects.
include bin/CMakeFiles/paex_record.dir/flags.make

bin/CMakeFiles/paex_record.dir/paex_record.c.o: bin/CMakeFiles/paex_record.dir/flags.make
bin/CMakeFiles/paex_record.dir/paex_record.c.o: ../src/paex_record.c
bin/CMakeFiles/paex_record.dir/paex_record.c.o: bin/CMakeFiles/paex_record.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object bin/CMakeFiles/paex_record.dir/paex_record.c.o"
	cd /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/build/bin && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT bin/CMakeFiles/paex_record.dir/paex_record.c.o -MF CMakeFiles/paex_record.dir/paex_record.c.o.d -o CMakeFiles/paex_record.dir/paex_record.c.o -c /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/src/paex_record.c

bin/CMakeFiles/paex_record.dir/paex_record.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/paex_record.dir/paex_record.c.i"
	cd /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/build/bin && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/src/paex_record.c > CMakeFiles/paex_record.dir/paex_record.c.i

bin/CMakeFiles/paex_record.dir/paex_record.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/paex_record.dir/paex_record.c.s"
	cd /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/build/bin && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/src/paex_record.c -o CMakeFiles/paex_record.dir/paex_record.c.s

# Object files for target paex_record
paex_record_OBJECTS = \
"CMakeFiles/paex_record.dir/paex_record.c.o"

# External object files for target paex_record
paex_record_EXTERNAL_OBJECTS =

bin/paex_record: bin/CMakeFiles/paex_record.dir/paex_record.c.o
bin/paex_record: bin/CMakeFiles/paex_record.dir/build.make
bin/paex_record: bin/CMakeFiles/paex_record.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable paex_record"
	cd /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/build/bin && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/paex_record.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
bin/CMakeFiles/paex_record.dir/build: bin/paex_record
.PHONY : bin/CMakeFiles/paex_record.dir/build

bin/CMakeFiles/paex_record.dir/clean:
	cd /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/build/bin && $(CMAKE_COMMAND) -P CMakeFiles/paex_record.dir/cmake_clean.cmake
.PHONY : bin/CMakeFiles/paex_record.dir/clean

bin/CMakeFiles/paex_record.dir/depend:
	cd /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/src /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/build /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/build/bin /Users/mymac/Desktop/CS196/gazelle/test_automate_compress_playback/build/bin/CMakeFiles/paex_record.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : bin/CMakeFiles/paex_record.dir/depend

