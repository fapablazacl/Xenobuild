
include_directories("../../dependencies/Catch2/single_include")

include_directories("../core-entity/src")

set (target core-entity-test)

set (source_path "src/borc/core/entity")
set (sources 
	"${source_path}/TestComponent.cpp"
	"${source_path}/TestDependency.cpp"
	"${source_path}/TestPackage.cpp"
	"${source_path}/TestVersion.cpp"
)

source_group ("borc\\core\\entity" FILES ${sources})
source_group ("borc\\core\\entity" FILES ${sources})

add_executable(${target} CoreEntityTest.cpp ${sources})

target_link_libraries(${target} core-entity)
