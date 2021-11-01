
import os

from enum import IntEnum


class Project:
    def __init__(self, name, path=".", targets=[]):
        self.name = name
        self.path = path
        self.targets = targets

        for target in self.targets:
            target.project = self

        self.outPath = ".borc"


class TargetType(IntEnum):
    Executable = 1
    DynamicLibrary = 2
    StaticLibrary = 3


class Target:
    def __init__(self, name, path="", type=TargetType.Executable, includes=None, items=None):
        self.name = name
        self.path = path
        self.type = type
        self.items = items
        self.includes = includes
        self.dependencies = []
        self.project = None

    def getItemPath(self, item):
        return os.path.join(self.project.path, self.path, item)

    def getItemOutPath(self, item):
        base, ext = os.path.splitext(item)
        item = base + ext + ".obj"

        return os.path.join(self.project.path, self.project.outPath, self.path, item)

    def getOutFilename(self):
        return os.path.join(self.project.path, self.project.outPath, self.path, self.name)
