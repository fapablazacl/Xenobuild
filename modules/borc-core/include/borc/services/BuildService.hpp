
#ifndef __BORC_SERVICES_BUILDSERVICE_HPP__
#define __BORC_SERVICES_BUILDSERVICE_HPP__

#include <string>

namespace borc {
    class Package;
    class BuildService {
    public:
        virtual ~BuildService();

        virtual void build(Package *package) = 0;
    };
}

#endif
