
#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>

// #include "borc/cli/Controller.hpp"
// #include "borc/cli/ControllerFactory.hpp"

/*
int main(int argc, char **argv) {
    try {
        if (argc < 2) {
            throw std::runtime_error("No subcommand supplied.");
        }

        std::vector<char*> args = {argv + 1, argv + argc};

        borc::ControllerFactory controllerFactory;
        auto controller = controllerFactory.create(args[0]);

        controller->perform(args.size(), args.data());
    
        return 0;
    } catch (const std::exception &exp) {
        std::cout << "ERROR: " << exp.what() << std::endl;

        return 1;
    }

    return 0;
}
*/

// configure --toolchain=vc --build-type=all --var=BoostPath:C:\Boost --var=BoostVersion:1.7.1

/*
int main(int argc, char **argv) {
    if (argc < 2) {
        throw std::runtime_error("No subcommand supplied.");
    }

    std::vector<char*> args = {argv + 1, argv + argc};

    borc::ControllerFactory controllerFactory;
    auto controller = controllerFactory.create(args[0]);

    controller->perform(args.size(), args.data());
    
    return 0;
}
*/

#include <borc/core/pipeline/Pipe.hpp>
#include <borc/core/pipeline/Pipeline.hpp>
#include <borc/core/pipeline/Matcher.hpp>

#include <borc/core/package/Component.hpp>
#include <borc/core/package/Package.hpp>
#include <borc/core/package/Version.hpp>

class ExtensionMatcher : public borc::Matcher {
public:
    ExtensionMatcher(const borc::Pipeline *pipeline, const std::string &fileTypeId) 
        : Matcher(pipeline, fileTypeId) {}


    virtual bool match(const std::string &fileName) override {
        std::size_t index = fileName.find_last_of('.', 0);
        std::string ext = fileName.substr(index);

        auto it = std::find(extensions.begin(), extensions.end(), ext);

        return it != extensions.end();
    }


    void setExtensions(const std::vector<std::string> &value) {
        extensions = value;
    }


    std::vector<std::string> getExtensions() const {
        return extensions;
    }

private:
    std::vector<std::string> extensions;
};


std::unique_ptr<borc::Pipeline> setupMockBuildPipeline() {
    auto pipeline = std::unique_ptr<borc::Pipeline>();

    auto cppSourceMatcher = pipeline->createMatcher<ExtensionMatcher>("c++/source");
    cppSourceMatcher->setExtensions({
        ".cpp", ".cc", ".cxx", ".c++"
    });

    auto cppObjectMatcher = pipeline->createMatcher<ExtensionMatcher>("c++/object");
    cppObjectMatcher->setExtensions({".obj"});

    auto compilePipe = pipeline->createPipe("c++/compile");

    compilePipe->setInputPin("source", borc::Pipe::InputPin {
        "compiler", borc::Pipe::Cardinality::One, cppSourceMatcher
    });

    compilePipe->setOutputPin("object", borc::Pipe::OutputPin {
        borc::Pipe::Cardinality::One
    });

    auto linkPipe = pipeline->createPipe("c++/link");

    linkPipe->setInputPin("objects", borc::Pipe::InputPin {
        "linker", borc::Pipe::Cardinality::Many, cppObjectMatcher
    });

    linkPipe->setOutputPin("component", borc::Pipe::OutputPin {
        borc::Pipe::Cardinality::One
    });

    return pipeline;
}


std::unique_ptr<borc::Package> setupMockPackage() {
    auto package = std::make_unique<borc::Package>();

    package
        ->setName("helloWorldApp")
        ->setAuthor("Some Author")
        ->setDescription("A simple hello world application");

    package->addComponent<borc::Component>("helloWorld")
        ->setLanguage(borc::Component::Language{"c++", "17"})
        ->set

    return package;
}


int main(int argc, char **argv) {

    return 0;
}
