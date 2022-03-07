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
    

    inline std::ostream& operator<< (std::ostream &os, const CommandX &cmd) {
        os << "CommandX {" << cmd.name;
        
        if (cmd.args.size() > 0) {
            os << ", {";

            for (size_t i = 0; i < cmd.args.size(); i++) {
                os << cmd.args[i];

                if (i < cmd.args.size() - 1) {
                    os << ",";
                }
            }

            os << "}";
        }

        os << "}";

        return os;
    }


    struct CommandResult {
        int exitCode = 0;
        std::vector<std::string> out;
        std::vector<std::string> err;
        
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


    inline std::ostream& operator<< (std::ostream &os, const CommandBatch &batch) {
        os << "CommandBatch {" << std::endl;

        for (const CommandX &cmd : batch.commands) {
            os << "  " << cmd << std::endl;
        }

        os << "}" << std::endl;

        return os;
    }


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
