
#pragma once

#include <string>

namespace bok {
    class Logger {
    public:
        enum MessageType {
            Info,
            Warn,
            Error
        };

    public:
        virtual ~Logger();

        virtual void message(const MessageType type, const std::string &text) = 0;

        void info(const std::string &text) {
            this->message(Info, text);
        }

        void warn(const std::string &text) {
            this->message(Warn, text);
        }

        void error(const std::string &text) {
            this->message(Error, text);
        }
    };
}
