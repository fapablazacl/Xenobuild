
find_package(Boost
    REQUIRED
    COMPONENTS program_options filesystem
)

include_directories(
    "../borc-core/include"
    "./src"
    ${Boost_INCLUDE_DIRS}
)

link_directories(${Boost_LIBRARY_DIRS})

add_executable(borc 
    "src/Main.cpp"
    "src/borc/cli/ControllerStub.hpp"
    "src/borc/cli/Controller.cpp"
    "src/borc/cli/Controller.hpp"
    "src/borc/cli/build/BuildController.cpp"
    "src/borc/cli/build/BuildController.hpp"
    "src/borc/cli/build/BuildControllerOptions.cpp"
    "src/borc/cli/build/BuildControllerOptions.hpp"
    "src/borc/cli/configure/ConfigureController.cpp"
    "src/borc/cli/configure/ConfigureController.hpp"
    "src/borc/cli/configure/ConfigureControllerOptions.cpp"
    "src/borc/cli/configure/ConfigureControllerOptions.hpp"
    "src/borc/cli/ControllerFactory.cpp"
    "src/borc/cli/ControllerFactory.hpp"
)

if (WIN32)
	set (PTHREAD_LIB )
else()
	set (PTHREAD_LIB pthread)
endif()

target_link_libraries(borc borc-core ${Boost_LIBRARIES} ${PTHREAD_LIB})

install(TARGETS borc
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
