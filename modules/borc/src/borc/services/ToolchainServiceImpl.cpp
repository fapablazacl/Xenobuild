
#include "ToolchainServiceImpl.hpp"

#include <boost/optional/optional.hpp>
#include <boost/hana.hpp>
#include <nlohmann/json.hpp>
#include <borc/toolchain/ManagedToolchainImpl.hpp>
#include <borc/services/FileService.hpp>
#include <borc/entity/ToolchainEntity.hpp>
#include <borc/parsing/JSONDeserializer.hpp>

BOOST_HANA_ADAPT_STRUCT(borc::ToolchainEntity::EnumeratedOption, name, flag);
BOOST_HANA_ADAPT_STRUCT(borc::ToolchainEntity::Switches, dialects, optimizationLevels, debugInformation, includePath, compile, outputFile, generateBuildDependencies, importLibrary, libraryPath);
BOOST_HANA_ADAPT_STRUCT(borc::ToolchainEntity::BuildRuleInput, fileType, moduleTypes);
BOOST_HANA_ADAPT_STRUCT(borc::ToolchainEntity::BuildRuleOutput, fileType, fileName);
BOOST_HANA_ADAPT_STRUCT(borc::ToolchainEntity::BuildRule, input, flags, output);
BOOST_HANA_ADAPT_STRUCT(borc::ToolchainEntity::Tool, type, command, buildRules, switches);
BOOST_HANA_ADAPT_STRUCT(borc::ToolchainEntity, name, tools);

namespace borc {
    ToolchainServiceImpl::ToolchainServiceImpl(const FileService *fileService) {
        this->fileService = fileService;
    }


    std::unique_ptr<Toolchain> ToolchainServiceImpl::createToolchain(const boost::filesystem::path &definitionFullPath, boost::optional<boost::filesystem::path> installationPath) const {
        const auto toolchainFilePath = definitionFullPath / "toolchain.borc.json";
        const auto toolchainJsonContent = fileService->load(toolchainFilePath.string());
        const auto toolchainJson = nlohmann::json::parse(toolchainJsonContent);

        ToolchainEntity toolchainEntity;

        deserialize(toolchainEntity, toolchainJson);

        return std::make_unique<ManagedToolchainImpl>(toolchainEntity, installationPath);
    }
}
