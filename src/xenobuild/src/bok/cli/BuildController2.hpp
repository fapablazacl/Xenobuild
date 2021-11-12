
#pragma once 

#include <string>
#include <vector>
#include <memory>
#include <bok/cli/Controller.hpp>

namespace bok {
    class Package;
    class Module;

    class BuildController2 : public Controller {
    public:
        virtual void perform(int argc, char **argv) override;

    private:
        struct ComponentIO {
            std::string name;
            std::string type;
            std::string language;
            std::vector<std::string> sources;
        };

        struct PackageIO {
            std::string name;
            std::vector<ComponentIO> components;
        };

    private:
        PackageIO parse(const std::string &rootFile) const;

        std::unique_ptr<Package> createPackage(const PackageIO &data) const;
    };
}
