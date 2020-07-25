
#include <bok/core/Logger_Console.hpp>

#include <iostream>

namespace bok {
    void Logger_Console::message(const MessageType type, const std::string &text) {
        std::string prefix;

        switch (type) {
            case MessageType::Info: prefix = "[INFO]"; break;
            case MessageType::Warn: prefix = "[WARN]"; break;
            case MessageType::Error: prefix = "[ERROR]"; break;
        }

        std::cout << prefix << " " << text << std::endl;
    }
}
