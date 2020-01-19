
#ifndef __BORC_SERVICES_LOGGINGSERVICEIMPL_HPP__
#define __BORC_SERVICES_LOGGINGSERVICEIMPL_HPP__

#include "LoggingService.hpp"

namespace borc {
    class LoggingServiceImpl : public LoggingService {
    public:
        explicit LoggingServiceImpl(const std::string &name);

        virtual ~LoggingServiceImpl();

        virtual void log(const std::string &message) override;
        
        virtual void warn(const std::string &message) override;

        virtual void error(const std::string &message) override;

    private:
        std::string name;
    };
}

#endif
