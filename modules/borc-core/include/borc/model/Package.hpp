
#ifndef __BORC_MODEL_PACKAGE_HPP__
#define __BORC_MODEL_PACKAGE_HPP__

#include <string>
#include <vector>
#include <memory>

#include "Language.hpp"

namespace borc {
    class Artifact;
    class Package {
    public:
        Package(const std::string &name);

        ~Package();

        std::string getName() const {
            return name;
        }

        std::vector<Artifact*> getArtifacts() const;

        Artifact* createArtifact();

    private:
        std::string name;
        std::vector<std::unique_ptr<Artifact>> artifacts;
    };
}

#endif
