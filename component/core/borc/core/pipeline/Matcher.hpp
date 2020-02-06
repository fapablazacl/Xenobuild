
#pragma once 

#include <string>

namespace borc {
    class Pipeline;
    class Matcher {
    public:
        explicit Matcher(const Pipeline *pipeline, const std::string &fileTypeId);

        virtual ~Matcher();

        virtual bool match(const std::string &fileName) = 0;

        const Pipeline* getPipeline() const {
            return pipeline;
        }

        std::string getFileTypeId() const {
            return fileTypeId;
        }

    private:
        const Pipeline *pipeline = nullptr;
        std::string fileTypeId;
    };
}
