
set (target core-toolchain-vc)

set (sources 
	"include/bok/core/toolchain/vc/ServiceFactoryVC.hpp"
	
	"src/ServiceFactoryVC.cpp"
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
target_link_libraries(${target} core core-toolchain core-package)
