
project(borc)

cmake_minimum_required(VERSION 3.7)

include_directories(src/libborc)

add_subdirectory(src/grammar)
add_subdirectory(src/borc)
add_subdirectory(src/libborc)
add_subdirectory(src/borc-tdd)
