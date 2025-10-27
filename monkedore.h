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
    monkedore_Byte carry;

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

void monkedore_InitVM(monkedore_Vm* vm) {
    *vm = (monkedore_Vm){ 0 };

    monkedore_Color default_palette[] = {
        {  },
    };

}

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
#define STACK_ELEMENT(stack, offset) (stack).data[(stack).ptr-1-offset]
#define POP(stack) (stack).data[(stack).ptr-1]; (stack).ptr--
#define GET_RAM_WORD(address) ((vm->ram[address] << 8) | (vm->ram[(address+1) & 0xFFFF]))
#define SET_RAM_WORD(word, address) vm->ram[address] = word >> 8; vm->ram[(address+1) & 0xFFFF] = word & 0xFF
#define BINARY_OP(op) { monkedore_Word b = POP(vm->data_stack); monkedore_Word a = POP(vm->data_stack); PUSH(op, vm->data_stack); } break;
#define BINARY_OP_CARRY(op, cmp) { monkedore_Word b = POP(vm->data_stack); monkedore_Word a = POP(vm->data_stack); monkedore_Word result = op; vm->carry = (result cmp a) ? 1 : 0; PUSH(result, vm->data_stack); } break;

monkedore_ReturnStatus monkedore_ExecuteVmCycle(monkedore_Vm* vm) {

    monkedore_Byte opcode = vm->ram[vm->ip++];

    switch (opcode) {
        /* NOP   */ case 0x00: break;
        /* HALT  */ case 0x01: return monkedore_HALTED;

        /* PUSH  */ case 0x02: PUSH(FETCH_WORD(), vm->data_stack); break;
        /* DUP   */ case 0x03: PUSH(STACK_TOP(vm->data_stack), vm->data_stack); break;
        /* OVER  */ case 0x04: PUSH(STACK_ELEMENT(vm->data_stack, 1), vm->data_stack); break;

        /* POP   */ case 0x05: vm->data_stack.ptr--; break;
        /* NIP   */ case 0x06: STACK_ELEMENT(vm->data_stack, 1) = STACK_TOP(vm->data_stack); vm->data_stack.ptr--; break;

        /* SWAP  */ case 0x07: { monkedore_Word b = POP(vm->data_stack); monkedore_Word a = POP(vm->data_stack); PUSH(b, vm->data_stack); PUSH(a, vm->data_stack); } break;
        /* ROT   */ case 0x08: { monkedore_Word c = POP(vm->data_stack); monkedore_Word b = POP(vm->data_stack); monkedore_Word a = POP(vm->data_stack);
                                   PUSH(b, vm->data_stack); PUSH(c, vm->data_stack); PUSH(a, vm->data_stack); } break;

        /* LOAD  */ case 0x09: { monkedore_Word address = POP(vm->data_stack); PUSH(GET_RAM_WORD(address), vm->data_stack); } break;
        /* STORE */ case 0x0A: { monkedore_Word address = POP(vm->data_stack); monkedore_Word word = POP(vm->data_stack); SET_RAM_WORD(word, address); } break;
        /* LOADb */ case 0x0B: { monkedore_Word address = POP(vm->data_stack); PUSH(vm->ram[address], vm->data_stack); } break;
        /* STOREb*/ case 0x0C: { monkedore_Word address = POP(vm->data_stack); monkedore_Word word = POP(vm->data_stack); vm->ram[address] = word & 0xFF; } break;

        /* ADD   */ case 0x0D: BINARY_OP_CARRY(a + b, <); /* SUB   */ case 0x0E: BINARY_OP_CARRY(a - b, >);
        /* ADDc  */ case 0x0F: BINARY_OP_CARRY(a + b + vm->carry, <);
        /* SUBc  */ case 0x10: BINARY_OP_CARRY(a - b - vm->carry, >);
        
        /* SHL   */ case 0x11: BINARY_OP(a << b); 
        /* SHR   */ case 0x12: BINARY_OP(a >> b); 

        /* bNAND */ case 0x13: BINARY_OP(~(a & b)); 
        /* NAND  */ case 0x14: BINARY_OP(!(a && b)); 

        /* EQUAL */ case 0x15: BINARY_OP(a == b);
        /* MORE  */ case 0x16: BINARY_OP(a > b);
        /* LESS  */ case 0x17: BINARY_OP(a < b);

        /* JUMP  */ case 0x18: vm->ip = FETCH_WORD(); break;
        /* BRANCH*/ case 0x19: vm->ip += (monkedore_SignedByte) vm->ram[vm->ip++]; break;
        /* BIF0  */ case 0x1A: if (STACK_TOP(vm->data_stack) == 0) { vm->data_stack.ptr--; vm->ip += (monkedore_SignedByte) vm->ram[vm->ip++]; } else vm->ip++; break;
        /* BIFN0 */ case 0x1B: if (STACK_TOP(vm->data_stack) != 0) { vm->data_stack.ptr--; vm->ip += (monkedore_SignedByte) vm->ram[vm->ip++]; } else vm->ip++; break;
        /* CALL  */ case 0x1C: PUSH(vm->ip+2, vm->return_stack); vm->ip = FETCH_WORD(); break;
        /* RET   */ case 0x1D: vm->ip = POP(vm->return_stack); break;
    
    }

    // Check for over/underflow
    if (vm->data_stack.ptr < 0) return monkedore_ERROR_UNDERFLOW;
    if (vm->return_stack.ptr < 0) return monkedore_ERROR_UNDERFLOW;

    if (vm->data_stack.ptr > 0xFF) return monkedore_ERROR_OVERFLOW;
    if (vm->return_stack.ptr > 0xFF) return monkedore_ERROR_OVERFLOW;

    return monkedore_SUCCESS;
}

#undef BINARY_OP_CARRY
#undef BINARY_OP
#undef SET_RAM_WORD
#undef GET_RAM_WORD
#undef POP
#undef STACK_ELEMENT
#undef STACK_TOP
#undef FETCH_WORD
#undef PUSH

#endif
