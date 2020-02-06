
#include "Pipe.hpp"

namespace borc {
    Pipe::Pipe(const Pipeline *pipeline_, const std::string &toolName_) 
        : pipeline(pipeline_), toolName(toolName_) {}


    Pipe::~Pipe() {}


    void Pipe::setInputPin(const std::string &inputPinName, const Pipe::InputPin &inputPin) {
        inputPins[inputPinName] = inputPin;
    }


    void Pipe::setInputArg(const std::string &inputArgName, const Pipe::Argument &inputArg) {
        arguments[inputArgName] = inputArg;
    }


    void Pipe::setOutputPin(const std::string &outputPinName, const Pipe::OutputPin &outputPin) {
        outputPins[outputPinName] = outputPin;
    }


    std::optional<Pipe::InputPin> Pipe::getInputPin(const std::string &name) const {
        if (auto it = inputPins.find(name); it != inputPins.end()) {
            return it->second;
        }

        return {};
    }


    std::optional<Pipe::Argument> Pipe::getInputArg(const std::string &name) const {
        if (auto it = arguments.find(name); it != arguments.end()) {
            return it->second;
        }

        return {};
    }


    std::optional<Pipe::OutputPin> Pipe::getOutputPin(const std::string &name) const {
        if (auto it = outputPins.find(name); it != outputPins.end()) {
            return it->second;
        }

        return {};
    }

    
    int Pipe::getInputPinCount() const {
        return static_cast<int>(inputPins.size());
    }


    int Pipe::getInputArgCount() const {
        return static_cast<int>(arguments.size());
    }


    int Pipe::getOutputPinCount() const {
        return static_cast<int>(outputPins.size());
    }
}
