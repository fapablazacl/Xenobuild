
#pragma once 

#include <vector>
#include <functional>
#include <bok/core/Types.hpp>

namespace bok {
    class SourceScanner {
    public:
        enum ScanType {
            Linear,
            Recursive
        };

    public:
        virtual ~SourceScanner();
        
        virtual std::vector<path> scan(const path &folder, const ScanType scanType) const = 0;

        virtual std::vector<path> scan(const path &folder, const ScanType scanType, std::function<bool (const path &)> filter) const = 0;
    };
}
