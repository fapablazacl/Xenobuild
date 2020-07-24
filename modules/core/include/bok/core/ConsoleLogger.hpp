
#pragma once

#include "Logger.hpp"

namespace bok {
    class ConsoleLogger : public Logger {
    public:
        void message(const MessageType type, const std::string &text) override;
    };
}
