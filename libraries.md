## Usage of shared libraries
cmount is divided into modules: the core, parser, CVM (C JIT LLVM), runner, builder, and server (implementation for different languages).

If you want to use cmount via the API, copy the desired import library from the ``lib`` directory and the associated shared library from the ``bin`` directory. Make sure to also copy the core library.

**Each library is dependent on the core**, but never between each other.
The core exists to reduce boilerhead, it isn't intended for third party usage.

### CVM: C language just in time virtual machine
### Server: HTTP servers for implementation of different languages
### Parser: Project configuration analysis
### Runner: Interface for debugging
### Builder: Compilation and building for different configurations