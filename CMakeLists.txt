cmake_minimum_required(VERSION 2.6)
project(RayTracer)

# Set the build type
set(CMAKE_BUILD_TYPE Release)

# Set the directory to the root
set(RAT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# Setup where we want to build the output binaries
set(EXECUTABLE_OUTPUT_PATH ${RAT_ROOT}/bin/)

# Add the source directories to the search path
include_directories(${RAT_ROOT}/src/)
include_directories(${RAT_ROOT}/include/)

#
#
# Make the pano_viewer program
file(GLOB_RECURSE RAY_SRC ${RAT_ROOT}/src/*.cpp
						  ${RAT_ROOT}/include/*.cpp)
add_executable(ray_trace ${RAY_SRC})




