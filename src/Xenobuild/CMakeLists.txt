
set (target "Xenobuild")

set (sources 
    "include/Xenobuild/Common.h"
    "include/Xenobuild/Controller.h"
    "include/Xenobuild/ControllerFactory.h"
    "include/Xenobuild/BuildController.h"
    "include/Xenobuild/ConfigureController.h"
    "include/Xenobuild/SetupController.h"
    
    "src/main.cpp"
    "src/BuildController.cpp"
    "src/ConfigureController.cpp"
    "src/SetupController.cpp"
)

add_executable(${target} ${sources})

target_include_directories(${target} PUBLIC "./include/")
target_link_libraries(${target} core build-core)
