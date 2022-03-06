
set (target "libxenocli-test")
set (sources 
    "libxenocli-test.cpp"
    "ConfigureControllerTest.cpp"
)

add_executable(${target} ${sources})
target_link_libraries(${target} libxenocli Catch2WithMain)
