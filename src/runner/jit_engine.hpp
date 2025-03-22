#pragma once
#include <llvm/ExecutionEngine/Orc/LLJIT.h>


using namespace std;
using namespace llvm;
using namespace orc;

class jit_engine {
    ~jit_engine();

    void add_module(const string &dylib_name, unique_ptr<llvm::Module> module);
    void remove_module(const string &dylib_name, const string &module_name);
    void reload_module(const string &dylib_name, unique_ptr<llvm::Module> module);

    llvm::JITSymbol get_symbol(const string &name);

    template <typename T>
    T get_function(const string &name);

private:
    unique_ptr<LLJIT> jit_machine;
    ResourceTrackerSP resource_tracker;
    JITDylib &stable_dylib;
    JITDylib &lazy_dylib;
    JITDylib &hot_dylib;

    JITDylib &get_jit_dylib();
}