
#ifndef __BOK_MODEL_COMMAND_COMMAND_HPP__
#define __BOK_MODEL_COMMAND_COMMAND_HPP__

#include <string>
#include <vector>
#include <ostream>
#include <boost/filesystem/path.hpp>

namespace Xenobuild {
    struct CommandData {
        boost::filesystem::path path;
        std::string name;
        std::vector<std::string> args;
        
        friend std::ostream& operator<< (std::ostream& os, const CommandData& commandData) {
            os << commandData.name << " (";

            for (const auto& arg : commandData.args) {
                os << "'" << arg << "' ";
            }

            os << ")";

            return os;
        }
    };

    class Command {
    public:
        virtual ~Command();

        [[deprecated]]
        virtual void execute() = 0;
    };
}

#endif
