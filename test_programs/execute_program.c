#define MONKEDORE_IMPLEMENTATION
#include "../monkedore.h"

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    monkedore_Byte data[0x10000/2];
    monkedore_Word size;
} Rom;

void LoadByteCode(Rom* dest, char* path) {
    FILE* fp = fopen(path, "rb");

    if (fp == NULL) {
        printf("Could not open file\n");
        exit(-1);
    }
    fread(dest, sizeof(Rom), 1, fp);

    fclose(fp);
}

void PrintStacks(monkedore_Vm* vm) {

    printf("== RETURN STACK ==\n");
    for (int i=vm->return_stack.ptr-1; i>=0; i--) {
        if (i == vm->return_stack.ptr-1) printf("------------------\n");
        printf("%02x: %0x04x\n", i, vm->return_stack.data[i]);
        if (i == vm->return_stack.ptr-1) printf("------------------\n");
    }
    printf("==================\n");
    printf("\n\n");

    printf("== DATA STACK ==\n");
    for (int i=vm->data_stack.ptr-1; i>=0; i--) {
        if (i == vm->data_stack.ptr-1) printf("----------------\n");
        printf("%02x: 0x%04x | %d\n", i, vm->data_stack.data[i], vm->data_stack.data[i]);
        if (i == vm->data_stack.ptr-1) printf("----------------\n");
    }
    printf("================\n");


}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("No file specified\n");
        return -1;
    }

    monkedore_Vm creature_of_steel = { 0 };
    monkedore_InitVM(&creature_of_steel);

    Rom source = { 0 };
    LoadByteCode(&source, argv[1]);

    /*for (int i=0; i<100; i++) {
        printf("%02x ", source.data[i]);
        if (i % 16 == 15) printf("\n");
    }
    printf("\n");
    printf("size: %d\n", source.size);
    return -1;*/

    monkedore_LoadProgram(&creature_of_steel, source.data, source.size);
    monkedore_ReturnStatus status = monkedore_SUCCESS;

    while(status == monkedore_SUCCESS) {
        monkedore_Byte current_instruction = creature_of_steel.ram[creature_of_steel.ip-1];
        status = monkedore_ExecuteVmCycle(&creature_of_steel);
        // Step debugging
        if (argc == 3) {
            int temp = 0;
            scanf("%d", &temp);

            PrintStacks(&creature_of_steel);

            printf("IP: %04x(%d)\n", creature_of_steel.ip, creature_of_steel.ip);
            printf("Instruction: %02x\n", creature_of_steel.ip);
        }
    }

    switch (status) {
        case monkedore_ERROR:
            printf("An error has occured\n");
            break;
        case monkedore_ERROR_OVERFLOW:
            printf("Stack overflow\n");
            break;
        case monkedore_ERROR_UNDERFLOW:
            printf("Stack underflow\n");
            break;
        default:
            printf("Program ran successfully :)\n");
            break;
    }
    printf("\n\n");
    PrintStacks(&creature_of_steel);

    return 0;
}
