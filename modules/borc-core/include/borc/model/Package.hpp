
#ifndef __BORC_MODEL_PACKAGE_HPP__
#define __BORC_MODEL_PACKAGE_HPP__

#include <string>
#include <vector>
#include <memory>

namespace borc {
    class Artifact;
    class Package {
    public:
        Package();

        ~Package();

        std::string getName() const {
            return name;
        }

    private:
        std::string name;
        std::vector<std::unique_ptr<Artifact>> artifacts;
    };
}

#endif
