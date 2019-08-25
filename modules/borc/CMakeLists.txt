
find_package(Boost
    REQUIRED
    COMPONENTS program_options filesystem
)

include_directories(
    "../borc-core/include"
    ${Boost_INCLUDE_DIRS}
)

link_directories(${Boost_LIBRARY_DIRS})

add_executable(borc 
    "src/Main.cpp"
    "src/borc/cli/Controller.cpp"
    "src/borc/cli/Controller.hpp"
    "src/borc/cli/BuildController.cpp"
    "src/borc/cli/BuildController.hpp"
    "src/borc/cli/ConfigureController.cpp"
    "src/borc/cli/ConfigureController.hpp"
    "src/borc/cli/ControllerFactory.cpp"
    "src/borc/cli/ControllerFactory.hpp"
)

target_link_libraries(borc borc-core ${Boost_LIBRARIES} pthread)

install(TARGETS borc
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
