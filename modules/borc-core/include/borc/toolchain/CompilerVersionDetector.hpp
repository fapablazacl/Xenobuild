
#ifndef __BORC_TOOLCHAIN_COMPILERVERSIONDETECTOR_HPP__
#define __BORC_TOOLCHAIN_COMPILERVERSIONDETECTOR_HPP__

namespace borc {
    struct Version;
    
    class CompilerVersionDetector {
    public:
        virtual ~CompilerVersionDetector();

        virtual Version detectVersion() const = 0;
    };
}

#endif
