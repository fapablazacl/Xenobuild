
set (target core-package)

set (sources 
    ./include/bok/core/package/Context.hpp
    ./include/bok/core/package/Package.hpp
    ./include/bok/core/package/Language.hpp
    ./include/bok/core/package/PackageRegistryFactory.hpp
    ./include/bok/core/package/PackageRegistry.hpp
    ./include/bok/core/package/PackageFactory.hpp
	./include/bok/core/package/PackageFactory_FS.hpp
	./include/bok/core/package/Module.hpp
	./include/bok/core/package/Source.hpp
	
    ./src/PackageFactory.cpp
    ./src/Package.cpp
	./src/PackageFactory_FS.cpp
    ./src/Module.cpp
    ./src/Source.cpp
    ./src/PackageRegistry.cpp
    ./src/Context.cpp
    ./src/PackageRegistryFactory.cpp
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
target_link_libraries(${target} core core-toolchain core-io)
