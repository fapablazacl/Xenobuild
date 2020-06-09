
set (target utility)

set (sources 
	"./include/bok/utility/string.hpp"
	"./src/string.cpp"
)

add_library(${target} ${sources})
