
set (target core-io)

set (sources 
	"./include/bok/core/io/ComponentEntity.hpp"
	"./include/bok/core/io/Decoder.hpp"
	"./include/bok/core/io/Encoder.hpp"
	"./include/bok/core/io/JsonModel.hpp"
	"./include/bok/core/io/LanguageEntity.hpp"
	"./include/bok/core/io/PackageEntity.hpp"
	"./include/bok/core/io/ToolchainEntity.hpp"
	"./include/bok/core/io/YamlModel.hpp" 
	
	src/Foo.cpp
)

add_library(${target} ${sources})

target_include_directories(${target} PUBLIC "include/")
target_link_libraries(${target} core)
