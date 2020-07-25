
#include <bok/core/toolchain/Compiler_Mock.hpp>

namespace bok {
    static std::string toString(const CompileTargetArchitecture arch) {
        switch (arch) {
        case CompileTargetArchitecture::Native: return "native";
        case CompileTargetArchitecture::x86_32: return "x86_32";
        case CompileTargetArchitecture::x86_64: return "x86_64";
        default: return "<arch-unknown>";
        }
    }

    static std::string toString(const CompileLanguage lang) {
        switch (lang) {
        case CompileLanguage::Cplusplus98: return "C++/98";
        case CompileLanguage::Cplusplus03: return "C++/03";
        case CompileLanguage::Cplusplus11: return "C++/11";
        case CompileLanguage::Cplusplus14: return "C++/14";
        case CompileLanguage::Cplusplus17: return "C++/17";
        case CompileLanguage::Cplusplus20: return "C++/20";
        default: return "<lang-unknown>";
        }
    }

    static std::string toString(const CompileOptimization opt) {
        switch (opt) {
        case CompileOptimization::NoOptimization: return "no-optimization";
        case CompileOptimization::FavorSize: return "favor-size";
        case CompileOptimization::FavorSpeed: return "favor-speed";
        case CompileOptimization::FullSpeed: return "full-speed";
        default: return "<opt-unknown>";
        }
    }

    static std::string toString(const CompileRuntimeLink runtime) {
        switch (runtime) {
        case CompileRuntimeLink::DynamicDebug: return "dynamic-debug";
        case CompileRuntimeLink::DynamicRelease: return "dynamic-release";
        case CompileRuntimeLink::StaticDebug: return "static-debug";
        case CompileRuntimeLink::StaticRelease: return "static-release";
        default: return "<runtime-unknown>";
        }
    }

    CompileOutput Compiler_Mock::generateCompileOutput(const CompileInput& input) const {
        CompileOutput output;
        
        output.compileCommand.name = "compile-c++";
        output.compileCommand.args.push_back(input.sourceFilePath);
        output.compileCommand.args.push_back("--output=" + input.outputFilePath);
        output.compileCommand.args.push_back("--debug-information=" + std::to_string(input.debugInformation));
        output.compileCommand.args.push_back("--language=" + toString(input.language));
        output.compileCommand.args.push_back("--optimization=" + toString(input.optimization));
        output.compileCommand.args.push_back("--target-arch=" + toString(input.targetArchitecture));
        output.compileCommand.args.push_back("--runtime-link=" + toString(input.runtimeLink));
        
        for (const auto& pair : input.definitions) {
            std::string arg = "--define=" + pair.first + ":" + pair.second;

            output.compileCommand.args.push_back(arg);
        }

        std::transform(
            input.includePaths.begin(), input.includePaths.end(), 
            std::back_inserter(output.compileCommand.args), 
            [](const std::string &includePath) { return "--include-path=" + includePath; });

        return output;
    }


    void computeDependencies() {
        //std::vector<boost::filesystem::path> computeFileDependencies(const Source *source, const CompileOptions &options) const {
//    if (switches.generateDependencies == "") {
//        return {};
//    }

//    const auto sourceFilePath = source->getFilePath();

//    std::vector<std::string> commandOptions;

//    boost::split(commandOptions, switches.generateDependencies, boost::is_any_of(" "));

//    commandOptions.push_back(sourceFilePath.string());

//    // compute system include directories
//    // for (const std::string &path : configuration.systemIncludePaths) {
//    //     const std::string includeOption = switches.includePath + path;
//    // 
//    //     commandOptions.push_back(includeOption);
//    // }

//    // compute additional include directories
//    for (const std::string &path : options.includePaths) {
//        const std::string includeOption = switches.includePath + path;

//        commandOptions.push_back(includeOption);
//    }

//    // add additional compiler options
//    // commandOptions.insert(commandOptions.end(), std::begin(configuration.flags), std::end(configuration.flags));

//    boost::filesystem::path compilerPath = boost::process::search_path(commandPath);
//    boost::process::ipstream pipeStream;
//    boost::process::child childProcess {compilerPath, boost::process::args(commandOptions), boost::process::std_out > pipeStream};

//    std::string line;
//    std::vector<std::string> specs;

//    while (pipeStream && std::getline(pipeStream, line) && !line.empty()) {
//        specs.push_back(line);
//    }

//    childProcess.wait();

//    // from the third element onwards we have the dependencies ...
//    std::vector<boost::filesystem::path> dependencies;
//    for (int i=2; i<specs.size(); i++) {
//        std::string dependency = specs[i];

//        boost::algorithm::replace_all(dependency, "\\", "");
//        boost::algorithm::trim(dependency);

//        dependencies.push_back(dependency);
//    }

//    return dependencies;
//}
    }

        // TODO: Refactor at the Pipeline level?
        //boost::filesystem::path getObjectFilePath(const boost::filesystem::path &outputPath, const Source *source) const {
        //    const boost::filesystem::path objectFileName = source->getFilePath().filename().string() + ".obj";
        //    const boost::filesystem::path objectFileParentPath 
        //        = outputPath 
        //        / source->getRelativeFilePath().parent_path();

        //    const boost::filesystem::path objectFilePath = objectFileParentPath / objectFileName;
        //    
        //    return objectFilePath;
        //}
}
