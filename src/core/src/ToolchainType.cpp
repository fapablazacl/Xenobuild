

#include <Xenobuild/core/ToolchainType.h>


namespace Xenobuild {
    boost::optional<std::string> encode(const ToolchainType toolchainType) {
        switch (toolchainType) {
        case ToolchainType::Clang:
            return "clang";
            
        case ToolchainType::MicrosoftVC:
            return "vc";
            
        case ToolchainType::AppleClang:
            return "appleclang";
            
        case ToolchainType::GnuGCC:
            return "gcc";
        }

        return {};
    }

    boost::optional<ToolchainType> decode(const std::string &value) {
        if (value == "clang") {
            return ToolchainType::Clang;
        }
        else if (value == "vc") {
            return ToolchainType::MicrosoftVC;
        }
        else if (value == "appleclang") {
            return ToolchainType::AppleClang;
        }
        else if (value == "gcc") {
            return ToolchainType::GnuGCC;
        }

        return {};
    }
}
