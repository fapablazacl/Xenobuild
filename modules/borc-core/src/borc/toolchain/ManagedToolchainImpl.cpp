
#include "ManagedToolchainImpl.hpp"

#include <boost/filesystem.hpp>
#include <boost/process.hpp>
#include <borc/model/Module.hpp>
#include <borc/model/Source.hpp>
#include <borc/model/Version.hpp>
#include <borc/model/Command.hpp>
#include <borc/entity/ToolchainEntity.hpp>
#include <borc/toolchain/SourceChecker.hpp>
#include <borc/toolchain/ModuleChecker.hpp>
#include <borc/toolchain/CompilerImpl.hpp>
#include <borc/toolchain/LinkerImpl.hpp>


namespace borc {
    // TODO: Grab them from a service
    static std::initializer_list<std::string> cppSourceFiles = {
        "*.cpp", "*.cxx", "*.c++", "*.cc"
    };

    static std::initializer_list<Module::Type> moduleLinkerModuleTypes ({
        Module::Type{"application", "cli"},
        Module::Type{"application", "gui"},
        Module::Type{"library", "dynamic"}
    });

    static std::initializer_list<Module::Type> archiveLinkerModuleTypes ({
        Module::Type{"library", "static"}
    });
    

    struct ToolchainDetectorInfo {
        std::string commandTemplate;
        std::string executableFilePath;
    };


    struct ManagedToolchainImpl::Private {
        std::vector<std::unique_ptr<Compiler>> compilers;
        std::vector<std::unique_ptr<Linker>> linkers;

        CommandFactory commandFactory;

        void appendCompiler(const ToolchainEntity::Tool &tool) {
            CompilerImpl::Switches switches;

            switches.compile = tool.switches.compile;
            switches.objectFileOutput = tool.switches.outputFile;
            // compilerSwitches.zeroOptimization = "/Od";
            switches.includePath = tool.switches.includePath;
            switches.includeDebug = tool.switches.debugInformation;

            std::vector<CompilerImpl::BuildRule> buildRules;

            std::transform(tool.buildRules.begin(), tool.buildRules.end(), std::back_inserter(buildRules), [](const ToolchainEntity::BuildRule &buildRuleDef) {
                CompilerImpl::BuildRule buildRule;

                buildRule.input.fileType = buildRuleDef.input.fileType;
                buildRule.output.fileType = buildRuleDef.output.fileType;
                buildRule.output.fileName = buildRuleDef.output.fileName;

                return buildRule;
            });

            compilers.emplace_back(new CompilerImpl {
                &commandFactory, 
                tool.command, 
                switches,
                buildRules
            });
        }


        void appendLinker(const ToolchainEntity::Tool &tool) {
            LinkerImpl::Switches switches;

            switches.importLibrary = tool.switches.importLibrary;
            switches.outputFile = tool.switches.outputFile;
            switches.libraryPath = tool.switches.libraryPath;

            std::vector<LinkerImpl::BuildRule> buildRules;
            for (const ToolchainEntity::BuildRule &buildRuleEntity : tool.buildRules) {
                LinkerImpl::BuildRule buildRule;

                buildRule = {};
                buildRule.flags = buildRuleEntity.flags;

                buildRule.input.fileType = buildRuleEntity.input.fileType;

                std::transform(
                        buildRuleEntity.input.moduleTypes.begin(),
                        buildRuleEntity.input.moduleTypes.end(), 
                        std::back_inserter(buildRule.input.moduleTypes),
                    [](const std::string &moduleTypeStr) {
                        return *Module::Type::parse(moduleTypeStr);
                });

                buildRule.output.fileType = buildRuleEntity.output.fileType;
                buildRule.output.fileName = buildRuleEntity.output.fileName;

                buildRules.push_back(buildRule);
            }
            
            linkers.emplace_back(new LinkerImpl {
                &commandFactory,
                /*boost::filesystem::current_path(),*/
                "",
                tool.command,
                switches,
                buildRules
            });
        }


        ToolchainDetectorInfo getToolchainDetectorInfo() const {
            return {
                "gcc other/CXXCompilerVersionDetector.cpp -O0 -o${ExecutablePath}",
                "other/CXXCompilerVersionDetector"
            };
        }


        Version detectToolchainVersion(const ToolchainDetectorInfo &info) const {
            // 1. Compile C++ version detector
            // TODO: Use a internally-generated
            if (std::system("gcc other/CXXCompilerVersionDetector.cpp -O0 -oother/CXXCompilerVersionDetector") != 0) {
                throw std::runtime_error("Failed CXXCompilerVersionDetector compilation.");
            }

            // 2. Execute it, and grab the output
            boost::filesystem::path compilerPath = boost::filesystem::path("./other/CXXCompilerVersionDetector");

            if (! boost::filesystem::exists(compilerPath)) {
                throw std::runtime_error("Compiler detector not found in path '" + compilerPath.string() + "'.");
            }

            boost::process::ipstream pipeStream;
            boost::process::child childProcess {compilerPath, boost::process::std_out > pipeStream};
        
            std::string line;
            std::vector<std::string> specs;

            while (pipeStream && std::getline(pipeStream, line) && !line.empty()) {
                specs.push_back(line);
            }

            childProcess.wait();

            if (specs.size() != 1) {
                throw std::runtime_error("Couldn't detect compiler type and version (unexpected output)");
            }

            std::vector<std::string> compilerDetectorOutput;

            boost::algorithm::split(compilerDetectorOutput, specs[0], boost::is_any_of("-"));

            if (compilerDetectorOutput.size() != 2) {
                throw std::runtime_error("Couldn't detect compiler type and version (unexpected output)");
            }

            // 3. Parse the output and return the result.
            return Version::parse(compilerDetectorOutput[1]);
        }
    };


    ManagedToolchainImpl::ManagedToolchainImpl(const ToolchainEntity &entity) {
        m_pimpl = new ManagedToolchainImpl::Private();

        for (const ToolchainEntity::Tool &tool : entity.tools) {
            if (tool.type == "compiler") {
                m_pimpl->appendCompiler(tool);
            } else if (tool.type == "linker") {
                m_pimpl->appendLinker(tool);
            }
        }
    }


    ManagedToolchainImpl::~ManagedToolchainImpl() {
        delete m_pimpl;
    }


    const Compiler* ManagedToolchainImpl::selectCompiler(const Source *source) const {
        for (const auto &compiler : m_pimpl->compilers) {
            if (compiler->isSourceLinkable(source)) {
                return compiler.get();
            }
        }

        return nullptr;
    }


    const Linker* ManagedToolchainImpl::selectLinker(const Module *module) const {
        for (const auto &linker : m_pimpl->linkers) {
            if (linker->isModuleLinkable(module)) {
                return linker.get();
            }
        }

        return nullptr;
    }

    Version ManagedToolchainImpl::detectVersion() const {
        return {0, 0, 0};
    }
}
