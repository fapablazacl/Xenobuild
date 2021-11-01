
from command import SubprocessCommand

import os


class Toolchain:
    def getItemBuildCommand(self, infile, outfile, desc=None):
        raise NotImplementedError()

    def getTargetBuildCommand(self, name, outfiles, desc=None):
        raise NotImplementedError()


class ToolchainGCC(Toolchain):
    def __init__(self, path):
        self.path = path

        self.compileTool = os.path.join(self.path, "gcc")
        self.linkTool = os.path.join(self.path, "gcc")

    def getItemBuildCommand(self, infile, outfile, desc=None):
        return SubprocessCommand(self.compileTool, ["-c", "-O0", "-g", infile, "-o", outfile], desc)

    def getTargetBuildCommand(self, outfilename, outfiles, desc=None):
        args = []
        args.extend(outfiles)
        args.extend(["-o", outfilename, "-lm", "-lstdc++"])

        return SubprocessCommand(self.linkTool, args, desc)


class ToolchainVC(Toolchain):
    pass


class ToolchainDebug(Toolchain):
    pass
