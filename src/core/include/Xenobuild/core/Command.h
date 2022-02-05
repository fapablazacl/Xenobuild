#pragma once 

#include <thread>
#include <string>
#include <vector>
#include <iostream>

namespace Xenobuild {
    class CommandExecutorException : public std::runtime_error {
    public:
        CommandExecutorException(const std::string& msg, const int exitCode)
            : runtime_error(msg), exitCode(exitCode) {}

        int getExitCode() const {
            return exitCode;
        }

    private:
        const int exitCode;
    };
    
    struct CommandX {
        std::string name;
        std::vector<std::string> args;
    };
    
    struct CommandResult {
        int exitCode;
        std::vector<std::string> stdout;
        std::vector<std::string> stderr;
        
        operator bool() const {
            return exitCode == 0;
        }
    };
    
    struct CommandBatch {
        CommandBatch() {}

        CommandBatch(std::string name, std::vector<std::string> args) {
            commands.push_back(CommandX{name, args});
        }

        CommandBatch(const std::vector<CommandX> &commands) : commands(commands) {}

        CommandBatch(const CommandX &command) {
            commands.push_back(command);
        }

        std::vector<CommandX> commands;
    };

    class CommandExecutor {
    public:
        virtual ~CommandExecutor() {}

        virtual CommandResult execute(const CommandX& command) = 0;

        virtual CommandResult execute(const CommandBatch& batch) = 0;

        inline CommandResult operator() (const CommandX& command) {
            return execute(command);
        }

        inline CommandResult operator() (const CommandBatch& batch) {
            return execute(batch);
        }
    };

    class SystemCommandExecutor : public CommandExecutor {
    public:
        virtual ~SystemCommandExecutor() {}

        CommandResult execute(const CommandX& command) override {
            const std::string cmdline = createCmdLine(command);
            const int code = std::system(cmdline.c_str());
            
            return {code, {}, {}};
        }

        CommandResult execute(const CommandBatch& batch) override {
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

    private:
        std::string createCmdLine(const CommandX& command) const {
            return command.name + " " + boost::join(command.args, " ");
        }
    };

    
    class BoostProcessCommandExecutor : public CommandExecutor {
    public:
        virtual ~BoostProcessCommandExecutor() {}

        CommandResult execute(const CommandX& command) override {
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
        
        
        CommandResult execute(const CommandBatch& batch) override {
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

    private:
        std::vector<std::string> grabStreamOutput(boost::process::ipstream &stream) const {
            std::string line;
            std::vector<std::string> lines;

            while (stream && std::getline(stream, line) && !line.empty()) {
                lines.push_back(line);
            }

            return lines;
        }
    };
}
