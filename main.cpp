#include "main.hpp"

int __attribute__((constructor)) Init() {
    
    InitializeInterfaces();
    
    InitializeVMTs();
    
    InitializeHooks();
    
    NetVarManager::dumpNetvars();
    
    Offsets::getOffsets();
    
    return 0;
}

void __attribute__((constructor)) DeInit() {
}
