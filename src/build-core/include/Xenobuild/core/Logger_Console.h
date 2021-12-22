
#pragma once

#include "Logger.h"

namespace Xenobuild {
    class Logger_Console : public Logger {
    public:
        void message(const MessageType type, const std::string &text) override;
    };
}
