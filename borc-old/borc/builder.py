
import os


def is_compilable(filename):
    base, ext = os.path.splitext(filename)
    exts = [".c", ".cpp", ".cxx", ".c++", ".cc"]

    return ext.lower() in exts


class Builder:
    """
    C/C++ Project Builder based on any toolchain
    """

    def __init__(self, toolchain):
        self.toolchain = toolchain

    def build(self, project):
        commands = []

        for target in project.targets:

            out_files = []

            for item in target.items:
                if not is_compilable(item):
                    continue

                in_file = target.getItemPath(item)
                out_file = target.getItemOutPath(item)

                out_files.append(out_file)

                command = self.toolchain.getItemBuildCommand(in_file, out_file, item + " ...")
                commands.append(command)

            command = self.toolchain.getTargetBuildCommand(target.getOutFilename(), out_files, "Linking")
            commands.append(command)

        #run those commands
        for command in commands:
            command.execute()

        print("Built", len(project.targets), "target(s)")
