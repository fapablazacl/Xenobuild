
import init
import argparse
import os


def buildParser():
    parser = argparse.ArgumentParser(description='The C++ Developer Init Assistant')
    parser.add_argument('--name', help='The name of the executable target', required=True)
    parser.add_argument('--type', help='The type of the required target', required=True, choices=['lib', 'exe'])
    parser.add_argument('--include-path', help='The public include path')

    return parser


def initializeTargetScript(name, type, includePath=None):
    path = "."
    sources = init.list_files(path, init.cpp_filter)
    template = init.cmake_gen_target_script(name, type, sources, includePath)

    if os.path.exists("CMakeLists.txt"):
        raise FileExistsError("Pre-existing CMakeLists.txt found.")

    with open("CMakeLists.txt", "w") as fileHandle:
        fileHandle.write(template)

parser = buildParser()
args = parser.parse_args()
initializeTargetScript(args.name, args.type, args.include_path)
