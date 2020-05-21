
#include "SourceScanner.hpp"

namespace bok {
    SourceScanner::~SourceScanner() {}

    std::vector<path> SourceScanner::scan(const path &folder, const ScanType scanType) const {
        return scan(folder, scanType, [](const path &file) {
            return true;
        });
    }
}
