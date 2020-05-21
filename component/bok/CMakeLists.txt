
# Boost
find_package(Boost
    REQUIRED
    COMPONENTS program_options filesystem
)

include_directories(
    "./src"
    ${Boost_INCLUDE_DIRS}
)

link_directories(${Boost_LIBRARY_DIRS})


set (target bok)

set (src
    ./src/main.cpp
    ./src/bok/cli/Controller.hpp
    ./src/bok/cli/build/BuildController.cpp
    ./src/bok/cli/build/BuildControllerOptions.hpp
    ./src/bok/cli/build/BuildController.hpp
    ./src/bok/cli/build/BuildControllerOptions.cpp
    ./src/bok/cli/ControllerStub.hpp
    ./src/bok/cli/ControllerFactory.cpp
    ./src/bok/cli/configure/ConfigureController.cpp
    ./src/bok/cli/configure/ConfigureController.hpp
    ./src/bok/cli/configure/ConfigureControllerOptions.hpp
    ./src/bok/cli/configure/ConfigureControllerOptions.cpp
    ./src/bok/cli/ControllerFactory.hpp
    ./src/bok/cli/init/InitController.cpp
    ./src/bok/cli/init/InitControllerOptions.hpp
    ./src/bok/cli/init/InitController.hpp
    ./src/bok/cli/init/InitControllerOptions.cpp
    ./src/bok/cli/Controller.cpp
)

add_executable(${target} ${src})

if (WIN32)
    set (PTHREAD_LIB )
else()
    set (PTHREAD_LIB pthread)
endif()

target_link_libraries(${target} ${Boost_LIBRARIES} ${PTHREAD_LIB} core feature)

install(
    TARGETS ${target}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
