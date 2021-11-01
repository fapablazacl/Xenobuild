
from builder import Builder
from toolchain import ToolchainGCC
from project import Target, Project


def testConsoleApp01():
    target = Target("ConsoleApp01", items=["ConsoleApp01.hpp", "ConsoleApp01.cpp"])
    project = Project("ConsoleApp01", path="data/samples/ConsoleApp01", targets=[target])

    return project


def testMyApp():
    myAppTarget = Target("MyApp", path="MyApp", items=["MyApp.cpp"])
    myLib01 = Target("MyLib01", path="MyLib01", items=["MyLib01.hpp", "MyLib01.cpp"])
    myLib02 = Target("MyLib02", path="MyLib02", items=["MyLib02.hpp", "MyLib02.cpp"])

    project = Project("MyApp", path="data/samples/MyApp", targets=[myAppTarget, myLib01, myLib02])

    return project

if __name__=="__main__":
    project = testMyApp()

    toolchain = ToolchainGCC("D:/mingw-w64/x86_64-6.2.0-posix-seh-rt_v5-rev1/mingw64/bin")

    builder = Builder(toolchain)
    builder.build(project)
