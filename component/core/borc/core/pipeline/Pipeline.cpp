
#include "Pipeline.hpp"

#include <stdexcept>
#include "Matcher.hpp"
#include "Pipe.hpp"

namespace borc {
    Pipeline::~Pipeline() {}


    Pipe* Pipeline::createPipe(const std::string &toolName) {
        auto pipe = new Pipe(this, toolName);

        pipes.emplace_back(pipe);

        return pipe;
    }
    

    int Pipeline::getPipeCount() const {
        return static_cast<int>(pipes.size());
    }


    void Pipeline::addMatcher(Matcher *matcher) {
        const std::string key = matcher->getFileTypeId();

        if (auto it = matchers.find(key); it != matchers.end()) {
            throw std::runtime_error("Already exists a '" + key + "' matcher in the current pipeline");
        }

        matchers[key] = std::unique_ptr<Matcher>(matcher);
    }


    int Pipeline::getMatcherCount() const {
        return static_cast<int>(matchers.size());
    }


    const Matcher* Pipeline::getMatcher(const std::string &fileTypeId) const {
        if (auto it = matchers.find(fileTypeId); it != matchers.end()) {
            return it->second.get();
        }

        return nullptr;
    }
}
