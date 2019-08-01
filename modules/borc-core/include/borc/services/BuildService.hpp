
#ifndef __BORC_SERVICES_BUILDSERVICE_HPP__
#define __BORC_SERVICES_BUILDSERVICE_HPP__

#include <string>
#include <memory>

namespace borc {
    class Dag;
    class Package;
    class BuildService {
    public:
        virtual ~BuildService();

        virtual std::unique_ptr<Dag> createBuildDag(Package *package) = 0;
    };
}

#endif
