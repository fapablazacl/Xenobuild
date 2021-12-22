
set (target "Xenobuild")

set (sources 
    "include/Xenobuild/Controller.h"
    "include/Xenobuild/ControllerFactory.h"

    "src/main.cpp"
)

add_executable(${target} ${sources})

target_include_directories(${target} PUBLIC "./include/")
target_link_libraries(${target} xb-core)
# target_link_libraries(${target}  ${Boost_LIBRARIES})
