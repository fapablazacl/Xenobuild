
set (target xb)

set (sources
    ./src/main.cpp
    ./src/bok/cli/Controller.cpp
    ./src/bok/cli/Controller.h
    ./src/bok/cli/BuildController.cpp
    ./src/bok/cli/BuildController.h
    ./src/bok/cli/BuildController2.cpp
    ./src/bok/cli/BuildController2.h
    ./src/bok/cli/ConfigureController.cpp
    ./src/bok/cli/ConfigureController.h
    ./src/bok/cli/InitController.cpp
    ./src/bok/cli/InitController.h
    ./src/bok/cli/ControllerFactory.cpp
	./src/bok/cli/ControllerFactory.h
)

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
