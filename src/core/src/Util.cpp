
#include <Xenobuild/core/Util.h>

#include <thread>
#include <boost/filesystem.hpp>
#include <Xenobuild/core/Command.h>
#include <Xenobuild/core/Triplet.h>
#include <Xenobuild/core/Dependency.h>

namespace Xenobuild {
    boost::optional<std::string> getenv(const std::string& var) {
        const char* val = std::getenv(var.c_str());

        if (val == nullptr) {
            return {};
        }

        return std::string(val);
    }

    unsigned getProcessorCount() {
        const auto processor_count = std::thread::hardware_concurrency();
        
        if (processor_count == 0) {
            return 1;
        }
        
        return processor_count;
    }

    std::string quote(const std::string& str) {
        return "\"" + str + "\"";
    }
    
    std::ostream& write(std::ostream &os, std::vector<std::string> lines) {
        for (const std::string &line : lines) {
            os << line << std::endl;
        }
        
        return  os;
    }
    
    std::vector<std::string> enumerateVCInstallations() {
        const std::vector<std::string> vars = {
            "VS2019INSTALLDIR"
        };

        std::vector<std::string> installations;

        for (const std::string& var : vars) {
            const boost::optional<std::string> value = getenv(var);

            if (! value.has_value()) {
                continue;
            }

            installations.push_back(value.get());
        }

        return installations;
    }

    CommandX createVCVars64Command(const boost::filesystem::path &prefixPath) {
        const auto vcvars = prefixPath / "VC\\Auxiliary\\Build\\vcvars64.bat";

        return { "call", { quote(vcvars.string()) } };
    }

    boost::filesystem::path getUserPath() {
        boost::optional<std::string> userPathStr;

        switch (getHostOS()) {
            case OS::Windows:
                userPathStr = getenv("USERPROFILE");
                break;
                
            case OS::MacOS:
            case OS::Linux:
                userPathStr = getenv("HOME");
                break;
        }

        if (!userPathStr) {
            return boost::filesystem::current_path();
        }

        return userPathStr.get();
    }
}
