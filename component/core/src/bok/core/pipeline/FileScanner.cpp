
#include "FileScanner.hpp"

namespace bok {
    FileScanner::~FileScanner() {}

    std::vector<path> FileScanner::scan(const path &folder, const ScanType scanType) const {
        return scan(folder, scanType, [](const path &file) {
            return true;
        });
    }
}
