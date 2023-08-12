
import sys
import os
import subprocess


def get_path_ext(path):
    split_tup = os.path.splitext(path)
    
    if len(split_tup) <= 1:
        return None

    return split_tup[1]


def is_ext_cpp_source(ext):
    return ext in [".cpp", ".cc", ".cxx", ".c++", ".c"]


def is_ext_cpp_header(ext):
    return ext in [".hpp", ".hh", ".hxx", ".h++", ".h"]


def is_ext_cpp(ext):
    return is_ext_cpp_source(ext) or is_ext_cpp_header(ext)


def cpp_filter(filePath):
    ext = get_path_ext(filePath)
    return is_ext_cpp(ext)


# return the list of all files in a folder
def list_files(path, filter = None):
    sources = []

    for entry in os.listdir(path):
        currentPath = path + "/" + entry

        if os.path.isdir(currentPath):
            sources.extend(list_files(currentPath, filter))
        elif os.path.isfile(currentPath):
            if filter is None:
                sources.append(currentPath)
            else:
                if filter(currentPath):
                    sources.append(currentPath)
            
    return sources


def cmake_gen_target_command(type):
    if type == "lib":
        return "add_library"
    
    if type == "exe":
        return "add_executable"

    return None


def cmake_gen_target_script(name, type, sources, includeDirectory=None):
    sources_value = "\n".join(sources)

    template = f""" # Automatically generated CMakeLists.txt

set (target {name})
set (sources {sources_value})

{cmake_gen_target_command(type)}(${{target}} ${{sources}})
"""
    if includeDirectory is not None:
        template += f'target_include_directories(${{target}} PUBLIC {includeDirectory})'

    return template
