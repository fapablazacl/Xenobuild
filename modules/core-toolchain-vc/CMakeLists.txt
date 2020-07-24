
set (target core-toolchain-vc)

set (sources 
	"include/bok/toolchain/vc/ServiceFactoryVC.hpp"
	
	"src/ServiceFactoryVC.cpp"
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
