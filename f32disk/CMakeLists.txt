# CMakeList.txt : CMake project for f32disk, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
file(GLOB_RECURSE PROJECT_SOURCE RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	./*)

add_executable (f32disk "f32disk.c" ${PROJECT_SOURCE})

target_include_directories(f32disk PUBLIC 
	${CMAKE_CURRENT_SOURCE_DIR}/
)

# Define the output directory for the binary
set(OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/../../bin)

# Ensure the output directory exists
file(MAKE_DIRECTORY ${OUTPUT_DIRECTORY})

# Custom command to copy the target to the bin directory
add_custom_command(TARGET f32disk POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:f32disk> ${OUTPUT_DIRECTORY}
    COMMENT "Copying f32disk to ${OUTPUT_DIRECTORY}"
)