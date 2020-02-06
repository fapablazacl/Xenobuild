
#pragma once 

#include <memory>
#include <vector>

namespace borc {
    class Pipe;
    class Matcher;

    class Pipeline {
    public:
        ~Pipeline();

    private:
        std::vector<std::unique_ptr<Pipe>> pipes;
        std::vector<std::unique_ptr<Matcher>> matchers;
    };
}
