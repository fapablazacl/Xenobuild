
#pragma once 

#include "SourceScanner.hpp"

namespace bok {
    class SourceScannerFileSystem : public SourceScanner {
    public:
        virtual ~SourceScannerFileSystem();
        
        std::vector<path> scan(const path &folder, const ScanType scanType) const override;

        std::vector<path> scan(const path &folder, const ScanType scanType, std::function<bool (const path &)> filter) const override;
    };
}
