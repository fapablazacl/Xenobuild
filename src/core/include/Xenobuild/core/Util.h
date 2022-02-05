
#pragma once

#include <thread>
#include <string>

namespace Xenobuild {
    inline unsigned getProcessorCount() {
        const auto processor_count = std::thread::hardware_concurrency();
        
        if (processor_count == 0) {
            return 1;
        }
        
        return processor_count;
    }

    inline std::string quote(const std::string& str) {
        return "\"" + str + "\"";
    }
    
    inline std::ostream& write(std::ostream &os, std::vector<std::string> lines) {
        for (const std::string &line : lines) {
            os << line << std::endl;
        }
        
        return  os;
    }
}
