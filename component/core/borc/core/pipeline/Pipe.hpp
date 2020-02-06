
#pragma once 

#include <string>
#include <vector>
#include <map>
#include <optional>

namespace borc {
    class Matcher;

    class Pipeline;
    class Pipe {
    public:
        explicit Pipe(const Pipeline *pipeline, const std::string &toolName);

        ~Pipe();

        enum class Cardinality {
            One, Many
        };

        struct InputPin {
            std::string toolArgumentName;
            Cardinality cardinality;
            Matcher *matcher = nullptr;
        };

        struct OutputPin {
            Cardinality cardinality;
        };

        enum class DataType {
            Void, String, Integer, Float, Folder, File
        };

        struct Argument {
            Cardinality cardinality;
            std::string toolArgumentName;
            DataType dataType;
        };

    public:
        std::string getToolName() const {
            return toolName;
        }

        void setInputPin(const std::string &inputPinName, const InputPin &inputPin);

        void setInputArg(const std::string &inputArgName, const Argument &inputArg);

        void setOutputPin(const std::string &outputPinName, const OutputPin &outputPin);

        std::optional<InputPin> getInputPin(const std::string &name) const;

        std::optional<Argument> getInputArg(const std::string &name) const;

        std::optional<OutputPin> getOutputPin(const std::string &name) const;

        int getInputPinCount() const;

        int getInputArgCount() const;

        int getOutputPinCount() const;

    private:
        const Pipeline *pipeline = nullptr;

        std::string toolName;
        std::map<std::string, InputPin> inputPins;
        std::map<std::string, Argument> arguments;
        std::map<std::string, OutputPin> outputPins;
    };
}
