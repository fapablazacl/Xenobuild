
set (target utility)

set (sources 
	"./include/bok/utility/string.hpp"
	"./src/string.cpp"
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
