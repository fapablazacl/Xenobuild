
#include <Xenobuild/core/Command.h>

#include <boost/algorithm/string/join.hpp>
#include <boost/process.hpp>


namespace Xenobuild {
    static std::string createCmdLine(const CommandX& command) {
        return command.name + " " + boost::join(command.args, " ");
    }
    
    static std::vector<std::string> grabStreamOutput(boost::process::ipstream &stream) {
        std::string line;
        std::vector<std::string> lines;

        while (stream && std::getline(stream, line) && !line.empty()) {
            lines.push_back(line);
        }

        return lines;
    }
}


namespace Xenobuild {
    CommandResult SystemCommandExecutor::execute(const CommandX& command)  {
        const std::string cmdline = createCmdLine(command);
        const int code = std::system(cmdline.c_str());
        
        return {code, {}, {}};
    }

    CommandResult SystemCommandExecutor::execute(const CommandBatch& batch)  {
        std::vector<std::string> cmdlines;

        std::transform(
            batch.commands.begin(),
            batch.commands.end(),
            std::back_inserter(cmdlines), [this](const auto command) {
            return createCmdLine(command);
        });

        const std::string cmdline = boost::join(cmdlines, "&&");

        const int code = std::system(cmdline.c_str());
        
        return {code, {}, {}};
    }
}


namespace Xenobuild {
    CommandResult BoostProcessCommandExecutor::execute(const CommandX& command)  {
        boost::filesystem::path commandPath = boost::process::search_path(command.name);
        
        if (! boost::filesystem::exists(commandPath)) {
            throw std::runtime_error("\"" + command.name + "\": Command couldn't be located within the current environment.");
        }
        
        boost::process::ipstream stdoutStream;
        boost::process::ipstream stderrStream;
        
        const std::string commandLine = commandPath.string() + " " + boost::join(command.args, " ");
        
        boost::process::child process {
            // commandPath, boost::process::args += command.args,
            commandLine,
            boost::process::std_out > stdoutStream,
            boost::process::std_err > stderrStream
        };

        const std::vector<std::string> stdoutOutput = grabStreamOutput(stdoutStream);
        const std::vector<std::string> stderrOutput = grabStreamOutput(stderrStream);

        process.wait();
        
        return {process.exit_code(), stdoutOutput, stderrOutput };
    }
    
    
    CommandResult BoostProcessCommandExecutor::execute(const CommandBatch& batch)  {
        assert(batch.commands.size() < 2);
        
        if (batch.commands.size() == 0) {
            return {0, {}, {}};
        }
        
        if (batch.commands.size() == 1) {
            return execute(batch.commands[0]);
        }
        
        // TODO: Add implementation
        return {0, {}, {}};
    }
}
