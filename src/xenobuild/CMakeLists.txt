
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
    ./src/bok/cli/Controller.cpp
    ./src/bok/cli/Controller.hpp
    ./src/bok/cli/BuildController.cpp
    ./src/bok/cli/BuildController.hpp
    ./src/bok/cli/BuildController2.cpp
    ./src/bok/cli/BuildController2.hpp
    ./src/bok/cli/ConfigureController.cpp
    ./src/bok/cli/ConfigureController.hpp
    ./src/bok/cli/InitController.cpp
    ./src/bok/cli/InitController.hpp
    ./src/bok/cli/ControllerFactory.cpp
	./src/bok/cli/ControllerFactory.hpp
)

source_group(\\ FILES ${src})

add_executable(${target} ${src})

if (WIN32)
    set (PTHREAD_LIB )
else()
    set (PTHREAD_LIB pthread)
endif()

target_link_libraries(${target} 
	${Boost_LIBRARIES} 
	${PTHREAD_LIB} 
	core 
	core-io 
	core-pipeline 
	core-package
	core-toolchain 
	# core-toolchain-clang 
	# core-toolchain-gcc 
	# core-toolchain-vc 
	feature
	yaml-cpp
)

install(
    TARGETS ${target}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
