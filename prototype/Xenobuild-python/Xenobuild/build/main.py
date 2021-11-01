
import sys
import os
import subprocess


class Module:
    def __init__(self):
        self.sources = ["test.cpp"]


class GenerateCommand:
    def __init__(self):
        pass

    def perform(self):
        print("GenerateCommand.perform()")



class ConfigManagerCMake:
    def __init__(self):
        self.version = self.__detectVersion()
        self.additionalFlagsDict = {
            "default": "",
            "debug": "-DCMAKE_BUILD_TYPE=Debug",
            "release": "-DCMAKE_BUILD_TYPE=release"
        }


    def __detectVersion(self):
        str = subprocess.check_output("cmake --version", shell=True).decode("utf-8")

        lineVer = str.splitlines()[0]
        version = lineVer.split(' ')[2]

        return version


    def perform(self, params):
        if len(params) == 0:
            sourceFolder = "."
        else:
            sourceFolder = params[0]

        if not os.path.isabs(sourceFolder):
            sourceFolder = os.path.abspath(sourceFolder)

        buildFolder = sourceFolder + "/.cmake"

        print("Current source folder: " + sourceFolder)
        print("Current build folder:" + buildFolder)

        if len(params) < 2:
            configName = "default"
            print("Performing a 'default' build configuration")
        else:
            configName = params[1]

        additionalFlags = self.additionalFlagsDict.get(configName)

        if additionalFlags is None:
            print("Unknown configuration name")
            exit(1)
        
        self._generateConfig(buildFolder, configName, additionalFlags)


    def _generateConfig(self, buildFolder, configName, additionalFlags):
        configBuildFolder = buildFolder + "/" + configName

        currentPath = os.getcwd()

        os.makedirs(configBuildFolder, exist_ok=True)
        os.chdir(configBuildFolder)
        os.system("cmake ../../ " + additionalFlags)
        os.chdir(currentPath)



class BuildCMake:
    def __init__(self):
        pass

    def perform(self, params):
        if len(params) == 0:
            sourceFolder = "."
        else:
            sourceFolder = params[0]

        if not os.path.isabs(sourceFolder):
            sourceFolder = os.path.abspath(sourceFolder)

        buildFolder = sourceFolder + "/.cmake"

        if len(params) < 2:
            configName = "default"
        else:
            configName = params[1]

        configBuildFolder = buildFolder + "/" + configName
        currentPath = os.getcwd()

        os.chdir(configBuildFolder)
        os.system("make")
        os.chdir(currentPath)


class BuildCommand:
    def __init__(self):
        self.sources = ["test.cpp"]

    def perform(self):
        for source in self.sources:
            os.system("clang++ -c " + source + " -o " + source + ".obj")

        sourceObjects = [source + ".obj" for source in self.sources]

        os.system("clang++ " + " ".join(sourceObjects) + " -o test")


class RunCommand:
    def __init__(self):
        pass

    def perform(self, params):
        if len(params) == 0:
            sourceFolder = "."
        else:
            sourceFolder = params[0]

        if not os.path.isabs(sourceFolder):
            sourceFolder = os.path.abspath(sourceFolder)

        buildFolder = sourceFolder + "/.cmake"

        if len(params) < 2:
            configName = "default"
        else:
            configName = params[1]

        configBuildFolder = buildFolder + "/" + configName
        currentPath = os.getcwd()

        os.chdir(configBuildFolder)
        os.system("./test")
        os.chdir(currentPath)


if __name__=='__main__':
    if len(sys.argv) <= 1:
        print("Ingrese subcomando")
        exit(1)

    subcommandName = sys.argv[1]

    subcommands = {
        "configure": ConfigManagerCMake(),
        "generate": GenerateCommand(),
        "build": BuildCMake(),
        "run": RunCommand()
    }

    subcommand = subcommands.get(subcommandName)

    if subcommand is None:
        print("Subcomando no conocido")
        exit(1)

    subcommand.perform(sys.argv[2:])
