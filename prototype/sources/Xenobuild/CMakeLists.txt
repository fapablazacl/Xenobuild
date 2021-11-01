
cmake_minimum_required(VERSION 3.10)

project(Xenobuild VERSION 1.0.0 LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 17)

# Boost dependencies
find_package (Boost 1.71 REQUIRED)

# Google Fruit dependency
# Installation of Google Fruit doesn't have a FindFruit.cmake script.
if (NOT GOOGLE_FRUIT_INSTALL_PREFIX)
	message (FATAL_ERROR "Required variable not defined. 'GOOGLE_FRUIT_INSTALL_PREFIX'")
endif ()

# main executable
set (target "Xenobuild")
set (sources "src/main.cpp")

add_executable (${target} ${sources})
target_include_directories (${target} PUBLIC "${GOOGLE_FRUIT_INSTALL_PREFIX}/include")
target_link_directories (${target} PUBLIC "${GOOGLE_FRUIT_INSTALL_PREFIX}/lib")
target_link_libraries (${target} PUBLIC Boost::boost Fruit)
