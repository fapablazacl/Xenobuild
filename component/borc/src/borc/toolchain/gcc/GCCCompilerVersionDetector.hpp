
#ifndef __BORC_TOOLCHAIN_GCCCOMPILERVERSIONDETECTOR_HPP__
#define __BORC_TOOLCHAIN_GCCCOMPILERVERSIONDETECTOR_HPP__

#include <vector>
#include <borc/model/Version.hpp>
#include <borc/toolchain/CompilerVersionDetector.hpp>

namespace bok {
    class GCCCompilerVersionDetector : public CompilerVersionDetector {
    public:
        virtual ~GCCCompilerVersionDetector();

        virtual Version detectVersion() const override;

    private:
        std::vector<std::string> getCompilerOutput() const;

        Version parseVersion(const std::string &versionLine) const;
    };
}

#endif
