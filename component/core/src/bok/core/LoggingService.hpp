
#ifndef __BOK_SERVICES_LOGGINGSERVICE_HPP__
#define __BOK_SERVICES_LOGGINGSERVICE_HPP__

#include <string>

namespace bok {
    class LoggingService {
    public:
        virtual ~LoggingService();

        virtual void log(const std::string &message) = 0;
        
        virtual void warn(const std::string &message) = 0;

        virtual void error(const std::string &message) = 0;
    };
}

#endif
