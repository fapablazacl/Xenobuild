
#include <bok/core/LoggingServiceImpl.hpp>

#include <iostream>

namespace bok {
    LoggingServiceImpl::LoggingServiceImpl(const std::string &name) {
        this->name = name;
    }

    LoggingServiceImpl::~LoggingServiceImpl() {}

    void LoggingServiceImpl::log(const std::string &message) {
        std::cout << "[" << name << "][LOG]: " << message << std::endl;
    }
    
    void LoggingServiceImpl::warn(const std::string &message) {
        std::cout << "[" << name << "][WARN]: " << message << std::endl;
    }

    void LoggingServiceImpl::error(const std::string &message) {
        std::cout << "[" << name << "][ERROR]: " << message << std::endl;
    }
}
