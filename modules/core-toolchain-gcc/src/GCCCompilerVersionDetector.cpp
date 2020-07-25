
#include <bok/core/toolchain/gcc/GCCCompilerVersionDetector.hpp>

#include <boost/process.hpp>
#include <boost/algorithm/string.hpp>

namespace bok {
    GCCCompilerVersionDetector::~GCCCompilerVersionDetector() {}

    Version GCCCompilerVersionDetector::detectVersion() const {
        const auto compilerOutputLines = this->getCompilerOutput();
        const auto version = this->parseVersion(compilerOutputLines[6]);

        return version;
    }

    std::vector<std::string> GCCCompilerVersionDetector::getCompilerOutput() const {
        boost::process::ipstream pipeStream;
        boost::process::child childProcess {"gcc -v", boost::process::std_err > pipeStream};

        std::string line;
        std::vector<std::string> specs;

        while (pipeStream && std::getline(pipeStream, line) && !line.empty()) {
            specs.push_back(line);
        }

        childProcess.wait();

        return specs;
    }

    Version GCCCompilerVersionDetector::parseVersion(const std::string &versionLine) const {
        std::vector<std::string> parts;
        boost::split(parts, versionLine, boost::is_any_of(" "));

        return Version::parse(parts[2]);
    }
}
