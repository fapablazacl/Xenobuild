
#pragma once

#include <bok/core/toolchain/Linker.hpp>

#include <optional>

namespace bok {
    class Linker_VC: public Linker {
    public:
        explicit Linker_VC(std::optional<std::string> path);

        LinkOutput generateLinkOutput(const LinkInput &input) const override;

    private:
        std::optional<std::string> path;
    };
} 
