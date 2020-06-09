
#pragma once 

#include <vector>
#include <functional>
#include <bok/core/Types.hpp>

namespace bok {
    //! TODO: Move to the 'utility' module
    class FileScanner {
    public:
        enum ScanType {
            Linear,
            Recursive
        };

    public:
        virtual ~FileScanner();
        
        virtual std::vector<path> scan(const path &folder, const ScanType scanType) const;

        virtual std::vector<path> scan(const path &folder, const ScanType scanType, std::function<bool (const path &)> filter) const = 0;
    };
}
