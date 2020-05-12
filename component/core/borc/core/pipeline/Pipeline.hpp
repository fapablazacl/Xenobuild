
#pragma once 

#include <memory>
#include <vector>
#include <string>
#include <map>

namespace borc {
    class Pipe;
    class Matcher;

    class Pipeline {
    public:
        ~Pipeline();

        Pipe* createPipe(const std::string &toolName);
        
        int getPipeCount() const;

        template<typename MatcherImpl>
        MatcherImpl* createMatcher(const std::string &fileTypeId) {
            auto matcher = new MatcherImpl(this, fileTypeId);

            this->addMatcher(matcher);

            return matcher;
        }

        const Matcher* getMatcher(const std::string &fileTypeId) const;

        int getMatcherCount() const;

    private:
        void addMatcher(Matcher *matcher);

    private:
        std::vector<std::unique_ptr<Pipe>> pipes;
        std::map<std::string, std::unique_ptr<Matcher>> matchers;
    };
}
