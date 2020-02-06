
set (target core)

set (src_core_build 
	
)

set (src_core_pipeline 
	"borc/core/pipeline/RegexMatcher.hpp"
	"borc/core/pipeline/RegexMatcher.cpp"
	"borc/core/pipeline/Matcher.hpp"
	"borc/core/pipeline/Matcher.cpp"
	"borc/core/pipeline/Pipeline.hpp"
	"borc/core/pipeline/Pipeline.cpp"
	"borc/core/pipeline/Pipe.hpp"
	"borc/core/pipeline/Pipe.cpp"
)

set (src_core_toolchain 
	"borc/core/toolchain/Toolchain.hpp"
	"borc/core/toolchain/Toolchain.cpp"
	"borc/core/toolchain/Tool.hpp"
	"borc/core/toolchain/Tool.cpp"
	"borc/core/toolchain/ToolArgument.hpp"
	"borc/core/toolchain/ToolArgument.cpp"
)

source_group ("borc\\core\\build" FILES ${src_core_build})
source_group ("borc\\core\\pipeline" FILES ${src_core_pipeline})
source_group ("borc\\core\\toolchain" FILES ${src_core_toolchain})

add_library (${target} ${src_core_build} ${src_core_pipeline} ${src_core_toolchain} )
