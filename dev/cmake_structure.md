# CMount's project configuration
CMount is built with CMake. 
The sources of the CMount project are in src and written in C.
---
The compiler on **Windows** is Clang, 
on **Linux** GCC and on **macOS** XCode.

There are dependencies configured as CMake scripts in `vendor/` 
and got listed in `vendor.cmake`:
* [libcurl](https://github.com/curl/curl) 
* [libyaml](https://github.com/yaml/libyaml/tree/master)
* [llvm-c & libclang](https://github.com/llvm/llvm-project)
* [dmon](https://github.com/septag/dmon)

CMake will take on, on the following sources:
* ``include/`` Header of CMount's components, every component has its own header but all uses `core.h`.
* ``resources/`` All assets CMount needs at runtime (e.g. scripts, configs, templates & data).
* ``src/`` The source of CMount that implements the header and will be built to the executable.
* ``vendor/`` Configurations for third party libraries.
* ``components.cmake`` CMount is participated in modular components as shared libraries. A component will be registered here.
* ``structure.cmake`` Defines the output structure of CMount's binaries.
* ``vendor.cmake`` The third party libraries will be registered in the script.

To build CMount with CMake you must have installed the compiler of your platform as well as the dependencies in the path or as arguments to ``$ cmake``.

### Only a release build comes with the pre-installed extensions
Notice, that CMount ships with extensions that provides the implementation of the documentation generator, 
Dart & Python.
The build of CMount does not include these `extensions/`. 
You have to build them individually from source and then install them with ``$ cmount install <extension>``.
The ``.github/workflows/build.yml`` continuous integration configuration does this with a release mode. 
In nightly mode CMount comes without the extensions.