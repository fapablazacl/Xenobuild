
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set (target libborc)

set (sources
    borc/pod/Buildable.hpp
    borc/pod/Buildable.cpp
    borc/pod/Project.hpp 
    borc/pod/Project.cpp
    borc/BorcFile.hpp
    borc/BorcFile.cpp
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

flex_target(borc_scanner borc/grammar/BorcFile.l ${CMAKE_CURRENT_BINARY_DIR}/BorcFile.l.cpp)
bison_target(borc_parser borc/grammar/BorcFile.yy ${CMAKE_CURRENT_BINARY_DIR}/BorcFile.yy.cpp)
add_flex_bison_dependency(borc_scanner borc_parser)

add_library(
    ${target} SHARED 
    ${sources}
    ${BISON_borc_parser_OUTPUTS}
    ${FLEX_borc_scanner_OUTPUTS}
)

target_link_libraries(${target} fl)
