#include "jit_engine.hpp"

jit_engine::~jit_engine() {
    Expected<unique_ptr<LLJIT>> lljit = LLJITBuilder().create();
    if (!lljit)
        throw std::runtime_error("JIT konnte nicht initialisiert werden");

    jit_machine = move(*lljit);

    stable_dylib = move(*jit_machine->createJITDylib("stable"));
    lazy_dylib = jit_machine->createJITDylib("lazy");
    hot_dylib = jit_machine->createJITDylib("hot");

    lazy_dylib.addToLinkOrder(stable_dylib);
    hot_dylib.addToLinkOrder(lazy_dylib);
}
