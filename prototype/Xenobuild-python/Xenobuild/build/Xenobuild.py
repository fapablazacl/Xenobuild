
import json
import sys
import os


def xeno_parse_source(source):
    index = source.find("://")

    if index == -1:
        pass


class GitRepository:
    def __init__(self, url):
        self.url = url

    def clone(self, destinationPath):
        os.system('git clone ' + self.url + ' ' + destinationPath)


class DependencySourceGit:
    def __init__(self, provider, author, name, repository):
        self.provider = provider
        self.author = author
        self.name = name
        self.repository = repository

    def download(self, downloadPath):
        pathSuffix = self.provider + '/' + self.author + '/' + self.name + '/'
        self.repository.clone(downloadPath + pathSuffix)


class CMakeDefinition:
    def __init__(self, name, value):
        self.name = name
        self.value = value

    def evaluate(self):
        value = self.__renderValue(None)

        return '-D' + self.name + ('' if value is None else ('=' + value))

    def __renderValue(self, context):
        return self.value


class CMakeProject:
    def __init__(self, sourcePath):
        self.sourcePath = sourcePath
    
    def createDefinition(self, name, value):
        return '-D' + name + '=' + value

    def configure(self, buildType, buildPath, installPath = None, prefixPaths = None, definitions = None):
        os.system("mkdir -p " + buildPath)

        commands = [
            'cmake ',
            self.sourcePath, 
            '-G "Unix Makefiles"',
            '-DCMAKE_OSX_ARCHITECTURES="arm64"',
            '-DCMAKE_BUILD_TYPE=' + buildType
        ]

        if installPath is not None:
            commands.append('-DCMAKE_INSTALL_PREFIX=' + installPath)

        if prefixPaths is not None:
            commands.append('-DCMAKE_PREFIX_PATH="' + ';'.join(prefixPaths) + '"')

        if definitions is not None:
            for key in definitions:
                definition = self.createDefinition(key, definitions[key])
                commands.append(definition)

        cmakeCommand = " ".join(commands)

        os.system("cd " + buildPath + ' && ' + cmakeCommand)

    def build(self, buildPath):
        os.system("cd " + buildPath + ' && ' + "make")

    def install(self, buildPath):
        os.system("cd " + buildPath + ' && ' + "make install")


class DependencyBuildSystemCMake:
    def __init__(self, definitions):
        self.definitions = definitions

        if definitions is None:
            self.definitions = []

    def configure(self):
        pass


    def build(self):
        pass


    def install(self):
        pass


class Dependency:
    def __init__(self, repository, buildSystem):
        self.repository = repository
        self.buildSystem = buildSystem

    


class DependencyFactory:
    def __init__(self):
        pass

    def create(self, dict):
        name = dict['name']
        repositoryType = dict['repositoryType']

        # Repository Type
        if repositoryType == 'git':
            gitDict = dict['git']
            source = DependencySourceGit(gitDict['provider'], gitDict['author'], gitDict['name'], GitRepository(gitDict['repository']))
        else:
            raise ValueError("Unknown repository type " + repositoryType)

        # build system
        buildSystem = dict['buildSystem']
        if buildSystem == 'cmake':
            cmakeDict = dict['cmake']



class Package:
    def __init__(self, sourcePath):
        self.sourcePath = sourcePath

        # read file
        with open(sourcePath + 'project.json', 'r') as jsonFile:
            jsonData = jsonFile.read()

        # parse file
        jsonContent = json.loads(jsonData)

        self.dependencies = jsonContent['dependencies']
        self.project = CMakeProject(self.sourcePath)


class Xenobuild:
    def __init__(self, sourcePath):
        self.sourcePath = sourcePath
        self.rootPath = '~/cpp-packages/'
        self.mainPackage = Package(self.sourcePath)
        
    def setup(self):
        buildTypes = ['Debug', 'Release']

        installPathsByBuildType = {}
        
        # setup dependencies
        for package in self.mainPackage.dependencies:
            packageName = package['name']

            repository = GitRepository(package['git']['repository'])

            pathSuffix = package['git']['provider'] + '/' + package['git']['author'] + '/' + packageName + '/'
            sourcePath = self.rootPath + 'sources/' + pathSuffix
            installBasePath = self.rootPath + 'packages/' + pathSuffix
            
            # download from a repote Git repository (GitHub)
            repository.clone(sourcePath)
            project = CMakeProject(sourcePath)

            # perform an installation for each build type configuration
            for buildType in buildTypes:
                buildPath = sourcePath + '.build/' + buildType
                installPath = installBasePath + buildType + '/'

                project.configure(buildType, buildPath, installPath, prefixPaths=None, definitions=package['cmake']['definitions'])
                project.build(buildPath)
                project.install(buildPath)

                if buildType in installPathsByBuildType:
                    installPathsByBuildType[buildType].append(installPath)
                else:
                    installPathsByBuildType[buildType] = [installPath]

        # setup project
        project = self.mainPackage.project

        for buildType in buildTypes:
            buildPath = project.sourcePath + '.build/' + buildType

            project.configure(buildType, buildPath, prefixPaths=installPathsByBuildType[buildType])


    def build(self):
        buildTypes = ['Debug', 'Release']
        project = self.mainPackage.project

        for buildType in buildTypes:
            buildPath = project.sourcePath + '.build/' + buildType
            project.build(buildPath)


if __name__ == "__main__":
    xenobuild = Xenobuild(os.getcwd() + '/')

    command = sys.argv[1]

    if command == "setup":
        xenobuild.setup()
    elif command == "build":
        xenobuild.build()
