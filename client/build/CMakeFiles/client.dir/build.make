# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_SOURCE_DIR = /home/ubuntu/workspace/project/3project/client

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/workspace/project/3project/client/build

# Include any dependencies generated for this target.
include CMakeFiles/client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client.dir/flags.make

CMakeFiles/client.dir/src/bmp.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/bmp.c.o: ../src/bmp.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/workspace/project/3project/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/client.dir/src/bmp.c.o"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/src/bmp.c.o   -c /home/ubuntu/workspace/project/3project/client/src/bmp.c

CMakeFiles/client.dir/src/bmp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/bmp.c.i"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/workspace/project/3project/client/src/bmp.c > CMakeFiles/client.dir/src/bmp.c.i

CMakeFiles/client.dir/src/bmp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/bmp.c.s"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/workspace/project/3project/client/src/bmp.c -o CMakeFiles/client.dir/src/bmp.c.s

CMakeFiles/client.dir/src/card.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/card.c.o: ../src/card.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/workspace/project/3project/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/client.dir/src/card.c.o"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/src/card.c.o   -c /home/ubuntu/workspace/project/3project/client/src/card.c

CMakeFiles/client.dir/src/card.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/card.c.i"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/workspace/project/3project/client/src/card.c > CMakeFiles/client.dir/src/card.c.i

CMakeFiles/client.dir/src/card.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/card.c.s"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/workspace/project/3project/client/src/card.c -o CMakeFiles/client.dir/src/card.c.s

CMakeFiles/client.dir/src/font.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/font.c.o: ../src/font.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/workspace/project/3project/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/client.dir/src/font.c.o"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/src/font.c.o   -c /home/ubuntu/workspace/project/3project/client/src/font.c

CMakeFiles/client.dir/src/font.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/font.c.i"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/workspace/project/3project/client/src/font.c > CMakeFiles/client.dir/src/font.c.i

CMakeFiles/client.dir/src/font.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/font.c.s"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/workspace/project/3project/client/src/font.c -o CMakeFiles/client.dir/src/font.c.s

CMakeFiles/client.dir/src/fsm.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/fsm.c.o: ../src/fsm.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/workspace/project/3project/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/client.dir/src/fsm.c.o"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/src/fsm.c.o   -c /home/ubuntu/workspace/project/3project/client/src/fsm.c

CMakeFiles/client.dir/src/fsm.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/fsm.c.i"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/workspace/project/3project/client/src/fsm.c > CMakeFiles/client.dir/src/fsm.c.i

CMakeFiles/client.dir/src/fsm.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/fsm.c.s"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/workspace/project/3project/client/src/fsm.c -o CMakeFiles/client.dir/src/fsm.c.s

CMakeFiles/client.dir/src/game.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/game.c.o: ../src/game.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/workspace/project/3project/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/client.dir/src/game.c.o"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/src/game.c.o   -c /home/ubuntu/workspace/project/3project/client/src/game.c

CMakeFiles/client.dir/src/game.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/game.c.i"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/workspace/project/3project/client/src/game.c > CMakeFiles/client.dir/src/game.c.i

CMakeFiles/client.dir/src/game.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/game.c.s"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/workspace/project/3project/client/src/game.c -o CMakeFiles/client.dir/src/game.c.s

CMakeFiles/client.dir/src/lcd.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/lcd.c.o: ../src/lcd.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/workspace/project/3project/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building C object CMakeFiles/client.dir/src/lcd.c.o"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/src/lcd.c.o   -c /home/ubuntu/workspace/project/3project/client/src/lcd.c

CMakeFiles/client.dir/src/lcd.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/lcd.c.i"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/workspace/project/3project/client/src/lcd.c > CMakeFiles/client.dir/src/lcd.c.i

CMakeFiles/client.dir/src/lcd.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/lcd.c.s"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/workspace/project/3project/client/src/lcd.c -o CMakeFiles/client.dir/src/lcd.c.s

CMakeFiles/client.dir/src/main.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/main.c.o: ../src/main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/workspace/project/3project/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building C object CMakeFiles/client.dir/src/main.c.o"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/src/main.c.o   -c /home/ubuntu/workspace/project/3project/client/src/main.c

CMakeFiles/client.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/main.c.i"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/workspace/project/3project/client/src/main.c > CMakeFiles/client.dir/src/main.c.i

CMakeFiles/client.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/main.c.s"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/workspace/project/3project/client/src/main.c -o CMakeFiles/client.dir/src/main.c.s

CMakeFiles/client.dir/src/mutex.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/mutex.c.o: ../src/mutex.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/workspace/project/3project/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building C object CMakeFiles/client.dir/src/mutex.c.o"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/src/mutex.c.o   -c /home/ubuntu/workspace/project/3project/client/src/mutex.c

CMakeFiles/client.dir/src/mutex.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/mutex.c.i"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/workspace/project/3project/client/src/mutex.c > CMakeFiles/client.dir/src/mutex.c.i

CMakeFiles/client.dir/src/mutex.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/mutex.c.s"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/workspace/project/3project/client/src/mutex.c -o CMakeFiles/client.dir/src/mutex.c.s

CMakeFiles/client.dir/src/table.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/table.c.o: ../src/table.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/workspace/project/3project/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building C object CMakeFiles/client.dir/src/table.c.o"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/src/table.c.o   -c /home/ubuntu/workspace/project/3project/client/src/table.c

CMakeFiles/client.dir/src/table.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/table.c.i"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/workspace/project/3project/client/src/table.c > CMakeFiles/client.dir/src/table.c.i

CMakeFiles/client.dir/src/table.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/table.c.s"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/workspace/project/3project/client/src/table.c -o CMakeFiles/client.dir/src/table.c.s

CMakeFiles/client.dir/src/tcp.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/tcp.c.o: ../src/tcp.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/workspace/project/3project/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building C object CMakeFiles/client.dir/src/tcp.c.o"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/src/tcp.c.o   -c /home/ubuntu/workspace/project/3project/client/src/tcp.c

CMakeFiles/client.dir/src/tcp.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/tcp.c.i"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/workspace/project/3project/client/src/tcp.c > CMakeFiles/client.dir/src/tcp.c.i

CMakeFiles/client.dir/src/tcp.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/tcp.c.s"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/workspace/project/3project/client/src/tcp.c -o CMakeFiles/client.dir/src/tcp.c.s

CMakeFiles/client.dir/src/touch.c.o: CMakeFiles/client.dir/flags.make
CMakeFiles/client.dir/src/touch.c.o: ../src/touch.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/workspace/project/3project/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building C object CMakeFiles/client.dir/src/touch.c.o"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/client.dir/src/touch.c.o   -c /home/ubuntu/workspace/project/3project/client/src/touch.c

CMakeFiles/client.dir/src/touch.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/client.dir/src/touch.c.i"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/ubuntu/workspace/project/3project/client/src/touch.c > CMakeFiles/client.dir/src/touch.c.i

CMakeFiles/client.dir/src/touch.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/client.dir/src/touch.c.s"
	/usr/arm/5.4.0/usr/bin/arm-linux-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/ubuntu/workspace/project/3project/client/src/touch.c -o CMakeFiles/client.dir/src/touch.c.s

# Object files for target client
client_OBJECTS = \
"CMakeFiles/client.dir/src/bmp.c.o" \
"CMakeFiles/client.dir/src/card.c.o" \
"CMakeFiles/client.dir/src/font.c.o" \
"CMakeFiles/client.dir/src/fsm.c.o" \
"CMakeFiles/client.dir/src/game.c.o" \
"CMakeFiles/client.dir/src/lcd.c.o" \
"CMakeFiles/client.dir/src/main.c.o" \
"CMakeFiles/client.dir/src/mutex.c.o" \
"CMakeFiles/client.dir/src/table.c.o" \
"CMakeFiles/client.dir/src/tcp.c.o" \
"CMakeFiles/client.dir/src/touch.c.o"

# External object files for target client
client_EXTERNAL_OBJECTS =

client: CMakeFiles/client.dir/src/bmp.c.o
client: CMakeFiles/client.dir/src/card.c.o
client: CMakeFiles/client.dir/src/font.c.o
client: CMakeFiles/client.dir/src/fsm.c.o
client: CMakeFiles/client.dir/src/game.c.o
client: CMakeFiles/client.dir/src/lcd.c.o
client: CMakeFiles/client.dir/src/main.c.o
client: CMakeFiles/client.dir/src/mutex.c.o
client: CMakeFiles/client.dir/src/table.c.o
client: CMakeFiles/client.dir/src/tcp.c.o
client: CMakeFiles/client.dir/src/touch.c.o
client: CMakeFiles/client.dir/build.make
client: CMakeFiles/client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/workspace/project/3project/client/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking C executable client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client.dir/build: client

.PHONY : CMakeFiles/client.dir/build

CMakeFiles/client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client.dir/clean

CMakeFiles/client.dir/depend:
	cd /home/ubuntu/workspace/project/3project/client/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/workspace/project/3project/client /home/ubuntu/workspace/project/3project/client /home/ubuntu/workspace/project/3project/client/build /home/ubuntu/workspace/project/3project/client/build /home/ubuntu/workspace/project/3project/client/build/CMakeFiles/client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client.dir/depend

