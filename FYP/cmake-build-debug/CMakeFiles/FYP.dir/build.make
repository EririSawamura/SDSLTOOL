# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/eriri/Downloads/clion/clion-2019.2.4/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/eriri/Downloads/clion/clion-2019.2.4/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/eriri/CLionProjects/FYP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/eriri/CLionProjects/FYP/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/FYP.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FYP.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FYP.dir/flags.make

CMakeFiles/FYP.dir/main.cpp.o: CMakeFiles/FYP.dir/flags.make
CMakeFiles/FYP.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/eriri/CLionProjects/FYP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FYP.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FYP.dir/main.cpp.o -c /home/eriri/CLionProjects/FYP/main.cpp

CMakeFiles/FYP.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FYP.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/eriri/CLionProjects/FYP/main.cpp > CMakeFiles/FYP.dir/main.cpp.i

CMakeFiles/FYP.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FYP.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/eriri/CLionProjects/FYP/main.cpp -o CMakeFiles/FYP.dir/main.cpp.s

# Object files for target FYP
FYP_OBJECTS = \
"CMakeFiles/FYP.dir/main.cpp.o"

# External object files for target FYP
FYP_EXTERNAL_OBJECTS =

FYP: CMakeFiles/FYP.dir/main.cpp.o
FYP: CMakeFiles/FYP.dir/build.make
FYP: /usr/local/opencv/lib/libopencv_dnn.so.4.1.0
FYP: /usr/local/opencv/lib/libopencv_gapi.so.4.1.0
FYP: /usr/local/opencv/lib/libopencv_highgui.so.4.1.0
FYP: /usr/local/opencv/lib/libopencv_ml.so.4.1.0
FYP: /usr/local/opencv/lib/libopencv_objdetect.so.4.1.0
FYP: /usr/local/opencv/lib/libopencv_photo.so.4.1.0
FYP: /usr/local/opencv/lib/libopencv_stitching.so.4.1.0
FYP: /usr/local/opencv/lib/libopencv_video.so.4.1.0
FYP: /usr/local/opencv/lib/libopencv_videoio.so.4.1.0
FYP: /usr/local/lib/libsdsl.a
FYP: /usr/local/lib/libdivsufsort.a
FYP: /usr/local/lib/libdivsufsort64.a
FYP: /usr/local/opencv/lib/libopencv_imgcodecs.so.4.1.0
FYP: /usr/local/opencv/lib/libopencv_calib3d.so.4.1.0
FYP: /usr/local/opencv/lib/libopencv_features2d.so.4.1.0
FYP: /usr/local/opencv/lib/libopencv_flann.so.4.1.0
FYP: /usr/local/opencv/lib/libopencv_imgproc.so.4.1.0
FYP: /usr/local/opencv/lib/libopencv_core.so.4.1.0
FYP: CMakeFiles/FYP.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/eriri/CLionProjects/FYP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable FYP"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FYP.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FYP.dir/build: FYP

.PHONY : CMakeFiles/FYP.dir/build

CMakeFiles/FYP.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FYP.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FYP.dir/clean

CMakeFiles/FYP.dir/depend:
	cd /home/eriri/CLionProjects/FYP/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/eriri/CLionProjects/FYP /home/eriri/CLionProjects/FYP /home/eriri/CLionProjects/FYP/cmake-build-debug /home/eriri/CLionProjects/FYP/cmake-build-debug /home/eriri/CLionProjects/FYP/cmake-build-debug/CMakeFiles/FYP.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FYP.dir/depend
