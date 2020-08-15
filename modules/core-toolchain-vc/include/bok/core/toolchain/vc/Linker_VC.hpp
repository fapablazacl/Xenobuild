
#pragma once

#include <bok/core/toolchain/Linker.hpp>

#include <optional>
#include <boost/filesystem/path.hpp>

namespace bok {
    class Linker_VC: public Linker {
    public:
        explicit Linker_VC(std::optional<boost::filesystem::path> path);

        LinkOutput generateLinkOutput(const LinkInput &input) const override;

    private:
        std::optional<boost::filesystem::path> path;
    };
} 
