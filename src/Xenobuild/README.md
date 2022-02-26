
# CLI for Xenobuild

- Subcommand-oriented CLI.
- Version 1.0: Wrapper for CLI management for CMake build configurations and dependency resolving

## Happy Workflow

1. Dev is at the source code folder
2. Dev adds a new build configuration, for the

```sh
$ xenocli configuration add default

Default target triplet is 'msvc-windows-x64'
Underlying build system is 'CMake-3.12.9', with generator "NMake Makefiles".
Installing the following dependencies
    yaml-cpp
    Catch2
Adding debug and release build configurations.
Setting 'msvc-windows-x64' as the default target triplet.

$ xenocli configuration list
    [1] msvc-windows-x64 *

$ xenocli configuration add mingw64-windows-x64
Underlying build system is 'CMake-3.12.9', with generator "NMake Makefiles".
Installing the following dependencies
    yaml-cpp
    Catch2
Adding debug and release build configurations
```
