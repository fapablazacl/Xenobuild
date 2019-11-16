
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

set (src "src/Main.cpp")

set (src_borc_cli 
    "src/borc/cli/ControllerStub.hpp"
    "src/borc/cli/Controller.cpp"
    "src/borc/cli/Controller.hpp"
	"src/borc/cli/ControllerFactory.cpp"
    "src/borc/cli/ControllerFactory.hpp"
)

set (src_borc_cli_init
    "src/borc/cli/init/InitController.cpp"
    "src/borc/cli/init/InitController.hpp"
    "src/borc/cli/init/InitControllerOptions.cpp"
    "src/borc/cli/init/InitControllerOptions.hpp"
)

set (src_borc_cli_build
    "src/borc/cli/build/BuildController.cpp"
    "src/borc/cli/build/BuildController.hpp"
    "src/borc/cli/build/BuildControllerOptions.cpp"
    "src/borc/cli/build/BuildControllerOptions.hpp"
)

set (src_borc_cli_configure
    "src/borc/cli/configure/ConfigureController.cpp"
    "src/borc/cli/configure/ConfigureController.hpp"
    "src/borc/cli/configure/ConfigureControllerOptions.cpp"
    "src/borc/cli/configure/ConfigureControllerOptions.hpp"
)

source_group ("\\src" FILES ${src})
source_group ("\\src\\borc\\cli" FILES ${src_borc_cli})
source_group ("\\src\\borc\\cli\\init" FILES ${src_borc_cli_init})
source_group ("\\src\\borc\\cli\\build" FILES ${src_borc_cli_build})
source_group ("\\src\\borc\\cli\\configure" FILES ${src_borc_cli_configure})

add_executable(borc 
	${src}
	${src_borc_cli}
	${src_borc_cli_init}
	${src_borc_cli_build}
	${src_borc_cli_configure}
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
