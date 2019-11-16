

#ifndef __BORC_TOOLCHAIN_COMPILEOPTIONS_HPP__
#define __BORC_TOOLCHAIN_COMPILEOPTIONS_HPP__

#include <vector>
#include <string>

namespace borc {
	struct CompileOptions {
		std::vector<std::string> includePaths;

        void mergeWith(const CompileOptions &other);
	};
}

#endif
