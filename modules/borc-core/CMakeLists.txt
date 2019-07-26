
include_directories("../borc-core/include")

add_library(borc-core 
    "include/borc/entity/Language.hpp"
    "include/borc/entity/Module.hpp"
    "include/borc/entity/Package.hpp"
    "include/borc/parsing/JSONDeserializer.hpp"
    "include/borc/parsing/JSONSerializer.hpp"
    "include/borc/services/FileService.hpp"
    "include/borc/services/FileServiceImpl.hpp"
    "include/borc/utility/MetaProperty.hpp"
    "src/borc/services/FileService.cpp"
    "src/borc/services/FileServiceImpl.cpp"
)
