
#pragma once 

#include "FileScanner.hpp"

namespace bok {
    class FileScanner_FS : public FileScanner {
    public:
        virtual ~FileScanner_FS();

        std::vector<path> scan(const path &folder, const ScanType scanType, std::function<bool (const path &)> filter) const override;
    };
}
