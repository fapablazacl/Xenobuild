
set (target "libxenocli-test")
set (sources 
    "ConfigureControllerTest.cpp"
    "ConfigureServiceTest.cpp"
    "libxenocli-test.cpp"
)

add_executable(${target} ${sources})
target_link_libraries(${target} libxenocli gtest gmock)

add_test(NAME ${target} COMMAND ${target})
