
set (target "xenocli")
set (sources "src/xenocli.cpp")

add_executable(${target} ${sources})

target_link_libraries(${target} core libxenocli)

install(TARGETS ${target} DESTINATION bin)
