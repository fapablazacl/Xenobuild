
#ifndef __BOK_TOOLCHAIN_COMPILERVERSIONDETECTOR_HPP__
#define __BOK_TOOLCHAIN_COMPILERVERSIONDETECTOR_HPP__

namespace bok {
    struct Version;
    
    class CompilerVersionDetector {
    public:
        virtual ~CompilerVersionDetector();

        virtual Version detectVersion() const = 0;
    };
}

#endif
