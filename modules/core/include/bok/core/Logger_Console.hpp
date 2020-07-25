
#pragma once

#include "Logger.hpp"

namespace bok {
    class Logger_Console : public Logger {
    public:
        void message(const MessageType type, const std::string &text) override;
    };
}
