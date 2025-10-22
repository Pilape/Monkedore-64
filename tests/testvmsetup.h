#ifndef TESTVMSETUP_HEADER
#define TESTVMSETUP_HEADER

#define VM_INIT() \
    monkedore_Vm vm = { 0 }; \
    monkedore_LoadProgram(&vm, program, sizeof(program)/sizeof(monkedore_Byte)) 

#define VM_STACK_TOP(stack) (stack).data[(stack).ptr-1]
#define VM_STACK_ELEMENT(stack, offset) (stack).data[(stack).ptr-1-offset]

#endif
