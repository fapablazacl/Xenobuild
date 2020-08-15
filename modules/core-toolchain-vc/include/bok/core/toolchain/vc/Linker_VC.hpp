
#pragma once

#include <bok/core/toolchain/Linker.hpp>

namespace bok {
    class Linker_VC: public Linker {
    public:
        LinkOutput generateLinkOutput(const LinkInput &input) const override;
    };
} 
