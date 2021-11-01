
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set (target grammar)
set (sources ast/Node.hpp Driver.cpp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

flex_target(borc_scanner lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
bison_target(borc_parser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)

add_flex_bison_dependency(borc_scanner borc_parser)

add_executable(
    ${target} 
    ${sources}
    ${BISON_borc_parser_OUTPUTS}
    ${FLEX_borc_scanner_OUTPUTS}
)

target_link_libraries(${target} fl)
