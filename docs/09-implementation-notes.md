## Implementation Details

### Useful CMake shell commands 
#### Clone a Dependency
    git clone --depth 1 --branch yaml-cpp-0.7.0 https://github.com/jbeder/yaml-cpp.git

#### Configure a Dependency (Debug)
    cmake -B build/debug -S . -DCMAKE_BUILD_TYPE=Debug -DCMAKE_INSTALL_PREFIX="D:\Development\packages\yaml-cpp-yaml-cpp-0.7.0"

#### Build a Dependency (Debug)
    cmake --build build/debug

#### Configure a Dependency (Release)
    cmake -B build/release -S . -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX="D:\Development\packages\yaml-cpp-yaml-cpp-0.7.0"

#### Build a Dependency (Release)
    cmake --build build/release

#### Install a Dependency
    cmake --build build/debug --target install 
    cmake --build build/release --target install

#### Link a Dependency
    -DCMAKE_PREFIX_PATH=D:\Development\packages\yaml-cpp-yaml-cpp-0.7.0
