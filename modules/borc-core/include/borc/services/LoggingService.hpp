
#ifndef __BORC_SERVICES_LOGGINGSERVICE_HPP__
#define __BORC_SERVICES_LOGGINGSERVICE_HPP__

#include <string>

namespace borc {
    class LoggingService {
    public:
        virtual ~LoggingService();

        virtual void log(const std::string &message) = 0;
        
        virtual void warn(const std::string &message) = 0;

        virtual void error(const std::string &message) = 0;
    };
}

#endif
