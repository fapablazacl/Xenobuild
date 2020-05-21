
#include "SourceScannerFileSystem.hpp"

#include <boost/filesystem.hpp>

namespace bok {
    SourceScannerFileSystem::~SourceScannerFileSystem() {}
        
    std::vector<path> SourceScannerFileSystem::scan(const path &folder, const ScanType scanType) const {
        return scan(folder, scanType, [](const path &file) {
            return true;
        });
    }


    template<typename Iterator>
    std::vector<path> scan_folder(const path &folder, std::function<bool (const path &)> filter) {
        std::vector<path> files;

        using iterator = boost::filesystem::directory_iterator;

        for (const auto &entry : iterator{folder}) {
            const auto file = entry.path();
            
            if (boost::filesystem::is_directory(file)) {
                continue;
            }

            if (! filter(file)) {
                continue;
            }

            files.push_back(boost::filesystem::relative(file, folder));
        }

        return files;
    }


    std::vector<path> SourceScannerFileSystem::scan(const path &folder, const ScanType scanType, std::function<bool (const path &)> filter) const {
        if (! boost::filesystem::exists(folder)) {
            return {};
        }

        std::vector<path> files;

        if (scanType == SourceScanner::Recursive) {
            using iterator = boost::filesystem::recursive_directory_iterator;

            return scan_folder<iterator>(folder, filter);

        } else {
            using iterator = boost::filesystem::directory_iterator;

            return scan_folder<iterator>(folder, filter);
        }

        return files;
    }
}
