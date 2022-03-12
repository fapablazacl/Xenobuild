
set (target "libxenocli")

set (sources 
    "include/Xenobuild/Common.h"
    "include/Xenobuild/Controller.h"
    "include/Xenobuild/ControllerFactory.h"
    "include/Xenobuild/BuildController.h"
    "include/Xenobuild/ConfigureController.h"
    "include/Xenobuild/ConfigureService.h"
    "include/Xenobuild/SetupController.h"
    
    "src/BuildController.cpp"
    "src/ConfigureController.cpp"
    "src/ConfigureService.cpp"
    "src/SetupController.cpp"
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "./include/")
target_link_libraries(${target} core)

if (XENOBUILD_ENABLE_TESTS)
    add_subdirectory("test")
endif()
