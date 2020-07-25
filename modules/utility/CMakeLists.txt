
set (target utility)

set (sources 
	"./include/bok/utility/string.hpp"
	"./include/bok/utility/WildcardClassifier.hpp"

	"./src/string.cpp"
	"./src/WildcardClassifier.cpp"
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
