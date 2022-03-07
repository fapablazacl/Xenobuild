
set (target "libxenocli-test")
set (sources 
    #"libxenocli-test.cpp"
    #"ConfigureControllerTest.cpp"
    "libxenocli-test-gtest.cpp"
)

add_executable(${target} ${sources})
target_link_libraries(${target} libxenocli gtest gmock)
