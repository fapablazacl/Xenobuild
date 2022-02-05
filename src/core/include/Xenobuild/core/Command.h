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

        CommandResult execute(const CommandX& command) override;

        CommandResult execute(const CommandBatch& batch) override;
    };

    
    class BoostProcessCommandExecutor : public CommandExecutor {
    public:
        virtual ~BoostProcessCommandExecutor() {}

        CommandResult execute(const CommandX& command) override;
        
        CommandResult execute(const CommandBatch& batch) override;
    };
}
