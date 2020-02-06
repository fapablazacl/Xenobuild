
#pragma once 

#include <string>
#include <vector>

namespace borc {
    class Matcher;

    class Pipe {
    public:
        ~Pipe();

        enum class Cardinality {
            one, many
        };

        struct InputPin {
            std::string name;
            std::string toolArgumentName;
            Cardinality cardinality;
            Matcher *matcher = nullptr;
        };

        struct OutputPin {
            std::string name;
            Cardinality cardinality;
        };

        enum class DataType {
            _void, string, integer, _float, path, file
        };

        struct Argument {
            std::string name;
            Cardinality cardinality;
            std::string toolArgumentName;
            DataType dataType;
        };


    public:
        std::string toolName;
        std::vector<InputPin> inputPins;
        std::vector<Argument> arguments;
        std::vector<OutputPin> outputPins;
    };
}
