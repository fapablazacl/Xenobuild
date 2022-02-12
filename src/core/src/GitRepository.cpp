
#include <Xenobuild/core/GitRepository.h>

#include <boost/filesystem.hpp>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Util.h>


namespace Xenobuild {
    GitRepository::GitRepository(const std::string& url, const std::string& tag)
        : url(url), tag(tag) {}

    CommandResult GitRepository::clone(CommandExecutor &execute, const boost::filesystem::path &sourcePath) const {
        if (boost::filesystem::exists(sourcePath)) {
            return {0, {}, {}};
        }

        CommandX command{
            "git", {"clone"}
        };

        if (tag != "") {
            command.args.push_back("--branch " + tag);
        }
        
        command.args.push_back("--depth 1");
        command.args.push_back(url);
        command.args.push_back(sourcePath.string());

        return execute(command);
    }
}
