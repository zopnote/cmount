#include <clang-c/Index.h>
#include <llvm-c/Core.h>
#include <llvm-c/IRReader.h>
#include <llvm-c/BitWriter.h>
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source.c>\n", argv[0]);
        return 1;
    }

    // Parse C source file using libclang
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit tu = clang_parseTranslationUnit(index, argv[1], NULL, 0, NULL, 0, CXTranslationUnit_None);
    if (!tu) {
        fprintf(stderr, "Failed to parse %s\n", argv[1]);
        return 1;
    }

    // Create LLVM context and module
    LLVMContextRef context = LLVMContextCreate();
    LLVMModuleRef module = LLVMModuleCreateWithName("my_module");

    // TODO: Translate AST from libclang to LLVM IR

    // Write LLVM IR to file
    if (LLVMPrintModuleToFile(module, "output.ll", NULL) != 0) {
        fprintf(stderr, "Failed to write LLVM IR to file\n");
        return 1;
    }

    // Cleanup
    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);
    LLVMDisposeModule(module);
    LLVMContextDispose(context);

    return 0;
}