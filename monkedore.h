#ifndef MONKEDORE_HEADER
#define MONKEDORE_HEADER

#ifndef MONKEDORE_NO_STDLIB
    #include <stdint.h>

    typedef uint8_t monkedore_Byte;
    typedef int8_t monkedore_SignedByte;

    typedef uint16_t monkedore_Word;
    typedef int16_t monkedore_SignedWord;
#else
    typedef unsigned char monkedore_Byte;
    typedef char monkedore_SignedByte;

    typedef unsigned short monkedore_Word;
    typedef short monkedore_SignedWord;
#endif

typedef struct {
    monkedore_Byte r, g, b;
} monkedore_Color;

struct monkedore_Stack {
    monkedore_SignedWord ptr;
    monkedore_Word data[0xFF+1];
};

typedef struct {
    monkedore_Byte ram[0xFFFF+1];
    monkedore_Word ip;

    struct monkedore_Stack data_stack;
    struct monkedore_Stack return_stack;

    monkedore_Byte framebuffer[(128*128)/2];

} monkedore_Vm;

typedef enum {
    monkedore_SUCCESS,
    monkedore_HALTED,
    monkedore_ERROR,
    monkedore_ERROR_OVERFLOW,
    monkedore_ERROR_UNDERFLOW,
} monkedore_ReturnStatus;

monkedore_ReturnStatus monkedore_LoadProgram(monkedore_Vm* vm, monkedore_Byte program[], monkedore_Word program_length);
monkedore_ReturnStatus monkedore_ExecuteVmCycle(monkedore_Vm* vm);

#endif

#ifdef MONKEDORE_IMPLEMENTATION
#undef MONKEDORE_IMPLEMENTATION

monkedore_ReturnStatus monkedore_LoadProgram(monkedore_Vm* vm, monkedore_Byte program[], monkedore_Word program_length) {
    if (program_length > 0xFFFF/2) return monkedore_ERROR;

    for (int i=0; i<program_length; i++) {
        vm->ram[i] = program[i];
    }

    return monkedore_SUCCESS;
}

#define PUSH(value, stack) (stack).data[stack.ptr] = value; (stack).ptr++
#define FETCH_WORD() ((monkedore_Word)(vm->ram[vm->ip++] << 8) | (monkedore_Word)vm->ram[vm->ip++])
#define STACK_TOP(stack) (stack).data[(stack).ptr-1]

monkedore_ReturnStatus monkedore_ExecuteVmCycle(monkedore_Vm* vm) {

    monkedore_Byte opcode = vm->ram[vm->ip++];

    switch (opcode) {
        /* NOP  */ case 0x00: break;
        /* HALT */ case 0x01: return monkedore_HALTED;

        /* PUSH */ case 0x02: PUSH(FETCH_WORD(), vm->data_stack); break;
        /* DUP  */ case 0x03: PUSH(STACK_TOP(vm->data_stack), vm->data_stack); break;
        /* OVER */ case 0x04: break;

        /* POP  */ case 0x05: break;
        /* NIP  */ case 0x06: break;

        /* SWAP */ case 0x07: break;
        /* ROT  */ case 0x08: break;

        /* GET  */ case 0x09: break;
        /* SET  */ case 0x0A: break;

        /* LOAD */ case 0x0B: break;
        /* STORE*/ case 0x0C: break;
    
    }

    // Check for over/underflow
    if (vm->data_stack.ptr < 0) return monkedore_ERROR_UNDERFLOW;
    if (vm->return_stack.ptr < 0) return monkedore_ERROR_UNDERFLOW;

    if (vm->data_stack.ptr > 0xFF) return monkedore_ERROR_OVERFLOW;
    if (vm->return_stack.ptr > 0xFF) return monkedore_ERROR_OVERFLOW;

    return monkedore_SUCCESS;
}

#undef STACK_TOP
#undef FETCH_WORD
#undef PUSH

#endif
