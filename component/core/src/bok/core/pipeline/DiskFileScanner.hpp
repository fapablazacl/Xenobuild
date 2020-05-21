
#pragma once 

#include "FileScanner.hpp"

namespace bok {
    class DiskFileScanner : public FileScanner {
    public:
        virtual ~DiskFileScanner();

        std::vector<path> scan(const path &folder, const ScanType scanType, std::function<bool (const path &)> filter) const override;
    };
}
