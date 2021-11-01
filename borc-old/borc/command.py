
import subprocess
import os


class Command:
    def execute(self):
        raise NotImplementedError()


class MetaCommand(Command):
    def __init__(self, commands=[]):
        self.commands = commands

    def execute(self):
        for command in self.commands:
            command.execute()


class SubprocessCommand(Command):
    def __init__(self, cmd, args, desc=None):
        self.cmd = cmd
        self.args = args
        self.desc = desc

    def execute(self):
        cmdline = [self.cmd]
        cmdline.extend(self.args)

        if self.desc is not None:
            print(self.desc)

        subprocess.call(cmdline)


class MkDirCommand(Command):
    def __init__(self, path):
        self.path = path

    def execute(self):
        os.mkdir(self.path)
