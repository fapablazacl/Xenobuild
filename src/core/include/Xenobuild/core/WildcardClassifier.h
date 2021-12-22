
#pragma once 

#include <map>
#include <vector>
#include <string>
#include <optional>

#include "string.h"

namespace Xenobuild {
    template<typename Category>
    class WildcardClassifier {
    public:
        void registerCategory(const Category& category, const std::vector<std::string>& wildcards) {
            typeWildCardMap[category] = wildcards;
        }

        void unregisterTag(const Category& category) {
            typeWildCardMap.erase(category);
        }

        std::optional<Category> getFileType(const std::string& path) const {
            const auto file = path;

            for (const auto& keyValue : typeWildCardMap) {
                for (const std::string& pattern : keyValue.second) {
                    if (wildcard_match(pattern.c_str(), file.c_str())) {
                        return keyValue.first;
                    }
                }
            }

            return {};
        }

    private:
        std::map<Category, std::vector<std::string>> typeWildCardMap;
    };
}
