
#include "ToolchainServiceImpl.hpp"

#include <boost/optional/optional.hpp>
#include <boost/hana.hpp>

#include <nlohmann/json.hpp>

#include <bok/core/common/FileService.hpp>
#include <bok/core/toolchain/ManagedToolchainImpl.hpp>
#include <bok/core/io/ToolchainEntity.hpp>
#include <bok/core/io/JsonModel.hpp>
#include <bok/core/io/Decoder.hpp>

BOOST_HANA_ADAPT_STRUCT(bok::ToolchainEntity::EnumeratedOption, name, flag);
BOOST_HANA_ADAPT_STRUCT(bok::ToolchainEntity::Switches, dialects, optimizationLevels, debugInformation, includePath, compile, outputFile, generateBuildDependencies, importLibrary, libraryPath);
BOOST_HANA_ADAPT_STRUCT(bok::ToolchainEntity::BuildRuleInput, fileType, moduleTypes);
BOOST_HANA_ADAPT_STRUCT(bok::ToolchainEntity::BuildRuleOutput, fileType, fileName);
BOOST_HANA_ADAPT_STRUCT(bok::ToolchainEntity::BuildRule, input, flags, output);
BOOST_HANA_ADAPT_STRUCT(bok::ToolchainEntity::Tool, type, command, buildRules, switches);
BOOST_HANA_ADAPT_STRUCT(bok::ToolchainEntity, name, tools);

namespace bok {
    ToolchainServiceImpl::ToolchainServiceImpl(const FileService *fileService) {
        this->fileService = fileService;
    }


    std::unique_ptr<Toolchain> ToolchainServiceImpl::createToolchain(const boost::filesystem::path &definitionFullPath, boost::optional<boost::filesystem::path> installationPath) const {
        const auto toolchainFilePath = definitionFullPath / "toolchain.bok.json";
        const auto toolchainJsonContent = fileService->load(toolchainFilePath.string());
        const auto toolchainJson = nlohmann::json::parse(toolchainJsonContent);

        const auto toolchainEntity = Decoder<JsonModel, ToolchainEntity>{toolchainJson}.decode();

        return std::make_unique<ManagedToolchainImpl>(toolchainEntity, installationPath);
    }
}
