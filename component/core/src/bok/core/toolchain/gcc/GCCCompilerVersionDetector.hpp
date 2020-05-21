
#ifndef __BOK_TOOLCHAIN_GCCCOMPILERVERSIONDETECTOR_HPP__
#define __BOK_TOOLCHAIN_GCCCOMPILERVERSIONDETECTOR_HPP__

#include <vector>
#include <bok/core/common/Version.hpp>
#include <bok/core/toolchain/CompilerVersionDetector.hpp>

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
