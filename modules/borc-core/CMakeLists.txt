
include_directories("../borc-core/include")

add_library(borc-core 
    "include/borc/model/Artifact.hpp"
    "include/borc/model/Package.hpp"
    "include/borc/model/Context.hpp"
    "include/borc/entity/LanguageEntity.hpp"
    "include/borc/entity/ModuleEntity.hpp"
    "include/borc/entity/PackageEntity.hpp"
    "include/borc/parsing/JSONDeserializer.hpp"
    "include/borc/parsing/JSONSerializer.hpp"
    "include/borc/services/FileService.hpp"
    "include/borc/services/FileServiceImpl.hpp"
    "include/borc/utility/MetaProperty.hpp"

    "src/borc/model/Artifact.cpp"
    "src/borc/model/Package.cpp"
    "src/borc/model/Context.cpp"
    "src/borc/services/FileService.cpp"
    "src/borc/services/FileServiceImpl.cpp"    
)
