
include_directories("../../dependencies/Catch2/single_include")
include_directories("../core/src")

set (target core-test)

set (src_core 
	"borc/core/CoreTest.cpp"
)

set (src_core_build )

set (src_core_pipeline 
	"borc/core/pipeline/RegexMatcherTest.cpp"
	"borc/core/pipeline/MatcherTest.cpp"
	"borc/core/pipeline/PipelineTest.cpp"
	"borc/core/pipeline/PipeTest.cpp"
)

set (src_core_toolchain 
	"borc/core/toolchain/ToolchainTest.cpp"
	"borc/core/toolchain/ToolTest.cpp"
	"borc/core/toolchain/ToolArgumentTest.cpp"
)

set (src_core_package
	"borc/core/package/PackageTest.cpp"
	"borc/core/package/ComponentTest.cpp"
	"borc/core/package/VersionTest.cpp"
)

source_group ("borc\\core" FILES ${src_core})
source_group ("borc\\core\\build" FILES ${src_core_build})
source_group ("borc\\core\\pipeline" FILES ${src_core_pipeline})
source_group ("borc\\core\\toolchain" FILES ${src_core_toolchain})
source_group ("borc\\core\\package" FILES ${src_core_package})

add_executable (${target} 
	${src_core} 
	${src_core_build} 
	${src_core_pipeline} 
	${src_core_toolchain}
	${src_core_package}
)

target_link_libraries(${target} core)
