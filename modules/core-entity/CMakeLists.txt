
set (target core-entity)
set (source_path "src/borc/core/entity")
set (sources 
	"${source_path}/Component.hpp"
	"${source_path}/Component.cpp"
	"${source_path}/Dependency.hpp"
	"${source_path}/Dependency.cpp"
	"${source_path}/Package.hpp"
	"${source_path}/Package.cpp"
	"${source_path}/Version.hpp"
	"${source_path}/Version.cpp"
)

source_group ("borc\\core\\entity" FILES ${sources})

add_library(${target} ${sources})
