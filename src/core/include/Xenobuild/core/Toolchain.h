
#pragma once 

#include <iostream>
#include <string>
#include <vector>
#include <boost/filesystem/path.hpp>
#include <boost/optional.hpp>


namespace Xenobuild {
    struct CommandX;
    struct CommandBatch;

    enum class ToolchainType {
        Clang,
        MicrosoftVC,
        AppleClang,
        GnuGCC
    };


    inline std::ostream& operator<< (std::ostream &ostream, const ToolchainType toolchain) {
        switch (toolchain) {
        case ToolchainType::Clang:
            ostream << "ToolchainType::Clang";
            break;
            
        case ToolchainType::MicrosoftVC:
            ostream << "ToolchainType::MicrosoftVC";
            break;
            
        case ToolchainType::AppleClang:
            ostream << "ToolchainType::AppleClang";
            break;
            
        case ToolchainType::GnuGCC:
            ostream << "ToolchainType::GnuGCC";
            break;
            
        default:
            ostream << "<ToolchainType: Missing case>";
            break;
        }
        
        return ostream;
    }

    /**
     * @brief Compiler toolchain abstraction.
     */
    class Toolchain {
    public:
        explicit Toolchain(const ToolchainType type);

        /**
         * @brief Creates a new CommandBatch, that has an implicit command to setup 
         * the toolchain environment ready for operation, if its required.
         */
        CommandBatch createCommandBatch(const CommandX command);

    private:
        ToolchainType type;
        boost::optional<std::string> installPath;
    };

    std::vector<std::string> enumerateVCInstallations();

    CommandX createVCVars64Command(const boost::filesystem::path &prefixPath);

    CommandBatch createToolchainCommandBatch(const CommandX command, const boost::filesystem::path &toolchainPrefix);
}
